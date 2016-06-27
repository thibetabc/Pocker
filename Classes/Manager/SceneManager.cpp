#include "SceneManager.h"
#include "LayerMain.h"
#include "LandScape.h"
#include "LayerWelcome.h"
#include "global_header.h"
#include "utils.h"
#include "LayerLogin.h"
#include "TipView.h"
#include "WaitingView.h"
#include "LoginManager.h"
#include "PokerDataManager.h"
#include "ChatMessageManager.h"
#include "cmd_def.h"
#include "GameTools.h"

using namespace cocos2d;


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "../../cocos2d/cocos/platform/android/jni/JniHelper.h"
#include <android/log.h>

extern "C"{
    JNIEXPORT void JNICALL Java_com_insul_poker_AppActivity_clickBack(JNIEnv* env, jobject thiz)
    {
        cocos2d::log("Java_org_cocos2dx_cpp_AppActivity_click_back");
        events::global_dispatch.raise_event(EVENT_ANDROID_BACK);
    }
}
#endif

bool SceneManager::landscape_ = false;

void SceneManager::refresh(Node *n)
{
    if(n == NULL)
        return;
    bool v = n->isVisible();
    n->setVisible(!v);
    n->setVisible(v);
    const Vector<Node*>& nodes = n->getChildren();
    for (auto iter = nodes.begin(); iter != nodes.end(); iter++) {
        refresh(*iter);
    }
}

BaseLayer* SceneManager::get_layer_by_name(std::string name)
{
    Scene* s = Director::getInstance()->getRunningScene();
    if (s == nullptr)
    {
        return nullptr;
    }
    
    auto result = s->getChildByName(name);
    
    return dynamic_cast<BaseLayer*>(result);
}

void SceneManager::add_layer(BaseLayer* node, int zorder)
{
    int tag = node->getTag();
    if (tag != TipViewTag && tag != LayerShareTag && tag != WaitingViewTag)
        HideOtherLayer();
    Scene* s = Director::getInstance()->getRunningScene();
    if (s == nullptr)
    {
        s = Scene::create();
        Director::getInstance()->runWithScene(s);
        
        ////////////////////////////////////////
        //line code add by lvzhangle 20160521
        Director::getInstance()->drawScene();
        ////////////////////////////////////////
    }
    s->addChild(node, zorder, node->GetName());
//    测试android返回键崩溃
//    createAndroidBack(s);
}
void SceneManager::createAndroidBack(Scene *scene)
{
    std::string imgPath = "image/ofc_game_new_res/image_will_leave.png";
    auto sizeScreen = Director::getInstance()->getVisibleSize();
    auto buttonBack = Button::create(imgPath, imgPath, imgPath);
    auto sizeButtonBack = buttonBack->getContentSize();
    buttonBack->setPosition(Vec2(sizeScreen.width-sizeButtonBack.width, sizeScreen.height - sizeButtonBack.height));
    buttonBack->addClickEventListener([&](Ref *ref)
    {
        events::global_dispatch.raise_event(EVENT_ANDROID_BACK);
    });
    scene->addChild(buttonBack, 999999999);
}

BaseLayer* SceneManager::GetMainLayer()
{
    return get_layer_by_name(typeid(LayerMain).name());
}

BaseLayer* SceneManager::GetTipLayer()
{
    return get_layer_by_name(typeid(TipView).name());
}

BaseLayer* SceneManager::GetWaitingLayer()
{
    return get_layer_by_name(typeid(WaitingView).name());
}

BaseLayer* SceneManager::GetRoomLayer()
{
    std::string name = NAME_LAYERGAMEROOM;
    return get_layer_by_name(name);
}

BaseLayer* SceneManager::GetLoginLayer()
{
    return get_layer_by_name(typeid(LayerLogin).name());
}

void SceneManager::clear()
{
    Scene* s = Director::getInstance()->getRunningScene();
    if (s)
        s->removeAllChildren();
    LayerPool::shared_instence().clear();
}


