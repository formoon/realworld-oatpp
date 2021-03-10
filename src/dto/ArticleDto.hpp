#pragma once

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include "dto/UserDto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class ArticleDbDto : public oatpp::DTO {
  DTO_INIT(ArticleDbDto, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(String, slug);
  DTO_FIELD(String, title);
  DTO_FIELD(String, description);
  DTO_FIELD(String, body);
  DTO_FIELD(Vector<String>, tag_list);
  DTO_FIELD(String, created_at);
  DTO_FIELD(String, updated_at);
  DTO_FIELD(Int32, favorites_count);

  DTO_FIELD(Int32, uid);
  DTO_FIELD(String, username);
  DTO_FIELD(String, bio);
  DTO_FIELD(String, image);
  DTO_FIELD(Int32, full_count);
};

//--------------------------------------
class ArticleJsonDto : public oatpp::DTO {
  DTO_INIT(ArticleJsonDto, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(String, slug);
  DTO_FIELD(String, title);
  DTO_FIELD(String, description);
  DTO_FIELD(String, body);
  DTO_FIELD(Object<UserProfileDto>, author);
  DTO_FIELD(Vector<String>, tagList);
  DTO_FIELD(String, createdAt);
  DTO_FIELD(String, updatedAt);
  DTO_FIELD(Int32, favoritesCount);
  DTO_FIELD(Boolean, favorited);
};

class ArticlesResultDto : public oatpp::DTO {
  DTO_INIT(ArticlesResultDto, DTO)

  DTO_FIELD(Vector<Object<ArticleJsonDto>>, articles);
  DTO_FIELD(Int64, articlesCount);
};
class ArticleResultDto : public oatpp::DTO {
  DTO_INIT(ArticleResultDto, DTO)

  DTO_FIELD(Object<ArticleJsonDto>, article);
};
//--------------------------------------
class TagDto: public oatpp::DTO {
  DTO_INIT(TagDto, DTO)

  DTO_FIELD(String, unnest);
};
class TagResultDto: public oatpp::DTO {
  DTO_INIT(TagResultDto, DTO)

  DTO_FIELD(Vector<String>, tags);
};

//--------------------------------------
class NewArticleDataDto: public oatpp::DTO {
  DTO_INIT(NewArticleDataDto, DTO)

  DTO_FIELD(String, title);
  DTO_FIELD(String, description);
  DTO_FIELD(String, body);
  DTO_FIELD(Vector<String>, tagList);
};
class NewArticleDto: public oatpp::DTO {
  DTO_INIT(NewArticleDto, DTO)

  DTO_FIELD(Object<NewArticleDataDto>, article);
};
//-------------------------------------------
class UpdateArticleDataDto: public oatpp::DTO {
  DTO_INIT(UpdateArticleDataDto, DTO)

  DTO_FIELD(String, title);
  DTO_FIELD(String, description);
  DTO_FIELD(String, body);
  DTO_FIELD(String, slug);
  DTO_FIELD(Vector<String>, tagList);
};
class UpdateArticleDto: public oatpp::DTO {
  DTO_INIT(UpdateArticleDto, DTO)

  DTO_FIELD(Object<UpdateArticleDataDto>, article);
};


#include OATPP_CODEGEN_END(DTO)

