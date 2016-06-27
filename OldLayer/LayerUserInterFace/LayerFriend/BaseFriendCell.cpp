//
//  BaseFriendCell.cpp
//  pokerclient
//
//  Created by ll on 15/8/24.
//
//

#include "BaseFriendCell.h"
#include "GameDataManager.h"
#include "utils.h"

bool BaseFriendCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    return true;
}
