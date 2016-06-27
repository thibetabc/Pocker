#include "RecordCell.h"
#include "RecordObject.h"
#include "utils.h"
#include "PokerDataManager.h"
#include "cmd_data.h"
#include "GameTools.h"

Size RecordCell::size()
{
    static Size s(321, 60);
    return s;
}

bool RecordCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("NodePlayRecord.csb");
    this->addChild(root);
    
    text_nickname_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_1"));
    text_amount_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_2"));
    sprite_card_bg_ = dynamic_cast<Node*>(CSLoader::seekNodeByName(root, "Node_1"));
    image_bg = dynamic_cast<ImageView *>(CSLoader::seekNodeByName(root, "Image_3"));
    btn_replay = dynamic_cast<Button *>(CSLoader::seekNodeByName(root, "Button_1"));
    btn_replay->addClickEventListener(std::bind(&RecordCell::click_replay, this));
    this->setContentSize(Size(321, 60));
    gr_cell_ = NULL;
    if(!GameTools::isChineseLanguage())
    {
        GameTools::set_pic_change(btn_replay,"btn_replay.png","btn_replay_click.png");
    }
    return true;
}

void RecordCell::click_replay()
{
    PDM->setIsDetail(true);
    PDM->send_poker_get_hand_history(this->hand_id_, true);
    
}

void RecordCell::update_data(const std::shared_ptr<msg::GameRecord>& obj)
{
    std::string nickname;
    if(obj->winnername_size() != 0) {
        nickname = obj->winnername(0);
        if(obj->winnername_size() != 1) {
            nickname += tools::local_string("etc.","等");
        }
    }
    gr_cell_ = obj;
    hand_id_ = obj->handid();
    
    
    text_nickname_->setString(nickname);
    text_amount_->setString(tools::to_string(obj->poolsize()));
    
    sprite_card_bg_->removeAllChildren();
    
    int32_t idx = 0;
    for (auto& iter : obj->holecard()) {
        const std::string image_name = PDM->get_card_image(iter);
        Sprite* card = Sprite::create(image_name);
        card->setScale(0.4);
        card->setPosition(Point(card->getContentSize().width * 0.5 * ((idx + 1) * 0.3), sprite_card_bg_->getContentSize().height * 0.5));
        sprite_card_bg_->addChild(card, 1);
        idx++;
    }
}
void RecordCell::select(const bool s)
{
//    image_bg->loadTexture("res/login_res/PlayRecord/bg_cell_click.png");
}