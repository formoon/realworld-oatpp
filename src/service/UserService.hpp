#pragma once

#include "db/UserDb.hpp"
#include "utils/UserAuth.hpp"
#include "utils/httpTools.hpp"

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/core/macro/component.hpp"
//#include "oatpp/orm/QueryResult.hpp"
#include "bcrypt/BCrypt.hpp"

class UserService {
private:
  typedef oatpp::web::protocol::http::Status Status;
  OATPP_COMPONENT(std::shared_ptr<UserDb>, m_database); // Inject database component
public:
  oatpp::Vector<oatpp::Object<UserDto>> getAll(const oatpp::UInt32& offset, const oatpp::UInt32& limit) const {
    oatpp::UInt32 countToFetch = limit;

    auto dbResult = m_database->getAll(offset, countToFetch);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    auto data=dbResult->fetch<oatpp::Vector<oatpp::Object<UserDto>>>();
    for(auto it: *data){
      it->hash="";
    };
    return data;
  }
  bool isFollowing(Int32 uid,Int32 auid) const { //previous follower after 
    auto dbResult = m_database->isFollowing(uid, auid);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    if(dbResult->hasMoreToFetch()) 
      return true;
    else
      return false;
  }
  oatpp::Object<UserAuthResultDto> login(const oatpp::String& email, const oatpp::String& password,
      Status &status, oatpp::String& errmsg) const {
    auto dbResult = m_database->getByEmail(email);
    OATPP_ASSERT_HTTP1(dbResult->isSuccess(), 
      Status::CODE_400, 
      mkErrMsg(dbResult->getErrorMessage()));

    OATPP_ASSERT_HTTP1(dbResult->hasMoreToFetch(), 
      Status::CODE_400, 
      mkErrMsg("Email or password not match"));

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<UserDto>>>();
    OATPP_ASSERT_HTTP1(result->size() == 1, 
      Status::CODE_400, 
      mkErrMsg("Unknown error"));

    auto it=result[0];
    OATPP_ASSERT_HTTP1(BCrypt::validatePassword(password->std_str(),it->hash->std_str()), 
        Status::CODE_400, 
        mkErrMsg("Email or password not match"));

    UserAuth ua {UserAuth::newExp(), it->id ,it->username};
    auto uad = oatpp::Object<UserAuthDto>::createShared();
    uad->username = it->username;
    uad->email = it->email;
    uad->bio = it->bio;
    uad->image = it->image;
    uad->token = ua.toToken();

    auto uard=Object<UserAuthResultDto>::createShared();
    uard->user=uad;

    return uard;
  }
  Object<UserAuthResultDto> newuser(const oatpp::String& username, 
      const oatpp::String& email, const oatpp::String& password,
      Status &status, oatpp::String& errmsg) const {
	  String hash = BCrypt::generateHash(password->std_str(), 12).c_str();
    auto dbResult = m_database->newuser(username,email,hash);
    OATPP_ASSERT_HTTP1(dbResult->isSuccess(), 
      Status::CODE_400, 
      mkErrMsg(dbResult->getErrorMessage()));

    OATPP_ASSERT_HTTP1(dbResult->hasMoreToFetch(), 
      Status::CODE_400, 
      mkErrMsg("Email or password not match"));

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<UserDto>>>();
    OATPP_ASSERT_HTTP1(result->size() == 1, 
      Status::CODE_400, 
      mkErrMsg("Unknown error"));

    auto it=result[0];

    UserAuth ua {UserAuth::newExp(), it->id ,it->username};
    auto uad = oatpp::Object<UserAuthDto>::createShared();
    uad->username = it->username;
    uad->email = it->email;
    uad->bio = it->bio;
    uad->image = it->image;
    uad->token = ua.toToken();

    auto uard=Object<UserAuthResultDto>::createShared();
    uard->user=uad;

    return uard;
  }

