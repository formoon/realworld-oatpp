#pragma once

#include <string>
#include <cstdio>
#include <cstdlib>
// #include <iostream>
#include "utils/Statics.hpp"
#include "utils/Config.hpp"
#include "utils/httpTools.hpp"
#include "service/CommentService.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/protocol/http/outgoing/StreamingBody.hpp"
#include "oatpp/core/async/Coroutine.hpp"
#include "oatpp/core/async/Executor.hpp"
#include "oatpp/web/protocol/http/incoming/Request.hpp"

#include "oatpp/web/mime/multipart/FileStreamProvider.hpp"
#include "oatpp/web/mime/multipart/InMemoryPartReader.hpp"
#include "oatpp/web/mime/multipart/Reader.hpp"
#include "oatpp/web/mime/multipart/PartList.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin codegen

class CommentController : public oatpp::web::server::api::ApiController {
  typedef oatpp::web::protocol::http::Status Status;
private:
  typedef CommentController __ControllerType;
  const CommentService m_commentService; // Create  service.
  OATPP_COMPONENT(std::shared_ptr<Config>, config);
public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  CommentController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper){
  }

  ENDPOINT_ASYNC("GET", "/api/articles/{slug}/comments", GetComments) {
    ENDPOINT_ASYNC_INIT(GetComments)
    Action act() override {
      auto slug=request->getPathVariable("slug");
      auto ard=controller->m_commentService.getBySlug(slug);
      return _return(controller->createDtoResponse(Status::CODE_200, ard));
    };
  };

  ENDPOINT_ASYNC("POST", "/api/articles/{slug}/comments", NewComment) {
    ENDPOINT_ASYNC_INIT(NewComment)
    Action act() override {
      return request->readBodyToDtoAsync<oatpp::Object<CommentInPakDto>>(
          controller->getDefaultObjectMapper()
      ).callbackTo(&NewComment::act1);
    }    
    Action act1(const oatpp::Object<CommentInPakDto>& cipd){
      auto ua=UserAuth::fromAuthHeader(request);
      if (ua.id == 0 && !controller->config->publicMode){
        return _return(controller->createResponse(Status::CODE_500,
          "Login first."));
      };
      auto slug=request->getPathVariable("slug");
      auto ctrd=controller->m_commentService.create(slug,cipd->comment->body,ua.id);
      return _return(controller->createDtoResponse(Status::CODE_200, ctrd));
    }
  };

  ENDPOINT_ASYNC("DELETE", "/api/articles/{slug}/comments/{cid}", DelComments) {
    ENDPOINT_ASYNC_INIT(DelComments)
    Action act() override {
      auto slug=request->getPathVariable("slug");
      Int32 cid=atoi(request->getPathVariable("cid")->c_str());
      auto ua=UserAuth::fromAuthHeader(request);
      if (ua.id == 0 && !controller->config->publicMode){
        return _return(controller->createResponse(Status::CODE_500,
          "Login first."));
      };
      auto drd=controller->m_commentService.del(slug,ua.id,cid);
      return _return(controller->createDtoResponse(Status::CODE_200, drd));
    };
  };


};

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- End codegen

