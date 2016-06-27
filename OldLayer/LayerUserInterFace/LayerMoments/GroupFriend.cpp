//
//  GroupFriend.cpp
//  pokerclient
//
//  Created by admin on 15-8-17.
//
//

#include "GroupFriend.h"
#include "GameDataManager.h"
#include "utils.h"
#include "cmd_def.h"
#include "ActionIssue.h"
#include "GroupFriendSquare.h"
#include "GroupFriendCell.h"
#include "SquareCell.h"
#include "GameTools.h"
#include "SceneManager.h"
#include "LayerComment.h"
#include "PokerDataManager.h"
#include "RecordHudLayer.h"
#include "SceneManager.h"
#include "Landscape.h"
#include "HudLayer.h"

#define FRIEND_CIRCLE_ID 0


bool GroupFriend::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    counter = 0;
    auto size = Director::getInstance()->getWinSize();
    
    auto layout= ui::Layout::create();
    layout->setContentSize(size);
    layout->setTouchEnabled(true);
    layout->setAnchorPoint(Point::ZERO);
    layout->setSwallowTouches(false);
    layout->addTouchCallback(std::bind(&GroupFriend::touch_image, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    layout->addClickEventListener([](Ref*){
        GameTools::editBoxLoseInputFocus();
    });
    this->addChild(layout, 9);
    
    
    auto root = CSLoader::createNode("LayerCommunity.csb");
    this->addChild(root);
    current_select_idx_ = 1;
    
    btn_square_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_square"));
    btn_hot_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_hot"));
    btn_promised_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_promised"));
    btn_mine_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_mine"));
    auto btn_release = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_release"));
    
    auto text_title = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    text_title->setString(tools::local_string("约友圈", "yueyouquan"));
    for (auto& iter : root->getChildren()) {
        log("child'name:%s", iter->getName().c_str());
    }
    
    btn_release->addClickEventListener(std::bind(&GroupFriend::click_btn_release, this, std::placeholders::_1));
    btn_square_->addClickEventListener(std::bind(&GroupFriend::click_btn_square, this, std::placeholders::_1));
    btn_hot_->addClickEventListener(std::bind(&GroupFriend::click_btn_hot, this, std::placeholders::_1));
    btn_promised_->addClickEventListener(std::bind(&GroupFriend::click_btn_promised, this, std::placeholders::_1));
    btn_mine_->addClickEventListener(std::bind(&GroupFriend::click_btn_mine, this, std::placeholders::_1));
    this->click_btn_promised(this);
    
    listview_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "listview"));

    tableview_fri = TableView::create(this, listview_->getContentSize());
    tableview_fri->setAnchorPoint(listview_->getAnchorPoint());
//    tableview_fri->setContentOffset(Vec2(0, 0));
    tableview_fri->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
//    tableview_fri->setVerticalFillOrder(TableView::VerticalFillOrder::BOTTOM_UP);
    tableview_fri->setPosition(listview_->getPosition());
    tableview_fri->setDelegate(this);
    root->addChild(tableview_fri);
    
//    listview_->removeFromParent();

    tableview_fri->touchBeganCallback_ = std::bind(&GroupFriend::touchBegan, this, std::placeholders::_1, std::placeholders::_2);
    tableview_fri->touchMovedCallback_ = std::bind(&GroupFriend::touchMoved, this, std::placeholders::_1, std::placeholders::_2);
    tableview_fri->touchEndedCallback_ = std::bind(&GroupFriend::touchEnded, this, std::placeholders::_1, std::placeholders::_2);
    tableview_fri->touchCancelledCallback_ = std::bind(&GroupFriend::touchCancel, this, std::placeholders::_1, std::placeholders::_2);
    
    dispatch_.register_event(EVENT_DELETE_MOMENT, BIND_FUNC(this, GroupFriend::handle_delete_updata));
    dispatch_.register_event(EVENT_TODO_COMMENT, BIND_FUNC(this, GroupFriend::handle_request_comment));
    
    isDown = false;
    return true;
}

void GroupFriend::click_btn_release(Ref* sender)
{
    ActionIssue* AI = ActionIssue::create();
    BaseLayer* parent1 = static_cast<BaseLayer*>(this->getParent());
    parent1->addChild(AI);
   
    
    if(btn_issue_callback_) {
        btn_issue_callback_();
    }

}

