#include "TextViewIos.h"
#include "platform/CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "extensions/ExtensionMacros.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "base/CCDirector.h"
#import "platform/ios/CCEAGLView-ios.h"

using namespace cocos2d;

CustomTextViewBridge* __createTextView(const cocos2d::Size& size)
{
    CustomTextViewBridge* result = new TextViewIos(size);
    result->autorelease();
    return result;
}


TextViewIos::TextViewIos(const cocos2d::Size& contentSize)
: CustomTextViewBridge()
{
    setContentSize(contentSize);
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *eaglview = (CCEAGLView *) view->getEAGLView();
    float scalex = view->getScaleX();
    float scaley = view->getScaleY();
    float scale = [[UIScreen mainScreen] scale];
    
    CGRect frame = CGRectMake(0, 0, contentSize.width/scale * scalex, contentSize.height/scale * scaley);
    
    textView_ = [[UITextView alloc] initWithFrame:frame];
    [textView_ setEditable:NO];
    
    [eaglview addSubview:textView_];

}

TextViewIos::~TextViewIos()
{
    [textView_ release];
}

void TextViewIos::setString(const std::string& text)
{
    NSString* t = [NSString stringWithUTF8String:text.c_str()];
    [textView_ setText:t];
}

void TextViewIos::setPosition(const cocos2d::Point& pos)
{
    Node::setPosition(pos);
    cocos2d::Point p;
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    float scalex = view->getScaleX();
    float scaley = view->getScaleY();
    auto winSize = view->getScreenSize();
    float scale = [[UIScreen mainScreen] scale];
    [textView_ setCenter:CGPointMake(pos.x/scale * scalex, (winSize.height - pos.y * scaley)/scale)];
}

void TextViewIos::onExit()
{
    CustomTextViewBridge::onExit();
    [textView_ removeFromSuperview];
}

#endif
