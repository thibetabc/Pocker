//
//  file_utils.cpp
//  Wesnoth_copy
//
//  Created by MirrorMiko on 13-9-12.
//  Copyright (c) 2013年 MirrorMiko. All rights reserved.
//

#include "file_utils.h"

#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include <algorithm>
#include "cocos2d.h"
#include <cstring>
#include <sstream>
#include <cerrno>
#include "utils.h"
#include <unistd.h>
#include "Scoped_resource.hpp"
#include <boost/move/move.hpp>
namespace {
    const mode_t AccessMode = 00770;
}
namespace file_utils{
    
    void get_files_in_dir(const std::string& directory,
                          std::vector<std::string>* files,
                          std::vector<std::string>* dirs,
                          file_name_option mode){
        struct stat st;
        DIR* dir = opendir(directory.c_str());
        
        if(dir == NULL) return;
        
        struct dirent* entry;
        
        const std::string dir_separators = directory.empty() || directory[directory.size()-1] == '/' ? "/" : "";
        
        while((entry = readdir(dir)) != NULL) {
            
            if(entry->d_name[0] == '.') continue;
            
            const std::string basename = entry->d_name;
            
            std::string fullname = directory + dir_separators + basename;
            
            if (::stat(fullname.c_str(), &st) != -1) {
                if (S_ISREG(st.st_mode)) {
                    if (files != NULL) {
                        if (mode == ENTIRE_FILE_PATH)
                            files->push_back(fullname);
                        else
                            files->push_back(basename);
                    }
                } else if (S_ISDIR(st.st_mode)) {
                    if (dirs != NULL) {
                        if (mode == ENTIRE_FILE_PATH)
                            dirs->push_back(fullname);
                        else
                            dirs->push_back(basename);
                    }
                }
            }
        }
        
        closedir(dir);
        
        if(files != NULL) std::sort(files->begin(),files->end());
        
        if (dirs != NULL) std::sort(dirs->begin(),dirs->end());
    }
    
    bool ends_with(const std::string& str, const std::string& suffix){
        return str.size() >= suffix.size() && std::equal(suffix.begin(),suffix.end(),str.end()-suffix.size());
    }
    
    bool file_exists(const std::string& name){
#ifdef _WIN32
        struct stat st;
        return (::stat(name.c_str(), &st) == 0);
#else
        struct stat st;
        return (::stat(name.c_str(), &st) != -1);
#endif
    }
    
    int file_size(const std::string& fname){
        struct stat buf;
        if(::stat(fname.c_str(),&buf) == -1)
            return -1;
        
        return buf.st_size;
    }
    
    // Analogous to POSIX basename(3), but for C++ string-object pathnames
    std::string file_name(const std::string& file){
#ifdef _WIN32
        static const std::string dir_separators = "\\/:";
#else
        static const std::string dir_separators = "/";
#endif
        
        std::string::size_type pos = file.find_last_of(dir_separators);
        
        if(pos == std::string::npos)
            return file;
        if(pos >= file.size()-1)
            return "";
        
        return file.substr(pos+1);
    }
    
    time_t file_create_time(const std::string& fname){
        struct stat buf;
        if(::stat(fname.c_str(),&buf) == -1)
            return 0;
        
        return buf.st_mtime;
    }
    
    bool is_gzip_file(const std::string& filename){
        return (filename.length() > 3
                && filename.substr(filename.length() - 3) == ".gz");
    }
    
    std::string get_next_filename(const std::string& name, const std::string& extension){
        std::string next_filename;
        int counter = 0;
        
        do {
            std::stringstream filename;
            
            filename << name;
            filename.width(3);
            filename.fill('0');
            filename.setf(std::ios_base::right);
            filename << counter << extension;
            counter++;
            next_filename = filename.str();
        } while(file_exists(next_filename) && counter < 1000);
        return next_filename;
    }
    
