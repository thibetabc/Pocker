//
//  LayerPublishMoment.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#include "LayerPublishMoment.h"
#include "NET_CMD.h"
#include "ProcessorManager.h"
#include "utils.h"

bool LayerPublishMoment::init()
{
    return true;
}

void LayerPublishMoment::register_events()
{
    dispatch_.register_event(REQ_PUBLISH_MOMENT,
                             BIND_FUNC(this, LayerPublishMoment::handle_publish_moment));
}

void LayerPublishMoment::handle_publish_moment(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    msg::Response processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    switch (processor.status())
    {
        case msg::Status::SUCCESS:
        {
            break;
        }
        case msg::Status::FAILED:
            ShowError(tools::local_string("publish_failed","发布失败"));
            break;
        default:
            break;
    }
    
}