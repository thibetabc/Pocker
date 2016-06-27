#pragma once


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#else
#endif

#include "MultiEditBox.h"

namespace cocos2d {
    namespace ui{
        
        class CC_GUI_DLL MultiEditBoxImpl
        {
        public:
            /**
             * @js NA
             */
            MultiEditBoxImpl(MultiEditBox* pEditBox) : _delegate(nullptr),_editBox(pEditBox) {}
            /**
             * @js NA
             * @lua NA
             */
            virtual ~MultiEditBoxImpl() {}
            
            virtual bool initWithSize(const Size& size) = 0;
            virtual void setFont(const char* pFontName, int fontSize) = 0;
            virtual void setFontColor(const Color4B& color) = 0;
            virtual void setBackgroundColor(const Color4B& color) = 0;
            virtual void setPlaceholderFont(const char* pFontName, int fontSize) = 0;
            virtual void setPlaceholderFontColor(const Color4B& color) = 0;
            virtual void setInputMode(MultiEditBox::InputMode inputMode) = 0;
            virtual void setInputFlag(MultiEditBox::InputFlag inputFlag) = 0;
            virtual void setMaxLength(int maxLength) = 0;
            virtual int  getMaxLength() = 0;
            virtual void setReturnType(MultiEditBox::KeyboardReturnType returnType) = 0;
            virtual bool isEditing() = 0;
            virtual void adjustTextFieldPosition() = 0;
            virtual void setText(const char* pText) = 0;
            virtual const char* getText(void) = 0;
            virtual void setPlaceHolder(const char* pText) = 0;
            virtual void doAnimationWhenKeyboardMove(float duration, float distance) = 0;
            
            virtual void openKeyboard() = 0;
            virtual void closeKeyboard() = 0;
            
            virtual void setPosition(const Vec2& pos) = 0;
            virtual void setVisible(bool visible) = 0;
            virtual void setContentSize(const Size& size) = 0;
            virtual void setAnchorPoint(const Vec2& anchorPoint) = 0;
            virtual void setEditable(const bool edit) = 0;
            virtual void setMultLine(const bool edit) = 0;
            
            virtual bool isEditable() = 0;

            /**
             * check the editbox's position, update it when needed
             */
            virtual void updatePosition(float dt){};
            /**
             * @js NA
             * @lua NA
             */
            virtual void visit(void) = 0;
            /**
             * @js NA
             * @lua NA
             */
            virtual void onEnter(void) = 0;
            
            
            void setDelegate(MultiEditBoxDelegate* pDelegate) { _delegate = pDelegate; };
            MultiEditBoxDelegate* getDelegate() { return _delegate; };
            MultiEditBox* getEditBox() { return _editBox; };
        protected:
            MultiEditBoxDelegate* _delegate;
            MultiEditBox* _editBox;
        };
        
        // This method must be implemented at each subclass of EditBoxImpl.
        extern MultiEditBoxImpl* __createSystemEditBox(MultiEditBox* pEditBox);
        
    }
}