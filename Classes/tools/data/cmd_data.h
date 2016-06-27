//
//  cmd_data.h
//  pokerclient
//
//  Created by MirrorMiko on 15/5/3.
//
//

#ifndef __pokerclient__cmd_data__
#define __pokerclient__cmd_data__

#include <memory>
#include <list>
#include <unordered_map>

class cmd_data {
public:
    virtual bool is_valid() = 0;
};
typedef std::shared_ptr<cmd_data> cmd_data_pointer;

//base type of data : bool、(unsigned) char、(u)int、long、string....
template <class T>
class cmd_data_complex : public cmd_data {
protected:
    T _value;
public:
    cmd_data_complex(){}
    cmd_data_complex(const T& v):_value(v){}
    virtual bool is_valid() { return true; }
    virtual void set_value(const T& value) {  _value = value; }
    virtual T get_value() const { return _value; }
};

typedef cmd_data_complex<bool> cmd_data_bool;
typedef cmd_data_complex<int> cmd_data_int;
typedef cmd_data_complex<uint32_t> cmd_data_uint32;
typedef cmd_data_complex<double> cmd_data_double;
typedef cmd_data_complex<std::string> cmd_data_string;
typedef cmd_data_complex<std::list<cmd_data_pointer>> cmd_data_list;
typedef cmd_data_complex<std::unordered_map<std::string, cmd_data_pointer>> cmd_data_map;
typedef cmd_data_complex<std::unordered_map<int, cmd_data_pointer>> cmd_data_int_map;
#endif /* defined(__pokerclient__cmd_data__) */
