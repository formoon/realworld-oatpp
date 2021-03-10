#pragma once

#include "dto/UserDto.hpp"
#include "oatpp-postgresql/orm.hpp"

#include OATPP_CODEGEN_BEGIN(DbClient) //<- Begin Codegen

class UserDb : public oatpp::orm::DbClient {
public:
  UserDb(const std::shared_ptr<oatpp::orm::Executor>& executor)
    : oatpp::orm::DbClient(executor) {}

  QUERY(newuser,
      "INSERT INTO users"
      "(username, email, bio, image, hash ) VALUES "
      "(:username, :email, NULL, NULL, :hash)"
      "RETURNING *;",
      PREPARE(true), // user prepared statement!
      PARAM(oatpp::String, username),
      PARAM(oatpp::String, email),
      PARAM(oatpp::String, hash)
      )

  QUERY(update,
      "UPDATE users "
      "SET "
      " username=:user.username, "
      " email=:user.email, "
      " bio=:user.bio, "
      " image=:user.image "
      "WHERE "
      " id=:uid "
      "RETURNING *;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::Object<UpdateUserDataDto>, user),
      PARAM(oatpp::Int32, uid))
  QUERY(updateWithPassword,
      "UPDATE users "
      "SET "
      " username=:user.username, "
      " email=:user.email, "
      " bio=:user.bio, "
      " hash=:user.password, "
      " image=:user.image "
      "WHERE "
      " id=:uid "
      "RETURNING *;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::Object<UpdateUserDataDto>, user),
      PARAM(oatpp::Int32, uid))

  QUERY(getById,
      "SELECT * FROM users WHERE id=:id;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::Int32, id))

  QUERY(getByEmail,
      "SELECT * FROM users WHERE email=:email;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::String, email))
  QUERY(getByName,
      "SELECT * FROM users WHERE username=:username;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::String, username))

  QUERY(getAll,
      "SELECT * FROM users LIMIT :limit OFFSET :offset;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::UInt32, offset),
      PARAM(oatpp::UInt32, limit))

  QUERY(deleteById,
      "DELETE FROM users WHERE id=:id;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::Int32, id))
  QUERY(deleteByEmail,
      "DELETE FROM users WHERE email=:email;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::String, email))

  QUERY(isFollowing,
      "select * from follows where "
      "follower=:uid and followed=:auid;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::Int32, uid),
      PARAM(oatpp::Int32, auid))
  QUERY(Follow,
      "insert into follows values ("
      ":fid,:uid );",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::Int32, fid),
      PARAM(oatpp::Int32, uid))
  QUERY(UnFollow,
      "delete from follows where "
      "follower=:uid and followed=:fid;",
      PREPARE(true), //<-- user prepared statement!
      PARAM(oatpp::Int32, uid),
      PARAM(oatpp::Int32, fid))

};

#include OATPP_CODEGEN_END(DbClient) //<- End Codegen

