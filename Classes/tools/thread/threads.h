//
//  thead_manager.h
//  pokerclient
//
//  Created by MirrorMiko on 15/5/3.
//
//

#ifndef __pokerclient__thead_manager__
#define __pokerclient__thead_manager__

#include <stdio.h>
#include <thread>
#include "macro.h"
#include "manager_interface.h"

namespace threads {
    class thread_manager : public SINGLETON<thread_manager>, public manager_interface {
    protected:
        virtual void init() { register_main_thread(); }
        void register_main_thread();
    private:
        std::thread::id _cur_thread_id;
    public:
        bool is_main_thread();
        virtual void update();
    };
}



#endif /* defined(__pokerclient__thead_manager__) */
