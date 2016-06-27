//
//  MultiEditBoxImpl-ios.cpp
//  MultiEditBox
//
//  Created by duanmt on 15/5/14.
//
//

#include "MultiEditBoxImpl-ios.h"
#include "platform/CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#define kLabelZOrder  9999
#include "MultiEditBox.h"
#include "base/CCDirector.h"
#include "2d/CCLabel.h"
#import "platform/ios/CCEAGLView-ios.h"

#define getMultiEditBoxImplIOS() ((cocos2d::ui::MultiEditBoxImplIOS*)editBox_)

static const int CC_EDIT_BOX_PADDING = 5;

@implementation UICustomUITextView
- (CGRect)textRectForBounds:(CGRect)bounds
{
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    
    float padding = CC_EDIT_BOX_PADDING * glview->getScaleX() / glview->getContentScaleFactor();
    return CGRectMake(bounds.origin.x + padding, bounds.origin.y + padding,
                      bounds.size.width - padding*2, bounds.size.height - padding*2);
}
- (CGRect)editingRectForBounds:(CGRect)bounds {
    return [self textRectForBounds:bounds];
}
@end


@implementation MultiEditBoxImplIOS_objc

@synthesize textField = textField_;
@synthesize editState = editState_;
@synthesize editBox = editBox_;

- (void)dealloc
{
    [textField_ resignFirstResponder];
    [textField_ removeFromSuperview];
    self.textField = NULL;
    [super dealloc];
}

-(id) initWithFrame: (CGRect) frameRect editBox: (void*) editBox
{
    self = [super init];
    
    if (self)
    {
        editState_ = NO;
        self.textField = [[[UICustomUITextView alloc] initWithFrame: frameRect] autorelease];
        
        [textField_ setTextColor:[UIColor whiteColor]];
        //TODO: need to delete hard code here.
        textField_.font = [UIFont systemFontOfSize:frameRect.size.height*2/3];
//        textField_.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
        textField_.backgroundColor = [UIColor clearColor];
//        textField_.borderStyle = UITextBorderStyleNone;
        textField_.delegate = self;
        textField_.hidden = true;
        textField_.returnKeyType = UIReturnKeyDefault;
//        [textField_ addTarget:self action:@selector(textChanged) forControlEvents:UIControlEventEditingChanged];
        [self setBackgroundColor:[UIColor redColor]];
        self.editBox = editBox;
    }
    
    return self;
}

-(void) setBackgroundColor:(UIColor*) color
{
    [textField_ setBackgroundColor:color];
}

-(void) doAnimationWhenKeyboardMoveWithDuration:(float)duration distance:(float)distance
{
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *eaglview = (CCEAGLView *) view->getEAGLView();
    
    [eaglview doAnimationWhenKeyboardMoveWithDuration:duration distance:distance];
}

-(void) setPosition:(CGPoint) pos
{
    CGRect frame = [textField_ frame];
    frame.origin = pos;
    [textField_ setFrame:frame];
}

-(void) setContentSize:(CGSize) size
{
    CGRect frame = [textField_ frame];
    frame.size = size;
    [textField_ setFrame:frame];
}

-(void) visit
{
    
}

-(void) openKeyboard
{
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *eaglview = (CCEAGLView *) view->getEAGLView();
    
    [eaglview addSubview:textField_];
    [textField_ becomeFirstResponder];
}

-(void) closeKeyboard
{
    [textField_ resignFirstResponder];
    [textField_ removeFromSuperview];
}

- (BOOL)textFieldShouldReturn:(UITextView *)sender
{
    if (sender == textField_) {
        [sender resignFirstResponder];
    }
    return NO;
}

-(void)animationSelector
{
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *eaglview = (CCEAGLView *) view->getEAGLView();
    
    [eaglview doAnimationWhenAnotherEditBeClicked];
}

