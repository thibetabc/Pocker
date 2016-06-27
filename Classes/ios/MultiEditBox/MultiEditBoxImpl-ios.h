#pragma once


#include "platform/CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "extensions/ExtensionMacros.h"
#include "MultiEditBoxImpl.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface UICustomUITextView : UITextView
{
}

@end


@interface MultiEditBoxImplIOS_objc : NSObject <UITextViewDelegate>
{
    UICustomUITextView* textField_;
    void* editBox_;
    BOOL editState_;
}

@property(nonatomic, retain) UITextView* textField;
@property(nonatomic, readonly, getter = isEditState) BOOL editState;
@property(nonatomic, assign) void* editBox;

-(id) initWithFrame: (CGRect) frameRect editBox: (void*) editBox;
-(void) doAnimationWhenKeyboardMoveWithDuration:(float)duration distance:(float)distance;
-(void) setPosition:(CGPoint) pos;
-(void) setContentSize:(CGSize) size;
-(void) visit;
-(void) openKeyboard;
-(void) closeKeyboard;
-(void) setBackgroundColor:(UIColor*) color;

@end

NS_CC_BEGIN

namespace ui {
    
    class EditBox;
    
    class MultiEditBoxImplIOS : public MultiEditBoxImpl
    {
    public:
        /**
         * @js NA
         */
        MultiEditBoxImplIOS(MultiEditBox* pEditText);
        /**
         * @js NA
         * @lua NA
         */
        virtual ~MultiEditBoxImplIOS();
        
        virtual bool initWithSize(const Size& size);
        virtual void setFont(const char* pFontName, int fontSize);
        virtual void setFontColor(const Color4B& color);
        virtual void setPlaceholderFont(const char* pFontName, int fontSize);
        virtual void setPlaceholderFontColor(const Color4B& color);
        virtual void setInputMode(MultiEditBox::InputMode inputMode);
        virtual void setInputFlag(MultiEditBox::InputFlag inputFlag);
        virtual void setMaxLength(int maxLength);
        virtual int  getMaxLength();
        virtual void setReturnType(MultiEditBox::KeyboardReturnType returnType);
        virtual bool isEditing();
        
        virtual void setText(const char* pText);
        virtual const char* getText(void);
        virtual void refreshInactiveText();
        virtual void setPlaceHolder(const char* pText);
        virtual void setPosition(const Vec2& pos);
        virtual void setVisible(bool visible);
        virtual void setContentSize(const Size& size);
        virtual void setAnchorPoint(const Vec2& anchorPoint);
        virtual void updatePosition(float dt) override;
        
        virtual void setEditable(const bool edit);
        virtual void setMultLine(const bool edit);
        virtual void setBackgroundColor(const Color4B& color);
        /**
         * @js NA
         * @lua NA
         */
        virtual void visit(void);
        /**
         * @js NA
         * @lua NA
         */
        virtual void onEnter(void);
        virtual void doAnimationWhenKeyboardMove(float duration, float distance);
        virtual void openKeyboard();
        virtual void closeKeyboard();
        
        virtual bool isEditable(){return true;}
        
        virtual void onEndEditing();
    private:
        void			initInactiveLabels(const Size& size);
        void			setInactiveText(const char* pText);
        void			adjustTextFieldPosition();
        void            placeInactiveLabels();
        
        Label*     _label;
        Label*     _labelPlaceHolder;
        Size          _contentSize;
        Vec2         _position;
        Vec2         _anchorPoint;
        MultiEditBoxImplIOS_objc* _systemControl;
        int             _maxTextLength;
    };
    
    
}

NS_CC_END


#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) */