  Object<UserAuthResultDto> updateUser(oatpp::Object<UpdateUserDataDto> uud,
      Int32 uid) const {
    std::shared_ptr<oatpp::orm::QueryResult> dbResult;
    if(uud->password){
  	  uud->password = BCrypt::generateHash(uud->password->std_str(), 12).c_str();
      dbResult = m_database->updateWithPassword(uud,uid);
    } else {
      dbResult = m_database->update(uud,uid);
    }
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), 
      Status::CODE_400, 
      mkErrMsg(dbResult->getErrorMessage()));

    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), 
      Status::CODE_400, 
      mkErrMsg("Unknow error"));

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<UserDto>>>();
    OATPP_ASSERT_HTTP(result->size() == 1, 
      Status::CODE_400, 
      mkErrMsg("Unknown error"));

    auto it=result[0];

    UserAuth ua {UserAuth::newExp(), it->id ,it->username};
    auto uad = oatpp::Object<UserAuthDto>::createShared();
    uad->username = it->username;
    uad->email = it->email;
    uad->bio = it->bio;
    uad->image = it->image;
    uad->token = ua.toToken();
    auto uard=Object<UserAuthResultDto>::createShared();
    uard->user=uad;

    return uard;
  }

  oatpp::Object<UserAuthDto> getAuthById(Int32 uid) const {
    auto dbResult = m_database->getById(uid);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), 
      Status::CODE_400, 
      mkErrMsg(dbResult->getErrorMessage()));

    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), 
      Status::CODE_400, 
      mkErrMsg("User not found"));

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<UserDto>>>();
    OATPP_ASSERT_HTTP(result->size() == 1, 
      Status::CODE_400, 
      mkErrMsg("Unknown error"));

    auto it=result[0];

    UserAuth ua {UserAuth::newExp(), it->id ,it->username};
    auto uad = oatpp::Object<UserAuthDto>::createShared();
    uad->username = it->username;
    uad->email = it->email;
    uad->bio = it->bio;
    uad->image = it->image;
    uad->token = ua.toToken();

    return uad;
  }
  oatpp::Object<UserProfileDto> getProfileByName(String username,Int32 uid) const {
    auto dbResult = m_database->getByName(username);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), 
      Status::CODE_400, 
      mkErrMsg(dbResult->getErrorMessage()));

    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), 
      Status::CODE_400, 
      mkErrMsg("User not found"));

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<UserDto>>>();
    OATPP_ASSERT_HTTP(result->size() == 1, 
      Status::CODE_400, 
      mkErrMsg("Unknown error"));

    auto it=result[0];

    // UserAuth ua {UserAuth::newExp(), it->id ,it->username};
    auto uad = oatpp::Object<UserProfileDto>::createShared();
    uad->username = it->username;
    // uad->email = it->email;
    uad->bio = it->bio;
    uad->image = it->image;
    uad->following = isFollowing(uid,it->id);

    return uad;
  }
  oatpp::Fields<oatpp::Any> follow(String username,Int32 uid) const {
    auto dbResult = m_database->getByName(username);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_500, "No this username to follow.");
    auto userRs = dbResult->fetch<oatpp::Vector<oatpp::Object<UserDto>>>();
    auto fid=userRs[0]->id;
    if (!isFollowing(uid,fid)){ //previous follower after 
      dbResult = m_database->Follow(uid,fid); //previous follower after 
      OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    }
    auto prs=UserProfileDto::createShared();
    prs->username=userRs[0]->username;
    prs->following=true;
    auto rs=oatpp::Fields<oatpp::Any>({
        {"profile", prs}
      });
    return rs;
  }
  oatpp::Fields<oatpp::Any> unfollow(String username,Int32 uid) const {
    auto dbResult = m_database->getByName(username);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_500, "No this username to follow.");
    auto userRs = dbResult->fetch<oatpp::Vector<oatpp::Object<UserDto>>>();
    auto fid=userRs[0]->id;

    dbResult = m_database->UnFollow(uid,fid); //previous follower after 
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    auto prs=UserProfileDto::createShared();
    prs->username=userRs[0]->username;
    prs->following=false;
    auto rs=oatpp::Fields<oatpp::Any>({
        {"profile", prs}
      });
    return rs;
  }


};
