
#pragma once

#include "cocos2d.h"

#include <list>
#include <iostream>
#include <map>
#include <mutex>
using namespace std;

#include <boost/smart_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <boost/utility.hpp>
#include "message_block.hpp"
#include <thread>

class net_interface : public boost::enable_shared_from_this<net_interface> {
    
public:
    static boost::asio::io_service& io_service() {
        static boost::asio::io_service inst;
        static boost::asio::io_service::work w(inst);
        return inst;
    }
    
    static void startup_io_service() {
        auto f = static_cast<std::size_t(boost::asio::io_service::*)()>(&boost::asio::io_service::run);
        std::thread(std::bind(f, &net_interface::io_service())).detach();
    }
    
    static std::string dns_lookup(const char *domain, const char *port) {
        
        using boost::asio::ip::tcp;
        tcp::resolver resolver(net_interface::io_service());
        tcp::resolver::query query(tcp::v4(), domain, port);
        tcp::resolver::iterator iterator = resolver.resolve(query);
        
        std::cout << (*iterator).endpoint().address().to_string() << std::endl;
        
        return (*iterator).endpoint().address().to_string();
    }
    
    static std::string dns_lookup_v6(const char *domain, const char *port) {
        
        using boost::asio::ip::tcp;
        tcp::resolver resolver(net_interface::io_service());
        tcp::resolver::query query(tcp::v6(), domain, port);
        tcp::resolver::iterator iterator = resolver.resolve(query);
        
        std::cout << (*iterator).endpoint().address().to_string() << std::endl;
        
        return (*iterator).endpoint().address().to_string();
    }
};

typedef std::function<void (uint32_t, const boost::system::error_code&)> status_handler;
typedef std::function<size_t (uint32_t, char*, size_t)> data_handler;

class net_interface2 : public boost::enable_shared_from_this<net_interface2> {
    
    typedef boost::shared_ptr<boost::asio::ip::tcp::socket> shared_ptr_socket;
    
    typedef boost::shared_ptr<net_interface2> shared_ptr_net_interface2;
    typedef   boost::weak_ptr<net_interface2>   weak_ptr_net_interface2;
    
    typedef std::pair<map<uint32_t,weak_ptr_net_interface2>,std::mutex> map_weak_ptr_net_interface2_with_mutex;
    
private:
    static map_weak_ptr_net_interface2_with_mutex& peers() {
        static map_weak_ptr_net_interface2_with_mutex peers_;
        return peers_;
    }
    
public:
    static uint32_t connect(const char *addr,
                            const uint32_t port,
                            const int addr_type,
                            const status_handler& connect_handler,
                            const status_handler& send_error,
                            const status_handler& recv_error,
                            const data_handler& data_handler) {
        static uint32_t index = 2012;
        std::lock_guard<std::mutex> scoped_lock(net_interface2::peers().second);
        shared_ptr_net_interface2 peer(new net_interface2(++index, addr, port, addr_type));
        peer->set_function_connect_status(connect_handler);
        peer->set_function_send_error(send_error);
        peer->set_function_recv_error(recv_error);
        peer->set_function_data(data_handler);
        net_interface2::peers().first[index] = peer;
        peer->async_connect_startup(); //先运行了 async_connect_startup
        cocos2d::log("net_interface2::connect startup %d", index);
        return index;
    }
    
    void set_function_connect_status(const status_handler& func){
        function_connect_status_ = func;
    }
    
    void set_function_send_error(const status_handler & func){
        function_send_error_ = func;
    }
    
    void set_function_recv_error(const status_handler & func){
        function_recv_error_ = func;
    }
    
    void set_function_data(const data_handler& func){
        function_data_ = func;
    }
    
    static int send(uint32_t index, const char *data, const uint32_t length) {
        
        std::lock_guard<std::mutex> scoped_lock(net_interface2::peers().second);
        
        map<uint32_t,weak_ptr_net_interface2>::iterator iter = net_interface2::peers().first.find(index);
        
        if (iter != net_interface2::peers().first.end()) {
            shared_ptr_net_interface2 sp = (*iter).second.lock();
            if (sp) {
                sp_message_block nb(new message_block(length));
                nb->copy(data, length);
                
                std::function<void()> f = std::bind(&net_interface2::send_impl, sp, nb);
                net_interface::io_service().post(f);
                return 0;
            }
        }
        return 1;
    }
    
