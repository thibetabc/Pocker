#pragma once

#include <vector>
#include <iostream>
#include "msg.pb.h"

class StepObject
{
public:
    uint32_t user_id;
    msg::TableAndUserUpdate update;
};