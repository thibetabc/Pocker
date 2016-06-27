#include "GameNameCell.h"
#include "utils.h"
#include "msg.pb.h"


bool GameNameCell::init()
{
    if(!Layout::init()) {
        return false;
    }
    
    in_game_ = false;
    
    auto root = CSLoader::createNode("NodeGameCell.csb");
    this->addChild(root);
    
    text_game_type_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_game_type"));
    image_game_type_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_game_type_icon"));
    image_game_type_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_game_type_bg"));
    
    image_game_type_->setTouchEnabled(true);
    image_game_type_->setSwallowTouches(false);
    image_game_type_->addTouchEventListener([=](Ref*, Widget::TouchEventType event){
        
//        if(!is_valid()) {
//            return;
//        }
        
        bool moved = false;
        switch (event) {
            case Widget::TouchEventType::BEGAN:
                
                break;
            case Widget::TouchEventType::MOVED: {
                moved = true;
                break;
            }
            case Widget::TouchEventType::ENDED:
            case Widget::TouchEventType::CANCELED:{
                if(callback_ && !moved) {
                    callback_(room_type);
                }
                break;
            }
            default:
                break;
        }
        
       
    });
    
    //this->setContentSize(image_game_type_->getContentSize());
    this->setContentSize(cocos2d::Size(250, 205));
    
    return true;
}

void GameNameCell::update_data()
{
    image_game_type_->setVisible(false);
    image_game_type_bg_->setVisible(false);
    text_game_type_->setVisible(false);
}

void GameNameCell::update_data(msg::RoomType gameID,std::string name, int room_count)
{
    room_type = gameID;
    text_game_type_->setString(name);
    
    /*  //后面再加看要不要显示数量。 
    char temp[10];
    sprintf(temp, "%d", room_count, 10);
    auto lable = Label::createWithSystemFont(temp,"Arial", 30);
    this->addChild(lable);
    */
    
    
    float type_bg_width = text_game_type_->getStringLength()*text_game_type_->getFontSize()+10;
    if (type_bg_width < 200)
        type_bg_width = 200;
    image_game_type_bg_->setContentSize(cocos2d::Size(type_bg_width, image_game_type_bg_->getContentSize().height));
    if (gameID == msg::RoomType::NLHOLDEM)
    {
        image_game_type_->loadTexture("login_res/roomlist_res/image_holdem.png");
        
    }
    else if (gameID == msg::RoomType::PLO)
    {
        image_game_type_->loadTexture("login_res/roomlist_res/image_omaha.png");
    }
    else if (gameID == msg::RoomType::OFC3)
    {
        image_game_type_->loadTexture("login_res/roomlist_res/image_pineapple.png");
    }
    else if (gameID == msg::RoomType::OFC4)
    {
        image_game_type_->loadTexture("login_res/roomlist_res/image_openface.png");
    }
    else if(gameID == msg::RoomType::PLOHILO)
    {
        image_game_type_->loadTexture("login_res/roomlist_res/image_omaha_hl.png");
    }
    else
    {
        image_game_type_->setVisible(false);
        image_game_type_bg_->setVisible(false);
        text_game_type_->setVisible(false);
    }
    
}

void GameNameCell::set_selected(const bool is_select)
{
//    if(is_select) {
//        image_game_type_->loadTexture("room_list/zhongjianfangkuai.png");
//    }
//    else {
//        image_game_type_->loadTexture("room_list/baisefangkuai.png");
//    }
}

bool GameNameCell::is_valid()
{
    auto parent_pos = this->getParent()->getPosition();
    auto pos = this->getPosition();
    float x = parent_pos.x + pos.x;
    float width = 399;
    if(in_game_) {
        width = 199;
    }
    if(x < 0 || x >= width) {
        return false;
    }
    return true;
}