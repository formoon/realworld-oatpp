#pragma once

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class UserDto : public oatpp::DTO {

  DTO_INIT(UserDto, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(String, username);
  DTO_FIELD(String, email);
  DTO_FIELD(String, bio);
  DTO_FIELD(String, image);
  DTO_FIELD(String, hash);
};
//---------------------------------------------

class UserAuthDto : public oatpp::DTO {

  DTO_INIT(UserAuthDto, DTO)

  DTO_FIELD(String, username);
  DTO_FIELD(String, email);
  DTO_FIELD(String, bio);
  DTO_FIELD(String, image);
  DTO_FIELD(String, token);
};

class UserAuthResultDto : public oatpp::DTO {
  DTO_INIT(UserAuthResultDto, DTO)

  DTO_FIELD(Object<UserAuthDto>, user);
};

//---------------------------------------------

class UserProfileDto : public oatpp::DTO {
  DTO_INIT(UserProfileDto, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(String, username);
  DTO_FIELD(String, bio);
  DTO_FIELD(String, image);
  DTO_FIELD(Boolean, following);
};
class UserProfileResultDto : public oatpp::DTO {
  DTO_INIT(UserProfileResultDto, DTO)

  DTO_FIELD(Object<UserProfileDto>, profile);
};

//---------------------------------------------

class LoginUserDataDto : public oatpp::DTO {
  DTO_INIT(LoginUserDataDto, DTO)

  DTO_FIELD(String, email);
  DTO_FIELD(String, password);
};

class LoginUserDto : public oatpp::DTO {
  DTO_INIT(LoginUserDto, DTO)

  DTO_FIELD(Object<LoginUserDataDto>, user);
};
//---------------------------------------------
class NewUserDataDto : public oatpp::DTO {
  DTO_INIT(NewUserDataDto, DTO)

  DTO_FIELD(String, username);
  DTO_FIELD(String, email);
  DTO_FIELD(String, password);
};

class NewUserDto : public oatpp::DTO {
  DTO_INIT(NewUserDto, DTO)

  DTO_FIELD(Object<NewUserDataDto>, user);
};
//---------------------------------------------

class UpdateUserDataDto : public oatpp::DTO {
  DTO_INIT(UpdateUserDataDto, DTO)

  DTO_FIELD(String, username);
  DTO_FIELD(String, email);
  DTO_FIELD(String, bio);
  DTO_FIELD(String, image);
  DTO_FIELD(String, password);
};
class UpdateUserDto : public oatpp::DTO {
  DTO_INIT(UpdateUserDto, DTO)

  DTO_FIELD(Object<UpdateUserDataDto>, user);
};


#include OATPP_CODEGEN_END(DTO)

