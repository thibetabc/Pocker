//
//  ShaderTexture.cpp
//  pokerclient
//
//  Created by MirrorMiko on 15/7/8.
//
//

#include "ShaderSprite.h"
#include "GameDataManager.h"
#include "FileManager.h"
#include "cmd_def.h"
#include "GameTools.h"


NS_CC_BEGIN

ShaderSprite::ShaderSprite()
:_mask(nullptr)
{
    
}

ShaderSprite* ShaderSprite::create(const std::string& filename, const std::string& maskname)
{
    ShaderSprite *sprite = new (std::nothrow) ShaderSprite();
    if (sprite && sprite->initWithFile(filename, maskname))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool ShaderSprite::initWithFile(const std::string& filename, const std::string& maskname){
    CCASSERT(filename.size()>0 && maskname.size() > 0, "Invalid filename for sprite");
    _fileName = filename;
    bool ok = false;
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(filename);
    _mask = Director::getInstance()->getTextureCache()->addImage(maskname);
    if(nullptr == _mask)
    {
        log("Warning: [%s] Invalid maskname!", maskname.c_str());
    }
    if (texture)
    {
        Rect rect = Rect::ZERO;
        rect.size = texture->getContentSize();
        ok = initWithTexture(texture, rect, false);
//        cocos2d::log("rect:%f, %f", rect.size.width, rect.size.height);
    }
    // don't release here.
    // when load texture failed, it's better to get a "transparent" sprite then a crashed program
    // this->release();
    return true;
}

bool ShaderSprite::initWithTexture(Texture2D *texture, const Rect& rect, bool rotated){
    bool r = Sprite::initWithTexture(texture, rect, rotated);
    
    if (r){
        const std::string key = "MASK_SPRITE";
        
        GLProgram *glProgram = GLProgramCache::getInstance()->getGLProgram(key);
        if(!glProgram ){
            glProgram = new GLProgram();
            glProgram->initWithFilenames("mask.vsh", "mask.fsh");
            glProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
            glProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
            glProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
            glProgram->link();
            glProgram->updateUniforms();
            
            GLProgramCache::getInstance()->addGLProgram(glProgram, key);
        }
        setGLProgramState(GLProgramState::getOrCreateWithGLProgram(glProgram));
        this->setGLProgram(glProgram);
        if (_mask) {
            getGLProgramState()->_mask_textureID = _mask->getName();
        }
    }
    return r;
}



ShaderSprite* ShaderSprite::createAndDownload(const std::string &fileName, const std::string &defaultName, const std::string &maskName)
{
    ShaderSprite *imgIcon;
    if(GDM->is_file_exist(fileName))
    {
        std::string img = fileName;
        if(!GameTools::isValidPicture(img))
        {
            img = defaultName;
        }
        imgIcon = ShaderSprite::create(img, maskName);
    }
    else
    {
        imgIcon = ShaderSprite::create(defaultName, maskName);
    }
    imgIcon->_defaultFileName = defaultName;
    imgIcon->setSpriteTexture(fileName);
    return imgIcon;
}

void ShaderSprite::setSpriteTexture(const std::string &fileName)
{
    CCAssert(_mask, "Invalid _mask for sprite");
    _fileName = fileName;
    if(GDM->is_file_exist(_fileName))
    {
        std::string img = fileName;
        if(!GameTools::isValidPicture(img))
        {
            img = _defaultFileName;
        }
        this->setTexture(img);
        this->setScale(_mask->getContentSize().width / this->getContentSize().width,
                          _mask->getContentSize().height / this->getContentSize().height);
    }
    else
    {
        //下载图片
        this->setScale(_mask->getContentSize().width / this->getContentSize().width,
                          _mask->getContentSize().height / this->getContentSize().height);
        this->_dispatch.register_event(EVENT_DOWLOAD_FILE, BIND_FUNC(this, ShaderSprite::handle_download_event));
        FM.download_file(_fileName);
    }
}

void ShaderSprite::handle_download_event(cmd_data_pointer data)
{
    auto str = std::dynamic_pointer_cast<cmd_data_string>(data);
    auto url = str->get_value();
    assert (GDM->is_file_exist(url));
    if(_fileName == url)
    {
        this->setTexture(url);
        this->setScale(_mask->getContentSize().width / this->getContentSize().width,
                         _mask->getContentSize().height / this->getContentSize().height);
    }
}

NS_CC_END