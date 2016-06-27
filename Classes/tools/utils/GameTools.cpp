
//
//  File.cpp
//  DK2.0
//
//  Created by hefeng on 14-10-11.
//
//

#include "GameTools.h"
#include "UIImageView.h"
#include "UIText.h"
#include "utils.h"
#include "PlatformSdk.h"
#include "global_header.h"
#include <regex>

using namespace cocos2d;
using namespace cocos2d::ui;

std::map<GameTools::COLOR_TYPE, GLProgram *> GameTools::programs_pointer = std::map<GameTools::COLOR_TYPE, GLProgram *>();
std::list<std::pair<Vec3, GLProgram *> > GameTools::color_programs_pointer = std::list<std::pair<Vec3, GLProgram *> >();


void GameTools::changeWidgetColorToGray(Widget *widget)
{
    Node *node = widget->getVirtualRenderer();
    changeNodeColorToGray(node);
}

void GameTools::changeNodeColorToGray(Node *node)
{
    GLProgram * p = nullptr;
    auto iter = programs_pointer.find(COLOR_TYPE::NODE_GRAY);
    if(iter != programs_pointer.end()) {
        p = iter->second;
    }
    else {
        p = new GLProgram();
        p->initWithFilenames("gray.vsh", "gray.fsh");
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        p->link();
        p->updateUniforms();
        
        programs_pointer.insert(std::make_pair(COLOR_TYPE::NODE_GRAY, p));
    }
    node->setGLProgram(p);
    if(iter == programs_pointer.end()) {
        p->autorelease();
    }
    for(auto& n : node->getChildren()) {
        changeNodeColorToGray(n);
    }
}
void GameTools::changeNodeColorToNormal(Node *node)
{
    GLProgram * p = nullptr;
    auto iter = programs_pointer.find(COLOR_TYPE::NODE_NORMAL);
    if(iter != programs_pointer.end()) {
        p = iter->second;
    }
    else {
        p = new GLProgram();
        p->initWithFilenames("normal.vsh", "normal.fsh");
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        p->link();
        p->updateUniforms();
        
        programs_pointer.insert(std::make_pair(COLOR_TYPE::NODE_NORMAL, p));
    }
    
    node->setGLProgram(p);
    if(iter == programs_pointer.end()) {
        p->autorelease();
    }
    for(auto& n : node->getChildren()) {
        changeNodeColorToNormal(n);
    }
}

void GameTools::changeWidgetColorToNormal(Widget *widget)
{
    Node *node = widget->getVirtualRenderer();
    changeNodeColorToNormal(node);
}


