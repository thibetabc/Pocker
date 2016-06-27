//
//  net_data_recv.h
//  pokerclient
//
//  Created by MirrorMiko on 15/5/5.
//
//

#ifndef __pokerclient__net_data_recv__
#define __pokerclient__net_data_recv__

#include <stdio.h>
#include "net_data.h"

class net_data_recv : public net_data{
    
public:
    virtual bool parse(Buffer& buffer) = 0;
};

class net_data_recv_header : public net_data_recv {
public:
    virtual bool is_valid();
    virtual size_t size();
    virtual bool parse(Buffer& buffer);
public:
    uint8_t checkout_bit[4];
    uint16_t request_code;
    uint32_t total_size;
    uint16_t head_size;
};

class net_data_recv_package : public net_data_recv {
public:
    virtual bool is_valid();
    virtual size_t size();
    virtual bool parse(Buffer& buffer);
    ~net_data_recv_package();
public:
    net_data_recv_header packagehead;
    char* body = nullptr;
    char* head = nullptr;
    int32_t bodysize;
};

#endif /* defined(__pokerclient__net_data_recv__) */
