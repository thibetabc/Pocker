#pragma mark
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace cocos2d;
using namespace cocos2d::ui;
class SpriteHeader : ImageView {
public:
    
    static SpriteHeader* create(const std::string& filename);
    virtual bool init(const std::string& filename);
    
};