    std::string get_cwd(){
        char buf[1024];
        
        const char* const res = getcwd(buf,sizeof(buf));
        if(res != NULL) {
            std::string str(res);
#ifdef _WIN32
            std::replace(str.begin(),str.end(),'\\','/');
#endif
            return str;
        } else {
            return "";
        }
    }
    
    std::string get_dir(const std::string& dir_path){
        DIR* dir = opendir(dir_path.c_str());
        if(dir == NULL) {
            const int res = mkdir(dir_path.c_str(),AccessMode);
            if(res == 0) {
                dir = opendir(dir_path.c_str());
            } else {
                std::cout << "could not open or create directory: " << dir_path << '\n';
            }
        }
        
        if(dir == NULL) return "";
        
        closedir(dir);
        
        return dir_path;
    }
    
    static bool is_directory_internal(const std::string& fname){
#ifdef _WIN32
        _finddata_t info;
        const long handle = _findfirst((fname + "/*").c_str(),&info);
        if(handle >= 0) {
            _findclose(handle);
            return true;
        } else {
            return false;
        }
#else
        struct stat dir_stat;
        if(::stat(fname.c_str(), &dir_stat) == -1) {
            return false;
        }
        return S_ISDIR(dir_stat.st_mode);
#endif
    }
    
    bool is_directory(const std::string& fname){
        if(fname.empty()) {
            return false;
        }
        return is_directory_internal(fname);
    }
    
    bool make_directory(const std::string& path){
        return (mkdir(path.c_str(),AccessMode) == 0);
    }
    
    bool create_directory_if_missing(const std::string& dirname){
        if(is_directory(dirname)) {
            std::cout << "directory " << dirname << " exists, not creating\n";
            return true;
        } else if(file_exists(dirname)) {
            std::cout << "cannot create directory " << dirname << "; file exists\n";
            return false;
        }
        std::cout << "creating missing directory " << dirname << '\n';
        return make_directory(dirname);
    }
    
    bool create_directory_if_missing_recursive(const std::string& dirname){
        if (is_directory(dirname) == false && dirname.empty() == false)
        {
            std::string tmp_dirname = dirname;
            // remove trailing slashes or backslashes
            while ((tmp_dirname[tmp_dirname.size()-1] == '/' ||
                    tmp_dirname[tmp_dirname.size()-1] == '\\') &&
                   !tmp_dirname.empty())
            {
                tmp_dirname.erase(tmp_dirname.size()-1);
            }
            
            // create the first non-existing directory
            size_t pos = tmp_dirname.rfind("/");
            
            // we get the most right directory and *skip* it
            // we are creating it when we get back here
            if (tmp_dirname.rfind('\\') != std::string::npos &&
                tmp_dirname.rfind('\\') > pos )
                pos = tmp_dirname.rfind('\\');
            
            if (pos != std::string::npos)
                create_directory_if_missing_recursive(tmp_dirname.substr(0,pos));
            
            return create_directory_if_missing(tmp_dirname);
        }
        return create_directory_if_missing(dirname);
    }
    
    bool delete_directory(const std::string& path, const bool keep_pbl){
        bool ret = true;
        std::vector<std::string> files;
        std::vector<std::string> dirs;
        
        get_files_in_dir(path, &files, &dirs, ENTIRE_FILE_PATH);
        
        if(!files.empty()) {
            for(std::vector<std::string>::const_iterator i = files.begin(); i != files.end(); ++i) {
                errno = 0;
                if(remove((*i).c_str()) != 0) {
                    std::cout << "remove(" << (*i) << "): " << strerror(errno) << "\n";
                    ret = false;
                }
            }
        }
        
        if(!dirs.empty()) {
            for(std::vector<std::string>::const_iterator j = dirs.begin(); j != dirs.end(); ++j) {
                if(!delete_directory(*j))
                    ret = false;
            }
        }
        
        errno = 0;
#ifdef _WIN32
        // remove() doesn't delete directories on windows.
        int (*remove)(const char*);
        if(is_directory(path))
            remove = rmdir;
        else
            remove = ::remove;
#endif
        if(remove(path.c_str()) != 0) {
            std::cout << "remove(" << path << "): " << strerror(errno) << "\n";
            ret = false;
        }
        return ret;
    }
    
