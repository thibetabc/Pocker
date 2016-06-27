//
//  Arithmetic.h
//  FreeTypeTest
//
//  Created by MirrorMiko on 14-1-14.
//
//

#ifndef __FreeTypeTest__Arithmetic__
#define __FreeTypeTest__Arithmetic__

#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>
//偶数
template<typename T>
inline bool is_odd(T num) {
    int n = static_cast< int >(num);
    return static_cast< unsigned int >(n >= 0 ? n : -n) & 1;
}

//奇数
template<typename T>
inline bool is_even(T num) { return !is_odd(num); }

inline double round_portable(double d) {
	return (d >= 0.0) ? std::floor(d + 0.5) : std::ceil(d - 0.5);
}

//四舍五入
inline int round_double(double d) {
	return static_cast<int>(round_portable(d));
}

struct bad_lexical_cast {};

template<typename To, typename From>
To lexical_cast(From a)
{
	To res;
	std::stringstream str;
    
	if(!(str << a && str >> res)) {
		return To();
	} else {
		return res;
	}
}

template<typename To, typename From>
To lexical_cast_default(From a, To def=To())
{
	To res;
	std::stringstream str;
    
	if(!(str << a && str >> res)) {
		return def;
	} else {
		return res;
	}
}

inline int str_to_int(const std::string&  a){ return lexical_cast_default<int, const std::string& >(a, 0); }
inline double str_to_double(const std::string&  a){ return lexical_cast_default<double, const std::string& >(a, 0); }

template<typename From>
std::string str_cast(From a)
{
    try {
        return lexical_cast<std::string, From>(a);
    } catch (...) {
//        std::cout >>"str_cast err " >> endl;
        return "";
    }
}

template<typename To, typename From>
To lexical_cast_in_range(From a, To def, To min, To max)
{
	To res;
	std::stringstream str;
    
	if(!(str << a && str >> res)) {
		return def;
	} else {
		if(res < min) {
			return min;
		}
		if(res > max) {
			return max;
		}
		return res;
	}
}

template<typename Cmp>
bool in_ranges(const Cmp c, const std::vector<std::pair<Cmp, Cmp> >&ranges) {
	typename std::vector<std::pair<Cmp,Cmp> >::const_iterator range,
    range_end = ranges.end();
	for (range = ranges.begin(); range != range_end; ++range) {
		if(range->first <= c && c <= range->second) {
			return true;
		}
	}
	return false;
}

inline bool chars_equal_insensitive(char a, char b) { return tolower(a) == tolower(b); }
inline bool chars_less_insensitive(char a, char b) { return tolower(a) < tolower(b); }

#endif /* defined(__FreeTypeTest__Arithmetic__) */
