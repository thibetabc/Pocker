#include "RecordAnimationLayer.h"
#include "utils.h"


bool RecordAnimationLayer::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("NodeRecordAnimation.csb");
    this->addChild(root);
    
    auto& winSize = Director::getInstance()->getWinSize();
    
    auto bg = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "bg"));
    root->setPosition(Point(winSize) * 0.5 - Point(bg->getContentSize()) * 0.5);
    
    sprite_microphone_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "sprite_microphone"));
    sprite_cancel_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "sprite_cancel"));
    sprite_record_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "sprite_record"));
    text_note_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_note"));
    image_red_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_red"));
    
    return true;
}

void RecordAnimationLayer::record()
{
    sprite_cancel_->setVisible(false);
    text_note_->setTextColor(Color4B::WHITE);
    text_note_->setString(tools::local_string("", ""));
    sprite_microphone_->setVisible(true);
    sprite_record_->setVisible(true);
    image_red_->setVisible(false);
    
    Animation* animation = Animation::create();
    animation->setDelayPerUnit(0.1f);
    for (int i = 1; i < 6; i++) {
        std::string filename = "login_res/record_voice/voice-" + tools::to_string(i) + "_05.png";
        animation->addSpriteFrameWithFile(filename);
    }
    Animate* animate = Animate::create(animation);
    sprite_record_->runAction(RepeatForever::create(animate));
}
void RecordAnimationLayer::cancel()
{
    sprite_microphone_->setVisible(false);
    sprite_record_->stopAllActions();
    sprite_record_->setVisible(false);
    sprite_cancel_->setVisible(true);
    text_note_->setVisible(true);
    image_red_->setVisible(false);
    text_note_->setString(tools::local_string("move_finger_cancel", "手指上划，取消发送"));
}

void RecordAnimationLayer::giveup()
{
    sprite_microphone_->setVisible(false);
    sprite_record_->stopAllActions();
    sprite_record_->setVisible(false);
    sprite_cancel_->setVisible(true);
    text_note_->setVisible(true);
    image_red_->setVisible(true);
    text_note_->setString(tools::local_string("release_finger_cancel", "松开手指，取消发送"));
}