#pragma once

#include <chrono>
#include <ctime>
#include <ratio>
#include <string>

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "utils/Config.hpp"
#include "utils/httpTools.hpp"
#include "jwt/jwt.hpp"

using namespace jwt::params;
using namespace oatpp;

class UserAuth {
typedef oatpp::web::protocol::http::incoming::Request IncomingRequest;
public:
    Int64 exp;
    Int32 id;
    String username;

    UserAuth(Int64 exp,Int32 pid, String pusername):
        exp(exp),id(pid),username(pusername) {
    };
    static Int64 newExp(){
        std::chrono::duration<int> days (60*60*24 * 60);
        auto e=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now() + days);
        return e;
    };
    String toToken(){
        OATPP_COMPONENT(std::shared_ptr<Config>, m_config);
        jwt::jwt_object obj{algorithm("HS256"),secret(m_config->secretKey->c_str())};
        obj
            .add_claim("exp",(uint64_t) exp)
            .add_claim("id",(uint64_t)id)
            .add_claim("username",username);

        auto enc_str = obj.signature();
        return String(enc_str.c_str());
    }
    static UserAuth fromToken(String token){
        OATPP_COMPONENT(std::shared_ptr<Config>, m_config);
        auto dec_obj = jwt::decode(
            token->c_str(), 
            algorithms({"HS256"}), 
            secret(m_config->secretKey->c_str()));
        Int64 exp=(Int64)dec_obj.payload().get_claim_value<uint64_t>("exp");
        Int32 id=(Int32)dec_obj.payload().get_claim_value<uint64_t>("id");
        String username=String(dec_obj.payload().get_claim_value<std::string>("username").c_str());
        OATPP_LOGD("UserAuth","fromToken, user: %s",username->c_str());
        auto e=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        if (exp < e) {
            exp = (int64_t)0LL;
            id = 0;
            username = ""; 
        };
        return UserAuth {exp,id,username};
    };
    static UserAuth fromAuthHeader(std::shared_ptr<IncomingRequest> request){
        auto authheader=request->getHeader("authorization");
        if(authheader){
            const char *as=authheader->c_str()+6;
            return UserAuth::fromToken(as); //skip string "Token "
        } else {
            return UserAuth((int64_t)0LL,0,"");
        }
    };

    String toString(){
        String output = stringFormat("UserAuth[exp:%ld id:%d name:%s]",
            (long long)exp,(int)id,username->c_str()).c_str();
        return output;
    };
};

