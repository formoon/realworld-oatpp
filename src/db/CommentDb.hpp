#pragma once

#include "dto/CommentDto.hpp"
#include "oatpp-postgresql/orm.hpp"

#include OATPP_CODEGEN_BEGIN(DbClient) //<- Begin Codegen

class CommentDb : public oatpp::orm::DbClient {
public:
  CommentDb(const std::shared_ptr<oatpp::orm::Executor>& executor)
    : oatpp::orm::DbClient(executor) {}

  QUERY(getBySlug,
      "select comments.id,comments.body,users.username,"
      "to_char(comments.created_at, 'YYYY-MM-DD\"T\"HH24:MI:SS.MS\"Z\"') as created_at,"
      "to_char(comments.updated_at, 'YYYY-MM-DD\"T\"HH24:MI:SS.MS\"Z\"') as updated_at "
      "from comments "
      "inner join articles on comments.article = articles.id "
      "inner join users on comments.author=users.id "
      "where articles.slug = :slug "
      "order by comments.updated_at desc;",
      PREPARE(true), // user prepared statement!
      PARAM(String, slug))

  QUERY(getArticleBySlug,
      "select articles.id from articles "
      "where articles.slug = :slug;",
      PREPARE(true), // user prepared statement!
      PARAM(String, slug))
  QUERY(create,
      "insert into comments values ("
      "DEFAULT,:comment.body,:comment.article,:comment.author,"
      "current_timestamp,current_timestamp);",
      PREPARE(true), // user prepared statement!
      PARAM(oatpp::Object<CommentDbDto>,comment))
  QUERY(checkExist,
      "select comments.id from comments "
      "inner join articles on comments.article = articles.id "
      "where articles.slug = :slug and "
      "comments.author=:uid and comments.id=:cid;",
      PREPARE(true), // user prepared statement!
      PARAM(String, slug),
      PARAM(Int32, uid),
      PARAM(Int32, cid)
      )

  QUERY(del,
      "delete from comments "
      "where id=:cid;",
      PREPARE(true), // user prepared statement!
      PARAM(Int32, cid))
};

#include OATPP_CODEGEN_END(DbClient) //<- End Codegen

