#pragma once

#include "dto/ArticleDto.hpp"
#include "oatpp-postgresql/orm.hpp"

#include OATPP_CODEGEN_BEGIN(DbClient) //<- Begin Codegen

class ArticleDb : public oatpp::orm::DbClient {
public:
  ArticleDb(const std::shared_ptr<oatpp::orm::Executor>& executor)
    : oatpp::orm::DbClient(executor) {}

  QUERY(getAll,
      "select articles.id,articles.slug,articles.title,articles.description,"
      "articles.body,articles.tag_list,"
      "to_char(articles.created_at, 'YYYY-MM-DD\"T\"HH24:MI:SS.MS\"Z\"') as created_at,"
      "to_char(articles.updated_at, 'YYYY-MM-DD\"T\"HH24:MI:SS.MS\"Z\"') as updated_at,"
      "articles.favorites_count,count(*) OVER() AS full_count,"
      "users.id as uid,users.username,users.bio,users.image "
      "from articles "
      "inner join users on users.id = articles.author "
      "order by articles.updated_at desc "
      "LIMIT :limit OFFSET :offset;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::UInt32, offset),
      PARAM(oatpp::UInt32, limit))
  QUERY(getAllByTag,
      "select articles.id,articles.slug,articles.title,articles.description,"
      "articles.body,articles.tag_list,"
      "to_char(articles.created_at, 'YYYY-MM-DD\"T\"HH24:MI:SS.MS\"Z\"') as created_at,"
      "to_char(articles.updated_at, 'YYYY-MM-DD\"T\"HH24:MI:SS.MS\"Z\"') as updated_at,"
      "articles.favorites_count,"
      "users.id as uid,users.username,users.bio,users.image "
      "from articles "
      "inner join users on users.id = articles.author "
      "where articles.tag_list && array[:tag] "
      "order by articles.updated_at desc "
      "LIMIT :limit OFFSET :offset;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(String, tag),
      PARAM(oatpp::UInt32, offset),
      PARAM(oatpp::UInt32, limit))
  QUERY(getAllByAuthor,
      "select articles.id,articles.slug,articles.title,articles.description,"
      "articles.body,articles.tag_list,"
      "to_char(articles.created_at, 'YYYY-MM-DD\"T\"HH24:MI:SS.MS\"Z\"') as created_at,"
      "to_char(articles.updated_at, 'YYYY-MM-DD\"T\"HH24:MI:SS.MS\"Z\"') as updated_at,"
      "articles.favorites_count,"
      "users.id as uid,users.username,users.bio,users.image "
      "from articles "
      "inner join users on users.id = articles.author "
      "where users.username = :author "
      "order by articles.updated_at desc "
      "LIMIT :limit OFFSET :offset;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(String, author),
      PARAM(oatpp::UInt32, offset),
      PARAM(oatpp::UInt32, limit))

  QUERY(getAllByFavorite,
      "select articles.id,articles.slug,articles.title,articles.description,"
      "articles.body,articles.tag_list,"
      "to_char(articles.created_at, 'YYYY-MM-DD\"T\"HH24:MI:SS.MS\"Z\"') as created_at,"
      "to_char(articles.updated_at, 'YYYY-MM-DD\"T\"HH24:MI:SS.MS\"Z\"') as updated_at,"
      "articles.favorites_count,"
      "users.id as uid,users.username,users.bio,users.image "
      "from articles "
      "inner join users on users.id = articles.author "
      "where articles.id IN (SELECT favorites.article FROM favorites WHERE favorites.user = :fid) "
      "order by articles.updated_at desc "
      "LIMIT :limit OFFSET :offset;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(Int32, fid),
      PARAM(oatpp::UInt32, offset),
      PARAM(oatpp::UInt32, limit))