    // Analogous to POSIX dirname(3), but for C++ string-object pathnames
    std::string directory_name(const std::string& file){
#ifdef _WIN32
        static const std::string dir_separators = "\\/:";
#else
        static const std::string dir_separators = "/";
#endif
        
        std::string::size_type pos = file.find_last_of(dir_separators);
        
        if(pos == std::string::npos)
            return "";
        
        return file.substr(0,pos+1);
    }
    
    static std::string read_stream(std::istream& s){
        std::stringstream ss;
        ss << s.rdbuf();
        return std::move(ss.str());
    }
    
    std::string read_file(const std::string& fname)
    {
        std::string content;
        std::string path = fname;
        std::ifstream file(path.c_str(), std::ios::binary);
        
        if (!file.is_open()) {
            std::cerr << "file " << fname << " open failed!" << std::endl;
            return content;
        }
        
        const size_t block_size = 4096;
        char buf[block_size];
        do {
            file.read(buf, block_size);
            content.append(buf, file.gcount());
        } while (file.gcount() == block_size);
        
        file.close();
        return std::move(content);
    }
    
    std::istream *istream_file(const std::string& fname){
        std::cout << "Streaming " << fname << " for reading.\n";
        if (fname.empty())
        {
            std::cout << "Trying to open file with empty name.\n";
            std::ifstream *s = new std::ifstream();
            s->clear(std::ios_base::failbit);
            return s;
        }
#ifndef _WIN32
        // TODO: Should also be done for Windows; but *nix systems should
        // already be sufficient to catch most offenders.
        if (fname[0] != '/') {
            std::cout << "Trying to open file with relative path: '" << fname << "'.\n";
#if 0
            std::ifstream *s = new std::ifstream();
            s->clear(std::ios_base::failbit);
            return s;
#endif
        }
#endif
        std::ifstream *s = new std::ifstream(fname.c_str(),std::ios_base::binary);
        if (s->is_open())
            return s;
        std::cout << "Could not open '" << fname << "' for reading.\n";
        return s;
        
    }
    
    std::ostream *ostream_file(std::string const &fname){
        std::cout << "streaming " << fname << " for writing.\n";
        return new std::ofstream(fname.c_str(), std::ios_base::binary);
    }
    
    // Throws io_exception if an error occurs
    void write_file(const std::string& fname, const std::string& data){
        //const util::scoped_resource<FILE*,close_FILE> file(fopen(fname.c_str(),"wb"));
        const util::scoped_FILE file(fopen(fname.c_str(),"wb"));
        if(file.get() == NULL) {
            //            throw std::exception("Could not open file for writing: '" + fname + "'");
            cocos2d::log("open file faild:%s", fname.c_str());
        }
        else {
            cocos2d::log("open file succ:%s", fname.c_str());
        }
        
        const size_t block_size = 4096;
        char buf[block_size];
        
        for(size_t i = 0; i < data.size(); i += block_size) {
            const size_t bytes = std::min<size_t>(block_size,data.size() - i);
            std::copy(data.begin() + i, data.begin() + i + bytes,buf);
            const size_t res = fwrite(buf,1,bytes,file.get());
            if(res != bytes) {
                //                throw io_exception("Error writing to file: '" + fname + "'");
                cocos2d::log("Error writing to file: %s", fname.c_str());
            }
        }
        fclose(file);  // added by ronaldz. this must be mistake if dont close.
        cocos2d::log("write_file_finished!");
    }
    
    scoped_istream& scoped_istream::operator=(std::istream *s)
    {
        delete stream;
        stream = s;
        return *this;
    }
    
    scoped_istream::~scoped_istream()
    {
        delete stream;
    }
    
    scoped_ostream& scoped_ostream::operator=(std::ostream *s)
    {
        delete stream;
        stream = s;
        return *this;
    }
    
    scoped_ostream::~scoped_ostream()
    {
        delete stream;
    }
    
}
