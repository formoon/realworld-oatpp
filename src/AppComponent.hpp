#pragma once

#include "oatpp/web/server/AsyncHttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/core/Types.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp-zlib/EncoderProvider.hpp"
#include "oatpp/web/server/interceptor/AllowCorsGlobal.hpp"

#include "utils/Config.hpp"

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
public:
  OATPP_CREATE_COMPONENT(std::shared_ptr<Config>, config)([&] {
    auto cfgs=std::make_shared<Config>("./config.json");
    return cfgs;
  }());
  // std::shared_ptr<Config> cfgs=Config::getConfig("./config.json");

  /* void readConfig(const char *filename){
    std::shared_ptr<Config> cfgs;
    static bool first=true;
    if (first){
      first=false;

      cfgs=Config::getConfig(filename);
      auto threadCfg = cfgs->cfgs["thread"].retrieve<oatpp::Fields<oatpp::Any>>();
      dataThread=(int)threadCfg["dataThread"].retrieve<oatpp::Float64>();
      ioThread=(int)threadCfg["ioThread"].retrieve<oatpp::Float64>();
      timerThread=(int)threadCfg["timerThread"].retrieve<oatpp::Float64>();
      auto server = cfgs->cfgs["server"].retrieve<oatpp::Fields<oatpp::Any>>();
      address= server["address"].retrieve<oatpp::String>();
      port=(v_uint16)server["port"].retrieve<oatpp::Float64>();
    }
  } */

  /**
   * Create Async Executor
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor)([&] {
    OATPP_COMPONENT(std::shared_ptr<Config>, config);
    return std::make_shared<oatpp::async::Executor>(
      config->dataThread, //4 /* Data-Processing threads */,
      config->ioThread, //1 /* I/O threads */,
      config->timerThread //1 /* Timer threads */
    );
  }());
  
  /**
   *  Create ConnectionProvider component which listens on the port
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([&] {
    OATPP_COMPONENT(std::shared_ptr<Config>, config);
    return oatpp::network::tcp::server::ConnectionProvider::createShared(
        {config->hostAddress, config->hostPort});
  }());
  
  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());
  
  /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
    OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor); // get Async executor component
    auto components = std::make_shared<oatpp::web::server::HttpProcessor::Components>(router);
    auto encoders = std::make_shared<oatpp::web::protocol::http::encoding::ProviderCollection>();
    encoders->add(std::make_shared<oatpp::zlib::GzipEncoderProvider>());
    encoders->add(std::make_shared<oatpp::zlib::DeflateEncoderProvider>());
    // encoders->Preferred="gzip";
    components->contentEncodingProviders = encoders;
    auto connectionHandler = std::make_shared<oatpp::web::server::AsyncHttpConnectionHandler>(components, executor);
    /* Add CORS-enabling interceptors */
    connectionHandler->addRequestInterceptor(std::make_shared<oatpp::web::server::interceptor::AllowOptionsGlobal>());
    connectionHandler->addResponseInterceptor(std::make_shared<oatpp::web::server::interceptor::AllowCorsGlobal>());
    // return std::make_shared<oatpp::web::server::AsyncHttpConnectionHandler>(components, executor);
    return connectionHandler;
  }());
  
  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
    return oatpp::parser::json::mapping::ObjectMapper::createShared();
  }());

};

