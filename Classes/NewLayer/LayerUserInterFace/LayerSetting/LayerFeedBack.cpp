//
//  LayerFeedBack.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#include "LayerFeedBack.h"
#include "NET_CMD.h"
#include "ProcessorManager.h"
#include "utils.h"

bool LayerFeedBack::init()
{
    return true;
}

void LayerFeedBack::register_events()
{
    dispatch_.register_event(REQ_FEED_BACK,
                             BIND_FUNC(this, LayerFeedBack::handle_feed_back));
}

void LayerFeedBack::handle_feed_back(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    processor.ParseFromArray(rec->body, rec->bodysize);
    switch (processor.status()) {
        case msg::Status::SUCCESS:
            break;
        case msg::Status::FAILED:
            ShowError(tools::local_string("send_message_faild","发送信息失败，请查看网络或者重新发送"));
            break;
        default:
            break;
    }
    
    
}