bool SceneManager::is_layer_on_top(BaseLayer* node)
{
    Scene* s = Director::getInstance()->getRunningScene();
    for (auto layer = s->getChildren().rbegin(); layer != s->getChildren().rend();layer++)
        if ((*layer)->getTag() != TipViewTag && (*layer)->getTag() != WaitingViewTag){
            BaseLayer* next = dynamic_cast<BaseLayer*>(*layer);
            if (!next) continue;
            return *layer == node;
        }
    return false;
}

BaseLayer* SceneManager::addTipLayer()
{
    auto layer = GetTipLayer();
    if (layer)
        layer->removeFromParent();
    TipView* tip = TipView::create();
    tip->setTag(TipViewTag);
    add_layer(tip);
    return tip;
}

void SceneManager::addWaitingLayer()
{
    auto layer = GetWaitingLayer();
    if (layer == nullptr)
    {
        WaitingView* waitlayer = WaitingView::create();
        waitlayer->setTag(WaitingViewTag);
        add_layer(waitlayer);
    }
}

void SceneManager::addLoginLayer(bool enable,std::string s)
{
    auto layer = GetLoginLayer();
    if (layer) return;
    clear();
    LayerLogin* login = LayerLogin::create();
    login->set_loginbtn_enable(enable,s);
    add_layer(login);
}

void SceneManager::addWelcomeLayer()
{
    clear();
    LayerWelcome* login = LayerWelcome::create();
    add_layer(login);
}

void SceneManager::addMainLayer()
{
    clear();
    LayerMain* lm = LayerMain::create();
    add_layer(lm);
    GetMainLayer();
}

void SceneManager::HideOtherLayer()
{
    Scene* s = Director::getInstance()->getRunningScene();
    if (s)
        for (auto node: s->getChildren()){
            if (dynamic_cast<BaseLayer*>(node) && node->getTag() != TipViewTag && node->getTag() != WaitingViewTag)
                node->setVisible(false);
        }
}

std::list<BaseLayer*> LayerPool::layers_ = std::list<BaseLayer*>();

BaseLayer::BaseLayer()
{
    if (IsIndepedent())
        LayerPool::shared_instence().push_layer(this);
}

BaseLayer::~BaseLayer()
{
    
}

const std::string BaseLayer::GetName(){
    return typeid(*this).name();
}

void BaseLayer::AddGlobalLayer(BaseLayer* layer)
{
    SceneManager::add_layer(layer);
}

bool BaseLayer::check_request(std::shared_ptr<net_data_recv_package> package)
{
    bool result = false;
    auto iter = requestlist.find(package->packagehead.request_code);
    if (iter == requestlist.end()){
        result = false;
    }
    else{
        msg::ClientResponseHead head;
        head.ParseFromArray(package->head, package->packagehead.head_size);
        auto timeiter = iter->second.find(head.timemark());
        if (timeiter == iter->second.end())
            result = false;
        else{
            requestlist.erase(iter);
            //iter->second.erase(head.timemark());
            result = true;
        }
    }
    if (requestlist.size() == 0)
    {
        removeWaiting();
    }
    return result;
}

void BaseLayer::send_data(const MyMessage& processor, int request_code,bool isNeedUserid)
{
    showWaiting();
    uint64_t timemarkCur = tools::date();
    auto iter = requestlist.find(request_code);
    if (iter == requestlist.end()){
        std::map<uint64_t, bool> requests;
        requests.insert(std::make_pair(timemarkCur, isNeedUserid));
        requestlist.insert(std::make_pair(request_code, requests));
        
        net_manager::shared_instence().send_data(processor, request_code, timemarkCur,isNeedUserid);
    }
    else{
        //iter->second.insert(std::make_pair(timemarkCur, isNeedUserid));
    }
    
}

void BaseLayer::handle_android_back()
{
    removeFromParent();
}

void BaseLayer::handle_connect_break(const std::shared_ptr<server_status_data>& data)
{
    removeWaiting();
    if (SceneManager::is_layer_on_top(this))
    {
        ShowError(tools::local_string("disconnect"));
        requestlist.clear();
        LayerDisconnect();
    }
}

