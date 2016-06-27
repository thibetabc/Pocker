//
//  game_manager.h
//  pokerclient
//
//  Created by MirrorMiko on 15/5/3.
//
//

#ifndef __pokerclient__game_manager__
#define __pokerclient__game_manager__

#include <stdio.h>
#include <list>
#include "manager_interface.h"

class game_manager : public SINGLETON<game_manager>, public manager_interface {
protected:
    std::list<manager_interface*> _managers;
public:
    virtual void init();
    virtual void update();
    void coco_update(float);
};

#endif /* defined(__pokerclient__game_manager__) */