void GroupFriend::click_btn_square(Ref* sender)
{
    this->m_nTag = 1;
//    GroupFriendSquare* GFS = GroupFriendSquare::create();
    //BaseLayer* parent1 = static_cast<BaseLayer*>(this->getParent());
    //    this->removeFromParent();
    //parent1->addChild(GFS);
    GDM->circletype = GameDataManager::TYPE_SQUARE;
    GDM->send_get_moment(FRIEND_CIRCLE_ID,0, msg::MomentDisplayType::SQUARE);
    dis_type_selected = msg::MomentDisplayType::SQUARE;
    if(btn_issue_callback_) {
        btn_issue_callback_();
    }
    
    btn_square_->setTouchEnabled(false);
    btn_hot_->setTouchEnabled(true);
    btn_promised_->setTouchEnabled(true);
    btn_mine_->setTouchEnabled(true);
    
    btn_square_->setBright(false);
    btn_hot_->setBright(true);
    btn_promised_->setBright(true);
    btn_mine_->setBright(true);
    
    btn_square_->setTitleColor(Color3B(255, 255, 255));
    btn_hot_->setTitleColor(Color3B(26, 26, 26));
    btn_promised_->setTitleColor(Color3B(26, 26, 26));
    btn_mine_->setTitleColor(Color3B(26, 26, 26));

    current_select_idx_ = 1;
}

void GroupFriend::click_btn_hot(Ref* sender)
{
    GDM->circletype = GameDataManager::TYPE_RECOMMEND;
    this->m_nTag = 2;
    GDM->send_get_moment(FRIEND_CIRCLE_ID,0, msg::MomentDisplayType::RECOMMEND);
    dis_type_selected = msg::MomentDisplayType::RECOMMEND;
    if(btn_issue_callback_) {
        btn_issue_callback_();
    }

    btn_square_->setTouchEnabled(true);
    btn_hot_->setTouchEnabled(false);
    btn_promised_->setTouchEnabled(true);
    btn_mine_->setTouchEnabled(true);
    
    btn_square_->setBright(true);
    btn_hot_->setBright(false);
    btn_promised_->setBright(true);
    btn_mine_->setBright(true);
    
    btn_square_->setTitleColor(Color3B(26, 26, 26));
    btn_hot_->setTitleColor(Color3B(255, 255, 255));
    btn_promised_->setTitleColor(Color3B(26, 26, 26));
    btn_mine_->setTitleColor(Color3B(26, 26, 26));
    
    current_select_idx_ = 2;
}
void GroupFriend::click_btn_promised(Ref* sender)
{
    GDM->circletype = GameDataManager::TYPE_FRIEND;
    this->m_nTag = 3;
    GDM->send_get_moment(FRIEND_CIRCLE_ID,0,msg::MomentDisplayType::FRIEND);
    dis_type_selected = msg::MomentDisplayType::FRIEND;
    if(btn_issue_callback_) {
        btn_issue_callback_();
    }

    btn_square_->setTouchEnabled(true);
    btn_hot_->setTouchEnabled(true);
    btn_promised_->setTouchEnabled(false);
    btn_mine_->setTouchEnabled(true);
    
    btn_square_->setBright(true);
    btn_hot_->setBright(true);
    btn_promised_->setBright(false);
    btn_mine_->setBright(true);
    
    btn_square_->setTitleColor(Color3B(26, 26, 26));
    btn_hot_->setTitleColor(Color3B(26, 26, 26));
    btn_promised_->setTitleColor(Color3B(255, 255, 255));
    btn_mine_->setTitleColor(Color3B(26, 26, 26));
    
    current_select_idx_ = 3;
}
void GroupFriend::click_btn_mine(Ref* sender)
{
    GDM->circletype = GameDataManager::TYPE_MINE;
    this->m_nTag = 4;
    GDM->send_get_moment(FRIEND_CIRCLE_ID,0, msg::MomentDisplayType::MINE);
    dis_type_selected = msg::MomentDisplayType::MINE;
    if(btn_issue_callback_) {
        btn_issue_callback_();
    }
    
    btn_square_->setTouchEnabled(true);
    btn_hot_->setTouchEnabled(true);
    btn_promised_->setTouchEnabled(true);
    btn_mine_->setTouchEnabled(false);
    
    btn_square_->setBright(true);
    btn_hot_->setBright(true);
    btn_promised_->setBright(true);
    btn_mine_->setBright(false);
    
    btn_square_->setTitleColor(Color3B(26, 26, 26));
    btn_hot_->setTitleColor(Color3B(26, 26, 26));
    btn_promised_->setTitleColor(Color3B(26, 26, 26));
    btn_mine_->setTitleColor(Color3B(255, 255, 255));

    current_select_idx_ = 4;
}
void GroupFriend::click_btn_search(Ref* sender)
{
    if(btn_search_callback_) {
        btn_search_callback_();
    }
}
void GroupFriend::click_btn_send(Ref* sender)
{
    tf_comment_->getText();
    processor.mutable_commentinfo()->set_content(tf_comment_->getText());
    
    /*//发送评论
    GDM->get_user_data()->get_id();
    GDM->get_user_data()->get_nickname();
    GDM->get_user_data()->get_user_name();
    processor.mutable_commentinfo()->mutable_userinfo()->set_nickname(GDM->get_user_data()->get_nickname());
    processor.mutable_commentinfo()->mutable_userinfo()->set_picname(GDM->get_user_data()->get_head_pic_name());
    processor.mutable_commentinfo()->mutable_userinfo()->set_sex(GDM->get_user_data()->get_sex());
    processor.mutable_commentinfo()->set_type(msg::CommentType::COMMENT);
    GDM->send_publish_comment(processor);
    
    tf_comment_->setText("");*/
}

