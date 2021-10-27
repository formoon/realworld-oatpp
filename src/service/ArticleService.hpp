#pragma once

#include "db/ArticleDb.hpp"
#include "utils/UserAuth.hpp"
#include "utils/httpTools.hpp"

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/core/macro/component.hpp"

class ArticleService {
private:
  typedef oatpp::web::protocol::http::Status Status;
  OATPP_COMPONENT(std::shared_ptr<ArticleDb>, m_database); // Inject database component
public:
  bool isFavorited(Int32 uid,Int32 aid) const {
    auto dbResult = m_database->isFavorited(uid, aid);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    if(dbResult->hasMoreToFetch()) 
      return true;
    else
      return false;
  };
  bool isFollowing(Int32 uid,Int32 auid) const {
    auto dbResult = m_database->isFollowing(uid, auid);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    if(dbResult->hasMoreToFetch()) 
      return true;
    else
      return false;
  }
  oatpp::Vector<oatpp::Object<ArticleJsonDto>> toResult(
        oatpp::Vector<oatpp::Object<ArticleDbDto>> data,
        Int32 uid,
        bool needFollowing) const {
      auto jsonrs = oatpp::Vector<oatpp::Object<ArticleJsonDto>>::createShared();
      for(auto it: *data){
        auto item=ArticleJsonDto::createShared();
        item->id = it->id;
        item->slug = it->slug;
        item->title = it->title;
        item->description = it->description;
        item->body = it->body;
        item->favoritesCount = it->favorites_count;
        item->createdAt = it->created_at;
        item->updatedAt = it->updated_at;
        auto author = UserProfileDto::createShared();
        author->id = it->uid;
        author->username = it->username;
        author->bio = it->bio;
        author->image = it->image;

        if (needFollowing)
          author->following = isFollowing(uid,it->uid);
        // author->following = false;
        item->favorited = isFavorited(uid,it->id);      

        item->author=author;
        item->tagList=it->tag_list;
        jsonrs->push_back(item);
      };
      return jsonrs;
  };

