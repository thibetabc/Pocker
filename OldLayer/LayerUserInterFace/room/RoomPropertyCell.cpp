#include <string>
#include "RoomPropertyCell.h"
#include "utils.h"
#include "GameTools.h"
#include "ListLayer.h"

RoomPropertyCell* RoomPropertyCell::create(int AParentTag, int ACellIndex){
    RoomPropertyCell* result = RoomPropertyCell::create();
    result->selfIndex = ACellIndex;
    result->parentTag = AParentTag;
    return result;
}

bool RoomPropertyCell::init()
{
    if(!Widget::init()) {
        return false;
    }
    
    select_ = false;
    property_ = NULL;
    
    auto root = CSLoader::createNode("NodePropertyCell.csb");
    this->addChild(root);
    root_ = root;
    text_property_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_property_name"));
    
    text_value_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_value"));

    check_box_ = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(root, "checkbox"));
    sprite_bg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_1"));
    arrow = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_arrow"));
    
    //std::function<void(Ref*,CheckBox::EventType)>
    check_box_->addEventListener(std::bind(&RoomPropertyCell::click_check_box, this, std::placeholders::_1, std::placeholders::_2));
    text_menu = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_Menu"));
    
    text_menu->addClickEventListener(std::bind(&RoomPropertyCell::text_menu_click, this,std::placeholders::_1));
    sprite_bg->addClickEventListener(std::bind(&RoomPropertyCell::text_menu_click, this, std::placeholders::_1));
    auto image_bg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_bg"));
    image_bg->addTouchCallback([this](Ref*, Touch* touch, Widget::TouchEventType event){
        ListView* listview = (ListView*) getParent();
        if(listview) {
            switch (event) {
                case Widget::TouchEventType::BEGAN:
                    listview->onTouchBegan(touch, nullptr);
                    break;
                
                case Widget::TouchEventType::MOVED:
                    listview->onTouchMoved(touch, nullptr);
                    break;
                case Widget::TouchEventType::ENDED:
                    listview->onTouchEnded(touch, nullptr);
                    break;
                case Widget::TouchEventType::CANCELED:
                    listview->onTouchCancelled(touch, nullptr);
                    break;
                default:
                    break;
            }
        }
    });

//this->setContentSize(image_bg->getContentSize() + Size(0, 2));
    this->setContentSize(image_bg->getContentSize());
    
    return true;
}

void RoomPropertyCell::update_data(msg::Property *property)
{
    property_ = property;
    text_property_->setString(property->displayname());
    switch (property->type()) {
        case msg::PropertyType::INT:
            arrow->setVisible(false);
            sprite_bg->setVisible(false);
            text_menu->setVisible(false);
            text_value_->setVisible(true);
            text_value_->setString(tools::to_string(property->intvalue().value()));
            check_box_->setVisible(false);
            break;
        case msg::PropertyType::BOOL:
            arrow->setVisible(false);
            sprite_bg->setVisible(false);
            text_menu->setVisible(false);
            text_value_->setVisible(false);
            check_box_->setVisible(true);
            check_box_->setSelected(property->boolvalue());
            break;
        case msg::PropertyType::ENUM:
            arrow->setVisible(true);
            sprite_bg->setVisible(true);
            text_menu->setVisible(true);
            check_box_->setVisible(false);
            text_value_->setVisible(false);
            for (int i = 0; i < property_->enumvalue().items().size(); i++)
            {
                if (property_->enumvalue().items(i).value() == property_->enumvalue().value())
                {
                    text_menu->setString(property_->enumvalue().items(i).text());
                    break;
                }
            }
            break;
        default:
            break;
    }

}
void RoomPropertyCell::editBoxReturn(ui::EditBox* editBox)
{
    std::string str = editBox->getText();
    int value = tools::stoi(str);
    property_->mutable_intvalue()->set_value(value);
}

void RoomPropertyCell::click_check_box(Ref* sender, CheckBox::EventType event)
{
    property_->set_boolvalue(check_box_->isSelected());
}

void RoomPropertyCell::text_menu_click(Ref *sender)
{
    this->setZOrder(255);
    ListLayer *pBg = ListLayer::create(parentTag, selfIndex);
    getParent()->getParent()->getParent()->addChild(pBg);
    int distance = -60;
    
    Point base = convertToWorldSpace(text_menu->getPosition());
    Point bg_base = convertToWorldSpace(sprite_bg->getPosition());
    pBg->update(bg_base, base, distance, property_);
}

const uint32_t RoomPropertyCell::get_property_value()
{
    uint32_t result = 0;
    switch (property_->type()) {
        case msg::PropertyType::INT:     //integer
            result = tools::stoul(text_value_->getString());
            break;
        case msg::PropertyType::BOOL:
            result = check_box_->isSelected();
            break;  //bool
        default:
            break;
    }
    return result;
}

const std::string RoomPropertyCell::get_property_name()
{
    return property_->displayname();
}

const msg::PropertyType RoomPropertyCell::get_property_type()
{
    return property_->type();
}

void RoomPropertyCell::keyBoardWillShow(const float duration, const float distance)
{
    if(!select_) {
        return;
    }

    float dis = distance;
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
//    dis *= glview->getScaleY();
//    float scale = glview->getDesignResolutionSize().height/glview->getScreenSize().height;
    float scale = glview->getDesignResolutionSize().height/(glview->getScreenSize().height / glview->getScaleY());
    dis *= scale;
    
//    Point p = tf_value_->getPosition() + tf_value_->getParent()->getPosition() - Point(tf_value_->getAnchorPoint().x * tf_value_->getContentSize().width, tf_value_->getAnchorPoint().y * tf_value_->getContentSize().height);
//    log("p.x, p.y:%f, %f", p.x, p.y);
//    
//    if(p.y < dis) {
//        Point p1 = this->getParent()->getPosition() + Point(0, dis - p.y + tf_value_->getContentSize().height);
//        this->getParent()->stopAllActions();
//        this->getParent()->runAction(MoveTo::create(0.2f, p1));
//    }
}
void RoomPropertyCell::keyBoardWillHide(const float duration, const float distance)
{
    if(!select_) {
        return;
    }
    this->getParent()->stopAllActions();
    this->getParent()->setPosition(Point::ZERO);
}

void RoomPropertyCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}
