//
//  ShaderTexture.h
//  pokerclient
//
//  Created by MirrorMiko on 15/7/8.
//
//

#ifndef __pokerclient__ShaderTexture__
#define __pokerclient__ShaderTexture__

#include "cocos2d.h"
#include "events.h"
NS_CC_BEGIN

class CC_DLL ShaderSprite : public Sprite {
public:
    ShaderSprite();
    static ShaderSprite* create(const std::string& filename, const std::string& maskname);
    virtual bool initWithFile(const std::string& filename, const std::string& maskname);
    virtual bool initWithTexture(Texture2D *texture, const Rect& rect, bool rotated);
    
    std::string getFileName(){return _fileName;}
    
public:
    static ShaderSprite* createAndDownload(const std::string &fileName, const std::string &defaultName, const std::string &maskName);
    void setSpriteTexture(const std::string &fileName);
    void handle_download_event(cmd_data_pointer data);
protected:
    Texture2D* _mask;
    std::string _fileName;
    std::string _defaultFileName;
    events::event_dispatch _dispatch;
};

NS_CC_END
#endif /* defined(__pokerclient__ShaderTexture__) */
