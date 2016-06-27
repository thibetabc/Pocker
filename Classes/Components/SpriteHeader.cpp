#include "SpriteHeader.h"


SpriteHeader* SpriteHeader::create(const std::string& filename)
{
    SpriteHeader* header = new SpriteHeader();
    if(!header->init(filename)) {
        delete header;
        return nullptr;
    }
    header->release();
    return header;
}

bool SpriteHeader::init(const std::string &filename)
{
    if(!ImageView::init(filename)) {
        return false;
    }
    
    
    
    return true;
}