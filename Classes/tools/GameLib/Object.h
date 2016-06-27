//
//  object.h
//  godofwar
//
//  Created by MirrorMiko on 7/30/14.
//
//

#ifndef __godofwar_Object_h
#define __godofwar_Object_h

#include <iostream>
#include <sstream>
#include <typeinfo>
#include <memory>

class Object : public std::enable_shared_from_this<Object>{
public:
    Object() {}
    virtual ~Object() {}
    
public:
    
    template<class T>
    std::shared_ptr<T> shared_ptr_this(){
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }
    
    virtual std::string ToString()
    {
        std::ostringstream os;
        os << "{" << typeid(this).name() << ":";
        os.fill('0');
        os.width(8);
        os << std::uppercase << std::hex << this << std::dec;
        os.fill();
        os.width();
        os << "}" << std::endl;
        return os.str();
    }
};

#endif
