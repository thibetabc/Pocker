//
//  RecordDetailLayer.cpp
//  pokerclient
//
//  Created by NaughtyDog on 16/3/10.
//
//

#include "RecordDetailLayer.h"
#include "utils.h"
#include "PokerDataManager.h"
#include "RecordDetailCell.h"
#include "cmd_def.h"
#include "LayerShare.h"
#include "GameDataManager.h"
#include "Gametools.h"

#define ROOT_TAG 10001
bool RecordDetailLayer::init()
{
    if (!PokerChildLayer::init()) {
        return false;
    }
    
    isDetail  =false;
    
    LayerColor *layer = LayerColor::create(Color4B(100,100,100,100));
    this->addChild(layer);
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    this->setTouchEnabled(true);
    this->setSwallowsTouches(true);
    
    auto root = CSLoader::createNode("LayerRecordDetail.csb");

    this->addChild(root);
    root->setTag(ROOT_TAG);
    root_ = root;

    auto text_hand = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_hand"));
    auto text_flop = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_flop"));
    auto text_turn = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_turn"));
    auto text_river = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_river"));
    
    text_hand->setString(tools::local_string("hand_poker","手牌"));
    text_flop->setString(tools::local_string("flop","翻牌"));
    text_turn->setString(tools::local_string("turn","转牌"));
    text_river->setString(tools::local_string("river","河牌"));
    
    
    
    
    auto text_time = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_time"));

    text_blind = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_blind"));
    
    auto btn_replay = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_replay"));

    btn_replay->addClickEventListener([=](Ref*){
        PDM->setIsDetail(false);
        PDM->send_poker_get_hand_history(records.handid(), true);
        this->removeFromParent();
    });
    
    auto btn_detail = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_detail"));
    
    btn_detail->addClickEventListener([=](Ref*){
        isDetail = !isDetail;
        tableview_record_->reloadData();
    });
    auto btn_share = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_share"));
    btn_share->addClickEventListener(std::bind(&RecordDetailLayer::click_btn_share, this, std::placeholders::_1));
    
    
    auto btn_close = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_close"));
    btn_close->setZOrder(1);
    btn_close->addClickEventListener([=](Ref*){
        this->click_btn_close(nullptr);
    });
    
    text_time->setString(tools::local_string("time","时间"));
    text_blind->setString("100/200");
    
    
    auto listview_record = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "ListView_content"));
    tableview_record_ = TableView::create(this, listview_record->getContentSize());
    tableview_record_->setAnchorPoint(listview_record->getAnchorPoint());
    tableview_record_->setContentOffset(Vec2(0, 0));
    tableview_record_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_record_->setPosition(listview_record->getPosition());
    tableview_record_->setDelegate(this);
    root->addChild(tableview_record_);
    
    listview_record->removeFromParent();
    
    auto title_image=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "title"));
    if(!GameTools::isChineseLanguage())
    {
    GameTools::set_pic_change_imageview(title_image, "word_play_record.png");
    GameTools::set_pic_change(btn_replay,"record_btn_replay_normal.png","record_btn_replay_press.png");
    GameTools::set_pic_change(btn_share,"record_btn_share_normal.png","record_btn_share_press.png");
    GameTools::set_pic_change(btn_detail,"record_btn_detail_normal.png","record_btn_detail_press.png");
        
    }

    
    isShow = false;
    
    return true;
}
bool RecordDetailLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    return true;
}

void RecordDetailLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
}

void RecordDetailLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    this->removeFromParent();
}

void RecordDetailLayer::update_data(const msg::Processor_300_PokerGetHandHistory_DOWN& processor)
{
    records = processor;
    int bigblind = records.steps().replaystep(0).propupdate().tableinfo().bigblind();
    int blind = bigblind/2;
    text_blind->setString(tools::to_string(blind) + "/" + tools::to_string(bigblind));
    
    
    int stepSize = records.steps().replaystep_size();
    
    int cardSize = records.steps().replaystep(stepSize - 1).propupdate().tableinfo().holecards_size();
    int secardSize = records.steps().replaystep(stepSize - 1).propupdate().tableinfo().secondholecards_size();
    
    bool isUp = false;
    for (int i = 0; i < cardSize; i++) {
        int index = records.steps().replaystep(stepSize - 1).propupdate().tableinfo().holecards(i);
        
        if (secardSize > 0) {
            int index2 = records.steps().replaystep(stepSize - 1).propupdate().tableinfo().secondholecards(i);
            if (index != index2) {
                isUp = true;
            }
        }
        const std::string image_name = PDM->get_card_image(index);
        Sprite* card = Sprite::create(image_name);
        card->setScale(0.6);
        if(i < 3)
        {
            card->setPosition(339 + card->getContentSize().width * (i * 0.6f),isUp? 910:880);
        }
        else
        {
            card->setPosition(501 + card->getContentSize().width * ((i-3) * 2.02f), isUp? 910:880);
        }
      
        root_->addChild(card, 1);
        
        if (secardSize > 0 && isUp) {
            
            int index = records.steps().replaystep(stepSize - 1).propupdate().tableinfo().secondholecards(i);
            
            const std::string image_name = PDM->get_card_image(index);
            Sprite* card = Sprite::create(image_name);
            card->setScale(0.6);
            if(i < 3)
            {
                card->setPosition(339 + card->getContentSize().width * (i * 0.6f), 860);
            }
            else
            {
                card->setPosition(501 + card->getContentSize().width * ((i-3) * 2.02f), 860);
            }
            
            root_->addChild(card, 1);
        }
    }
    
    
    int count = records.players().replayplayer_size();
    for (int i = 0; i < count; i++) {
        getCellHeightForIndex(i);
    }
    
    getSequence();
    
    tableview_record_->reloadData();
}
void RecordDetailLayer::getSequence()
{
    int count = records.players().replayplayer_size();
    for (int i = 0; i < count; i++) {
        seq.push_back(i);
    }
    
    for(auto& replayStep : records.steps().replaystep())
    {
        if (replayStep.pokerstep().has_blinds()) {
            for(auto& blind : replayStep.pokerstep().blinds().blind())
            {
                
                switch (blind.type()) {
                    case msg::BlindType::Smallblind:
                    {
                        for (int i = 0; i < count; i++) {
                            int userid = records.players().replayplayer(i).userdetailinfo().userid();
                            
                            if (blind.userid() == userid) {
                                if (seq[0] != i) {
                                    std::swap(seq[0],seq[i]);
                                }
                                
                            }
                        }
                        
                        
                    }
                        break;
                    case msg::BlindType::Bigblind:
                    {
                        
                        for (int i = 0; i < count; i++) {
                            int userid = records.players().replayplayer(i).userdetailinfo().userid();
                            
                            if (blind.userid() == userid) {
                                if (seq[1] != i) {
                                    std::swap(seq[1],seq[i]);
                                }
                                
                            }
                        }
                        
                    }
                        break;

                        
                    default:
                        break;
                }
            }
        }
    }
    
}