    static void shutdown(uint32_t index) {
        cocos2d::log("shutdown:%u", index);
        std::lock_guard<std::mutex> scoped_lock(net_interface2::peers().second);
        map<uint32_t,weak_ptr_net_interface2>::iterator iter = net_interface2::peers().first.find(index);
        if (iter != net_interface2::peers().first.end()) {
            shared_ptr_net_interface2 sp = (*iter).second.lock();
            if (sp) {
                sp->shutdown_impl();
            }
            net_interface2::peers().first.erase(iter);
        }
    }
    
private:
    net_interface2(const uint32_t &index, const char* addr, uint32_t port, int addr_type)
    : index_(index)
    , peer_(new boost::asio::ip::tcp::socket(net_interface::io_service()))
    {
        if (AF_INET == addr_type)
        {
            endpoint_ = boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(addr), port);
        }else if (AF_INET6 == addr_type)
        {
            endpoint_ = boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v6::from_string(addr), port);
        }
    }
    
    void async_connect_startup() {
        peer_->async_connect(endpoint_,
                             boost::bind(&net_interface2::async_connect_handler,
                                         shared_from_this(),
                                         peer_,
                                         boost::asio::placeholders::error));
	}
    
    void async_connect_handler(shared_ptr_socket peer, const boost::system::error_code& ec) {
        if(function_connect_status_) {
            function_connect_status_(index_, ec);
        }
        
        if (ec) {
            CCLOGINFO("net_interface2::async_connect_handler %d FAILED %s", index_, ec.message().c_str());
//            if(function_connect_status_) {
//                function_connect_status_(index_, ec.value());
//            }
            
		} else {
            CCLOGINFO("net_interface2::async_connect_handler %d SUCCEED, prepare receive ... ", index_);
            sp_message_block buffer(new message_block(1024*64));
            async_receive_startup(buffer);
		}
	}
    
    void async_receive_startup(sp_message_block buffer) {
        if (buffer->capacity()) {
            buffer->crunch();
        } else {
            std::shared_ptr<message_block> nb(new message_block(buffer->max_size() * 2));
            nb->copy(buffer->data(), buffer->length());
            buffer.swap(nb);
        }
        
        peer_->async_receive(boost::asio::buffer(buffer->wr_ptr(),buffer->capacity()),
                             boost::bind<int>(&net_interface2::async_receive_handler,
                                              shared_from_this(),
                                              buffer,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));
    }
    
    
    int async_receive_handler(sp_message_block mb,
                              boost::system::error_code ec,
                              size_t bytes_transferred)
    {
        if (ec)
        {
            cocos2d::log("net_interface2::async_receive_handler %d FAILED %s", index_, ec.message().c_str());
            if(function_recv_error_)
            {
                function_recv_error_(index_, ec);
            }
        }
        else
        {
            CCLOGINFO("net_interface2::async_receive_handler %d RECEIVED %d bytes", index_, bytes_transferred);
            mb->wr_ptr(bytes_transferred);
            
            char* pos = mb->data();
            size_t bytes = mb->length();
            
            size_t read_len = 0;
            if(function_data_ && bytes != 0) {
                read_len = function_data_(index_, pos, bytes);
            }
            
            mb->rd_ptr(read_len);
            
            if (mb->length()){
                mb->crunch();
            }
            
            async_receive_startup(mb);
            
        }
        
        return 0;
    }
    
    
    void send_impl(sp_message_block mb) {
        if (buffer_queue_out_.empty()) {
            buffer_queue_out_.push_back(mb);
            async_send_startup();
        } else {
            buffer_queue_out_.push_back(mb);
        }
    }
    
    void async_send_startup() {
//        boost::asio::buffer(buffer_queue_out_.front()->rd_ptr(),buffer_queue_out_.front()->length())
        auto data = buffer_queue_out_.front()->rd_ptr();
        auto size = buffer_queue_out_.front()->length();
        auto buffer = boost::asio::buffer(data,size);
        peer_->async_send(buffer,boost::bind<int>(&net_interface2::async_send_handler,
                                 shared_from_this(),
                                 buffer_queue_out_.front(),
                                 boost::asio::placeholders::error,
                                 boost::asio::placeholders::bytes_transferred)
                          );
    }
    
    
    int async_send_handler(sp_message_block mb,
                           boost::system::error_code ec,
                           size_t bytes_transferred) {
        if (ec) {
            CCLOGINFO("net_interface2::async_send_handler %d FAILED %s", index_, ec.message().c_str());
            if (function_send_error_){
                function_send_error_(index_, ec);
            }
        } else {
            mb->rd_ptr(bytes_transferred);
            if (mb->empty()) {
                buffer_queue_out_.pop_front();
                if (buffer_queue_out_.empty()) {
                    
                } else {
                    async_send_startup();
                }
            } else
                async_send_startup();
        }
        return 0;
    }
    
    void shutdown_impl() {
        boost::system::error_code ec;
        peer_->cancel(ec);
        peer_->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        peer_->close(ec);
    }
    
public:
    ~net_interface2() {
        CCLOGINFO("net_interface2::~net_interface2 %d LEFT us", index_);
    }
    
private:
    const uint32_t index_;
    boost::asio::ip::tcp::endpoint endpoint_;
    std::list<sp_message_block> buffer_queue_out_;
    boost::shared_ptr<boost::asio::ip::tcp::socket> peer_;
    
    status_handler function_connect_status_;
    status_handler function_send_error_;
    status_handler function_recv_error_;
    data_handler function_data_;
};
