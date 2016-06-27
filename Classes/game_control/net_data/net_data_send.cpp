//
//  net_data_send.cpp
//  pokerclient
//
//  Created by MirrorMiko on 15/5/5.
//
//

#include "net_data_send.h"
#include "macro.h"
#include "net_manager.h"
const uint8_t net_data_send_header::checkout_bit[4] = {'D', 'Z', 'P', 'K'};

net_data_send_header::net_data_send_header()
:head_size(0)
,request_code(0)
,total_size(0){
}

send_header_pointer net_data_send_header::package_header(){
    return send_header_pointer(new net_data_send_header());
}

bool net_data_send_header::is_valid(){
    return true;
}

void net_data_send_header::package(Buffer& buffer){
    buffer.WriteBytes(checkout_bit, 4);
    buffer.WriteUInt16(request_code);
    buffer.WriteUInt16(head_size);
    buffer.WriteUInt32(total_size);
}

size_t net_data_send_header::size(){
    return 12;
}

#pragma mark - packages

net_data_send_package::net_data_send_package(uint16_t cmd){
    packagehead.request_code = cmd;
}

net_data_send_package::~net_data_send_package(){
    if (body != nullptr)
        delete []body;
    if (head != nullptr)
        delete []head;
}

bool net_data_send_package::is_valid(){
    bool valid = packagehead.is_valid();
    return valid && (head != nullptr) && (body != nullptr);
}

size_t net_data_send_package::size(){
    return packagehead.size() + headsize + bodysize;
}

void net_data_send_package::package(Buffer &buffer){
    packagehead.total_size = size();
    packagehead.head_size = headsize;
    packagehead.package(buffer);
    if (head != nullptr)
        buffer.WriteBytes(head, headsize);
    if (body != nullptr)
        buffer.WriteBytes(body, bodysize);
}