void GroupFriend::touch_image(Ref* sender, Touch* touch, ui::Widget::TouchEventType event)
{
    switch (event) {
        case Widget::TouchEventType::BEGAN:
            move_ = false;
            began_location_ = touch->getLocation();
            break;
        case Widget::TouchEventType::MOVED:
            move_ = true;
            break;
        case Widget::TouchEventType::ENDED:{
            Point p = touch->getLocation();
            float move = p.x - began_location_.x;
            if(move > 400) {
                GroupFriend::move_to_left();
            }
            else if(move < -400) {
                GroupFriend::move_to_right();
            }
            break;
        }
        case Widget::TouchEventType::CANCELED:
            
            break;
            
        default:
            break;
    }
}
void GroupFriend::move_to_left()
{
    current_select_idx_--;
    if(current_select_idx_ <= 0 || current_select_idx_ >= 5) {
        current_select_idx_++;
        return;
    }
    if(current_select_idx_ == 1){
        click_btn_square(this);
    }else if(current_select_idx_ == 2){
        click_btn_hot(this);
    }else if(current_select_idx_ == 3){
        click_btn_promised(this);
    }else if(current_select_idx_ == 4){
        click_btn_mine(this);
    }
}

void GroupFriend::move_to_right()
{
    current_select_idx_++;
    if(current_select_idx_ <= 0 || current_select_idx_ >= 5) {
        current_select_idx_--;
        return;
    }
    if(current_select_idx_ == 1){
        click_btn_square(this);
    }else if(current_select_idx_ == 2){
        click_btn_hot(this);
    }else if(current_select_idx_ == 3){
        click_btn_promised(this);
    }else if(current_select_idx_ == 4){
        click_btn_mine(this);
    }
}
#pragma mark touch
bool GroupFriend::touchBegan(Touch* touch, Event* event)
{
    tableview_touch_location_ = touch->getLocation();
    return true;
}
void GroupFriend::touchMoved(Touch* touch, Event* event)
{
    Point p = touch->getLocation();
}
void GroupFriend::touchEnded(Touch* touch, Event* event)
{
    Point p = touch->getLocation();
    if(fabsf(p.y - tableview_touch_location_.y) > 200) {
        
    }
}
void GroupFriend::touchCancel(Touch* touch, Event* event)
{
    
}

TableViewCell* GroupFriend::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto data = moment_data.mutable_moment((int)idx);
    if (data != nullptr) {
        int pic_number = data->mutable_momentinfo()->mutable_normalmomentinfo()->picname().size();
        SquareCell * cell = dynamic_cast<SquareCell*>(table->dequeueCell());
        if (!cell)
        {
            cell = SquareCell::create();
        }
        cell->update_property(data, pic_number);
        return cell;
    }
}

#pragma mark event call
void GroupFriend::update_table_view(const msg::Processor_201_GetMoment_DOWN& processor)
{
    moment_data.CopyFrom(processor);
    counter = 0;
    tableview_fri->reloadData();
}

void GroupFriend::handle_delete_updata(cmd_data_pointer data)
{
    GDM->send_get_moment(FRIEND_CIRCLE_ID,0, dis_type_selected);
    tableview_fri->reloadData();
}

void GroupFriend::handle_request_comment(cmd_data_pointer data)
{
    LayerComment *event = LayerComment::create();
    this->getParent()->addChild(event, 0, GroupFriendCommentTag);
    
    // 发给layercomment，让他去取comments
    events::global_dispatch.raise_event(EVENT_REQUEST_COMMENT_COM, data);
}

void GroupFriend::handle_todo_comment(cmd_data_pointer data)
{
    auto recv = std::dynamic_pointer_cast<cmd_data_uint32>(data);
    if (recv) {
        uint32_t momment_id = recv->get_value();
        processor.mutable_commentinfo()->set_momentid(momment_id);
        processor.mutable_commentinfo()->set_type(msg::COMMENT);
    }
    
    tf_comment_->touchDownAction(tf_comment_, cocos2d::ui::Widget::TouchEventType::ENDED);
    
}

#pragma mark tableview datasource --
ssize_t GroupFriend::numberOfCellsInTableView(TableView *table)
{
    ssize_t size_t = moment_data.moment().size();
    return size_t;
}

