//
//  scoped_resource.hpp
//  FreeTypeTest
//
//  Created by MirrorMiko on 14-1-14.
//
//

#ifndef SCOPED_RESOURCE_H_INCLUDED
#define SCOPED_RESOURCE_H_INCLUDED

namespace util
{
    template<typename T,typename ReleasePolicy>
    class scoped_resource
    {
        T resource;
        
        scoped_resource(const scoped_resource&);
        scoped_resource& operator=(const scoped_resource&);
    public:
        typedef T resource_type;
        typedef ReleasePolicy release_type;
        
        scoped_resource(resource_type res = resource_type())
        : resource(res) {}
        
        virtual ~scoped_resource()
        {
            release_type()(resource);
        }
        
        operator resource_type() const { return resource; }
        
        resource_type get() const { return resource; }
        
        
        resource_type operator->() const { return resource; }
        
        void assign(const resource_type& o) {
            release_type()(resource);
            resource = o;
        }
    };
    
    struct delete_item {
        template<typename T>
        void operator()(T*& p) const { delete p; p = 0; }
    };
    
    struct delete_array {
        template<typename T>
        void operator()(T*& p) const { delete [] p; p = 0; }
    };
    
    template<typename T>
    struct scoped_ptr : public scoped_resource<T*,delete_item>
    {
        explicit scoped_ptr(T* p) : scoped_resource<T*,delete_item>(p) {}
    };
    
    template<typename T>
    struct scoped_array : public scoped_resource<T*,delete_array>
    {
        explicit scoped_array(T* p) : scoped_resource<T*,delete_array>(p) {}
    };
    
    struct close_FILE
    {
        void operator()(std::FILE* f) const { if(f != NULL) { std::fclose(f); } }
    };
    typedef scoped_resource<std::FILE*,close_FILE> scoped_FILE;
    
}

#endif