void BaseLayer::handle_connect_success(const std::shared_ptr<server_status_data>& data)
{
    ClearError();
    if (SceneManager::is_layer_on_top(this))
        RefreshLayer();
}

cocos2d::Vec2 BaseLayer::GetErrorPoint()
{
    Size winSize = Director::getInstance()->getWinSize();
    return Vec2(winSize.width/2,winSize.height-184);
}

void BaseLayer::ShowMessage(const std::string& message, bool autoclose)
{
    TipView* tip = TipView::showAlertView(message);
    if (autoclose)
        tip->set_tipview_close_callback([=]{
            SceneManager::addLoginLayer();
        });
}

void BaseLayer::RefreshLayer()
{
    
}

void BaseLayer::LayerDisconnect()
{
//    WaitingView::remove();
    removeWaiting();
}

void BaseLayer::ShowError(const std::string& message)
{
    Node* child = getChildByName("textTip");
    if( child )
    {
        Text* text = dynamic_cast<Text*>(child);
        text->setString(message);
    }
    else
    {
        Text* textTip = Text::create(message, "HiraginoSansGB-W6", 24);
        addChild(textTip,0,"textTip");
        textTip->setColor(Color3B(202,79 ,141));
        textTip->setPosition(GetErrorPoint());
    }
    
}

void BaseLayer::ClearError()
{
    auto text = getChildByName("textTip");
    if (text){
        text->removeFromParent();
    }
}

bool BaseLayer::init()
{
    if(!Layer::init()) {
        return false;
    }
    
    layout_ = ui::Layout::create();
    layout_->setContentSize(GetLayerSize());
    layout_->setTouchEnabled(true);
    layout_->setAnchorPoint(GetAnchorPoint());
    layout_->addClickEventListener([](Ref*){
        GameTools::editBoxLoseInputFocus();
    });
    this->addChild(layout_, -10);
    
    
    set_show_dir(DEFAULT_DIR);
    
    register_events();
    return true;
}

cocos2d::Size BaseLayer::GetLayerSize()
{
    auto size = Director::getInstance()->getWinSize();
    size = Size(std::min(size.width, size.height), std::max(size.width, size.height));
    return size;
}

cocos2d::Vec2 BaseLayer::GetAnchorPoint()
{
    return Point::ZERO;
}


void BaseLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    cocos2d::Point start_position;
    const Point origin = this->getPosition();
    auto& size = cocos2d::Director::getInstance()->getWinSize();
    switch (show_dir_) {
        case DIRECTION_NONE:
            
            break;
        case TOP_TO_BOTTOM:
            start_position = Point(origin.x, origin.y + size.height);
            break;
        case BOTTOM_TO_TOP:
            start_position = Point(origin.x, origin.y - size.height);
            break;
        case LEFT_TO_RIGHT:
            start_position = Point(origin.x - size.width, origin.y);
            break;
        case RIGHT_TO_LEFT:
            start_position = Point(origin.x + size.width, origin.y);
            break;
            
        default:
            break;
    }
    
    if(start_position != Point::ZERO) {
        this->setPosition(start_position);
        log("origin:%f, %f", origin.x, origin.y);
        
        EaseOut * inMove = EaseOut::create(MoveTo::create(0.2f, origin),5);
        this->runAction(inMove);
    }
    SceneManager::refresh(layout_);
}

