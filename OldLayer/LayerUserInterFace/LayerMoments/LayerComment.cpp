//
//  LayerComment.cpp
//  pokerclient
//
//  Created by admin on 15-9-10.
//
//

#include "LayerComment.h"
#include "GameTools.h"
#include "CommentCell.h"
#include "SceneManager.h"
#include "GameDataManager.h"
#include "cmd_def.h"

bool LayerComment::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    counter = 0;
    auto root = CSLoader::createNode("LayerComment.csb");
    this->addChild(root);
    
//    image_bg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_1"));
//    image_bg->setZOrder(4);
    btn_cancel =  dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_cancel->addClickEventListener(std::bind(&LayerComment::click_btn_cancel, this, std::placeholders::_1));
    btn_send = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_send"));
    btn_send->addClickEventListener(std::bind(&LayerComment::click_btn_send, this, std::placeholders::_1));
    btn_send->setZOrder(5);
    input_bg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_1"));
    input_bg->setZOrder(3);
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "listview"));
    listview_ = listview;
    
//    listview_->setZOrder(-2);
    
    tableview_ = TableView::create(this, listview_->getContentSize());
    tableview_->setAnchorPoint(listview_->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);


    
    root->addChild(tableview_);
//    listview->removeFromParent();
    
    auto tf_comment = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_chat"));
    tf_comment_ = GameTools::createEditBoxFromTextField(this, tf_comment);
    tf_comment_->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
    
    tf_comment_->setDelegate(this);
    tf_comment_->setZOrder(6);
    
    if(SceneManager::get_landscape()) {
        tf_comment_->setContentSize(tf_comment->getContentSize() * 0.5);
        tf_comment_->setPosition(tf_comment_->getPosition() + Point(-100, 20));
        tf_comment_->setFontSize(tf_comment->getFontSize() / 2);
        tf_comment_->setPlaceHolder("");
    }
/*
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    tf_comment_->setKeyBoardWillShowCallback(std::bind(&LayerComment::keyBoardWillShow, this, std::placeholders::_1, std::placeholders::_2));
    tf_comment_->setKeyBoardWillHideCallback(std::bind(&LayerComment::keyBoardWillHide, this, std::placeholders::_1, std::placeholders::_2));
#endif
 */
    
    node_about_textfileds_.insert(std::make_pair(btn_send, btn_send->getPosition()));
    node_about_textfileds_.insert(std::make_pair(tf_comment_, tf_comment_->getPosition()));
   // node_about_textfileds_.insert(std::make_pair(image_bg, image_bg->getPosition()));
    node_about_textfileds_.insert(std::make_pair(listview_, listview_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(input_bg, input_bg->getPosition()));
    
    
    auto& size = Director::getInstance()->getWinSize();
    auto layout = ui::Layout::create();
    layout->setContentSize(size);
    layout->setTouchEnabled(true);
    layout->setAnchorPoint(Point::ZERO);
    layout->setSwallowTouches(false);
    layout->addClickEventListener([](Ref*){
        GameTools::editBoxLoseInputFocus();
    });
    this->addChild(layout, 10);
    
    dispatch_.register_event(EVENT_REQUEST_COMMENT_COM, BIND_FUNC(this, LayerComment::handle_request_comment));
    
    dispatch_.register_event(EVENT_SUCCESS_COMMENT, BIND_FUNC(this, LayerComment::update_data));
    dispatch_.register_event(EVENT_DOWLOAD_FILE, BIND_FUNC(this, LayerComment::update_data));
    
    return true;
    
}

void LayerComment::update_table_view(const msg::Processor_204_GetComment_DOWN& processor)
{
    commentdata.CopyFrom(processor);
    tableview_->reloadData();
}

void LayerComment::update_data(cmd_data_pointer data)
{
    cocos2d::log("momment_id:%d",momment_id);
    GDM->send_get_comment(momment_id);
}

void LayerComment::handle_request_comment(cmd_data_pointer data)
{
    auto recv = std::dynamic_pointer_cast<cmd_data_uint32>(data);
    if (recv) {
        momment_id = recv->get_value();
        processor.mutable_commentinfo()->set_momentid(momment_id);
        GDM->send_get_comment(momment_id);
    }
}

void LayerComment::click_btn_send(Ref* sender)
{
    tf_comment_->getText();
    processor.mutable_commentinfo()->set_content(tf_comment_->getText());
    
    //发送评论
    processor.mutable_commentinfo()->mutable_userinfo()->set_userid(GDM->get_user_id());
    processor.mutable_commentinfo()->set_type(msg::CommentType::COMMENT);
    GDM->send_publish_comment(processor);
    
    tf_comment_->setText("");
}
void LayerComment::click_btn_cancel(Ref* sender)
{
    removeFromParent();
    if(btn_cancel_callback_ != nullptr) {
        btn_cancel_callback_();
    }
}

#pragma mark tableview datasource --
Size LayerComment::cellSizeForTable(TableView *table) {
    //根据评论中的content中size，来设置tfcontent的contentsize；
    
    counter++;
    return Size(DESIGN_WIDTH, 202);
};
TableViewCell* LayerComment::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto data = commentdata.mutable_commentinfo()->Mutable((int)idx);
    comment_data_ = nullptr;
    comment_data_ = data;
    
    if (comment_data_ != nullptr) {
        CommentCell* cell = dynamic_cast<CommentCell*>(table->dequeueCell());
        if(!cell) {
            cell = CommentCell::create();
        }
        auto data = commentdata.mutable_commentinfo((int)idx);
        cell->update_property(data);
        
        return cell;
    }

}



Size LayerComment::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    auto data = commentdata.mutable_commentinfo()->Mutable((int)idx);
    
    auto text_publish_content = TextField::create();
    
    auto str =data->content();
    
    text_publish_content->setString(str.c_str());
    text_publish_content->setFontSize(25);
    text_publish_content->setFontName("Thonburi");
    auto render_size = text_publish_content->getAutoRenderSize();
    if (render_size.height == 0 ){
        render_size.height = 500;
    }
    auto size = Size(580, 80);
    
    
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
    
    return Size(DESIGN_WIDTH, 200 + height_change);
    
    
}
ssize_t LayerComment::numberOfCellsInTableView(TableView *table)
{
    ssize_t size_t = commentdata.mutable_commentinfo()->size();
    
    return size_t;
    
}

#pragma mark tableview delegate --
void LayerComment::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CommentCell* message_cell = dynamic_cast<CommentCell*>(cell);
    auto comment_data = message_cell->get_comment_data();
}

#pragma mark EditBox delegate
void LayerComment::editBoxEditingDidBegin(EditBox* editBox)
{}

void LayerComment::editBoxEditingDidEnd(EditBox* editBox)
{}

void LayerComment::editBoxTextChanged(EditBox* editBox, const std::string& text)
{}

void LayerComment::editBoxReturn(EditBox* editBox)
{}

void LayerComment::keyBoardWillShow(const float duration, const float distance)
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

void LayerComment::keyBoardWillHide(const float duration, const float distance)
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

