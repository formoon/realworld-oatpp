#pragma once

#include "db/UserDb.hpp"
#include "db/ArticleDb.hpp"
#include "db/CommentDb.hpp"
#include "utils/Config.hpp"

class DatabaseComponent {
public:

  OATPP_COMPONENT(std::shared_ptr<Config>, config);
  /* Create database-specific ConnectionProvider */
  std::shared_ptr<oatpp::postgresql::ConnectionProvider> m_ConnectionProvider = 
    std::make_shared<oatpp::postgresql::ConnectionProvider>(config->dbUrl);

  /* Create database-specific ConnectionPool */
  std::shared_ptr<oatpp::postgresql::ConnectionPool> m_ConnectionPool = 
    oatpp::postgresql::ConnectionPool::createShared(
        m_ConnectionProvider,
        config->dbPool /* max-connections */,
        std::chrono::seconds(config->dbTtl) /* connection TTL */);

  /* Create database-specific Executor */
  std::shared_ptr<oatpp::postgresql::Executor> m_DbExecutor = 
    std::make_shared<oatpp::postgresql::Executor>(m_ConnectionPool);

  /**
   * Create database client
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<UserDb>, userDb)([&] {
    /* Create MyClient database client */
    return std::make_shared<UserDb>(m_DbExecutor);
  }());
  OATPP_CREATE_COMPONENT(std::shared_ptr<ArticleDb>, articleDb)([&] {
    /* Create MyClient database client */
    return std::make_shared<ArticleDb>(m_DbExecutor);
  }());
  OATPP_CREATE_COMPONENT(std::shared_ptr<CommentDb>, commentDb)([&] {
    /* Create MyClient database client */
    return std::make_shared<CommentDb>(m_DbExecutor);
  }());
};

