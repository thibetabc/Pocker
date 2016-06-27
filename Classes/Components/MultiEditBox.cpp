//
//  MultiEditBox.cpp
//  MultiEditBox
//
//  Created by duanmt on 15/5/14.
//
//

#include "MultiEditBox.h"
#include "MultiEditBoxImpl.h"
#include "cocos2d.h"

NS_CC_BEGIN

namespace ui {
    
    static const float CHECK_EDITBOX_POSITION_INTERVAL = 0.1f;
    
    MultiEditBox::MultiEditBox(void)
    : _editBoxImpl(nullptr)
    , _delegate(nullptr)
    , _editBoxInputMode(MultiEditBox::InputMode::SINGLE_LINE)
    , _editBoxInputFlag(MultiEditBox::InputFlag::INTIAL_CAPS_ALL_CHARACTERS)
    , _keyboardReturnType(KeyboardReturnType::DEFAULT)
    , _backgroundSprite(nullptr)
    , _fontSize(-1)
    , _placeholderFontSize(-1)
    , _colText(Color3B::WHITE)
    , _colPlaceHolder(Color3B::GRAY)
    , _maxLength(0)                                                                      
    , _adjustHeight(0.0f)
    , Widget()
#if CC_ENABLE_SCRIPT_BINDING
    , _scriptEditBoxHandler(0)
#endif
    {
    }
    
    MultiEditBox::~MultiEditBox(void)
    {
        CC_SAFE_DELETE(_editBoxImpl);
#if CC_ENABLE_SCRIPT_BINDING
        unregisterScriptEditBoxHandler();
#endif
    }
    
    
    void MultiEditBox::touchDownAction(Ref *sender, TouchEventType controlEvent)
    {
        if (controlEvent == Widget::TouchEventType::ENDED) {
            _editBoxImpl->openKeyboard();
        }
    }
    
    MultiEditBox* MultiEditBox::create(const Size& size,
                             const std::string& normalSprite,
                             TextureResType texType /*= TextureResType::LOCAL*/)
    {
        MultiEditBox* pRet = new MultiEditBox();
        
        if (pRet != nullptr && pRet->initWithSizeAndBackgroundSprite(size, normalSprite, texType))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(pRet);
        }
        
