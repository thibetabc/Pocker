//
//  LayerGroupTrackRecord.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/11.
//
//

#include "LayerScore.h"
#include "NET_CMD.h"
#include "ProcessorManager.h"
#include "NodeScoreListCell.h"
#include "utils.h"
#include "cmd_def.h"
#include "GameDataManager.h"
#include "FileManager.h"
#include "GameTools.h"
#define LAYER_SCORE_PHOTO_WIDTH 100
#define LAYER_SCORE_PHOTO_HEIGHT 100
LayerScore* LayerScore::create(int roomid)
{
    LayerScore* score = new LayerScore;
    if(score->init(roomid))
    {
        score->autorelease();
        return score;
    }
    return nullptr;
}
bool LayerScore::init(int roomid)
{
    if (!BaseLayer::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerScoreList.csb");
    this->addChild(root_);
    
    mvp_sprite_ = nullptr;
    bigfish_sprite_ = nullptr;
    maxbuy_sprite_ = nullptr;
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString("");
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerScore::click_btn_back, this,std::placeholders::_1));
    
    image_middle_bg_first_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_middle_bg_first"));
    image_middle_bg_first_->setVisible(false);
    
    text_game_time_show_first_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_game_time_show_first"));
    text_game_time_show_first_->setVisible(false);
    
    text_game_time_show_second_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_game_time_show_second"));
    text_game_time_show_second_->setVisible(false);
    
    text_whole_num_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_whole_num"));
    text_whole_num_->setString(tools::local_string("total_number","总数"));
    text_whole_number_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_whole_number"));
    text_whole_buying_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_whole_buying"));
    text_whole_buying_->setString(tools::local_string("total_buy_chip","总买入"));
    text_whole_buying_num_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_whole_buying_num"));
    
    text_mvp_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mvp"));
    text_mvp_->setString("MVP");
    text_big_fish_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_big_fish"));
    text_big_fish_->setString(tools::local_string("bigfish","大鱼"));
    text_max_buy_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_max_buy"));
    text_max_buy_->setString(tools::local_string("highestbuyin","土豪"));
    
    text_mvp_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_mvp_name"));
    text_big_fish_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_big_fish_name"));
    text_max_buy_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_max_buy_name"));
    
    node_mvp_photo_ = dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "node_mvp_photo"));
    node_big_fish_photo_ = dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "node_big_fish_photo"));
    node_max_buy_photo_ = dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "node_max_buy_photo"));
    
    
    
    liastview_room_track_record_first_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "liastview_room_track_record_first"));
    tableView_first_ = TableView::create(this, liastview_room_track_record_first_->getContentSize());
    tableView_first_->setAnchorPoint(liastview_room_track_record_first_->getAnchorPoint());
    tableView_first_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableView_first_->setPosition(liastview_room_track_record_first_->getPosition());
    tableView_first_->setDelegate(this);
    
    root_->addChild(tableView_first_);
    liastview_room_track_record_first_->removeFromParent();

    
    liastview_room_track_record_second_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "liastview_room_track_record_second"));
    tableView_second_ = TableView::create(this, liastview_room_track_record_second_->getContentSize());
    tableView_second_->setAnchorPoint(liastview_room_track_record_second_->getAnchorPoint());
    tableView_second_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableView_second_->setPosition(liastview_room_track_record_second_->getPosition());
    tableView_second_->setDelegate(this);
    
    root_->addChild(tableView_second_);
    liastview_room_track_record_second_->removeFromParent();

    auto processor = PM->PokerGetScore_UP(roomid);
    send_data(processor, REQ_GET_SCORE);
    return true;
}



void LayerScore::isFirstTableViewVisible(bool visible)
{
    if(visible)
    {
        tableView_first_->setVisible(true);
        image_middle_bg_first_->setVisible(true);
        text_game_time_show_first_->setVisible(true);
        text_game_time_show_second_->setVisible(false);
        tableView_second_->removeFromParent();
        tableView_second_ = nullptr;
    }
    else
    {
        tableView_second_->setVisible(true);
        text_game_time_show_first_->setVisible(false);
        text_game_time_show_second_->setVisible(true);
        image_middle_bg_first_->setVisible(false);
        tableView_first_->removeFromParent();
        tableView_first_ = nullptr;
    }
}

void LayerScore::register_events()
{
    dispatch_.register_event(REQ_GET_SCORE,
                             BIND_FUNC(this, LayerScore::handle_get_score_list_data));
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, LayerScore::handle_downloadPhoto));
}

