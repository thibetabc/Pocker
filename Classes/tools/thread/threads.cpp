//
//  thead_manager.cpp
//  pokerclient
//
//  Created by MirrorMiko on 15/5/3.
//
//

#include "threads.h"

void threads::thread_manager::register_main_thread(){
    _cur_thread_id = std::this_thread::get_id();
}

bool threads::thread_manager::is_main_thread(){
    return _cur_thread_id == std::this_thread::get_id();
}

void threads::thread_manager::update(){
    
}