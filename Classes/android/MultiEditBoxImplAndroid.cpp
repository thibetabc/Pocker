//
//  MultiEditBoxAndroid.cpp
//  pokerclient
//
//  Created by MirrorMiko on 6/13/15.
//
//

#include "MultiEditBoxImplAndroid.h"
#include "cocos2d.h"
#include "EditBoxInterface.h"
#define kLabelZOrder  9999
static const int CC_EDIT_BOX_PADDING = 5;




#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "../../cocos2d/cocos/platform/android/jni/JniHelper.h"
#include <android/log.h>
//using namespace cocos2d;
NS_CC_BEGIN
extern "C"{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_EditBoxPool_editBoxEditingDidBegin(JNIEnv* env, jobject thiz, int index)
    {
        cocos2d::log("Java_org_cocos2dx_cpp_EditBoxPool_editBoxEditingDidBegin:%d", index);
        cocos2d::log("size:%d", ui::MultiEditBoxImplAndroid::edit_boxes_.size());
        auto iter = ui::MultiEditBoxImplAndroid::edit_boxes_.find(index);
        if(iter != ui::MultiEditBoxImplAndroid::edit_boxes_.end()) {
            if(iter->second->getDelegate()) {
                iter->second->getDelegate()->editBoxEditingDidBegin(iter->second->getEditBox());
            }
        }
        else {
            
        }
    }
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_EditBoxPool_editBoxEditingDidEnd(JNIEnv* env, jobject thiz, int index)
    {
        cocos2d::log("Java_org_cocos2dx_cpp_EditBoxPool_editBoxEditingDidEnd:%d", index);
        auto iter = ui::MultiEditBoxImplAndroid::edit_boxes_.find(index);
        if(iter != ui::MultiEditBoxImplAndroid::edit_boxes_.end()) {
            if(iter->second->getDelegate()) {
                iter->second->getDelegate()->editBoxEditingDidEnd(iter->second->getEditBox());
            }
        }
    }
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_EditBoxPool_editBoxTextChanged(JNIEnv* env, jobject thiz, int index, jstring text)
    {
        cocos2d::log("Java_org_cocos2dx_cpp_EditBoxPool_editBoxTextChanged:%d", index);
        auto iter = ui::MultiEditBoxImplAndroid::edit_boxes_.find(index);
        if(iter != ui::MultiEditBoxImplAndroid::edit_boxes_.end()) {
            if(iter->second->getDelegate()) {
                std::string str = JniHelper::jstring2string(text);
                iter->second->getDelegate()->editBoxTextChanged(iter->second->getEditBox(), str);
            }
        }
    }
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_EditBoxPool_editBoxReturn(JNIEnv* env, jobject thiz, int index)
    {
        cocos2d::log("Java_org_cocos2dx_cpp_EditBoxPool_editBoxReturn:%d", index);
        auto iter = ui::MultiEditBoxImplAndroid::edit_boxes_.find(index);
        if(iter != ui::MultiEditBoxImplAndroid::edit_boxes_.end()) {
            if(iter->second->getDelegate()) {
                iter->second->getDelegate()->editBoxReturn(iter->second->getEditBox());
            }
            else {
                cocos2d::log("Java_org_cocos2dx_cpp_EditBoxPool_editBoxReturn delegate is nil");
            }
        }
        else {
            cocos2d::log("Java_org_cocos2dx_cpp_EditBoxPool_editBoxReturn not found");
        }
    }
    
    
}

#endif

//public static native void editBoxEditingDidBegin(int idx);
//public static native void editBoxEditingDidEnd(int idx);

namespace ui {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    std::map<int, MultiEditBoxImplAndroid*> MultiEditBoxImplAndroid::edit_boxes_;
    MultiEditBoxImpl* __createSystemEditBox(MultiEditBox* pEditBox)
    {
        //CCLOGERROR("android __createSystemEditBox");
        return new MultiEditBoxImplAndroid(pEditBox);
    }
#endif
    
    MultiEditBoxImplAndroid::MultiEditBoxImplAndroid(MultiEditBox* pEditText)
    : MultiEditBoxImpl(pEditText)
    , _anchorPoint(Vec2(0.5f, 0.5f))
    , _systemControl(nullptr)
    , _maxTextLength(-1)
    {
        _systemControl = new EditBoxInterface();
        edit_boxes_.insert(std::make_pair(_systemControl->get_control_id(), this));
    }
    
    MultiEditBoxImplAndroid::~MultiEditBoxImplAndroid()
    {
        edit_boxes_.erase(_systemControl->get_control_id());
    }
    