        return pRet;
    }
    
    
    MultiEditBox* MultiEditBox::create(const cocos2d::Size &size, cocos2d::ui::Scale9Sprite *normalSprite, ui::Scale9Sprite *pressedSprite, Scale9Sprite* disabledSprite)
    {
        MultiEditBox* pRet = new (std::nothrow) MultiEditBox();
        
        if (pRet != nullptr && pRet->initWithSizeAndBackgroundSprite(size, normalSprite))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(pRet);
        }
        
        return pRet;
    }
    
    bool MultiEditBox::initWithSizeAndBackgroundSprite(const cocos2d::Size &size, cocos2d::ui::Scale9Sprite *pNormal9SpriteBg)
    {
        if (Widget::init())
        {
            CCLOGERROR("initWithSizeAndBackgroundSprite");
            _editBoxImpl = __createSystemEditBox(this);
            CCLOGERROR("initWithSizeAndBackgroundSprite1");
            _editBoxImpl->initWithSize(size);
            _editBoxImpl->setInputMode(MultiEditBox::InputMode::ANY);
            CCLOGERROR("initWithSizeAndBackgroundSprite2111111");

            _backgroundSprite = pNormal9SpriteBg;
            
//            auto glview = cocos2d::Director::getInstance()->getOpenGLView();
            this->setContentSize(size);
            this->setPosition(Vec2(0, 0));
            
            _backgroundSprite->setPosition(Vec2(_contentSize.width/2, _contentSize.height/2));
            _backgroundSprite->setContentSize(size);
            this->addProtectedChild(_backgroundSprite);
            
            this->setTouchEnabled(true);
            
            this->addTouchEventListener(CC_CALLBACK_2(MultiEditBox::touchDownAction, this));
            
            return true;
        }
        return false;
    }
    
    
    bool MultiEditBox::initWithSizeAndBackgroundSprite(const Size& size,
                                                  const std::string& pNormal9SpriteBg,
                                                  TextureResType texType)
    {
        if (Widget::init())
        {
            _editBoxImpl = __createSystemEditBox(this);
            _editBoxImpl->initWithSize(size);
            _editBoxImpl->setInputMode(MultiEditBox::InputMode::ANY);
            
            if (texType == Widget::TextureResType::LOCAL)
            {
                _backgroundSprite = Scale9Sprite::create(pNormal9SpriteBg);
            }
            else
            {
                _backgroundSprite = Scale9Sprite::createWithSpriteFrameName(pNormal9SpriteBg);
            }
            this->setContentSize(size);
            this->setPosition(Vec2(0, 0));
            
            _backgroundSprite->setPosition(Vec2(_contentSize.width/2, _contentSize.height/2));
            _backgroundSprite->setContentSize(size);
            this->addProtectedChild(_backgroundSprite);
            
            this->setTouchEnabled(true);
            
            this->addTouchEventListener(CC_CALLBACK_2(MultiEditBox::touchDownAction, this));
            
            return true;
        }
        return false;
    }
    
    void MultiEditBox::setDelegate(MultiEditBoxDelegate* pDelegate)
    {
        _delegate = pDelegate;
        if (_editBoxImpl != nullptr)
        {
            _editBoxImpl->setDelegate(pDelegate);
        }
    }
    
    MultiEditBoxDelegate* MultiEditBox::getDelegate()
    {
        return _delegate;
    }
    
    void MultiEditBox::setText(const char* pText)
    {
        if (pText != nullptr)
        {
            _text = pText;
            if (_editBoxImpl != nullptr)
            {
                _editBoxImpl->setText(pText);
            }
        }
    }
    
    void MultiEditBox::setEditable(const bool edit)
    {
        if(_editBoxImpl != nullptr) {
            _editBoxImpl->setEditable(edit);
        }
    }
    
    bool MultiEditBox::isEditable()
    {
        if(_editBoxImpl != nullptr) {
            return _editBoxImpl->isEditable();
        }
        return false;
    }
    
    void MultiEditBox::setMultLine(const bool edit)
    {
        if(_editBoxImpl != nullptr) {
            _editBoxImpl->setMultLine(edit);
        }
    }
    
    const char* MultiEditBox::getText(void)
    {
        if (_editBoxImpl != nullptr)
        {
            const char* pText = _editBoxImpl->getText();
            log("pText:%s", pText);
            if(pText != nullptr)
                return pText;
        }
        
        return "";
    }
    
    void MultiEditBox::setFont(const char* pFontName, int fontSize)
    {
        _fontName = pFontName;
        _fontSize = fontSize;
        if (pFontName != nullptr)
        {
            if (_editBoxImpl != nullptr)
            {
                _editBoxImpl->setFont(pFontName, fontSize);
            }
        }
    }
    
    void MultiEditBox::setFontName(const char* pFontName)
    {
        _fontName = pFontName;
        if (_editBoxImpl != nullptr && _fontSize != -1)
        {
            _editBoxImpl->setFont(pFontName, _fontSize);
        }
    }
    
    void MultiEditBox::setFontSize(int fontSize)
    {
        _fontSize = fontSize;
        if (_editBoxImpl != nullptr && _fontName.length() > 0)
        {
            _editBoxImpl->setFont(_fontName.c_str(), _fontSize);
        }
    }
    
    void MultiEditBox::setFontColor(const Color3B& color)
    {
        setFontColor(Color4B(color));
    }
    
    void MultiEditBox::setFontColor(const Color4B& color)
    {
        _colText = color;
        if (_editBoxImpl != nullptr)
        {
            _editBoxImpl->setFontColor(color);
        }
    }
    
    void MultiEditBox::setPlaceholderFont(const char* pFontName, int fontSize)
    {
        _placeholderFontName = pFontName;
        _placeholderFontSize = fontSize;
        if (pFontName != nullptr)
        {
            if (_editBoxImpl != nullptr)
            {
                _editBoxImpl->setPlaceholderFont(pFontName, fontSize);
            }
        }
    }
    
    void MultiEditBox::setPlaceholderFontName(const char* pFontName)
    {
        _placeholderFontName = pFontName;
        if (_editBoxImpl != nullptr && _placeholderFontSize != -1)
        {
            _editBoxImpl->setPlaceholderFont(pFontName, _fontSize);
        }
    }
    
    void MultiEditBox::setPlaceholderFontSize(int fontSize)
    {
        _placeholderFontSize = fontSize;
        if (_editBoxImpl != nullptr && _placeholderFontName.length() > 0)
        {
            _editBoxImpl->setPlaceholderFont(_placeholderFontName.c_str(), fontSize);
        }
    }
    
    void MultiEditBox::setPlaceholderFontColor(const Color3B& color)
    {
        setPlaceholderFontColor(Color4B(color));
    }
    
    void MultiEditBox::setPlaceholderFontColor(const Color4B& color)
    {
        _colPlaceHolder = color;
        if (_editBoxImpl != nullptr)
        {
            _editBoxImpl->setPlaceholderFontColor(color);
        }
    }
    
    void MultiEditBox::setPlaceHolder(const char* pText)
    {
        if (pText != nullptr)
        {
            _placeHolder = pText;
            if (_editBoxImpl != nullptr)
            {
                _editBoxImpl->setPlaceHolder(pText);
            }
        }
    }
    
    const char* MultiEditBox::getPlaceHolder(void)
    {
        return _placeHolder.c_str();
    }
    
    void MultiEditBox::setInputMode(MultiEditBox::InputMode inputMode)
    {
        _editBoxInputMode = inputMode;
        if (_editBoxImpl != nullptr)
        {
            _editBoxImpl->setInputMode(inputMode);
        }
    }
    
    void MultiEditBox::setMaxLength(int maxLength)
    {
        _maxLength = maxLength;
        if (_editBoxImpl != nullptr)
        {
            _editBoxImpl->setMaxLength(maxLength);
        }
    }
    
    
    int MultiEditBox::getMaxLength()
    {
        return _maxLength;
    }
    
    void MultiEditBox::setInputFlag(MultiEditBox::InputFlag inputFlag)
    {
        _editBoxInputFlag = inputFlag;
        if (_editBoxImpl != nullptr)
        {
            _editBoxImpl->setInputFlag(inputFlag);
        }
    }
    
    void MultiEditBox::setReturnType(MultiEditBox::KeyboardReturnType returnType)
    {
        if (_editBoxImpl != nullptr)
        {
            _editBoxImpl->setReturnType(returnType);
        }
    }
    
    /* override function */
    void MultiEditBox::setPosition(const Vec2& pos)
    {
        Widget::setPosition(pos);
        if (_editBoxImpl != nullptr)
        {
            _editBoxImpl->setPosition(pos);
        }
    }
    
    void MultiEditBox::setVisible(bool visible)
    {
        Widget::setVisible(visible);
        if (_editBoxImpl != nullptr)
        {
            _editBoxImpl->setVisible(visible);
        }
    }
    
    void MultiEditBox::setContentSize(const Size& size)
    {
        Widget::setContentSize(size);
        if (_editBoxImpl != nullptr)
        {
            _editBoxImpl->setContentSize(size);
        }
    }
    
    void MultiEditBox::adaptRenderers()
    {
        if (_contentSizeDirty)
        {
            _backgroundSprite->setContentSize(_contentSize);
            _backgroundSprite->setPosition(Vec2(_contentSize.width/2, _contentSize.height/2));
        }
    }
    
    void MultiEditBox::setAnchorPoint(const Vec2& anchorPoint)
    {
        Widget::setAnchorPoint(anchorPoint);
        if (_editBoxImpl != nullptr)
        {
            _editBoxImpl->setAnchorPoint(anchorPoint);
        }
    }
    
    std::string MultiEditBox::getDescription() const
    {
        return "MultiEditBox";
    }
    
    void MultiEditBox::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags)
    {
        Widget::visit(renderer, parentTransform, parentFlags);
        if (_editBoxImpl != nullptr)
        {
            _editBoxImpl->visit();
//            _editBoxImpl->adjustTextFieldPosition();
        }
    }
    
    void MultiEditBox::onEnter(void)
    {
#if CC_ENABLE_SCRIPT_BINDING
        if (_scriptType == kScriptTypeJavascript)
        {
            if (ScriptEngineManager::sendNodeEventToJSExtended(this, kNodeOnEnter))
                return;
        }
#endif
        
        Widget::onEnter();
        if (_editBoxImpl != nullptr)
        {
            _editBoxImpl->onEnter();
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        this->schedule(CC_SCHEDULE_SELECTOR(MultiEditBox::updatePosition), CHECK_EDITBOX_POSITION_INTERVAL);
#endif
    }
    
    void MultiEditBox::updatePosition(float dt)
    {
        if (nullptr != _editBoxImpl) {
            _editBoxImpl->updatePosition(dt);
        }
    }
    
    
    void MultiEditBox::onExit(void)
    {
        cocos2d::log("MultiEditBox::onExit");
        Widget::onExit();
        if (_editBoxImpl != nullptr)
        {
            // remove system edit control
            _editBoxImpl->closeKeyboard();
        }
    }
    
    static Rect getRect(Node * pNode)
    {
        Size contentSize = pNode->getContentSize();
        Rect rect = Rect(0, 0, contentSize.width, contentSize.height);
        return RectApplyTransform(rect, pNode->getNodeToWorldTransform());
    }
    
    void MultiEditBox::keyboardWillShow(IMEKeyboardNotificationInfo& info)
    {
        CCLOG("MultiEditBox::keyboardWillShow.......");
        Rect rectTracked = getRect(this);
        // some adjustment for margin between the keyboard and the edit box.
        rectTracked.origin.y -= 4;
        
        // if the keyboard area doesn't intersect with the tracking node area, nothing needs to be done.
        if (!rectTracked.intersectsRect(info.end))
        {
            CCLOG("needn't to adjust view layout.");
            return;
        }
        
        // assume keyboard at the bottom of screen, calculate the vertical adjustment.
        _adjustHeight = info.end.getMaxY() - rectTracked.getMinY();
        // CCLOG("CCEditBox:needAdjustVerticalPosition(%f)", _adjustHeight);
        
        if (_editBoxImpl != nullptr)
        {
//            _editBoxImpl->doAnimationWhenKeyboardMove(info.duration, _adjustHeight);
        }
        if(_keyBoardWillShowCallback) {
            _keyBoardWillShowCallback(info.duration, _adjustHeight);
        }
        
    }
    
    void MultiEditBox::keyboardDidShow(IMEKeyboardNotificationInfo& info)
    {
        if(_keyBoardDidShowCallback) {
            _keyBoardDidShowCallback();
        }
    }
    
    void MultiEditBox::keyboardWillHide(IMEKeyboardNotificationInfo& info)
    {
        // CCLOG("CCEditBox::keyboardWillHide");
        if (_editBoxImpl != nullptr)
        {
//            _editBoxImpl->doAnimationWhenKeyboardMove(info.duration, -_adjustHeight);
        }
        if(_keyBoardWillHideCallback) {
            _keyBoardWillHideCallback(info.duration, _adjustHeight);
        }
    }
    
    void MultiEditBox::keyboardDidHide(IMEKeyboardNotificationInfo& info)
    {
        if(_keyBoardDidHideCallback) {
            _keyBoardDidHideCallback();
        }
    }
    
    void MultiEditBox::setBackgroundColor(const Color4B& color)
    {
        _editBoxImpl->setBackgroundColor(color);
    }
    
#if CC_ENABLE_SCRIPT_BINDING
    void MultiEditBox::registerScriptEditBoxHandler(int handler)
    {
        unregisterScriptEditBoxHandler();
        _scriptEditBoxHandler = handler;
    }
    
    void MultiEditBox::unregisterScriptEditBoxHandler(void)
    {
        if (0 != _scriptEditBoxHandler)
        {
            ScriptEngineManager::getInstance()->getScriptEngine()->removeScriptHandler(_scriptEditBoxHandler);
            _scriptEditBoxHandler = 0;
        }
    }
#endif
    
}

NS_CC_END
