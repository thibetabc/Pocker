//
//  string_utils.cpp
//  Wesnoth_copy
//
//  Created by MirrorMiko on 13-9-13.
//  Copyright (c) 2013年 MirrorMiko. All rights reserved.
//

#include "string_utils.h"
//#include "log.h"
#include "Arithmetic.hpp"
#include <algorithm>

namespace string_utils{
	bool isnewline(const char c){
		return c == '\r' || c == '\n';
	}

	// Make sure that we can use Mac, DOS, or Unix style text files on any system
	// and they will work, by making sure the definition of whitespace is consistent
	bool portable_isspace(const char c){
		// returns true only on ASCII spaces
		if (static_cast<unsigned char>(c) >= 128){
			return false;
		}
		return isnewline(c) || isspace(c);
	}

	// Make sure we regard '\r' and '\n' as a space, since Mac, Unix, and DOS
	// all consider these differently.
	bool notspace(const char c){
		return !portable_isspace(c);
	}

	std::string &strip(std::string &str){
		// If all the string contains is whitespace,
		// then the whitespace may have meaning, so don't strip it
		std::string::iterator it = std::find_if(str.begin(), str.end(), notspace);
		if (it == str.end()){
			return str;
		}

		str.erase(str.begin(), it);
		str.erase(std::find_if(str.rbegin(), str.rend(), notspace).base(), str.end());
		return str;
	}

	std::vector< std::string > split(std::string const &val, char c, int flags){
		std::vector< std::string > res;

		std::string::const_iterator i1 = val.begin();
		std::string::const_iterator i2 = val.begin();

		while (i2 != val.end()) {
			if (*i2 == c) {
				std::string new_val(i1, i2);
				if (flags & STRIP_SPACES){
					strip(new_val);
				}
				if (!(flags & REMOVE_EMPTY) || !new_val.empty()){
					res.push_back(new_val);
				}
				++i2;
				if (flags & STRIP_SPACES) {
					while (i2 != val.end() && *i2 == ' ')
						++i2;
				}

				i1 = i2;
			} else {
				++i2;
			}
		}

		std::string new_val(i1, i2);
		if (flags & STRIP_SPACES){
			strip(new_val);
		}
		if (!(flags & REMOVE_EMPTY) || !new_val.empty()){
			res.push_back(new_val);
		}

		return std::move(res);
	}


	std::vector< std::string > parenthetical_split(std::string const &val, 
		const char separator, std::string const &left, 
		std::string const &right,int flags){

		std::vector< std::string > res;
		std::vector<char> part;
		bool in_parenthesis = false;

		std::string::const_iterator i1 = val.begin();
		std::string::const_iterator i2 = val.begin();

		std::string lp=left;
		std::string rp=right;

		if(left.size()!=right.size()){
//			LOG_V("Left and Right Parenthesis lists not same length");
			return res;
		}

		while (i2 != val.end()) {
			if(!in_parenthesis && separator && *i2 == separator){
				std::string new_val(i1, i2);
				if (flags & STRIP_SPACES)
					strip(new_val);
				if (!(flags & REMOVE_EMPTY) || !new_val.empty())
					res.push_back(new_val);
				++i2;
				if (flags & STRIP_SPACES) {
					while (i2 != val.end() && *i2 == ' ')
						++i2;
				}
				i1=i2;
				continue;
			}
			if(!part.empty() && *i2 == part.back()){
				part.pop_back();
				if(!separator && part.empty()){
					std::string new_val(i1, i2);
					if (flags & STRIP_SPACES)
						strip(new_val);
					res.push_back(new_val);
					++i2;
					i1=i2;
				}else{
					if (part.empty())
						in_parenthesis = false;
					++i2;
				}
				continue;
			}
			bool found=false;
			for(size_t i=0; i < lp.size(); i++){
				if (*i2 == lp[i]){
					if (!separator && part.empty()){
						std::string new_val(i1, i2);
						if (flags & STRIP_SPACES)
							strip(new_val);
						res.push_back(new_val);
						++i2;
						i1=i2;
					}else{
						++i2;
					}
					part.push_back(rp[i]);
					found=true;
					break;
				}
			}
			if(!found){
				++i2;
			} else
				in_parenthesis = true;
		}

		std::string new_val(i1, i2);
		if (flags & STRIP_SPACES)
			strip(new_val);
		if (!(flags & REMOVE_EMPTY) || !new_val.empty())
			res.push_back(new_val);

		if(!part.empty()){
//				LOG_V("Mismatched parenthesis:%s",val.c_str());
		}

		return std::move(res);
	}



