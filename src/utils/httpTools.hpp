#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>  
#include <regex>
#include <cstdlib>
#include <ctime>

#include "../3rd/MimeTypes.h"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

using namespace std;

typedef oatpp::web::protocol::http::outgoing::Response OutgoingResponse;
typedef oatpp::web::protocol::http::incoming::Request IncomingRequest;

bool is_email_valid(const std::string& email){
   // define a regular expression
   const std::regex pattern
      ("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

   // try to match the string with the regular expression
   return std::regex_match(email, pattern);
}

std::string getGmtTimeNow() {
  /**
   * Generate a UTC ISO8601-formatted timestamp
   * and return as std::string
   */
  auto now = std::chrono::system_clock::now();
  auto itt = std::chrono::system_clock::to_time_t(now);

  std::ostringstream ss;
  ss << std::put_time(gmtime(&itt), "%FT%TZ");
  return ss.str();
}

template<typename ... Args>
std::string stringFormat( const std::string& format, Args ... args ){
    int size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    std::unique_ptr<char[]> buf( new char[ size ] ); 
    snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
};

oatpp::String mkErrMsg(oatpp::String e){
    std::string r="{\"errors\": {\"" + e
         +"\":[\"Error\"]}}";
    return oatpp::String(r.c_str());
}

#define OATPP_ASSERT_HTTP1(COND, STATUS, MESSAGE) \
if(!(COND)) { status=STATUS;errmsg=MESSAGE;return nullptr; }

std::shared_ptr<OutgoingResponse> allowCors(std::shared_ptr<OutgoingResponse> response){
    response->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
    response->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
    response->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
    // response->putHeader("access-control-allow-headers",
    //         "DNT, User-Agent, X-Requested-With, If-Modified-Since, "
    //         "Cache-Control, Content-Type, Range, Authorization");
    return response;
};
std::shared_ptr<OutgoingResponse> addType(std::shared_ptr<OutgoingResponse> response,
    const char *fname){
    response->putHeaderIfNotExists("Content-Type",MimeTypes::getType(fname));
    return response;
};

oatpp::List<oatpp::String> split(oatpp::String src, oatpp::String dlt){
      auto rs=oatpp::List<oatpp::String>::createShared();
      std::string s = src;
      std::string delimiter = dlt;//"%%";
      size_t pos = 0;
      std::string token;
      while ((pos = s.find(delimiter)) != std::string::npos) {
          token = s.substr(0, pos);
          rs->push_back(token.c_str());
          s.erase(0, pos + delimiter.length());
      }
      if (s.length())
        rs->push_back(s.c_str());
      return rs;
};

string urlEncode(string str){
    string new_str;
    char c;
    int ic;
    const char* chars = str.c_str();
    char bufHex[10];
    int len = strlen(chars);

    for(int i=0;i<len;i++){
        c = chars[i];
        ic = c;
        // uncomment this if you want to encode spaces with +
        /*if (c==' ') new_str += '+';   
        else */if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') new_str += c;
        else {
            sprintf(bufHex,"%X",c);
            if(ic < 16) 
                new_str += "%0"; 
            else
                new_str += "%";
            new_str += bufHex;
        }
    }
    return new_str;
 }

string urlDecode(string str){
    string ret;
    char ch;
    int i, ii, len = str.length();

    for (i=0; i < len; i++){
        if(str[i] != '%'){
            if(str[i] == '+')
                ret += ' ';
            else
                ret += str[i];
        }else{
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        }
    }
    return ret;
}

string randomString(int n){
    string seed="abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHJKLMNOPQRSTUVWXYZ"
        "1234567890";
    srand(time(0));
    string rs;
    for(int i=0;i<n;i++){
        rs += seed[rand()%seed.length()];
    };
    return rs;
}
