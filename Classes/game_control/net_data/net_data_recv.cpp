//
//  net_data_recv.cpp
//  pokerclient
//
//  Created by MirrorMiko on 15/5/5.
//
//

#include "net_data_recv.h"

bool net_data_recv_header::is_valid(){
    bool chekout_valid =
    checkout_bit[0] == 'D' &&
    checkout_bit[1] == 'Z' &&
    checkout_bit[2] == 'P' &&
    checkout_bit[3] == 'K';
    
    bool size_valid = total_size != 0;
    return chekout_valid && size_valid;
}

size_t net_data_recv_header::size(){
    return 12;
}

bool net_data_recv_header::parse(Buffer& buffer){
    if (buffer.Length() < size()) {
        return false;
    }else{
        buffer.ReadBytes(checkout_bit, 4);
        buffer.ReadUInt16(request_code);
        buffer.ReadUInt16(head_size);
        buffer.ReadUInt32(total_size);
    }
    return is_valid();
}

#pragma mark - recv packages

bool net_data_recv_package::is_valid(){
    bool valid = packagehead.is_valid();
    return valid && (head != nullptr) && (body != nullptr);
}

size_t net_data_recv_package::size(){
    return packagehead.total_size;
}

net_data_recv_package::~net_data_recv_package(){
    if (body != nullptr)
        delete []body;
    if (head != nullptr)
        delete []head;
}

bool net_data_recv_package::parse(Buffer& buffer){
    if (packagehead.parse(buffer)) {
        if (buffer.Length() < packagehead.total_size - packagehead.size()) {
            return false;
        }
        head = new char[packagehead.head_size];
        buffer.ReadBytes(head, packagehead.head_size);
        bodysize = packagehead.total_size - packagehead.size() - packagehead.head_size;
        body = new char[bodysize];
        buffer.ReadBytes(body, bodysize);
    }else{
        return false;
    }
    return true;
}