	std::pair< int, int > parse_range(std::string const &str){
		const std::string::const_iterator dash = std::find(str.begin(), str.end(), '-');
		const std::string a(str.begin(), dash);
		const std::string b = dash != str.end() ? std::string(dash + 1, str.end()) : a;
		std::pair<int,int> res(str_to_int(a), str_to_int(b));
		if (res.second < res.first)
			res.second = res.first;

		return std::move(res);
	}

	std::vector< std::pair< int, int > > parse_ranges(std::string const &str){
		std::vector< std::pair< int, int > > to_return;
		std::vector<std::string> strs = string_utils::split(str);
		std::vector<std::string>::const_iterator i, i_end=strs.end();
		for(i = strs.begin(); i != i_end; ++i) {
			to_return.push_back(parse_range(*i));
		}
		return std::move(to_return);
	}
    
    std::string Base64Encode(const unsigned char* Data,int DataByte){
        //编码表
        const char EncodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        //返回值
        std::string strEncode;
        unsigned char Tmp[4]={0};
        int LineLength=0;
        for(int i=0;i<(int)(DataByte / 3);i++){
            Tmp[1] = *Data++;
            Tmp[2] = *Data++;
            Tmp[3] = *Data++;
            strEncode+= EncodeTable[Tmp[1] >> 2];
            strEncode+= EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
            strEncode+= EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
            strEncode+= EncodeTable[Tmp[3] & 0x3F];
            if(LineLength+=4,LineLength==76) {strEncode+="\r\n";LineLength=0;}
        }
        //对剩余数据进行编码
        int Mod=DataByte % 3;
        if(Mod==1){
            Tmp[1] = *Data++;
            strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
            strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4)];
            strEncode+= "==";
        }else if(Mod==2){
            Tmp[1] = *Data++;
            Tmp[2] = *Data++;
            strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
            strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
            strEncode+= EncodeTable[((Tmp[2] & 0x0F) << 2)];
            strEncode+= "=";
        }
        
        return std::move(strEncode);
    }
    
    std::string Base64Encode(const std::string& input){
        std::string result = Base64Encode((unsigned char*)input.c_str(), input.size());
        return std::move(result);
    }
    
    std::string Base64Decode(const char* Data,int DataByte,int& OutByte){
        //解码表
        const char DecodeTable[] ={
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            62, // '+'
            0, 0, 0,
            63, // '/'
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
            0, 0, 0, 0, 0, 0, 0,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
            13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
            0, 0, 0, 0, 0, 0,
            26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
            39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
        };
        //返回值
        std::string strDecode;
        int nValue;
        int i= 0;
        while (i < DataByte){
            if (*Data != '\r' && *Data!='\n'){
                nValue = DecodeTable[*Data++] << 18;
                nValue += DecodeTable[*Data++] << 12;
                strDecode+=(nValue & 0x00FF0000) >> 16;
                OutByte++;
                if (*Data != '='){
                    nValue += DecodeTable[*Data++] << 6;
                    strDecode+=(nValue & 0x0000FF00) >> 8;
                    OutByte++;
                    if (*Data != '='){
                        nValue += DecodeTable[*Data++];
                        strDecode+=nValue & 0x000000FF;
                        OutByte++;
                    }
                }
                i += 4;
            }else{
                // 回车换行,跳过
                Data++;
                i++;
            }
        }
        return std::move(strDecode);
    }
    
    std::string Base64Decode(const std::string& input){
        int byte = 0;
        std::string result = Base64Decode(input.c_str(), input.size(), byte);
        return std::move(result);
    }
}