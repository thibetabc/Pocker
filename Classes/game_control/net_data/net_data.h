//
//  net_data.h
//  pokerclient
//
//  Created by MirrorMiko on 15/5/3.
//
//

#ifndef __pokerclient__net_data__
#define __pokerclient__net_data__

#include <stdio.h>
#include <vector>
#include "cmd_data.h"
#include "Buffer.h"

class net_data : public cmd_data {
public:
    virtual size_t size() = 0;
};


#endif /* defined(__pokerclient__net_data__) */