  oatpp::Object<ArticlesResultDto> getAll(Int32 uid,
      const oatpp::UInt32& offset, const oatpp::UInt32& limit) const {
    auto dbResult = m_database->getAll(offset, limit);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    auto data=dbResult->fetch<oatpp::Vector<oatpp::Object<ArticleDbDto>>>();
    auto jsonrs=toResult(data,uid,false);
    auto rs=ArticlesResultDto::createShared();
    rs->articles=jsonrs;
    rs->articlesCount=data[0]->full_count;
    return rs;
  }
  oatpp::Object<ArticlesResultDto> getAllByAuthor(String author, Int32 uid,
      const oatpp::UInt32& offset, const oatpp::UInt32& limit) const {
    auto dbResult = m_database->getAllByAuthor(author, offset, limit);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    auto data=dbResult->fetch<oatpp::Vector<oatpp::Object<ArticleDbDto>>>();
    auto jsonrs=toResult(data,uid,false);
    auto rs=ArticlesResultDto::createShared();
    rs->articles=jsonrs;
    rs->articlesCount=data->size();
    return rs;
  }
  oatpp::Object<ArticlesResultDto> getAllByFavorited(String favorited, Int32 uid,
      const oatpp::UInt32& offset, const oatpp::UInt32& limit) const {
    auto dbResult = m_database->GetFavoritedUserByName(favorited);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), 
      Status::CODE_400, 
      mkErrMsg("No this user!"));
    auto userRs = dbResult->fetch<oatpp::Vector<oatpp::Object<UserDto>>>();
    dbResult = m_database->getAllByFavorite(userRs[0]->id, offset, limit);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    auto data=dbResult->fetch<oatpp::Vector<oatpp::Object<ArticleDbDto>>>();
    auto jsonrs=toResult(data,uid,false);
    auto rs=ArticlesResultDto::createShared();
    rs->articles=jsonrs;
    rs->articlesCount=data->size();
    return rs;
  }
  oatpp::Object<ArticlesResultDto> getAllByTag(String tag, Int32 uid,
      const oatpp::UInt32& offset, const oatpp::UInt32& limit) const {
    auto dbResult = m_database->getAllByTag(tag, offset, limit);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    auto data=dbResult->fetch<oatpp::Vector<oatpp::Object<ArticleDbDto>>>();
    auto jsonrs=toResult(data,uid,false);
    auto rs=ArticlesResultDto::createShared();
    rs->articles=jsonrs;
    rs->articlesCount=data->size();
    return rs;
  }
  oatpp::Object<ArticlesResultDto> getFeed(Int32 uid,
      const oatpp::UInt32& offset, const oatpp::UInt32& limit) const {
    auto dbResult = m_database->getFeed(uid,offset, limit);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    auto data=dbResult->fetch<oatpp::Vector<oatpp::Object<ArticleDbDto>>>();
    auto jsonrs=toResult(data,uid,false);
    auto rs=ArticlesResultDto::createShared();
    rs->articles=jsonrs;
    rs->articlesCount=data->size();
    return rs;
  }

  oatpp::Object<ArticleResultDto> getOne(String slug, Int32 uid) const {
    auto dbResult = m_database->getOneBySlug(slug);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), 
      Status::CODE_400, 
      mkErrMsg("Article not found along this slug!"));
    auto data=dbResult->fetch<oatpp::Vector<oatpp::Object<ArticleDbDto>>>();
    auto jsonrs=toResult(data,uid,true);
    auto rs=ArticleResultDto::createShared();
    rs->article=jsonrs[0];
    return rs;
  }

  oatpp::Object<TagResultDto> getTags() const {
    auto dbResult = m_database->getTags();
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    auto data = dbResult->fetch<oatpp::Vector<oatpp::Object<TagDto>>>();
    auto vs=Vector<String>::createShared();
    for(auto it:*data){
      vs->push_back(it->unnest);
    };
    auto rs=TagResultDto::createShared();
    rs->tags=vs;
    return rs;
  }
  oatpp::Object<ArticleResultDto> favorite(String slug,Int32 uid) const {
    auto dbResult = m_database->getOneBySlug(slug);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_500, "No this article to favorite.");
    auto data=dbResult->fetch<oatpp::Vector<oatpp::Object<ArticleDbDto>>>();
    Int32 aid=data[0]->id;
    int n=0;
    if (!isFavorited(uid,aid)){ //previous favorite after 
      dbResult = m_database->favorite_insert(uid,aid); //previous favorite after 
      OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
      dbResult = m_database->favorite_inc(aid); //previous favorite after 
      OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
      n=1;
    };
    auto jsonrs=toResult(data,uid,true);
    auto rs=ArticleResultDto::createShared();
    rs->article=jsonrs[0];
    rs->article->favoritesCount = rs->article->favoritesCount+n;
    return rs;
  }
  oatpp::Object<ArticleResultDto> unfavorite(String slug,Int32 uid) const {
    auto dbResult = m_database->getOneBySlug(slug);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_500, "No this article to favorite.");
    auto data=dbResult->fetch<oatpp::Vector<oatpp::Object<ArticleDbDto>>>();
    Int32 aid=data[0]->id;
    int n=0;
    if (isFavorited(uid,aid)){ //previous favorite after 
      dbResult = m_database->favorite_delete(uid,aid); //previous favorite after 
      OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
      dbResult = m_database->favorite_dec(aid); //previous favorite after 
      OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
      n=1;
    };
    auto jsonrs=toResult(data,uid,true);
    auto rs=ArticleResultDto::createShared();
    rs->article=jsonrs[0];
    rs->article->favoritesCount = rs->article->favoritesCount-n;
    return rs;
  }
  oatpp::Object<ArticleResultDto> newArticle(oatpp::Object<NewArticleDataDto>nadd,Int32 uid) const {
    string slug=slugify(nadd->title)+"_"+randomString(10);

    auto dbResult = m_database->createArticle(uid,slug.c_str(),nadd);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    dbResult = m_database->getOneBySlug(slug.c_str());
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), 
      Status::CODE_400, 
      mkErrMsg("Unknow error with new article's slug!"));
    auto data=dbResult->fetch<oatpp::Vector<oatpp::Object<ArticleDbDto>>>();
    auto jsonrs=toResult(data,uid,true);
    auto rs=ArticleResultDto::createShared();
    rs->article=jsonrs[0];
    return rs;
  }
  oatpp::Object<ArticleResultDto> updateArticle(oatpp::Object<UpdateArticleDataDto>uadd,Int32 uid) const {
    String orgSlug=uadd->slug;
    string slug=slugify(uadd->title)+"_"+randomString(10);
    uadd->slug=slug.c_str();

    auto dbResult = m_database->updateArticle(uid,orgSlug,uadd);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    dbResult = m_database->getOneBySlug(uadd->slug);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), 
      Status::CODE_400, 
      mkErrMsg("Article not found along this slug!"));
    auto data=dbResult->fetch<oatpp::Vector<oatpp::Object<ArticleDbDto>>>();
    auto jsonrs=toResult(data,uid,true);
    auto rs=ArticleResultDto::createShared();
    rs->article=jsonrs[0];
    return rs;
  }
  String deleteArticle(String slug,Int32 uid) const {
    auto dbResult = m_database->deleteArticle(slug,uid);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    String rs="Article success deleted.";
    return rs;
  }

};