void LayerScore::handle_get_score_list_data(cmd_data_pointer data){
    std::shared_ptr<net_data_recv_package> rec=dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor = PM->PokerGetScore_DOWN(rec);
    
    std::string time = processor.time();
    std::string roomName = processor.roomname();
    text_title_->setString(roomName);
    msg::RoomType roomtype = processor.roomtype();
    room_type_ = roomtype;
    int handcount = processor.handcount();
    bool istournament = processor.istournament();
    whole_buy_in_=0;
    score_lsit_data_.clear();
    mvp_info_.Clear();
    big_fish_info_.Clear();
    maxBuy_info_.Clear();
    if (processor.playerscores().score_size()>0)
    {
        mvp_info_.CopyFrom(processor.playerscores().score(0));
        big_fish_info_.CopyFrom(processor.playerscores().score(0));
        maxBuy_info_.CopyFrom(processor.playerscores().score(0));
        ShowTipMessage(false);
    }
    else
    {
        ShowTipMessage(true,tools::local_string("no_players","房间未曾有人进入\n无战绩"));
    }
    for (int i = 0; i<processor.playerscores().score_size(); i++)
    {
        if (mvp_info_.totalwin()<processor.playerscores().score(i).totalwin())
        {
            mvp_info_.Clear();
            mvp_info_.CopyFrom(processor.playerscores().score(i));
        }
        if (maxBuy_info_.totalbuyin()<processor.playerscores().score(i).totalbuyin())
        {
            maxBuy_info_.Clear();
            maxBuy_info_.CopyFrom(processor.playerscores().score(i));
        }
        if (big_fish_info_.totalwin()>processor.playerscores().score(i).totalwin())
        {
            big_fish_info_.Clear();
            big_fish_info_.CopyFrom(processor.playerscores().score(i));
        }
        whole_buy_in_=whole_buy_in_+processor.playerscores().score(i).totalbuyin();
        msg::PlayerScore playerscore;
        playerscore.CopyFrom(processor.playerscores().score(i));
        score_lsit_data_.push_back(playerscore);
    }
    auto sortfuncitonWin = [](msg::PlayerScore a,msg::PlayerScore b){return a.totalwin() > b.totalwin();};
    std::sort(score_lsit_data_.begin(), score_lsit_data_.end(), sortfuncitonWin);
    if (istournament)
    {
        this->isFirstTableViewVisible(false);
        text_game_time_show_second_->setString(time);
    }
    else
    {
        if(roomtype==msg::RoomType::NLHOLDEM || roomtype==msg::RoomType::PLO || roomtype==msg::RoomType::PLOHILO )
        {
            this->isFirstTableViewVisible(true);
            text_game_time_show_first_->setString(time);
            text_whole_number_->setString(tools::to_string(handcount));
            text_whole_buying_num_->setString(tools::to_string(whole_buy_in_));
            text_mvp_name_->setString(mvp_info_.userdetailinfo().nickname());
            text_big_fish_name_->setString(big_fish_info_.userdetailinfo().nickname());
            text_max_buy_name_->setString(maxBuy_info_.userdetailinfo().nickname());
            this->addFirstPartPlayerPhoto(mvp_info_.userdetailinfo().picname(), mvp_sprite_, node_mvp_photo_);
            this->addFirstPartPlayerPhoto(big_fish_info_.userdetailinfo().picname(), bigfish_sprite_, node_big_fish_photo_);
            this->addFirstPartPlayerPhoto(maxBuy_info_.userdetailinfo().picname(), maxbuy_sprite_, node_max_buy_photo_);
        }
        else
        {
            this->isFirstTableViewVisible(false);
            text_game_time_show_second_->setString(time);
        }
    }
    if(tableView_first_)
    {
        tableView_first_->reloadData();
    }
    if(tableView_second_)
    {
        tableView_second_->reloadData();
    }
    
}

void LayerScore::addFirstPartPlayerPhoto(std::string picname, cocos2d::ShaderSprite *sprite,Node* node)
{
    
    if(!GDM->is_file_exist(picname) && picname != "")
    {
        FM.download_file(picname);
    }
    else
    {
        if(picname == "" || !GameTools::isValidPicture(picname))
        {
            picname = "Default/image_defualt_user_icon_large.png";
        }
        if (sprite && sprite->getParent()) {
            sprite->removeFromParent();
        }
        if (picname != "")
        {
            sprite = ShaderSprite::create(picname, "Default/head_mask_circle_second.png");
            node->addChild(sprite);
            auto width = sprite->getContentSize().width;
            auto height = sprite->getContentSize().height;
            float scaleX = 1;
            float scaleY = 1;
            if (width>LAYER_SCORE_PHOTO_WIDTH)
            {
                scaleX = LAYER_SCORE_PHOTO_WIDTH/width;
            }
            
            if (height>LAYER_SCORE_PHOTO_HEIGHT)
            {
                scaleY = LAYER_SCORE_PHOTO_HEIGHT/height;
            }
            sprite->setScale(scaleX,scaleY);
        }
        
    }
}

void LayerScore::handle_downloadPhoto(cmd_data_pointer data)
{
    auto s = std::dynamic_pointer_cast<cmd_data_string>(data);
    if (s->get_value() == mvp_info_.userdetailinfo().picname())
    {
        this->addFirstPartPlayerPhoto(mvp_info_.userdetailinfo().picname(), mvp_sprite_, node_mvp_photo_);
    }
    else if (s->get_value() == big_fish_info_.userdetailinfo().picname())
    {
        this->addFirstPartPlayerPhoto(big_fish_info_.userdetailinfo().picname(), bigfish_sprite_, node_big_fish_photo_);
    }
    else if (s->get_value() == maxBuy_info_.userdetailinfo().picname())
    {
        this->addFirstPartPlayerPhoto(maxBuy_info_.userdetailinfo().picname(), maxbuy_sprite_, node_max_buy_photo_);
    }
    else
    {
        return;
    }
}

void LayerScore::click_btn_back(cocos2d::Ref *sender)
{
    this->removeFromParent();
}

#pragma mark datasource
TableViewCell* LayerScore::tableCellAtIndex(TableView *table, ssize_t idx)
{
    bool is_show_buy_num = true;
    if ( room_type_ == msg::RoomType::OFC3 || room_type_ == msg::RoomType::OFC4)
    {
        is_show_buy_num = false;
    }
    NodeScoreListCell* cell = dynamic_cast<NodeScoreListCell*>(table->dequeueCell());
    if(!cell) {
        cell = NodeScoreListCell::create(false,is_show_buy_num);
    }
    cell->update_data(score_lsit_data_.at(idx));
    if (idx==1)
    {
        cell->set_silver_visible();
    }
    if (idx==0)
    {
        cell->set_gold_visible();
    }
    return cell;
}
ssize_t LayerScore::numberOfCellsInTableView(TableView *table)
{
    return score_lsit_data_.size();
}