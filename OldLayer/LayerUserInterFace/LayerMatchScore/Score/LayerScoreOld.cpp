//
//  LayerScoreOld.cpp
//  pokerclient
//
//  Created by ll on 15/10/27.
//
//

#include "LayerScoreOld.h"

#include "GameDataManager.h"
#include "cmd_def.h"
#include "utils.h"

#include "NodeScoreCellOld.h"

bool LayerScoreOld::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerScoreOld.csb");
    this->addChild(root_);
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
    btn_share_=dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_share"));
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    text_room_name_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_time_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_time"));
    text_time_->setString("");
    
    text_mark_1_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mark_1"));
    text_mark_2_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mark_2"));
    text_mark_3_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mark_3"));
    
    text_mark_name_1_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mark_name_1"));
    text_mark_name_1_->enableOutline(Color4B(0, 0, 0, 255), 2);
    text_mark_name_1_->setString(tools::local_string("mvp", "MVP"));
    text_mark_name_2_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mark_name_2"));
    text_mark_name_2_->enableOutline(Color4B(0, 0, 0, 255), 2);
    text_mark_name_1_->setString(tools::local_string("bigfish", "大鱼"));
    text_mark_name_3_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mark_name_3"));
    text_mark_name_3_->enableOutline(Color4B(0, 0, 0, 255), 2);
    text_mark_name_1_->setString(tools::local_string("highestbuyin", "土豪"));
    
    
    text_mark_num_1_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mark_num_1"));
    text_mark_num_1_->setString("");
    text_mark_num_2_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mark_num_2"));
    text_mark_num_1_->setString("");
    text_mark_num_3_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mark_num_3"));
    text_mark_num_1_->setString("");
    
    image_avatar_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "Image_avatar_1"));
    image_avatar_2_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "Image_avatar_2"));
    image_avatar_3_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "Image_avatar_3"));

    image_avatar_frame_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "Image_avatar_frame1"));
    image_avatar_frame_2_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "Image_avatar_frame2"));
    image_avatar_frame_3_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "Image_avatar_frame3"));
    
    text_nick_name_1_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_nick_name_1"));
    text_nick_name_2_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_nick_name_2"));
    text_nick_name_3_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_nick_name_3"));
    
    text_nick_name_1_->setString("");
    text_nick_name_2_->setString("");
    text_nick_name_3_->setString("");
    
    award_usernames_.push_back(text_nick_name_1_);
    award_usernames_.push_back(text_nick_name_2_);
    award_usernames_.push_back(text_nick_name_3_);
    
    award_avators_.push_back(image_avatar_1_);
    award_avators_.push_back(image_avatar_2_);
    award_avators_.push_back(image_avatar_3_);
    
    tableview_ = TableView::create(this, listview->getContentSize());
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    root_->addChild(tableview_);
    
    listview->removeFromParent();
    
    btn_back_->addClickEventListener(std::bind(&LayerScoreOld::click_btn_back, this, std::placeholders::_1));
    btn_share_->setVisible(false);
    return true;
}