  QUERY(getFeed, // favorited articles
      "select articles.id,articles.slug,articles.title,articles.description,"
      "articles.body,articles.tag_list,"
      "to_char(articles.created_at, 'YYYY-MM-DD\"T\"HH24:MI:SS.MS\"Z\"') as created_at,"
      "to_char(articles.updated_at, 'YYYY-MM-DD\"T\"HH24:MI:SS.MS\"Z\"') as updated_at,"
      "articles.favorites_count,"
      "users.id as uid,users.username,users.bio,users.image "
      "from articles "
      "inner join users on users.id = articles.author "
      "left join favorites on favorites.article = articles.id "
      "where favorites.user=:uid "
      "order by articles.updated_at desc "
      "LIMIT :limit OFFSET :offset;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::Int32, uid),
      PARAM(oatpp::UInt32, offset),
      PARAM(oatpp::UInt32, limit))

  QUERY(getOneBySlug,
      "select articles.id,articles.slug,articles.title,articles.description,"
      "articles.body,articles.tag_list,"
      "to_char(articles.created_at, 'YYYY-MM-DD\"T\"HH24:MI:SS.MS\"Z\"') as created_at,"
      "to_char(articles.updated_at, 'YYYY-MM-DD\"T\"HH24:MI:SS.MS\"Z\"') as updated_at,"
      "articles.favorites_count,"
      "users.id as uid,users.username,users.bio,users.image "
      "from articles "
      "inner join users on users.id = articles.author "
      "where articles.slug = :slug "
      "order by articles.updated_at desc ",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::String, slug))

  QUERY(getTags,
      "select distinct unnest(tag_list) from articles;",
      PREPARE(true) //<-- user prepared statement!
      )

  QUERY(GetFavoritedUserByName,
      "select * from users where "
      "username=:favorited;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(String, favorited))

  QUERY(isFavorited,
      "select * from favorites where "
      "\"user\"=:uid and article=:aid;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::Int32, uid),
      PARAM(oatpp::Int32, aid))
  QUERY(isFollowing,
      "select * from follows where "
      "follower=:uid and followed=:auid;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::Int32, uid),
      PARAM(oatpp::Int32, auid))
  QUERY(favorite_insert,
      "insert into favorites values (:uid , :aid);",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::Int32, uid),
      PARAM(oatpp::Int32, aid))
  QUERY(favorite_delete,
      "delete from favorites where "
      "\"user\"=:uid and article=:aid;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::Int32, uid),
      PARAM(oatpp::Int32, aid))
  QUERY(favorite_inc,
      "update articles set favorites_count=favorites_count+1 "
      "where id=:aid;",
      // "RETURNING *;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::Int32, aid))
  QUERY(favorite_dec,
      "update articles set favorites_count=favorites_count-1 "
      "where id=:aid;",
      // "RETURNING *;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::Int32, aid))
  QUERY(createArticle,
    "insert into articles "
    "(title,body,description,favorites_count,author,"
    "slug,created_at,updated_at,tag_list) VALUES "
    "(:nad.title,:nad.body,:nad.description,0,:uid,"
    ":slug,current_timestamp,current_timestamp,:nad.tagList);",
    PREPARE(true), //<-- user prepared statement!
    PARAM(oatpp::Int32, uid),
    PARAM(oatpp::String, slug),
    PARAM(oatpp::Object<NewArticleDataDto>,nad))
  QUERY(updateArticle,
    "update articles set "
    "title=:uad.title,body=:uad.body,description=:uad.description,"
    "updated_at=current_timestamp,slug=:uad.slug,tag_list=:uad.tagList "
    "where slug=:slug",
    PREPARE(true), //<-- user prepared statement!
    PARAM(oatpp::Int32, uid),
    PARAM(oatpp::String, slug),
    PARAM(oatpp::Object<UpdateArticleDataDto>,uad))
  QUERY(deleteArticle,
      "delete from articles where "
      "slug=:slug and author=:uid;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::String, slug),
      PARAM(oatpp::Int32, uid))

};

#include OATPP_CODEGEN_END(DbClient) //<- End Codegen