Size GroupFriend::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    auto data = moment_data.mutable_moment((int)idx);
    auto str = data->mutable_momentinfo()->mutable_normalmomentinfo()->wordcontent();
    
    int pic_number = data->mutable_momentinfo()->mutable_normalmomentinfo()->picname().size();
   
    
    std::string contentstr = std::string(str);
    
    // 为每个weblink添加一个图标， 供点击。
    std::vector<std::string> strings = tools::split(str, " ");
    for (std::string substr : strings) {
        if (substr.find("http://")==0 || substr.find("HTTP://")== 0 || substr.find("Http://") ==0 ||
            substr.find("https://")==0 || substr.find("HTTPS://")== 0 || substr.find("Https://") ==0 ||
            substr.find("www")==0 || substr.find("WWW")== 0 ){
            pic_number ++; // we add a weblink picture to it .
            long index = contentstr.find(substr);
            contentstr.replace(index, std::string("[网页链接]").length(), "[网页链接]");
        }
    }
    
    int lines = pic_number / 3;
    if (pic_number % 3 > 0)
    {
        lines ++;
    }
    
    auto text_publish_content = TextField::create();
    
    text_publish_content->setString(contentstr.c_str());
    text_publish_content->setFontSize(30);
    text_publish_content->setFontName("Thonburi");
    auto render_size = text_publish_content->getAutoRenderSize();
    if (render_size.height == 0 ){
        int char_count = text_publish_content->getCharCount();
        auto line_count = ceilf((float)char_count/18.5);
        render_size.height = 40 * line_count;
    }
    auto size = Size(580, 110);
    
    
    auto orisize = size;
    if(render_size.width > size.width) {
        int line_count = ceilf(render_size.width / size.width);
        size.height = render_size.height * line_count;
    }
    else {
        size.height = render_size.height;

    }
    text_publish_content->setContentSize(size);   // ronaldz. here to adjust to the content size.
    


    
    int height_change = size.height-orisize.height;
    //height_change = 0 ;
    
    if (data->mutable_momentinfo()->mutable_normalmomentinfo()->has_handid()){
        return Size(DESIGN_WIDTH, 252 + height_change + 1*(2+580/3));
    }else{
        return Size(DESIGN_WIDTH, 252 + height_change + lines*(2+580/3));  // this is the height of the picture.
    }

    
}

#pragma mark tableview delegate --
void GroupFriend::tableCellTouched(TableView* table, TableViewCell* cell)
{
    cocos2d::log("touched cell");
   
    log("touched:%zd",cell->getIdx());
    
    /*cell->getIdx();
    uint32_t i = GDM->mutable_Moment_->mutable_moment()->Mutable(cell->getIdx())->momentid();
    set_momment_id(i);*/
}

void GroupFriend::update_data(cmd_data_pointer)
{
    
}

#pragma mark EditBox delegate
void GroupFriend::editBoxEditingDidBegin(EditBox* editBox)
{}
void GroupFriend::editBoxEditingDidEnd(EditBox* editBox)
{}
void GroupFriend::editBoxTextChanged(EditBox* editBox, const std::string& text)
{}
void GroupFriend::editBoxReturn(EditBox* editBox)
{}
void GroupFriend::keyBoardWillShow(const float duration, const float distance)
{
    float dis = distance;
    if(SceneManager::get_landscape()) {
        dis *= 2;
        dis += 20;
    }
    
    
    log("dis:%f", dis);
    
    auto& items = listview_->getItems();
    if(!items.empty()) {
        auto back = items.back();
        Point p = back->convertToWorldSpace(Point::ZERO);
        if(p.y < dis + tf_comment_->getContentSize().height) {
            listview_->setBackGroundColor(Color3B::ORANGE);
            Point p1 = listview_->getPosition() + Point(0, dis - p.y + tf_comment_->getContentSize().height + back->getContentSize().height/2);
            listview_->stopAllActions();
            listview_->runAction(MoveTo::create(duration, p1));
        }
    }
    
    
    
    for (auto& iter : node_about_textfileds_) {
        if(iter.first != listview_) {
            Point p = iter.first->getPosition() + Point(0, dis);
            iter.first->stopAllActions();cocos2d::log("%f",dis);
            iter.first->runAction(MoveTo::create(duration, p));
        }
    }
    
}

void GroupFriend::keyBoardWillHide(const float duration, const float distance)
{
    cocos2d::log("GroupLayerChatOld::keyBoardWillHide");
    float dis = distance;
    if (dis < 0.0f) dis = 0.0f;
    
    Point p = Point(0, 0);
    
    for (auto& iter : node_about_textfileds_) {
        iter.first->stopAllActions();
        iter.first->runAction(MoveTo::create(duration, iter.second));
    }
}