void LayerScoreOld::handle_get_score(cmd_data_pointer data)
{
    match_objects_.clear();
    
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (rec){
       
        msg::Processor_420_PokerGetScore_DOWN processor;
        processor.ParseFromArray(rec->body, rec->bodysize);
        
        if (processor.playerscores().score().size() == 0)
        {
            text_time_->setString("房间未曾有人进入 无战绩");
        }else
        {
            std::string submit_time = processor.time();
            if (submit_time == "")
            {
                text_time_->setString("游戏时长：无");
            }else
            {
                text_time_->setString(tools::local_string("game_time", "游戏时长：") + submit_time);
            }
        }
        
        if (processor.roomtype() == msg::RoomType::NLHOLDEM ||
            processor.roomtype() == msg::RoomType::PLO ||
            processor.roomtype() == msg::RoomType::PLOHILO)
        {
            text_mark_2_->setVisible(false);
            text_mark_num_2_->setVisible(false);
            
            int32_t totalBuyIn=0;
            for (auto& it: processor.playerscores().score()) {
                ::std::string nickname = it.userdetailinfo().nickname();
                ::std::string head_pic_name = it.userdetailinfo().picname();
                int32_t buy_amount = it.totalbuyin();
                int32_t win_amount = it.totalwin();
                uint32_t user_id=it.userdetailinfo().userid();
                totalBuyIn+=buy_amount;
                std::shared_ptr<MATCH_OBJECT> obj(new MATCH_OBJECT());
                obj->is_group_user_rank = false;
                obj->nickname = nickname;
                obj->head_pic_name = head_pic_name;
                obj->buy_amount = buy_amount;
                obj->win_amount = win_amount;
                obj->user_id = user_id;
                obj->room_type = processor.roomtype();
                obj->hp_lost = it.hplost();
                match_objects_.push_back(obj);
            }
            
            text_room_name_->setString(processor.roomname());
            
            text_mark_num_3_->setString(tools::to_string(totalBuyIn));
            text_mark_num_1_->setString(tools::to_string(processor.handcount()));
            
            if(match_objects_.size() !=0 ){
                
                std::shared_ptr<AWARD_OBJECT> awardobj1(new AWARD_OBJECT());
                awardobj1->matchobject = *match_objects_[0];
                awardobj1->awardname="MVP";
                
                std::shared_ptr<AWARD_OBJECT> awardobj2(new AWARD_OBJECT());
                awardobj2->matchobject = *match_objects_[0];
                awardobj2->awardname="大鱼";
                
                std::shared_ptr<AWARD_OBJECT> awardobj3(new AWARD_OBJECT());
                awardobj3->matchobject = *match_objects_[0];
                awardobj3->awardname="土豪";
                for (int i = 0; i < match_objects_.size(); i++)
                {
                    if (match_objects_[i]->win_amount > awardobj1->matchobject.win_amount)
                        awardobj1->matchobject = *match_objects_[i];
                    if (match_objects_[i]->win_amount < awardobj2->matchobject.win_amount)
                        awardobj2->matchobject = *match_objects_[i];
                    if (match_objects_[i]->buy_amount > awardobj3->matchobject.buy_amount)
                        awardobj3->matchobject = *match_objects_[i];
                }
                award_objects_.push_back(awardobj1);
                award_objects_.push_back(awardobj2);
                award_objects_.push_back(awardobj3);
                shader_sprites_.push_back(nullptr);
                shader_sprites_.push_back(nullptr);
                shader_sprites_.push_back(nullptr);
                
                auto sortfuncitonWin = [](std::shared_ptr<MATCH_OBJECT> a, std::shared_ptr<MATCH_OBJECT> b){return a->win_amount  > b->win_amount;};
                std::sort(match_objects_.begin(), match_objects_.end(), sortfuncitonWin);// 按照赢筹码排序
                
                update_ui();
                
            }
        }else if (processor.roomtype() == msg::RoomType::OFC3 ||
                  processor.roomtype() == msg::RoomType::OFC4)
        {
            for (auto& it: processor.playerscores().score()) {
                ::std::string nickname = it.userdetailinfo().nickname();
                ::std::string head_pic_name = it.userdetailinfo().picname();
                int32_t buy_amount = it.totalbuyin();
                int32_t win_amount = it.totalwin();
                uint32_t user_id=it.userdetailinfo().userid();
                std::shared_ptr<MATCH_OBJECT> obj(new MATCH_OBJECT());
                obj->is_group_user_rank = false;
                obj->nickname = nickname;
                obj->head_pic_name = head_pic_name;
                obj->buy_amount = buy_amount;
                obj->win_amount = win_amount;
                obj->user_id = user_id;
                obj->room_type = processor.roomtype();
                obj->hp_lost = it.hplost();
                match_objects_.push_back(obj);
            }
            auto sortfuncitonWin = [](std::shared_ptr<MATCH_OBJECT> a, std::shared_ptr<MATCH_OBJECT> b){return a->win_amount  > b->win_amount;};
            std::sort(match_objects_.begin(), match_objects_.end(), sortfuncitonWin);// 按照赢筹码排序
        
            text_room_name_->setString(processor.roomname());
            
            text_mark_1_->setVisible(false);
            text_mark_2_->setVisible(false);
            text_mark_3_->setVisible(false);
            
            text_mark_num_1_->setVisible(false);
            text_mark_num_2_->setVisible(false);
            text_mark_num_3_->setVisible(false);
            
//            text_mark_name_1_->setString("First");
//            text_mark_name_2_->setString("Second");
//            text_mark_name_3_->setString("Third");
            text_mark_name_1_->setVisible(false);
            text_mark_name_2_->setVisible(false);
            text_mark_name_3_->setVisible(false);
            
            image_avatar_1_->setVisible(false);
            image_avatar_2_->setVisible(false);
            image_avatar_3_->setVisible(false);
            
            image_avatar_frame_1_->setVisible(false);
            image_avatar_frame_2_->setVisible(false);
            image_avatar_frame_3_->setVisible(false);
            
            text_nick_name_1_->setVisible(false);
            text_nick_name_2_->setVisible(false);
            text_nick_name_3_->setVisible(false);
            
            //not display by lvzhangle
//            if (match_objects_.size() > 0)
//            {
//                if (match_objects_.size() < 3)
//                {
//                    for (int i = 0; i < match_objects_.size(); i++)
//                    {
//                        std::shared_ptr<AWARD_OBJECT> awardobj1(new AWARD_OBJECT());
//                        awardobj1->awardname = tools::to_string(i+1);
//                        awardobj1->matchobject = *match_objects_[i];
//                        award_objects_.push_back(awardobj1);
//                        shader_sprites_.push_back(nullptr);
//                    }
//                }else
//                {
//                    std::shared_ptr<AWARD_OBJECT> awardobj1(new AWARD_OBJECT());
//                    awardobj1->awardname="1";
//                    
//                    std::shared_ptr<AWARD_OBJECT> awardobj2(new AWARD_OBJECT());
//                    awardobj2->awardname="2";
//                    
//                    std::shared_ptr<AWARD_OBJECT> awardobj3(new AWARD_OBJECT());
//                    awardobj3->awardname="3";
//                    
//                    award_objects_.push_back(awardobj1);
//                    award_objects_.push_back(awardobj2);
//                    award_objects_.push_back(awardobj3);
//                    shader_sprites_.push_back(nullptr);
//                    shader_sprites_.push_back(nullptr);
//                    shader_sprites_.push_back(nullptr);
//                    
//                    for (int i = 0; i < award_objects_.size(); i++)
//                    {
//                        award_objects_[i]->matchobject = *match_objects_[i];
//                    }
//                }
//            }
//            update_ui();
        }
        
        tableview_->reloadData();
    }
}

