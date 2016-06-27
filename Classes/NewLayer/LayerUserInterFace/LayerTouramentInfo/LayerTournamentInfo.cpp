//
//  LayerTournamentInfo.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#include "LayerTournamentInfo.h"
#include "TipView.h"
#include "ProcessorManager.h"
#include "NET_CMD.h"

bool LayerTournamentInfo::init()
{
    return true;
}

void LayerTournamentInfo::register_events()
{
    dispatch_.register_event(REQ_CANCEL_MATCH, BIND_FUNC(this, LayerTournamentInfo::handle_cancel_match));
    dispatch_.register_event(REQ_SEND_JOIN,
                             BIND_FUNC(this, LayerTournamentInfo::handle_join));
}

void LayerTournamentInfo::handle_cancel_match(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    TipView::showAlertView(processor.message());
    if(processor.status() == msg::Status::SUCCESS )
    {
        //send_get_room_info();
    }
}

void LayerTournamentInfo::handle_join(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    TipView::showAlertView(processor.message());
}