//
//  net_data_send.h
//  pokerclient
//
//  Created by MirrorMiko on 15/5/5.
//
//

#ifndef __pokerclient__net_data_send__
#define __pokerclient__net_data_send__

#include <stdio.h>
#include "net_data.h"
#include "msg.pb.h"
class net_data_send : public net_data{
public:
    virtual void package(Buffer& buffer) = 0;
};

class net_data_send_header;
typedef std::shared_ptr<net_data_send_header> send_header_pointer;

class net_data_send_header : public net_data_send {
public:
    static send_header_pointer package_header();
    net_data_send_header();
    virtual bool is_valid();
    virtual void package(Buffer& buffer);
    virtual size_t size();
public:
    static const uint8_t checkout_bit[4];
    uint16_t head_size;
    uint16_t request_code;
    uint32_t total_size;
};

class net_data_send_package : public net_data_send {
public:
    net_data_send_package(uint16_t cmd);
    ~net_data_send_package();
    virtual bool is_valid();
    virtual size_t size();
    virtual void package(Buffer& buffer);
public:
    net_data_send_header packagehead;
    char* body = nullptr;
    uint32_t bodysize;
    char* head = nullptr;
    uint32_t headsize;
};

#endif /* defined(__pokerclient__net_data_send__) */