void BaseLayer::onExit()
{
    if (IsIndepedent())
        LayerPool::shared_instence().pop_layer(this);
    
    auto origin = this->getPosition();
    auto& size = cocos2d::Director::getInstance()->getWinSize();
    
    cocos2d::Point end_position;
    switch (show_dir_) {
        case DIRECTION_NONE:
            
            break;
        case TOP_TO_BOTTOM:
            end_position = Point(origin.x, origin.y + size.height);
            break;
        case BOTTOM_TO_TOP:
            end_position = Point(origin.x, origin.y - size.height);
            break;
        case LEFT_TO_RIGHT:
            end_position = Point(origin.x - size.width, origin.y);
            break;
        case RIGHT_TO_LEFT:
            end_position = Point(origin.x + size.width, origin.y);
            break;
            
        default:
            break;
    }
    
    if(end_position != Point::ZERO) {
        MoveTo* move = MoveTo::create(0.2f, end_position);
        CallFunc* call = CallFunc::create([=] {
            
            if(dismiss_callback_) {
                dismiss_callback_();
            }
            
        });
        Sequence* seq = Sequence::create(move, call, NULL);
        this->runAction(seq);
        Layer::onExit();
    }
    else {
        if(dismiss_callback_) {
            dismiss_callback_();
        }
        Layer::onExit();
    }
}

void BaseLayer::set_show_dir(const LAYER_SHOW_DIRECTION& dir)
{
    show_dir_ = dir;
    
    auto& size = cocos2d::Director::getInstance()->getWinSize();
    switch (show_dir_) {
        case DIRECTION_NONE:
            
            break;
        case TOP_TO_BOTTOM:
            layout_->setContentSize(Size(size.width, size.height * 2));
            layout_->setPosition(Point(0, -size.height));
            break;
        case BOTTOM_TO_TOP:
            layout_->setContentSize(Size(size.width, size.height * 2));
            layout_->setPosition(Point(0, 0));
            break;
        case LEFT_TO_RIGHT:
            layout_->setContentSize(Size(size.width * 2, size.height));
            layout_->setPosition(Point(0, 0));
            break;
        case RIGHT_TO_LEFT:
            layout_->setContentSize(Size(size.width * 2, size.height));
            layout_->setPosition(Point(-size.width, 0));
            break;
            
        default:
            break;
    }
    
}

void BaseLayer::ShowTipMessage(bool show, std::string message)
{
    if (show)
    {
        if (message == "")
        {
            message = tools::local_string("no_game_room_in_club","亲，您当前没有任何牌局\n快点右上角＋号，去组建牌局和小伙伴们一起玩耍吧~");
        }
        Node* tip = getChildByName("tip_message");
        if (tip)
        {
            Text* text = dynamic_cast<Text*>(tip);
            text->setString(message);
        }
        else
        {
            Text* text = Text::create(message,"HiraginoSansGB-W6",30);
            addChild(text, 0, "tip_message");
            text->setAnchorPoint(Vec2(0.5, 0.5));
            text->setTextHorizontalAlignment(TextHAlignment::CENTER);
            text->setTextColor(Color4B(181, 181, 181, 100));
            auto& size = cocos2d::Director::getInstance()->getWinSize();
            text->setPosition(Vec2(size.width/2, size.height/2));
        }
    }
    else
    {
        Node* tip = getChildByName("tip_message");
        if (tip)
        {
            removeChildByName("tip_message");
        }
    }
}

void BaseLayer::showWaiting()
{
    auto layer = getChildByTag(WaitingViewTag);
    if (layer == nullptr)
    {
        WaitingView* waitlayer = WaitingView::create();
        waitlayer->setTag(WaitingViewTag);
        addChild(waitlayer, 0, waitlayer->GetName());
    }
}

void BaseLayer::removeWaiting()
{
    auto layer = getChildByTag(WaitingViewTag);
    if (layer != nullptr)
    {
        layer->removeFromParent();
    }
}

void LayerPool::handle_android_back(cmd_data_pointer data)
{
    auto back = GetTopLayer();
    if(back)
        back->handle_android_back();
}

BaseLayer* LayerPool::GetTopLayer()
{
    if(!layers_.empty())
        return layers_.back();
    else
        return nullptr;
}

