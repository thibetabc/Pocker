#include "TextViewAndroid.h"
#include "TextViewInterface.h"

CustomTextViewBridge* __createTextView(const cocos2d::Size& size)
{
    CustomTextViewBridge* result = new TextViewAndroid(size);
    result->autorelease();
    return result;
}



TextViewAndroid::TextViewAndroid(const cocos2d::Size& contentSize)
: CustomTextViewBridge()
{
    setContentSize(contentSize);
    _systemControl = new TextViewInterface();
    _systemControl->setContentSize(contentSize);
}

TextViewAndroid::~TextViewAndroid()
{
    delete _systemControl;
}


void TextViewAndroid::setString(const std::string& text)
{
    _systemControl->setString(text);
}

void TextViewAndroid::setPosition(const cocos2d::Point& pos)
{
    CustomTextViewBridge::setPosition(pos);
    
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    float scalex = view->getScaleX();
    float scaley = view->getScaleY();
    cocos2d::Size contentSize = getContentSize();
    auto winSize = view->getVisibleSize();

    _systemControl->setPosition(
                cocos2d::Point(10,
                               (winSize.height - pos.y) * scaley - contentSize.height * scaley * (1 -_anchorPoint.y)));
    
}

void TextViewAndroid::onExit()
{
    CustomTextViewBridge::onExit();
}