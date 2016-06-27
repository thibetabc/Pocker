//
//  File.h
//  DK2.0
//
//  Created by hefeng on 14-10-11.
//
//

#ifndef __DK2_0__File__
#define __DK2_0__File__

#include <map>
#include "cocos2d.h"

#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "MultiEditBox.h"


class GameTools
{
public:
    
    enum class COLOR_TYPE
    {
        NODE_GRAY,
        NODE_NORMAL,
    };
    
    enum class INPUT_TYPE
    {
        NUMBER, //纯数字
        EMAIL,  //email
        PHONE_NUMBER, //手机号
        NUM_EN_UL, //数字、英文、下划线
        CN_EN_NUM_UL,//中文、英文、数字包括下划线
    };
    
public:
    
    
    /**
     *  将传进来的node渲染成灰色,子node也变
     *
     *  @param node 改变颜色的node,
     */
    static void changeNodeColorToGray(cocos2d::Node *node);
    static void changeWidgetColorToGray(cocos2d::ui::Widget *widget);
    /**
     *  恢复原色
     *
     *  @param node
     */
    static void changeNodeColorToNormal(cocos2d::Node *node);
    static void changeWidgetColorToNormal(cocos2d::ui::Widget *widget);


    
    static std::map<COLOR_TYPE, cocos2d::GLProgram *> programs_pointer;
    static std::list<std::pair<cocos2d::Vec3, cocos2d::GLProgram *> > color_programs_pointer;
    
    static cocos2d::ui::EditBox* createEditBoxFromTextField(cocos2d::Node* root, cocos2d::ui::TextField* tf);
    static cocos2d::ui::EditBox* createEditBoxFromText(cocos2d::Node* root, cocos2d::ui::Text* tf,std::string placeholder,cocos2d::Size size);
    static cocos2d::ui::Text* createTextFromEditBox(cocos2d::Node* root, cocos2d::ui::EditBox* tf ,std::string fontName, int fontSize);
    static cocos2d::ui::MultiEditBox* createMultiEditBoxFormTextField(cocos2d::Node* root, cocos2d::ui::TextField* tf, const char* file, const uint32_t line);
    
    
    const static std::string get_chip_image_name(const int amount);
    
    const static std::vector<std::pair<std::string, std::pair<std::string, std::string> > >& get_languages();
    
    static cocos2d::Texture2D* get_languages_pic(std::string key);
    static std::string get_languages_pic_key(std::string key);
    //设置按钮，精灵，图片多语言转换
    static void set_pic_change(cocos2d::ui::Button* btn,std::string normal_key , std::string press_key);
    static void set_pic_change_sprite(cocos2d::Sprite* btn,std::string pic_key );
    static void set_pic_change_imageview(cocos2d::ui::ImageView* btn,std::string pic_key);
    
    static void editBoxLoseInputFocus();
    
    //正则匹配
    static bool isRegexMath(const std::string str, INPUT_TYPE inputType = INPUT_TYPE::CN_EN_NUM_UL);
    
    //文字太长+显示省略号...
    static std::string stringOverflow(const std::string str, int max = 10);
    //返回字符中的数字
    static std::string getStringDigit(const std::string str);
    static bool isValidPicture(std::string fileName);
    
    //中文
    static bool isChineseLanguage();
    static bool isEnglishLanguage();
    static void setLanguage(int type);
    static int getLanguage();
    static inline long long millisecondNow()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    }
    
    
    
};

#endif /* defined(__DK2_0__File__) */
