//
//  LayerMoments.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#include "LayerMoments.h"
#include "NET_CMD.h"
#include "TipView.h"
#include "ProcessorManager.h"
#include "MomentsManager.h"
#include "utils.h"

bool LayerMoments::init()
{
    return true;
}

void LayerMoments::register_events()
{
    dispatch_.register_event(REQ_GET_MOMENT,
                             BIND_FUNC(this, LayerMoments::handle_get_moment));
    dispatch_.register_event(REQ_DELETE_MOMENT,
                             BIND_FUNC(this, LayerMoments::handle_delete_moment));
    dispatch_.register_event(REQ_PUBLISH_COMMENT,
                             BIND_FUNC(this, LayerMoments::handle_publish_comment));
    dispatch_.register_event(REQ_GET_COMMENT,
                             BIND_FUNC(this, LayerMoments::handle_get_comment));
}

void LayerMoments::handle_get_moment(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->GetMoment_Down(rec);
    
    for(int i = 0 ; i < processor.moment_size() ;i++)
        MM->add_moment(processor.moment(i));
    MM->save_moment_record();
}

void LayerMoments::handle_delete_moment(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    switch (processor.status())
    {
        case msg::Status::SUCCESS:
        {
            TipView::showAlertView(tools::local_string("delete_successed","删除成功"));
            break;
        }
        case msg::Status::FAILED:
            ShowError(tools::local_string("delete_failed","删除失败"));
            break;
        default:
            break;
    }
}

void LayerMoments::handle_publish_comment(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    
    switch (processor.status())
    {
        case msg::Status::SUCCESS:
        {
            break;
        }
        case msg::Status::FAILED:
            ShowError(tools::local_string("remark_failed","评论失败"));
            break;
        default:
            break;
    }
}

void LayerMoments::handle_get_comment(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->GetComment_Down(rec);
}

