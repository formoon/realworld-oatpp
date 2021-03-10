#pragma once

#include "db/CommentDb.hpp"
#include "utils/httpTools.hpp"

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/core/macro/component.hpp"

class CommentService {
private:
  typedef oatpp::web::protocol::http::Status Status;
  OATPP_COMPONENT(std::shared_ptr<CommentDb>, m_database); // Inject database component
public:
  oatpp::Vector<oatpp::Object<CommentJsonDto>> toResult(
        oatpp::Vector<oatpp::Object<CommentDto>> data) const {
      auto jsonrs = oatpp::Vector<oatpp::Object<CommentJsonDto>>::createShared();
      for(auto it: *data){
        auto item=CommentJsonDto::createShared();
        item->id = it->id;
        item->body = it->body;
        item->createdAt = it->created_at;
        item->updatedAt = it->updated_at;
        auto author = UserProfileDto::createShared();
        author->username = it->username;
        item->author=author;
        jsonrs->push_back(item);
      };
      return jsonrs;
  };

  oatpp::Fields<oatpp::Any> create(String slug,String body,Int32 uid) const {
     auto dbResult = m_database->getArticleBySlug(slug);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_500, "Article not found about this slug");
    auto data=dbResult->fetch<oatpp::Vector<oatpp::Object<CommentArticleDto>>>();
    auto commentDb=CommentDbDto::createShared();
    commentDb->article = data[0]->id;
    commentDb->body = body;
    commentDb->author = uid;
    dbResult = m_database->create(commentDb);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    auto rs=oatpp::Fields<oatpp::Any>({
        {"comments", oatpp::String("[]")}
      });
    return rs;
  }
  oatpp::Object<CommentJsonResultDto> getBySlug(String slug) const {
    auto dbResult = m_database->getBySlug(slug);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    oatpp::Vector<oatpp::Object<CommentJsonDto>> jsonrs=nullptr;
    if (dbResult->hasMoreToFetch()){
      auto data=dbResult->fetch<oatpp::Vector<oatpp::Object<CommentDto>>>();
      jsonrs=toResult(data);
    }
    auto rs=CommentJsonResultDto::createShared();
    rs->comments=jsonrs;
    return rs;
  }
  oatpp::Fields<oatpp::Any> del(String slug,Int32 uid,Int32 cid) const {
    auto dbResult = m_database->checkExist(slug,uid,cid);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_500, "Error: No authorization or no this id");
    dbResult = m_database->del(cid);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    auto rs=oatpp::Fields<oatpp::Any>({
        {"comments", oatpp::String("Del comment success.")}
      });
    return rs;
  }


};
