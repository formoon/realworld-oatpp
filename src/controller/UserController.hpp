#pragma once

#include <string>
#include <cstdio>
#include <cstdlib>
// #include <iostream>
#include "utils/Statics.hpp"
#include "utils/httpTools.hpp"
#include "service/UserService.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/protocol/http/outgoing/StreamingBody.hpp"
#include "oatpp/core/async/Coroutine.hpp"
#include "oatpp/core/async/Executor.hpp"
#include "oatpp/web/protocol/http/incoming/Request.hpp"

#include "oatpp/web/mime/multipart/FileStreamProvider.hpp"
#include "oatpp/web/mime/multipart/InMemoryPartReader.hpp"
#include "oatpp/web/mime/multipart/Reader.hpp"
#include "oatpp/web/mime/multipart/PartList.hpp"
// #include "opencvStitch.hpp"

// namespace multipart = oatpp::web::mime::multipart;

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin codegen

class UserController : public oatpp::web::server::api::ApiController {
  typedef oatpp::web::protocol::http::Status Status;
private:
  typedef UserController __ControllerType;
  const UserService m_userService; // Create user service.
public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  UserController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper){
  }

  ENDPOINT_ASYNC("GET", "/api/users/all/{offset}/{limit}", GetAll) {
    ENDPOINT_ASYNC_INIT(GetAll)
    Action act() override {
      oatpp::UInt32 offset = atoi(request->getPathVariable("offset")->c_str());
      oatpp::UInt32 limit = atoi(request->getPathVariable("limit")->c_str());
      auto response = controller->createDtoResponse(
          Status::CODE_200, controller->m_userService.getAll(offset, limit));
      return _return(response);
    }    
  };
  // ADD_CORS(Login)
  ENDPOINT_ASYNC("POST", "/api/users/login", Login) {
    ENDPOINT_ASYNC_INIT(Login)
    Action act() override {
      OATPP_LOGD("UserController", "Login act()");
      return request->readBodyToDtoAsync<oatpp::Object<LoginUserDto>>(
          controller->getDefaultObjectMapper()
      ).callbackTo(&Login::act1);
    }    
    Action act1(const oatpp::Object<LoginUserDto>& lud){
      Status status=Status::CODE_200;
      String errmsg;

      if(!lud->user->email || !lud->user->password){
        errmsg=mkErrMsg("Email or Password are invalid.");
        auto response=controller->createResponse(Status::CODE_400, errmsg);
        response->putHeaderIfNotExists("Content-Type","application/json");
        return _return(response);
      };
      OATPP_LOGD("UserController", "Login, user: %s",lud->user->email->c_str());
      auto uard=controller->m_userService.login(
          lud->user->email,lud->user->password,status,errmsg);
      if (status != Status::CODE_200){
        auto response=controller->createResponse(status, errmsg);
        response->putHeaderIfNotExists("Content-Type","application/json");
        return _return(response);
      }
      return _return(controller->createDtoResponse(Status::CODE_200, uard));
    }
  };
  ENDPOINT_ASYNC("GET", "/api/user", GetUserAuth) {
    ENDPOINT_ASYNC_INIT(GetUserAuth)
    Action act() override {
      auto ua=UserAuth::fromAuthHeader(request);
      if (ua.id == 0){
        return _return(controller->createResponse(Status::CODE_500,
          "Login first."));
      } else {
        auto uad=controller->m_userService.getAuthById(ua.id);
        auto uard=Object<UserAuthResultDto>::createShared();
        uard->user=uad;
        return _return(controller->createDtoResponse(Status::CODE_200, uard));
      }
    }    
  };
  ENDPOINT_ASYNC("POST", "/api/users", CreateUser) {
    ENDPOINT_ASYNC_INIT(CreateUser)
    Action act() override {
      return request->readBodyToDtoAsync<oatpp::Object<NewUserDto>>(
          controller->getDefaultObjectMapper()
      ).callbackTo(&CreateUser::act1);
    }    
    Action act1(const oatpp::Object<NewUserDto>& nud){
      Status status=Status::CODE_200;
      String errmsg;
      if(!nud->user->username || !nud->user->email || !nud->user->password){
        errmsg=mkErrMsg("All data must provide.");
        auto response=controller->createResponse(Status::CODE_400, errmsg);
        response->putHeaderIfNotExists("Content-Type","application/json");
        return _return(response);
      };
      if(nud->user->username->std_str().length() < 1 ||
          nud->user->password->std_str().length()<8){
        errmsg=mkErrMsg("min username length must > 0, min password length must > 8");
        auto response=controller->createResponse(Status::CODE_400, errmsg);
        response->putHeaderIfNotExists("Content-Type","application/json");
        return _return(response);
      };
      if(!is_email_valid(nud->user->email->std_str())){
        errmsg=mkErrMsg("email address invalid");
        auto response=controller->createResponse(Status::CODE_400, errmsg);
        response->putHeaderIfNotExists("Content-Type","application/json");
        return _return(response);
      };
      OATPP_LOGD("UserController", "NewUser, user: %s",nud->user->email->c_str());
      auto uard=controller->m_userService.newuser(
          nud->user->username,nud->user->email,nud->user->password,status,errmsg);
      if (status != Status::CODE_200){
        auto response=controller->createResponse(status, errmsg);
        response->putHeaderIfNotExists("Content-Type","application/json");
        return _return(response);
      }
      return _return(controller->createDtoResponse(Status::CODE_200, uard));
    }
  };
  ENDPOINT_ASYNC("GET", "/api/profiles/{username}", GetProfile) {
    ENDPOINT_ASYNC_INIT(GetProfile)
    Action act() override {
      auto ua=UserAuth::fromAuthHeader(request);
      if (ua.id == 0){
        return _return(controller->createResponse(Status::CODE_500,
          "Login first."));
      } else {
        String username = request->getPathVariable("username");
        auto uad=controller->m_userService.getProfileByName(username,ua.id);
        auto uard=Object<UserProfileResultDto>::createShared();
        uard->profile=uad;
        return _return(controller->createDtoResponse(Status::CODE_200, uard));
      }
    }    
  };

  ENDPOINT_ASYNC("POST", "/api/profiles/{username}/follow", Follow) {
    ENDPOINT_ASYNC_INIT(Follow)
    Action act() override {
      auto ua=UserAuth::fromAuthHeader(request);
      if (ua.id == 0){
        return _return(controller->createResponse(Status::CODE_500,
          "Login first."));
      };
      String username = request->getPathVariable("username");
      auto frd=controller->m_userService.follow(username,ua.id);
      return _return(controller->createDtoResponse(Status::CODE_200, frd));
    }    
  };

  ENDPOINT_ASYNC("DELETE", "/api/profiles/{username}/follow", UnFollow) {
    ENDPOINT_ASYNC_INIT(UnFollow)
    Action act() override {
      auto ua=UserAuth::fromAuthHeader(request);
      if (ua.id == 0){
        return _return(controller->createResponse(Status::CODE_500,
          "Login first."));
      };
      String username = request->getPathVariable("username");
      auto frd=controller->m_userService.unfollow(username,ua.id);
      return _return(controller->createDtoResponse(Status::CODE_200, frd));
    }    
  };

  ENDPOINT_ASYNC("PUT", "/api/user", UpdateUser) {
    ENDPOINT_ASYNC_INIT(UpdateUser)
    Action act() override {
      return request->readBodyToDtoAsync<oatpp::Object<UpdateUserDto>>(
          controller->getDefaultObjectMapper()
      ).callbackTo(&UpdateUser::act1);
    }    
    Action act1(const oatpp::Object<UpdateUserDto>& uud){
      auto ua=UserAuth::fromAuthHeader(request);
      if (ua.id == 0){
        return _return(controller->createResponse(Status::CODE_500,
          "Login first."));
      };

      if(!uud->user){
        String errmsg=mkErrMsg("json data error!");
        auto response=controller->createResponse(Status::CODE_400, errmsg);
        response->putHeaderIfNotExists("Content-Type","application/json");
        return _return(response);
      };

      auto uard=controller->m_userService.updateUser(uud->user,ua.id);
      return _return(controller->createDtoResponse(Status::CODE_200, uard));
    }
  };

};

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- End codegen