    void MultiEditBoxImplAndroid::doAnimationWhenKeyboardMove(float duration, float distance)
    {
        //cocos2d::log("MultiEditBoxImplAndroid::doAnimationWhenKeyboardMove");
        if ( _systemControl->is_focus() || distance < 0.0f)
        {
            _systemControl->doAnimationWhenKeyboardMoveWithDuration(duration, distance);
        }
    }
    
    bool MultiEditBoxImplAndroid::initWithSize(const Size& size)
    {
        do
        {
//            auto glview = cocos2d::Director::getInstance()->getOpenGLView();
            
            Rect rect = Rect(0, 0, size.width, size.height);
            //cocos2d::log("MultiEditBoxImplAndroid::initWithSize:%f, %f, %f, %f", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
//            float factor = glview->getFrameZoomFactor();
//            rect.size.width /= factor;
//            rect.size.height /= factor;
            
//            _systemControl = [[MultiEditBoxImplAndroid_objc alloc] initWithFrame:rect editBox:this];
            if (!_systemControl) break;
            
            initInactiveLabels(rect.size);
            setContentSize(rect.size);
            
            return true;
        }while (0);
        
        return false;
    }
    
    void MultiEditBoxImplAndroid::initInactiveLabels(const Size& size)
    {
        const char* pDefaultFontName = _systemControl->font_name().c_str();//[[_systemControl.textField.font fontName] UTF8String];
        
//        _label = Label::create();
//        _label->setAnchorPoint(Vec2(0, 0));
//        _label->setColor(Color3B::RED);
//        _label->setVisible(false);
//        _editBox->addChild(_label, kLabelZOrder);
//        
//        _labelPlaceHolder = Label::create();
//        // align the text vertically center
//        _labelPlaceHolder->setAnchorPoint(Vec2(0, 0.5f));
//        _labelPlaceHolder->setColor(Color3B::GREEN);
//        _editBox->addChild(_labelPlaceHolder, kLabelZOrder);
        
        setFont(pDefaultFontName, size.height);
        setPlaceholderFont(pDefaultFontName, size.height);
    }
    
    void MultiEditBoxImplAndroid::placeInactiveLabels()
    {
        //cocos2d::log("MultiEditBoxImplAndroid::placeInactiveLabels:%f, %f");
//        auto glview = cocos2d::Director::getInstance()->getOpenGLView();
//        float scalex = glview->getScaleX();
//        float scaley = glview->getScaleY();
//        Size contentSize = _editBox->getContentSize();
        
//        _label->setPosition(Point(0, ));
//        _labelPlaceHolder->setPosition(Point(0, 0));
    }
    
    void MultiEditBoxImplAndroid::setInactiveText(const char* pText)
    {
        if(_systemControl->is_password())
        {
            std::string passwordString;
            for(int i = 0; i < strlen(pText); ++i)
                passwordString.append("\u25CF");
            _systemControl->set_text(passwordString);
//            _label->setString(passwordString.c_str());
        }
        else {
//            _label->setString(getText());
            _systemControl->set_text(getText());
        }
        
        // Clip the text width to fit to the text box
//        float fMaxWidth = _editBox->getContentSize().width - CC_EDIT_BOX_PADDING * 2;
//        Size labelSize = _label->getContentSize();
//        if(labelSize.width > fMaxWidth) {
//            _label->setDimensions(fMaxWidth,labelSize.height);
//        }
    }
    
    void MultiEditBoxImplAndroid::setFont(const char* pFontName, int fontSize)
    {
        bool isValidFontName = true;
        if(pFontName == NULL || strlen(pFontName) == 0) {
            isValidFontName = false;
        }
        
//        _label->setSystemFontName(pFontName);
//        _label->setSystemFontSize(fontSize);
    }
    
    void MultiEditBoxImplAndroid::setFontColor(const Color4B& color)
    {
        _systemControl->set_text_color(color.r, color.g, color.b);
//        _label->setTextColor(color);
    }
    
    void MultiEditBoxImplAndroid::setBackgroundColor(const Color4B& color)
    {
        _systemControl->set_background_color(color.r, color.g, color.b);
    }
    
    void MultiEditBoxImplAndroid::setPlaceholderFont(const char* pFontName, int fontSize)
    {
//        _labelPlaceHolder->setSystemFontName(pFontName);
//        _labelPlaceHolder->setSystemFontSize(fontSize);
    }
    
    void MultiEditBoxImplAndroid::setPlaceholderFontColor(const Color4B &color)
    {
//        _labelPlaceHolder->setTextColor(color);
    }
    
