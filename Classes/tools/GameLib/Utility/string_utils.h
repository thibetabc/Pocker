//
//  string_utils.h
//  Wesnoth_copy
//
//  Created by MirrorMiko on 13-9-13.
//  Copyright (c) 2013年 MirrorMiko. All rights reserved.
//

#ifndef __Wesnoth_copy__string_utils__
#define __Wesnoth_copy__string_utils__

#include <iostream>
#include <sstream>
#include <vector>
//#include "utf8.h"
#include <boost/next_prior.hpp>

namespace string_utils {
    enum {
        REMOVE_EMPTY = 0x01,	/**< REMOVE_EMPTY : remove empty elements. */
        STRIP_SPACES  = 0x02	/**< STRIP_SPACES : strips leading and trailing blank spaces. */
    };
    bool isnewline(const char c);
    bool portable_isspace(const char c);
    bool notspace(char c);
    /** Remove whitespace from the front and back of the string 'str'. */
    std::string &strip(std::string &str);

    std::vector< std::string > split(std::string const &val, char c = ',', int flags = REMOVE_EMPTY | STRIP_SPACES);

    /**
     * Splits a string based either on a separator where text within parenthesis
     * is protected from splitting (Note that one can use the same character for
     * both the left and right parenthesis. In this mode it usually makes only
     * sense to have one character for the left and right parenthesis.)
     * or if the separator == 0 it splits a string into an odd number of parts:
     * - The part before the first '(',
     * - the part between the first '('
     * - and the matching right ')', etc ...
     * and the remainder of the string.
     * Note that this will find the first matching char in the left string
     * and match against the corresponding char in the right string.
     * In this mode, a correctly processed string should return with
     * an odd number of elements to the vector and
     * an empty elements are never removed as they are placeholders.
     * hence REMOVE EMPTY only works for the separator split.
     *
     * parenthetical_split("a(b)c{d}e(f{g})h",0,"({",")}") should return
     * a vector of <"a","b","c","d","e","f{g}","h">
     */
    std::vector< std::string > parenthetical_split(std::string const &val,
    	const char separator = 0 , std::string const &left="(",
    	std::string const &right=")", int flags = REMOVE_EMPTY | STRIP_SPACES);

    template <typename T>
    std::string join(T const &v, const std::string& s = ","){
            std::stringstream str;
            for(typename T::const_iterator i = v.begin(); i != v.end(); ++i) {
                    str << *i;
                    if (boost::next(i) != v.end())
                            str << s;
            }

            return str.str();
    }
    std::pair< int, int > parse_range(std::string const &str);

    std::vector< std::pair< int, int > > parse_ranges(std::string const &str);
    
    std::string Base64Encode(const std::string& input);
    std::string Base64Decode(const std::string& input);
    std::string Base64Encode(const unsigned char* Data,int DataByte);
    std::string Base64Decode(const char* Data,int DataByte,int& OutByte);
}


#endif /* defined(__Wesnoth_copy__string_utils__) */