#pragma mark tableview datasource --
TableViewCell* LayerScoreOld::tableCellAtIndex(TableView *table, ssize_t idx)
{
    NodeScoreCellOld* cell = NodeScoreCellOld::create();
//    match_objects_[idx]->rank = idx+1;
//    cell->update_data(match_objects_[idx]);
   
    return cell;
}
ssize_t LayerScoreOld::numberOfCellsInTableView(TableView *table)
{
   return match_objects_.size();
}

void LayerScoreOld::click_btn_back(Ref *sender)
{
    this->removeFromParent();
}
void LayerScoreOld::update_ui()
{
    for (int i=0 ; i < award_objects_.size(); i++) {
        award_usernames_[i]->setString(award_objects_[i]->matchobject.nickname);
     if(!(award_objects_[i]->matchobject.head_pic_name.empty()))
     {
        if(GDM->is_file_exist(award_objects_[i]->matchobject.head_pic_name)) {
            award_avators_[i]->setOpacity(0);
            //award_avators_[i]->loadTexture("login_res/score_res/image_avator.png");
            
            if (shader_sprites_[i] && shader_sprites_[i]->getParent()) {
                shader_sprites_[i]->removeFromParent();
            }
            shader_sprites_[i] = ShaderSprite::create(award_objects_[i]->matchobject.head_pic_name, "login_res/friend_res/node_friend_icon_mask.png");
            shader_sprites_[i]->setScale(award_avators_[i]->getContentSize().width * 0.9/ shader_sprites_[i]->getContentSize().width,
                                   award_avators_[i]->getContentSize().height * 0.9 / shader_sprites_[i]->getContentSize().height);
            shader_sprites_[i]->setPosition(award_avators_[i]->getPosition());
            root_->addChild(shader_sprites_[i], award_avators_[i]->getZOrder());
            
        }
        else {
            register_download_events();
            
            FM.download_file(award_objects_[i]->matchobject.head_pic_name);
        }
     }
    }
}
void LayerScoreOld::register_download_events()
{
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, LayerScoreOld::handle_download_events));
}
void LayerScoreOld::handle_download_events(cmd_data_pointer)
{
    update_ui();
}