    void MultiEditBoxImplAndroid::setInputMode(MultiEditBox::InputMode inputMode)
    {
        _systemControl->set_keyborad_type((int)inputMode);

//        switch (inputMode)
//        {
//            case MultiEditBox::InputMode::EMAIL_ADDRESS:
////                _systemControl.textField.keyboardType = UIKeyboardTypeEmailAddress;
//                break;
//            case MultiEditBox::InputMode::NUMERIC:
////                _systemControl.textField.keyboardType = UIKeyboardTypeDecimalPad;
//                break;
//            case MultiEditBox::InputMode::PHONE_NUMBER:
////                _systemControl.textField.keyboardType = UIKeyboardTypePhonePad;
//                break;
//            case MultiEditBox::InputMode::URL:
////                _systemControl.textField.keyboardType = UIKeyboardTypeURL;
//                break;
//            case MultiEditBox::InputMode::DECIMAL:
////                _systemControl.textField.keyboardType = UIKeyboardTypeDecimalPad;
//                break;
//            case MultiEditBox::InputMode::SINGLE_LINE:
////                _systemControl.textField.keyboardType = UIKeyboardTypeDefault;
//                break;
//            default:
////                _systemControl.textField.keyboardType = UIKeyboardTypeDefault;
//                break;
//        }
    }
    
    void MultiEditBoxImplAndroid::setMaxLength(int maxLength)
    {
        _maxTextLength = maxLength;
    }
    
    int MultiEditBoxImplAndroid::getMaxLength()
    {
        return _maxTextLength;
    }
    
    void MultiEditBoxImplAndroid::setInputFlag(MultiEditBox::InputFlag inputFlag)
    {
        _systemControl->set_keyborad_input_flag((int)inputFlag);
    }
    
    void MultiEditBoxImplAndroid::setReturnType(MultiEditBox::KeyboardReturnType returnType)
    {
//        cocos2d::log("MultiEditBoxImplAndroid::setReturnType:%d", returnType);
        _systemControl->set_return_type((int)returnType);
    }
    
    bool MultiEditBoxImplAndroid::isEditing()
    {
        return _systemControl->is_focus();
    }
    
    void MultiEditBoxImplAndroid::refreshInactiveText()
    {
        const char* text = getText();
        if(_systemControl->is_hidden())
        {
            setInactiveText(text);
            if(strlen(text) == 0)
            {
//                _label->setVisible(false);
//                _labelPlaceHolder->setVisible(true);
            }
            else
            {
//                _label->setVisible(true);
//                _labelPlaceHolder->setVisible(false);
            }
        }
    }
    
    void MultiEditBoxImplAndroid::setText(const char* text)
    {
        std::string t = text;
        cocos2d::log("MultiEditBoxImplAndroid::setText:%s", t.c_str());
        if (t != _systemControl->text()) {
            _systemControl->set_text(t);
        }
        refreshInactiveText();
    }
    
    void MultiEditBoxImplAndroid::setEditable(bool edit)
    {
        _systemControl->set_editable(edit);
    }
    
    bool MultiEditBoxImplAndroid::isEditable()
    {
        return _systemControl->is_editable();
    }
    
    void MultiEditBoxImplAndroid::setMultLine(bool mult){
        _systemControl->set_mult_line(mult);
    }

    const char*  MultiEditBoxImplAndroid::getText(void)
    {
        cocos2d::log("MultiEditBoxImplAndroid::getText:%s", _systemControl->text().c_str());
        text_ = _systemControl->text();
        return text_.c_str();
    }
    
    void MultiEditBoxImplAndroid::setPlaceHolder(const char* pText)
    {
//        _labelPlaceHolder->setString(pText);
        _systemControl->set_placeholder(pText);
    }
    
    static CCPoint convertDesignCoordToScreenCoord(const Vec2& designCoord)
    {
        auto glview = Director::getInstance()->getOpenGLView();
        
        Size size = glview->getFrameSize();
        float viewH = size.height;
        
        Vec2 visiblePos = Vec2(designCoord.x * glview->getScaleX(), designCoord.y * glview->getScaleY());
        Vec2 screenGLPos = visiblePos + glview->getViewPortRect().origin;
        
        Point screenPos = Point(screenGLPos.x, viewH - screenGLPos.y);
        
        float factor = glview->getFrameZoomFactor();
        screenPos.x = screenPos.x / factor;
        screenPos.y = screenPos.y / factor;
        
        //CCLOGINFO("[EditBox] pos x = %f, y = %f", screenGLPos.x, screenGLPos.y);
        return screenPos;
    }
    
