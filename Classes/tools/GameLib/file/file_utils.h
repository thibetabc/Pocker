//
//  file_utils.h
//  Wesnoth_copy
//
//  Created by MirrorMiko on 13-9-12.
//  Copyright (c) 2013年 MirrorMiko. All rights reserved.
//

#ifndef __Wesnoth_copy__file_utils__
#define __Wesnoth_copy__file_utils__

#include <iostream>
#include <vector>

enum file_name_option { ENTIRE_FILE_PATH, FILE_NAME_ONLY };

namespace file_utils{
    void get_files_in_dir(const std::string& dir,
                          std::vector<std::string>* files,
                          std::vector<std::string>* dirs=NULL,
                          file_name_option mode = FILE_NAME_ONLY);
    
    bool ends_with(const std::string&  str, const std::string&  suffix);
    /*Files*/
    
    bool file_exists(const std::string&  name);
        
    //Returns the size of a file, or -1 if the file doesn't exist.
    int file_size(const std::string&  fname);

    // Returns the base filename of a file, with directory name stripped.
    // Equivalent to a portable basename() function.
    std::string file_name(const std::string&  file);
    
    // Get the creation time of a file.
    time_t file_create_time(const std::string&  fname);
    
    // Returns true if the file ends with '.gz'.
    bool is_gzip_file(const std::string&  filename);

    std::string get_next_filename(const std::string&  name, const std::string&  extension);
    
    /*Dirs*/
    
    std::string get_cwd();
    
    std::string get_dir(const std::string& dir);
    
    bool is_directory(const std::string&  fname);
    
    bool make_directory(const std::string&  dirname);
        
    /**
     * Creates a directory if it does not exist already.
     *
     * @param dirname                 Path to directory. All parents should exist.
     * @returns                       True if the directory exists or could be
     *                                successfully created; false otherwise.
     */
    bool create_directory_if_missing(const std::string&  dirname);
    
    /**
     * Creates a recursive directory tree if it does not exist already
     * @param dirname                 Full path of target directory. Non existing parents
     *                                will be created
     * @return                        True if the directory exists or could be
     *                                successfully created; false otherwise.
     */
    bool create_directory_if_missing_recursive(const std::string&  dirname);
    
    bool delete_directory(const std::string&  dirname, const bool keep_pbl = false);
    /**
     * Returns the directory name of a file, with filename stripped.
     * Equivalent to a portable dirname()
     */
    std::string directory_name(const std::string&  file);
    
    /*IO*/
    std::string read_file(const std::string& fname);
    std::istream *istream_file(const std::string& fname);
    std::ostream *ostream_file(std::string const &fname);
    /* Throws io_exception if an error occurs. */
    void write_file(const std::string&  fname, const std::string&  data);
    
    class scoped_istream {
        std::istream *stream;
    public:
        scoped_istream(std::istream *s): stream(s) {}
        scoped_istream& operator=(std::istream *);
        std::istream &operator*() { return *stream; }
        std::istream *operator->() { return stream; }
        ~scoped_istream();
    };
    
    class scoped_ostream {
        std::ostream *stream;
    public:
        scoped_ostream(std::ostream *s): stream(s) {}
        scoped_ostream& operator=(std::ostream *);
        std::ostream &operator*() { return *stream; }
        std::ostream *operator->() { return stream; }
        ~scoped_ostream();
    };
}
#endif /* defined(__Wesnoth_copy__file_utils__) */
