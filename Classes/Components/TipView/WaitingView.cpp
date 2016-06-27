#include "WaitingView.h"
#include "utils.h"
#include "ui/CocosGUI.h"
#include "utils.h"

using namespace cocos2d;

void WaitingView::show()
{
    SceneManager::addWaitingLayer();
}

void WaitingView::remove()
{
    WaitingView* waitingview = dynamic_cast<WaitingView*>(SceneManager::GetWaitingLayer());
    if(waitingview)
        waitingview->removeFromParent();
}

bool WaitingView::init()
{
    if(!Layer::init()) {
        return false;
    }
    
//    auto& win_size = Director::getInstance()->getWinSize();
//    
//    auto layout = ui::Layout::create();
//    layout->setContentSize(win_size);
//    layout->setTouchEnabled(true);
//    layout->setAnchorPoint(Point::ZERO);
//    this->addChild(layout, -10);
    
//    text_ = cocos2d::ui::Text::create("", "Thonburi", 38);
//    text_->setPosition(Point(win_size.width/2, win_size.height-100));
//    text_->setTextColor(Color4B::YELLOW);
//    this->addChild(text_, 100);
    
    send_time = tools::date();
    Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(WaitingView::on_timer), this, 1.0f, false);
    return true;
}
//void WaitingView::setText(const std::string& pos)
//{
//    text_->setString(pos);
//}

void WaitingView::onEnter()
{
    BaseLayer::onEnter();
    auto& win_size = Director::getInstance()->getWinSize();
    sprite_waiting_ = Sprite::create("waitingview/round_1.png");
    
    sprite_waiting_->setPosition(Point(win_size.width/2-120, win_size.height-90/*win_size.height*0.666667*/));// by ronaldz. Point(win_size) * 0.5f);
    this->addChild(sprite_waiting_, 10);
    
    sprite_animation_ = Animation::create();
    for (int i = 1; i < 11; ++i) {
        std::string filename = "waitingview/round_" + tools::to_string(i) + ".png";
        sprite_animation_->addSpriteFrameWithFile(filename);
    }
    sprite_animation_->setDelayPerUnit(0.1f);
    Animate* animate = Animate::create(sprite_animation_);

    sprite_waiting_->runAction(RepeatForever::create(animate));

}

void WaitingView::on_timer(float t)
{
    if (tools::date() - send_time > 8.0f)
    {
        this->removeFromParent();
    }
}