void LayerPool::pop_layer(BaseLayer* Layer)
{
    for (auto iter = layers_.rbegin(); iter != layers_.rend(); iter++){
        if (*iter == Layer){
            layers_.remove(*iter);
            break;
        }
    }
    for (auto iterLayer : layers_)
    {
        log("last_layers_ = %s", typeid(*iterLayer).name());
    }
    Scene* s = Director::getInstance()->getRunningScene();
    if (Layer->getParent() == s)
        for (auto layer = s->getChildren().rbegin(); layer != s->getChildren().rend();layer++)
            if (*layer != Layer && (*layer)->getTag() != TipViewTag && (*layer)->getTag() != WaitingViewTag){
                BaseLayer* next = dynamic_cast<BaseLayer*>(*layer);
                if (!next) continue;
                next->RefreshLayer();
                (*layer)->setVisible(true);
                break;
            }
}

void LayerPool::push_layer(BaseLayer* node)
{
    if (!Inited){
        dispatch_.register_event(EVENT_ANDROID_BACK, BIND_FUNC(this, LayerPool::handle_android_back));
        dispatch_.register_event(EVENT_CONNECT_SUCCESS, BIND_FUNC(this, LayerPool::connect_success));
        dispatch_.register_event(EVENT_CONNECT_BREAK, BIND_FUNC(this, LayerPool::connect_break));
        dispatch_.register_event(EVENT_CONNECT_FAILED, BIND_FUNC(this, LayerPool::connect_failed));
        dispatch_.register_event(EVENT_RECV_ERROR, BIND_FUNC(this, LayerPool::recv_error));
        Inited = true;
    }
    layers_.push_back(node);
}

#pragma mark tcp callback --
void LayerPool::connect_success(cmd_data_pointer data)
{
    std::shared_ptr<server_status_data> rec = dynamic_pointer_cast<server_status_data>(data);
    switch (rec->serverid) {
        case net_manager::SERVER_LOGIN:
            LM->send_versioncheck();
            break;
        case net_manager::SERVER_RESOURCE:
            if (GDM->has_session_key_())
                GDM->login_resource_server();
            break;
        case net_manager::SERVER_ROOM:
            PDM->send_login_roomserver();
            break;
        default:
            break;
    }
    
    net_manager::shared_instence().send_buffer(rec->serverid);
    auto back = GetTopLayer();
    if(back && back->GetServerID() == rec->serverid)
        back->handle_connect_success(rec);
}

void LayerPool::connect_failed(cmd_data_pointer data)
{
    std::shared_ptr<server_status_data> rec = dynamic_pointer_cast<server_status_data>(data);
    switch (rec->serverid) {
        case net_manager::SERVER_LOGIN:
            SceneManager::addLoginLayer();
            SceneManager::GetLoginLayer()->ShowError(tools::local_string("connect_failed", "当前网络不可用，请检查网络设置！"));
            break;
        case net_manager::SERVER_RESOURCE:
            CM->set_chat_data_failed();
            if (SceneManager::GetMainLayer())
                net_manager::shared_instence().connect(net_manager::SERVER_RESOURCE); //无限重连
            else{
                SceneManager::addLoginLayer();
                SceneManager::GetLoginLayer()->ShowError(tools::local_string("connect_failed", "当前网络不可用，请检查网络设置！"));
            }
            break;
        case net_manager::SERVER_ROOM:
            net_manager::shared_instence().connect(net_manager::SERVER_ROOM); //无限重连
            break;
        default:
            break;
    }
}

void LayerPool::connect_break(cmd_data_pointer data)
{
    std::shared_ptr<server_status_data> rec = dynamic_pointer_cast<server_status_data>(data);
    switch (rec->serverid) {
        case net_manager::SERVER_RESOURCE:
            CM->set_chat_data_failed();
            break;
            
        default:
            break;
    }
    
    for (auto iterLayer : layers_)
    {
        iterLayer->handle_connect_break(rec);
    }
}

void LayerPool::recv_error(cmd_data_pointer data)
{
    std::shared_ptr<server_error_data> rec = dynamic_pointer_cast<server_error_data>(data);
    
    std::string message = tools::local_string("recv_error", "错误信息");
    message = message + "  " + rec->error;
    //auto back = GetTopLayer();
    //if(back)
    //    back->ShowError(message);
}
