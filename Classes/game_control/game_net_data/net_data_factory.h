//
//  net_data_factory.h
//  pokerclient
//
//  Created by MirrorMiko on 15/5/7.
//
//

#ifndef __pokerclient__net_data_factory__
#define __pokerclient__net_data_factory__

#include <stdio.h>
#include <map>
#include "macro.h"
#include "cmd_data.h"
#include "game_net_data.h"
#include "net_data_recv.h"

class net_data_factory {
protected:
   public:
    static net_data_factory& shared_instence(){
        static net_data_factory __instence;
        return __instence;
    }
    void recv_data(cmd_data_pointer data);
};

#endif /* defined(__pokerclient__net_data_factory__) */