- (BOOL)textFieldShouldBeginEditing:(UITextView *)sender        // return NO to disallow editing.
{
    CCLOG("textFieldShouldBeginEditing...");
    editState_ = YES;
    
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *eaglview = (CCEAGLView *) view->getEAGLView();
    
    if ([eaglview isKeyboardShown])
    {
        [self performSelector:@selector(animationSelector) withObject:nil afterDelay:0.0f];
    }
    cocos2d::ui::MultiEditBoxImplIOS* editBox = static_cast<cocos2d::ui::MultiEditBoxImplIOS*>(editBox_);
    
    cocos2d::ui::MultiEditBoxDelegate* pDelegate = editBox->getDelegate();
    if (pDelegate != NULL)
    {
        pDelegate->editBoxEditingDidBegin(editBox->getEditBox());
    }
    
#if CC_ENABLE_SCRIPT_BINDING
    cocos2d::ui::MultiEditBox*  pEditBox= editBox->getEditBox();
    if (NULL != pEditBox && 0 != pEditBox->getScriptEditBoxHandler())
    {
        cocos2d::CommonScriptData data(pEditBox->getScriptEditBoxHandler(), "began",pEditBox);
        cocos2d::ScriptEvent event(cocos2d::kCommonEvent,(void*)&data);
        cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
#endif
    return YES;
}

- (BOOL)textFieldShouldEndEditing:(UITextView *)sender
{
    CCLOG("textFieldShouldEndEditing...");
    editState_ = NO;
    getMultiEditBoxImplIOS()->refreshInactiveText();
    
    cocos2d::ui::MultiEditBoxDelegate* pDelegate = getMultiEditBoxImplIOS()->getDelegate();
    if (pDelegate != NULL)
    {
        pDelegate->editBoxEditingDidEnd(getMultiEditBoxImplIOS()->getEditBox());
        pDelegate->editBoxReturn(getMultiEditBoxImplIOS()->getEditBox());
    }
    
#if CC_ENABLE_SCRIPT_BINDING
    cocos2d::ui::MultiEditBox*  pEditBox= getMultiEditBoxImplIOS()->getEditBox();
    if (NULL != pEditBox && 0 != pEditBox->getScriptEditBoxHandler())
    {
        cocos2d::CommonScriptData data(pEditBox->getScriptEditBoxHandler(), "ended",pEditBox);
        cocos2d::ScriptEvent event(cocos2d::kCommonEvent,(void*)&data);
        cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
        memset(data.eventName, 0, sizeof(data.eventName));
        strncpy(data.eventName, "return", sizeof(data.eventName));
        event.data = (void*)&data;
        cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
#endif
    
    if(editBox_ != nil)
    {
        getMultiEditBoxImplIOS()->onEndEditing();
    }
    return YES;
}

/**
 * Delegate method called before the text has been changed.
 * @param textField The text field containing the text.
 * @param range The range of characters to be replaced.
 * @param string The replacement string.
 * @return YES if the specified text range should be replaced; otherwise, NO to keep the old text.
 */
- (BOOL)textField:(UITextView *) textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    if (getMultiEditBoxImplIOS()->getMaxLength() < 0)
    {
        return YES;
    }
    
    NSUInteger oldLength = [textField.text length];
    NSUInteger replacementLength = [string length];
    NSUInteger rangeLength = range.length;
    
    NSUInteger newLength = oldLength - rangeLength + replacementLength;
    
    return newLength <= getMultiEditBoxImplIOS()->getMaxLength();
}

/**
 * Called each time when the text field's text has changed.
 */
- (void) textChanged
{
    // NSLog(@"text is %@", self.textField.text);
    auto pDelegate = getMultiEditBoxImplIOS()->getDelegate();
    if (pDelegate != NULL)
    {
        pDelegate->editBoxTextChanged(getMultiEditBoxImplIOS()->getEditBox(), getMultiEditBoxImplIOS()->getText());
    }
    
#if CC_ENABLE_SCRIPT_BINDING
    auto  pEditBox= getMultiEditBoxImplIOS()->getEditBox();
    if (NULL != pEditBox && 0 != pEditBox->getScriptEditBoxHandler())
    {
        cocos2d::CommonScriptData data(pEditBox->getScriptEditBoxHandler(), "changed",pEditBox);
        cocos2d::ScriptEvent event(cocos2d::kCommonEvent,(void*)&data);
        cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
#endif
}

@end


NS_CC_BEGIN

namespace ui {
    
    MultiEditBoxImpl* __createSystemEditBox(MultiEditBox* pEditBox)
    {
        return new MultiEditBoxImplIOS(pEditBox);
    }
    
    MultiEditBoxImplIOS::MultiEditBoxImplIOS(MultiEditBox* pEditText)
    : MultiEditBoxImpl(pEditText)
    , _label(nullptr)
    , _labelPlaceHolder(nullptr)
    , _anchorPoint(Vec2(0.5f, 0.5f))
    , _systemControl(nullptr)
    , _maxTextLength(-1)
    {
    }
    
    MultiEditBoxImplIOS::~MultiEditBoxImplIOS()
    {
        [_systemControl release];
    }
    
    void MultiEditBoxImplIOS::doAnimationWhenKeyboardMove(float duration, float distance)
    {
        if ([_systemControl isEditState] || distance < 0.0f)
        {
            [_systemControl doAnimationWhenKeyboardMoveWithDuration:duration distance:distance];
        }
    }
    
    bool MultiEditBoxImplIOS::initWithSize(const Size& size)
    {
        do
        {
            auto glview = cocos2d::Director::getInstance()->getOpenGLView();
            
            CGRect rect = CGRectMake(0, 0, size.width * glview->getScaleX(),size.height * glview->getScaleY());
            
            CCEAGLView *eaglview = static_cast<CCEAGLView *>(glview->getEAGLView());
            float factor = eaglview.contentScaleFactor;
            rect.size.width /= factor;
            rect.size.height /= factor;
            
            _systemControl = [[MultiEditBoxImplIOS_objc alloc] initWithFrame:rect editBox:this];
            if (!_systemControl) break;
            
            initInactiveLabels(size);
            setContentSize(size);
            
            return true;
        }while (0);
        
        return false;
    }
    
    void MultiEditBoxImplIOS::initInactiveLabels(const Size& size)
    {
        const char* pDefaultFontName = [[_systemControl.textField.font fontName] UTF8String];
        
        _label = Label::create();
        _label->setAnchorPoint(Vec2(0, 0.5f));
        _label->setColor(Color3B::WHITE);
        _label->setVisible(false);
        _editBox->addChild(_label, kLabelZOrder);
        
        _labelPlaceHolder = Label::create();
        // align the text vertically center
        _labelPlaceHolder->setAnchorPoint(Vec2(0, 0.5f));
        _labelPlaceHolder->setColor(Color3B::GRAY);
        _editBox->addChild(_labelPlaceHolder, kLabelZOrder);
        
        setFont(pDefaultFontName, size.height*2/3);
        setPlaceholderFont(pDefaultFontName, size.height*2/3);
    }
    
    void MultiEditBoxImplIOS::placeInactiveLabels()
    {
        _label->setPosition(CC_EDIT_BOX_PADDING, _contentSize.height / 2.0f);
        _labelPlaceHolder->setPosition(CC_EDIT_BOX_PADDING, _contentSize.height / 2.0f);
    }
    
    void MultiEditBoxImplIOS::setInactiveText(const char* pText)
    {
        if(_systemControl.textField.secureTextEntry == YES)
        {
            std::string passwordString;
            for(int i = 0; i < strlen(pText); ++i)
                passwordString.append("\u25CF");
            _label->setString(passwordString.c_str());
        }
        else
            _label->setString(getText());
        
        // Clip the text width to fit to the text box
        float fMaxWidth = _editBox->getContentSize().width - CC_EDIT_BOX_PADDING * 2;
        Size labelSize = _label->getContentSize();
        if(labelSize.width > fMaxWidth) {
            _label->setDimensions(fMaxWidth,labelSize.height);
        }
    }
    
    void MultiEditBoxImplIOS::setFont(const char* pFontName, int fontSize)
    {
        bool isValidFontName = true;
        if(pFontName == NULL || strlen(pFontName) == 0) {
            isValidFontName = false;
        }
        
        CCEAGLView *eaglview = static_cast<CCEAGLView *>(cocos2d::Director::getInstance()->getOpenGLView()->getEAGLView());
        float retinaFactor = eaglview.contentScaleFactor;
        NSString * fntName = [NSString stringWithUTF8String:pFontName];
        
        auto glview = cocos2d::Director::getInstance()->getOpenGLView();
        
        float scaleFactor = glview->getScaleX();
        UIFont *textFont = nil;
        if (isValidFontName) {
            textFont = [UIFont fontWithName:fntName size:fontSize * scaleFactor / retinaFactor];
        }
        
        if (!isValidFontName || textFont == nil){
            textFont = [UIFont systemFontOfSize:fontSize * scaleFactor / retinaFactor];
        }
        
        if(textFont != nil) {
            [_systemControl.textField setFont:textFont];
        }
        
        _label->setSystemFontName(pFontName);
        _label->setSystemFontSize(fontSize);
    }
    
    void MultiEditBoxImplIOS::setFontColor(const Color4B& color)
    {
        _systemControl.textField.textColor = [UIColor colorWithRed:color.r / 255.0f green:color.g / 255.0f blue:color.b / 255.0f alpha:color.a / 255.f];
        _label->setTextColor(color);
    }
    
    void MultiEditBoxImplIOS::setPlaceholderFont(const char* pFontName, int fontSize)
    {
        _labelPlaceHolder->setSystemFontName(pFontName);
        _labelPlaceHolder->setSystemFontSize(fontSize);
    }
    
    void MultiEditBoxImplIOS::setPlaceholderFontColor(const Color4B &color)
    {
        _labelPlaceHolder->setTextColor(color);
    }
    
    void MultiEditBoxImplIOS::setInputMode(MultiEditBox::InputMode inputMode)
    {
        switch (inputMode)
        {
            case MultiEditBox::InputMode::EMAIL_ADDRESS:
                _systemControl.textField.keyboardType = UIKeyboardTypeEmailAddress;
                break;
            case MultiEditBox::InputMode::NUMERIC:
                _systemControl.textField.keyboardType = UIKeyboardTypeDecimalPad;
                break;
            case MultiEditBox::InputMode::PHONE_NUMBER:
                _systemControl.textField.keyboardType = UIKeyboardTypePhonePad;
                break;
            case MultiEditBox::InputMode::URL:
                _systemControl.textField.keyboardType = UIKeyboardTypeURL;
                break;
            case MultiEditBox::InputMode::DECIMAL:
                _systemControl.textField.keyboardType = UIKeyboardTypeDecimalPad;
                break;
            case MultiEditBox::InputMode::SINGLE_LINE:
                _systemControl.textField.keyboardType = UIKeyboardTypeDefault;
                break;
            default:
                _systemControl.textField.keyboardType = UIKeyboardTypeDefault;
                break;
        }
    }
    
    void MultiEditBoxImplIOS::setMaxLength(int maxLength)
    {
        _maxTextLength = maxLength;
    }
    
    int MultiEditBoxImplIOS::getMaxLength()
    {
        return _maxTextLength;
    }
    
    void MultiEditBoxImplIOS::setInputFlag(MultiEditBox::InputFlag inputFlag)
    {
        switch (inputFlag)
        {
            case MultiEditBox::InputFlag::PASSWORD:
                _systemControl.textField.secureTextEntry = YES;
                break;
            case MultiEditBox::InputFlag::INITIAL_CAPS_WORD:
                _systemControl.textField.autocapitalizationType = UITextAutocapitalizationTypeWords;
                break;
            case MultiEditBox::InputFlag::INITIAL_CAPS_SENTENCE:
                _systemControl.textField.autocapitalizationType = UITextAutocapitalizationTypeSentences;
                break;
            case MultiEditBox::InputFlag::INTIAL_CAPS_ALL_CHARACTERS:
                _systemControl.textField.autocapitalizationType = UITextAutocapitalizationTypeAllCharacters;
                break;
            case MultiEditBox::InputFlag::SENSITIVE:
                _systemControl.textField.autocorrectionType = UITextAutocorrectionTypeNo;
                break;
            default:
                break;
        }
    }
    
    void MultiEditBoxImplIOS::setReturnType(MultiEditBox::KeyboardReturnType returnType)
    {
        switch (returnType) {
            case MultiEditBox::KeyboardReturnType::DEFAULT:
                _systemControl.textField.returnKeyType = UIReturnKeyDefault;
                break;
            case MultiEditBox::KeyboardReturnType::DONE:
                _systemControl.textField.returnKeyType = UIReturnKeyDone;
                break;
            case MultiEditBox::KeyboardReturnType::SEND:
                _systemControl.textField.returnKeyType = UIReturnKeySend;
                break;
            case MultiEditBox::KeyboardReturnType::SEARCH:
                _systemControl.textField.returnKeyType = UIReturnKeySearch;
                break;
            case MultiEditBox::KeyboardReturnType::GO:
                _systemControl.textField.returnKeyType = UIReturnKeyGo;
                break;
            default:
                _systemControl.textField.returnKeyType = UIReturnKeyDefault;
                break;
        }
    }
    
    bool MultiEditBoxImplIOS::isEditing()
    {
        return [_systemControl isEditState] ? true : false;
    }
    
    void MultiEditBoxImplIOS::refreshInactiveText()
    {
        const char* text = getText();
        if(_systemControl.textField.hidden == YES)
        {
            setInactiveText(text);
            if(strlen(text) == 0)
            {
                _label->setVisible(false);
                _labelPlaceHolder->setVisible(true);
            }
            else
            {
                _label->setVisible(true);
                _labelPlaceHolder->setVisible(false);
            }
        }
    }
    
    void MultiEditBoxImplIOS::setText(const char* text)
    {
        NSString* nsText =[NSString stringWithUTF8String:text];
        if ([nsText compare:_systemControl.textField.text] != NSOrderedSame)
        {
            _systemControl.textField.text = nsText;
        }
        
        refreshInactiveText();
    }
    
    void MultiEditBoxImplIOS::setEditable(const bool edit)
    {
        [_systemControl.textField setEditable:edit];
    }
    
    void MultiEditBoxImplIOS::setMultLine(const bool edit){
        
    }
    
    NSString* removeSiriString_multi_editbox(NSString* str)
    {
        NSString* siriString = @"\xef\xbf\xbc";
        return [str stringByReplacingOccurrencesOfString:siriString withString:@""];
    }
    
    const char*  MultiEditBoxImplIOS::getText(void)
    {
        return [removeSiriString_multi_editbox(_systemControl.textField.text) UTF8String];
    }
    
    void MultiEditBoxImplIOS::setPlaceHolder(const char* pText)
    {
//        assert(0);
//        _systemControl.textField.placeholder = [NSString stringWithUTF8String:pText];
        _labelPlaceHolder->setString(pText);
    }
    
    static CGPoint convertDesignCoordToScreenCoord(const Vec2& designCoord)
    {
        auto glview = cocos2d::Director::getInstance()->getOpenGLView();
        CCEAGLView *eaglview = (CCEAGLView *) glview->getEAGLView();
        
        float viewH = (float)[eaglview getHeight];
        
        Vec2 visiblePos = Vec2(designCoord.x * glview->getScaleX(), designCoord.y * glview->getScaleY());
        Vec2 screenGLPos = visiblePos + glview->getViewPortRect().origin;
        
        CGPoint screenPos = CGPointMake(screenGLPos.x, viewH - screenGLPos.y);
        
        float factor = eaglview.contentScaleFactor;
        screenPos.x = screenPos.x / factor;
        screenPos.y = screenPos.y / factor;
        
        CCLOGINFO("[EditBox] pos x = %f, y = %f", screenGLPos.x, screenGLPos.y);
        return screenPos;
    }
    
    void MultiEditBoxImplIOS::setPosition(const Vec2& pos)
    {
        _position = pos;
        adjustTextFieldPosition();
    }
    
    void MultiEditBoxImplIOS::setVisible(bool visible)
    {
        //    _systemControl.textField.hidden = !visible;
    }
    
    void MultiEditBoxImplIOS::setContentSize(const Size& size)
    {
        _contentSize = size;
        //CCLOG("[Edit text] content size = (%f, %f)", size.width, size.height);
        placeInactiveLabels();
        auto glview = cocos2d::Director::getInstance()->getOpenGLView();
        CGSize controlSize = CGSizeMake(size.width * glview->getScaleX(),size.height * glview->getScaleY());
        
        CCEAGLView *eaglview = static_cast<CCEAGLView *>(glview->getEAGLView());
        float factor = eaglview.contentScaleFactor;
        controlSize.width /= factor;
        controlSize.height /= factor;
        
        [_systemControl setContentSize:controlSize];
    }
    
    void MultiEditBoxImplIOS::setAnchorPoint(const Vec2& anchorPoint)
    {
        //CCLOG("[Edit text] anchor point = (%f, %f)", anchorPoint.x, anchorPoint.y);
        _anchorPoint = anchorPoint;
        setPosition(_position);
    }
    
    void MultiEditBoxImplIOS::visit(void)
    {
    }
    
    void MultiEditBoxImplIOS::onEnter(void)
    {
        adjustTextFieldPosition();
        const char* pText = getText();
        if (pText) {
            setInactiveText(pText);
        }
    }
    
    void MultiEditBoxImplIOS::updatePosition(float dt)
    {
        if (nullptr != _systemControl) {
            this->adjustTextFieldPosition();
        }
    }
    
    void MultiEditBoxImplIOS::setBackgroundColor(const Color4B& color)
    {
        [_systemControl.textField setBackgroundColor:[UIColor colorWithRed:color.r/255.0 green:color.g/255.0 blue:color.b/255.0 alpha:1.0]];
    }
    
    void MultiEditBoxImplIOS::adjustTextFieldPosition()
    {
        Size contentSize = _editBox->getContentSize();
        Rect rect = Rect(0, 0, contentSize.width, contentSize.height);
        rect = RectApplyAffineTransform(rect, _editBox->nodeToWorldTransform());
        
        Vec2 designCoord = Vec2(rect.origin.x, rect.origin.y + rect.size.height);
        [_systemControl setPosition:convertDesignCoordToScreenCoord(designCoord)];
    }
    
    void MultiEditBoxImplIOS::openKeyboard()
    {
        _label->setVisible(false);
        _labelPlaceHolder->setVisible(false);
        
        _systemControl.textField.hidden = NO;
        [_systemControl openKeyboard];
    }
    
    void MultiEditBoxImplIOS::closeKeyboard()
    {
        [_systemControl closeKeyboard];
    }
    
    void MultiEditBoxImplIOS::onEndEditing()
    {
        _systemControl.textField.hidden = YES;
        if(strlen(getText()) == 0)
        {
            _label->setVisible(false);
            _labelPlaceHolder->setVisible(true);
        }
        else
        {
            _label->setVisible(true);
            _labelPlaceHolder->setVisible(false);
            setInactiveText(getText());
        }
    }
    
}

NS_CC_END
#endif