    void MultiEditBoxImplAndroid::setPosition(const Vec2& pos)
    {
        _position = pos;
        adjustTextFieldPosition();
    }
    
    void MultiEditBoxImplAndroid::setVisible(bool visible)
    {
        //CCLOG("[Edit text] setVisible:%d", visible);
        _systemControl->set_visible(visible);
    }
    
    void MultiEditBoxImplAndroid::setContentSize(const Size& size)
    {
        if(size.width == 0 || size.height == 0) return;
        _contentSize = size;
        //CCLOG("[Edit text] content size = (%f, %f)", size.width, size.height);
        placeInactiveLabels();
        auto glview = Director::getInstance()->getOpenGLView();
        
//        Rect rect = Rect(0, 0, size.width * glview->getScaleX(),size.height * glview->getScaleY());
        _systemControl->set_size(size.width * glview->getScaleX(), size.height * glview->getScaleY());
    }
    
    void MultiEditBoxImplAndroid::setAnchorPoint(const Vec2& anchorPoint)
    {
        //CCLOG("[Edit text] anchor point = (%f, %f)", anchorPoint.x, anchorPoint.y);
        _anchorPoint = anchorPoint;
        setPosition(_position);
    }
    
    void MultiEditBoxImplAndroid::visit(void)
    {
//        MultiEditBoxImpl::visit();
//        if (nullptr != _systemControl) {
//            this->adjustTextFieldPosition();
//        }
    }
    
    void MultiEditBoxImplAndroid::onEnter(void)
    {
        log("MultiEditBoxImplAndroid::onEnter");
        adjustTextFieldPosition();
        const char* pText = getText();
        if (pText) {
            setInactiveText(pText);
        }
    }
    
    void MultiEditBoxImplAndroid::updatePosition(float dt)
    {
        if (nullptr != _systemControl) {
            this->adjustTextFieldPosition();
        }
    }
    
    void MultiEditBoxImplAndroid::adjustTextFieldPosition()
    {
        auto glview = Director::getInstance()->getOpenGLView();
        float scalex = glview->getScaleX();
        float scaley = glview->getScaleY();
        Size contentSize = _contentSize;
//        contentSize = Size(contentSize.width * scalex, contentSize.height * scaley);
//        Rect rect = Rect(0, 0, contentSize.width, contentSize.height);
//        rect = RectApplyAffineTransform(rect, _editBox->nodeToWorldTransform());
//        rect = Rect(rect.origin.x, rect.origin.y, rect.size.width * scalex, rect.size.height * scaley);
        Vec2 designCoord = _position;
        auto winSize = glview->getVisibleSize();
        //cocos2d::log("designCoord:%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f", designCoord.x, designCoord.y, _anchorPoint.x, _anchorPoint.y, contentSize.width, contentSize.height, glview->getScreenSize().height, winSize.width, winSize.height, scalex, scaley);
//        _systemControl->set_position((designCoord.x * scalex - contentSize.width / 2) , (glview->getScreenSize().height - designCoord.y * scaley - contentSize.height/2));
        _systemControl->set_position((designCoord.x * scalex - contentSize.width * _anchorPoint.x * scalex) ,
                                     (winSize.height - designCoord.y) * scaley - contentSize.height * scaley * (1 -_anchorPoint.y));
    }
    
    void MultiEditBoxImplAndroid::openKeyboard()
    {
//        _label->setVisible(false);
//        _labelPlaceHolder->setVisible(false);
        //cocos2d::log("MultiEditBoxImplAndroid::openKeyboard");
        _systemControl->set_visible(true);
        _systemControl->open_keyborad();
    }
    
    void MultiEditBoxImplAndroid::closeKeyboard()
    {
//        _label->setString(_systemControl->text());
//        _label->setVisible(true);
//        _labelPlaceHolder->setVisible(false);
        
        _systemControl->set_visible(false);
        _systemControl->close_keyborad();
    }
    
    void MultiEditBoxImplAndroid::onEndEditing()
    {
        //cocos2d::log("MultiEditBoxImplAndroid::onEndEditing");
        _systemControl->set_visible(false);
        if(strlen(getText()) == 0)
        {
//            _label->setVisible(false);
//            _labelPlaceHolder->setVisible(true);
        }
        else
        {
//            _label->setVisible(true);
//            _labelPlaceHolder->setVisible(false);
            setInactiveText(getText());
        }
    }
    
}

NS_CC_END
