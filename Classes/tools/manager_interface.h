//
//  manager_interface.h
//  pokerclient
//
//  Created by MirrorMiko on 15/5/3.
//
//

#ifndef __pokerclient__manager_interface__
#define __pokerclient__manager_interface__

#include <stdio.h>
#include "macro.h"

class manager_interface {
public:
    virtual void init() = 0;
    virtual void update() = 0;
};

#endif /* defined(__pokerclient__manager_interface__) */