cocos2d::ui::EditBox* GameTools::createEditBoxFromTextField(cocos2d::Node* root, cocos2d::ui::TextField* tf)
{
    Scale9Sprite* sprite = Scale9Sprite::create();
    auto sizeTextField = tf->getContentSize();
    //为了兼容android输入法显示不全的bug，高度+20。by huangfeng
    auto edit_box = EditBox::create(Size(sizeTextField.width, sizeTextField.height + 20), sprite); //by lvzhangle
    //auto edit_box = EditBox::create(Size(tf->getContentSize().width * IP6TO5_SCALE_X, tf->getContentSize().height * IP6TO5_SCALE_Y), sprite);
    edit_box->setAnchorPoint(tf->getAnchorPoint());
    edit_box->setPosition(tf->getPosition());

    edit_box->setPlaceHolder(tf->getPlaceHolder().c_str());
    edit_box->setPlaceholderFont(tf->getFontName().c_str(), tf->getFontSize());
    edit_box->setPlaceholderFontColor(tf->getPlaceHolderColor());
    
    edit_box->setFont(tf->getFontName().c_str(), tf->getFontSize());
    edit_box->setFontColor(Color4B::BLACK);
    
    edit_box->setInputFlag(cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
    edit_box->setTag(tf->getTag());

    root->addChild(edit_box, tf->getZOrder());
    tf->removeFromParent();
    
    return edit_box;
}
cocos2d::ui::EditBox* GameTools::createEditBoxFromText(cocos2d::Node* root, cocos2d::ui::Text* tf,std::string placeholder,Size size)
{
    Scale9Sprite* sprite = Scale9Sprite::create();
    auto edit_box = EditBox::create(size, sprite);
    
    edit_box->setAnchorPoint(tf->getAnchorPoint());
    edit_box->setPosition(tf->getPosition());
    
    edit_box->setPlaceHolder(placeholder.c_str());
    edit_box->setPlaceholderFont(tf->getFontName().c_str(), tf->getFontSize());
    
    edit_box->setFont(tf->getFontName().c_str(), tf->getFontSize());
    edit_box->setFontColor(Color4B::BLACK);
    
    edit_box->setInputFlag(cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
    edit_box->setTag(tf->getTag());
    root->addChild(edit_box, tf->getZOrder());
    tf->removeFromParent();
    
    return edit_box;
}
cocos2d::ui::Text* GameTools::createTextFromEditBox(cocos2d::Node* root, cocos2d::ui::EditBox* tf ,std::string fontName, int fontSize)
{
    std::string s = tf->getText();
    if(s.empty())
    {
        s = tf->getPlaceHolder();
    }
    auto text = Text::create(s,fontName,fontSize);
    text->setColor(Color3B::BLACK);
    text->setAnchorPoint(tf->getAnchorPoint());
    text->setPosition(tf->getPosition());
    root->addChild(text, tf->getZOrder(),tf->getTag());
    tf->removeFromParentAndCleanup(true);
    
    return text;
}
cocos2d::ui::MultiEditBox* GameTools::createMultiEditBoxFormTextField(cocos2d::Node* root, cocos2d::ui::TextField* tf, const char* file, const uint32_t line)
{
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    float scalex = glview->getScaleX();
    float scaley = glview->getScaleY();
    CCLOG("createMultiEditBoxFormTextField from:%s, %u", file, line);
    Scale9Sprite* sprite = Scale9Sprite::create();
    
    cocos2d::Size contentSize = tf->getContentSize();
    //
    //    lc->setAnchorPoint(Point::ZERO);
    sprite->setContentSize(contentSize);
    //    sprite->addChild(lc);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    auto edit_box = MultiEditBox::create(Size(contentSize.width / 1, contentSize.height / 1), sprite);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    auto edit_box = MultiEditBox::create(Size(contentSize.width, contentSize.height), sprite);
#endif
//    edit_box->setAnchorPoint(tf->getAnchorPoint());
    //    edit_box->setBackgroundColor(Color4B(255, 0, 0, 255));
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    scalex = scaley = 1;
#endif
    edit_box->setPosition(Point(tf->getPosition().x, tf->getPosition().y));
    edit_box->setPlaceholderFont(tf->getFontName().c_str(), tf->getFontSize());
    edit_box->setFont(tf->getFontName().c_str(), tf->getFontSize());
    edit_box->setPlaceholderFontColor(tf->getPlaceHolderColor());
//    edit_box->setFontColor(Color4B::BLACK);
    root->addChild(edit_box, tf->getZOrder());
    tf->removeFromParent();
    
    return edit_box;
}

void GameTools::editBoxLoseInputFocus()
{
    auto pGlView = Director::getInstance()->getOpenGLView();
    if (pGlView)
    {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WP8 && CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
        pGlView->setIMEKeyboardState(false);
#else
        pGlView->setIMEKeyboardState(false, _inputText);
#endif
    }
}

const std::string GameTools::get_chip_image_name(const int amount)
{
    std::string result;
    if(amount <= 4) {
        result = "chip_1_4.png";
    }
    else if(amount <= 24) {
        result = "chip_5_24.png";
    }
    else if(amount <= 49) {
        result = "chip_25_49.png";
    }
    else if(amount <= 99) {
        result = "chip_50_99.png";
    }
    else if(amount <= 499) {
        result = "chip_100_499.png";
    }
    else if(amount <= 999) {
        result = "chip_500_999.png";
    }
    else {
        result = "chip_1000_.png";
    }
    result = "login_res/game_res/chips/" + result;
    return std::move(result);
}

const std::vector<std::pair<std::string, std::pair<std::string, std::string> > >& GameTools::get_languages()
{
    static std::vector<std::pair<std::string, std::pair<std::string, std::string> > > result;
    if(result.empty()) {
        cocos2d::log("get_languages");
        static rapidjson::Document doc;
        doc.SetArray();
        std::string path = cocos2d::FileUtils::getInstance()->fullPathForFilename("languages.json");
        const std::string content = tools::read_file(path);
        doc.Parse<0>(content.c_str());
        
        assert(!doc.HasParseError());
        for (auto iter = doc.onBegin(); iter != doc.onEnd(); ++iter) {
            auto& data = *iter;
            for (auto it = data.MemberonBegin(); it != data.MemberonEnd(); ++it) {
                std::pair<std::string, std::string> language;
                auto& value = it->value;
                language.first = value.onBegin()->GetString();
                language.second = value[1].GetString();
                result.push_back(std::make_pair(it->name.GetString(), language));
            }
        }
    }
    
    return result;
}

Texture2D* GameTools::get_languages_pic(std::string key)
{
    static std::map<std::string, std::string> language_pic_strings;
    
    if(language_pic_strings.empty()) {
        cocos2d::log("get_languages");
        static rapidjson::Document doc;
        doc.SetObject();
        std::string path = cocos2d::FileUtils::getInstance()->fullPathForFilename("language_pic_root.json");
            
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            const std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            const std::string content = tools::read_file(path);
#endif
        doc.Parse<0>(content.c_str());
        assert(!doc.HasParseError());
        doc.Parse<0>(content.c_str());
            for (auto iter = doc.MemberonBegin(); iter != doc.MemberonEnd(); ++iter) {
                language_pic_strings.insert(std::make_pair(iter->name.GetString(), iter->value.GetString()));
            }
    }
    std::string realkey="";
    if(isChineseLanguage()){
        realkey=key+"_ch";
    }
    else
    {
        realkey="en_"+key;
    }
    auto iter = language_pic_strings.find(realkey);
    if(iter != language_pic_strings.end()) {
        std::string imgpath=iter->second;
        Texture2D* texture = TextureCache::sharedTextureCache()->addImage(imgpath);
        return texture;
    }
    else {
        cocos2d::log("error:local %s description is empty!!", key.c_str());
        Texture2D* texture = TextureCache::sharedTextureCache()->addImage("image_en/en_word_time_record.png");
        return texture;
    }
}

std::string GameTools::get_languages_pic_key(std::string key)
{
    static std::map<std::string, std::string> language_pic_strings;
    
    if(language_pic_strings.empty()) {
        cocos2d::log("get_languages");
        static rapidjson::Document doc;
        doc.SetObject();
        std::string path = cocos2d::FileUtils::getInstance()->fullPathForFilename("language_pic_root.json");
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        const std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        const std::string content = tools::read_file(path);
#endif
        doc.Parse<0>(content.c_str());
        assert(!doc.HasParseError());
        doc.Parse<0>(content.c_str());
        for (auto iter = doc.MemberonBegin(); iter != doc.MemberonEnd(); ++iter) {
            language_pic_strings.insert(std::make_pair(iter->name.GetString(), iter->value.GetString()));
        }
    }
    std::string realkey="";
    
    int present_language_type=getLanguage();
    switch(present_language_type)
    {
        case (int)PlatformSdk::LanguageType::CHINESE:
            realkey=key;
            break;
        case (int)PlatformSdk::LanguageType::ENGLISH:
             realkey="en_"+key;
            break;
        case (int)PlatformSdk::LanguageType::TRADITIONAL_CHINESE:
            realkey="ft_"+key;
            break;
        case (int)PlatformSdk::LanguageType::ITALIAN:
            break;
        case (int)PlatformSdk::LanguageType::FRENCH:
            break;
        case (int)PlatformSdk::LanguageType::GERMAN:
            break;
        case (int)PlatformSdk::LanguageType::SPANISH:
            break;
        case (int)PlatformSdk::LanguageType::DUTCH:
            break;
        case (int)PlatformSdk::LanguageType::RUSSIAN:
            break;
        case (int)PlatformSdk::LanguageType::KOREAN:
            break;
        case (int)PlatformSdk::LanguageType::JAPANESE:
            break;
        default:
            break;
    }
    auto iter = language_pic_strings.find(realkey);
    if(iter != language_pic_strings.end()) {
        std::string imgpath=iter->second;
        return imgpath;
    }
    else {
        cocos2d::log("error:local %s description is empty!!", key.c_str());
        //Texture2D* texture = TextureCache::sharedTextureCache()->addImage("image_en/en_word_time_record.png");
        return "Nopic";
    }
}

void GameTools::set_pic_change_sprite(cocos2d::Sprite* btn,std::string pic_key )
{
    std::string texture_root=get_languages_pic_key(pic_key);
    if(texture_root.compare("Nopic")==0){
        return;
    }
    else
    {
        Texture2D* texture = TextureCache::sharedTextureCache()->addImage(texture_root);
        btn->setTexture(texture);
    }
}

void GameTools::set_pic_change_imageview(cocos2d::ui::ImageView* btn,std::string pic_key)
{
    std::string texture_root=get_languages_pic_key(pic_key);
    if(texture_root.compare("Nopic")==0){
        return;
    }
    else
    {
        btn->loadTexture(texture_root);
    }
}

void GameTools::set_pic_change(cocos2d::ui::Button* btn,std::string normal_key , std::string press_key)//设置btn 根据语言切换变换
{
    std::string texture_temp_normal=get_languages_pic_key(normal_key);
    std::string texture_temp_press=get_languages_pic_key(press_key);
    if((texture_temp_normal.compare("Nopic")==0)||(texture_temp_press.compare("Nopic")==0))
    {
        return;
    }
    else
    {
        btn->loadTextureNormal(texture_temp_normal,TextureResType::LOCAL );
        btn->loadTexturePressed(texture_temp_press,TextureResType::LOCAL );
    }
}

//正则匹配 http://www.cnblogs.com/zxin/archive/2013/01/26/2877765.html
bool GameTools::isRegexMath(const std::string str, INPUT_TYPE inputType/* = INPUT_TYPE::CN_EN_NUM_UL*/)
{
    
    switch(inputType)
    {
        case INPUT_TYPE::NUMBER:
            return std::regex_match(str, std::regex("^[0-9]*$"));
            break;
        case INPUT_TYPE::EMAIL:
            return std::regex_match(str, std::regex("^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$"));
            break;
        case INPUT_TYPE::PHONE_NUMBER:
            return std::regex_match(str, std::regex("^(13[0-9]|14[5|7]|15[0|1|2|3|5|6|7|8|9]|18[0|1|2|3|5|6|7|8|9])\d{8}$"));
            break;
        case INPUT_TYPE::NUM_EN_UL:
            return std::regex_match(str, std::regex("^\w+$"));
            break;
        case INPUT_TYPE::CN_EN_NUM_UL:
            return std::regex_match(str, std::regex("^[\u4E00-\u9FA5A-Za-z0-9_]+$"));
            break;
        default:
            return std::regex_match(str, std::regex("^[\u4E00-\u9FA5A-Za-z0-9_]+$"));
            break;
    }
}

//取前max个字符
std::string GameTools::stringOverflow(const std::string str, int max/* = 10*/)
{
    if(str.size() > max)
    {
        std::string t;
        int strLen = 0;
        for(int i = 0; i < str.size(); ++i)
        {
            
            if(++strLen > max)
            {
                t.append("...");
                break;
            }
            if((int)str[i] < 255 && str[i] > 0)//扩充的ASCII字符范围为0-255,如是,处理一个字节
            {
                t.append(str.substr(i,1));
            }
            else//<0,>255的是汉字,处理两个字节
            {
                t.append(str.substr(i,3));
                i+=2;
            }
        }
        return t;
    }
    else
    {
        return str;
    }
}

std::string GameTools::getStringDigit(const std::string str)
{
    std::string backStr;
    std::string::size_type pos = 0;
    while((pos = str.find_first_of(str, pos)) != std::string::npos)
    {
        if(isdigit(str[pos]))
        {
            backStr += str[pos];
        }
        ++pos;
    }
    return backStr;
}

#pragma mark - isValidPicture检测图片格式有效
bool GameTools::isValidPicture(std::string fileName)
{
    Image *image = new (std::nothrow) Image();
    if(nullptr != image)
    {
        bool bRet = image->initWithImageFile(fileName);
        if(!bRet)
        {
            CC_SAFE_RELEASE(image);
            return false;
        }
    }
    CC_SAFE_RELEASE(image);
    return true;
}


bool GameTools::isChineseLanguage()
{
    return PlatformSdk::LanguageType::CHINESE == (PlatformSdk::LanguageType)GameTools::getLanguage();
}

 bool GameTools::isEnglishLanguage()
{
     return PlatformSdk::LanguageType::ENGLISH == (PlatformSdk::LanguageType)GameTools::getLanguage();
}

void GameTools::setLanguage(int type)
{
    UserDefault::getInstance()->setIntegerForKey("LanguageSetting", type);
}


int GameTools::getLanguage()
{
    return UserDefault::getInstance()->getIntegerForKey("LanguageSetting", (int)PlatformSdk::getInstance()->getCurrentLanguage());
}


