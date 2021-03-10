#pragma once

#include "dto/UserDto.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

// post body ------------------------------------------
class CommentInBodyDto : public oatpp::DTO {
  DTO_INIT(CommentInBodyDto, DTO)

  DTO_FIELD(String, body);
};
class CommentInPakDto : public oatpp::DTO {
  DTO_INIT(CommentInPakDto, DTO)

  DTO_FIELD(Object<CommentInBodyDto>, comment);
};
// db ---------------------------------------------
// for write db
class CommentDbDto : public oatpp::DTO {
  DTO_INIT(CommentDbDto, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(String, body);
  DTO_FIELD(Int32, article);
  DTO_FIELD(Int32, author);
};
//for read db
class CommentDto : public oatpp::DTO {
  DTO_INIT(CommentDto, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(String, body);
  DTO_FIELD(String, username);
  DTO_FIELD(String, created_at);
  DTO_FIELD(String, updated_at);
};
// get article from slug
class CommentArticleDto : public oatpp::DTO {
  DTO_INIT(CommentArticleDto, DTO)

  DTO_FIELD(Int32, id);
};
// web response ----------------------------------------------
class CommentJsonDto : public oatpp::DTO {
  DTO_INIT(CommentJsonDto, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(String, body);
  DTO_FIELD(Object<UserProfileDto>, author);
  DTO_FIELD(String, createdAt);
  DTO_FIELD(String, updatedAt);
};

class CommentJsonResultDto : public oatpp::DTO {
  DTO_INIT(CommentJsonResultDto, DTO)

  DTO_FIELD(Vector<Object<CommentJsonDto>>, comments);
};

#include OATPP_CODEGEN_END(DTO)
