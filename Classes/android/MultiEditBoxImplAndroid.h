#pragma once

#include "MultiEditBoxImpl.h"
#include "EditBoxInterface.h"

NS_CC_BEGIN

namespace ui {
    class MultiEditBoxImplAndroid : public MultiEditBoxImpl{
    public:
        MultiEditBoxImplAndroid(MultiEditBox* pEditText);
        virtual ~MultiEditBoxImplAndroid();
        
        virtual bool initWithSize(const Size& size);
        virtual void setFont(const char* pFontName, int fontSize);
        virtual void setFontColor(const Color4B& color);
        virtual void setBackgroundColor(const Color4B& color);
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
        
        virtual void setEditable(bool edit);
        virtual bool isEditable();
        virtual void setMultLine(bool mult);
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
        
        virtual void onEndEditing();
        virtual void			adjustTextFieldPosition();
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        
        static std::map<int, MultiEditBoxImplAndroid*> edit_boxes_;
#endif
        
    private:
        void			initInactiveLabels(const Size& size);
        void			setInactiveText(const char* pText);
        
        void            placeInactiveLabels();
        
//        Label*     _label;
//        Label*     _labelPlaceHolder;
        
        Size          _contentSize;
        Vec2         _position;
        Vec2         _anchorPoint;
        EditBoxInterface* _systemControl;
        int             _maxTextLength;
        
        std::string text_;
    };
}

NS_CC_END