float RecordDetailLayer::getCellHeightForIndex(ssize_t idx)
{
    
    std::vector<int> lines(4,0);
    
    if (records.has_players()) {
        records.players().replayplayer(idx).userdetailinfo().picname();
        int userid = records.players().replayplayer(idx).userdetailinfo().userid();
        
        for(auto& replayStep : records.steps().replaystep())
        {
            if (replayStep.pokerstep().has_blinds()) {
                for(auto& blind : replayStep.pokerstep().blinds().blind())
                {
                    if (blind.userid() == userid) {
                        switch (replayStep.pokerstep().round()) {
                            case msg::PokerRoundType::Preflop:
                            {
                                lines.at(0)++;
                            }
                                break;
                            case msg::PokerRoundType::Flop:
                            {
                                lines.at(1)++;
                            }
                                break;
                            case msg::PokerRoundType::Turn:
                            {
                                lines.at(2)++;
                            }
                                break;
                            case msg::PokerRoundType::River:
                            {
                                lines.at(3)++;
                            }
                                break;
                                
                            default:
                                break;
                        }

                    }
                }
                
            }
            
            if (replayStep.pokerstep().has_playeraction())
            {
                if (replayStep.pokerstep().playeraction().userid() == userid) {
                    switch (replayStep.pokerstep().round()) {
                        case msg::PokerRoundType::Preflop:
                        {
                            lines.at(0)++;
                        }
                            break;
                        case msg::PokerRoundType::Flop:
                        {
                            lines.at(1)++;
                        }
                            break;
                        case msg::PokerRoundType::Turn:
                        {
                            lines.at(2)++;
                        }
                            break;
                        case msg::PokerRoundType::River:
                        {
                            lines.at(3)++;
                        }
                            break;
                            
                        default:
                            break;
                    }
                    
                }

            }
            
        }
        

    }
    
    std::sort(lines.begin(), lines.end());
    int n = lines.back();
    
    float height = 30.0f * n;
    
    cellHeights.push_back(height);
    return height;
}
Size RecordDetailLayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    if (isDetail) {

        return Size(150, 90 + cellHeights.at(idx));
    }
    else
    {
        return RecordDetailCell::size();

    }
}

TableViewCell* RecordDetailLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    RecordDetailCell* record_cell = dynamic_cast<RecordDetailCell*>(cell);
    if(!record_cell) {
        record_cell = RecordDetailCell::create();
    }
    float offset = 0;
    if (isDetail) {
        offset = cellHeights.at(idx);
    }
    record_cell->update_data(records,seq[idx],isDetail,offset);
    cell = record_cell;

    return cell;
}
ssize_t RecordDetailLayer::numberOfCellsInTableView(TableView *table)
{
    if (records.has_players()) {
        return records.players().replayplayer_size();
    }
    else
    {
        return 0;
    }
}

void RecordDetailLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

void RecordDetailLayer::click_btn_share(Ref* sender)
{
    utils::captureScreen(CC_CALLBACK_2(RecordDetailLayer::saveCallback, this),"share_record.png");
}
void RecordDetailLayer::saveCallback(bool succeed, const std::string& info)
{
    if(succeed)
    {
        log("RecordDetailLayer::saveCallback 微信分享-图片保存路径 = %s", info.c_str());
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, info]()
                                                                              {
                                                                                  auto layerShare = LayerShare::create();
                                                                                  layerShare->setShareRecordDetail(info);
                                                                                  AddGlobalLayer(layerShare);
                                                                              });
    }
    else
    {
        log("Capture screen failed.");
    }
    
}

void RecordDetailLayer::click_btn_close(Ref* sender)
{
    removeFromParent();
}

