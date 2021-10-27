#pragma once

#include <string>
#include <cstdio>
#include <cstdlib>
// #include <iostream>
#include "utils/Statics.hpp"
#include "utils/Config.hpp"
#include "utils/httpTools.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/protocol/http/outgoing/StreamingBody.hpp"
#include "oatpp/core/async/Coroutine.hpp"
#include "oatpp/core/async/Executor.hpp"

#include "oatpp/web/mime/multipart/FileStreamProvider.hpp"
#include "oatpp/web/mime/multipart/InMemoryPartReader.hpp"
#include "oatpp/web/mime/multipart/Reader.hpp"
#include "oatpp/web/mime/multipart/PartList.hpp"

namespace multipart = oatpp::web::mime::multipart;

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin codegen

const char *TMPFILENAME="tmp_uploadedFile.dat"; //in upload path

/**
 * Sample Api Controller.
 */
class BaseController : public oatpp::web::server::api::ApiController {
private:
  typedef BaseController __ControllerType;
  std::shared_ptr<Statics> m_static, m_dl;
  OATPP_COMPONENT(std::shared_ptr<Config>, config);
  std::shared_ptr<string> m_board_mode;
  
public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  BaseController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper){
      m_static = Statics::streamFiles(config->staticFilePath);
      m_dl = Statics::streamFiles(config->upAndDownPath);
      m_board_mode=std::make_shared<string>((std::string("window.board_mode=")
        +(config->publicMode ? "true" : "false")));
  }

  ENDPOINT_ASYNC("GET", "/", Root) {
    ENDPOINT_ASYNC_INIT(Root)
    Action act() override {
      auto response = controller->createResponse(Status::CODE_302, "");
      response->putHeader("Location", "/index.html");
      return _return(response);
    }    
  };
  ENDPOINT_ASYNC("GET", "/index.html/*", Index) {
    ENDPOINT_ASYNC_INIT(Index)
    Action act() override {
      auto body = std::make_shared<oatpp::web::protocol::http::outgoing::StreamingBody>(
        controller->m_static->getFileStream("index.html")
      );
      return _return(OutgoingResponse::createShared(Status::CODE_200, body));
    }    
  };

  ENDPOINT_ASYNC("GET", "js/board_mode.js", PublicMode) {
    ENDPOINT_ASYNC_INIT(PublicMode)
    Action act() override {
      return _return(controller->createResponse(Status::CODE_200, 
        controller->m_board_mode->c_str()));
    }    
  };
  
  ENDPOINT_ASYNC("GET", "/about", About) {
    ENDPOINT_ASYNC_INIT(About)
    Action act() override {
      return _return(allowCors(controller->createDtoResponse(Status::CODE_200, 
        oatpp::Fields<oatpp::Any>({
          {"code", oatpp::Int32(200)},
          {"message", oatpp::String("realworld in c++ std-11, depend on OATPP web framework.")}
        }))));
    }    
  };

  ENDPOINT_ASYNC("GET", "/static/*", getStaticFiles) {
    ENDPOINT_ASYNC_INIT(getStaticFiles)
    std::string filename;
    Action act() override {
      // Aquire all string after * ,
      // getPathTail() return oatpp::String, from 1.3.0 directly assign to std::string
      filename=request->getPathTail();
      int qpos=filename.find("?"); // for ?query=xxxx
      if (qpos != std::string::npos)
        filename = filename.substr(0,qpos);
      OATPP_LOGI("BaseController"," Url filename: %s", filename.c_str());
      if(controller->m_static->isStreaming()) {
        auto body = std::make_shared<oatpp::web::protocol::http::outgoing::StreamingBody>(
          controller->m_static->getFileStream(filename.c_str())
        );
        return _return(addType(OutgoingResponse::createShared(Status::CODE_200, body),
            filename.c_str()));
      }
      return _return(controller->createResponse(Status::CODE_200, 
              controller->m_static->getFile(filename.c_str())));
    }
  };

  ENDPOINT_ASYNC("GET", "/dl/*", getDownloadFiles) {
    ENDPOINT_ASYNC_INIT(getDownloadFiles)
    std::string filename;
    Action act() override {
      filename=request->getPathTail();
      int qpos=filename.find("?"); // for ?query=xxxx
      if (qpos != std::string::npos)
        filename = filename.substr(0,qpos);
      filename = urlDecode(filename);
      OATPP_LOGI("BaseController.hpp"," Url filename: %s", filename.c_str());
      if(controller->m_dl->isStreaming()) {
        auto body = std::make_shared<oatpp::web::protocol::http::outgoing::StreamingBody>(
          controller->m_dl->getFileStream(filename.c_str())
        );
        return _return(OutgoingResponse::createShared(Status::CODE_200, body));
      }
      return _return(controller->createResponse(Status::CODE_200, 
              controller->m_dl->getFile(filename.c_str())));
    }
  };

  ENDPOINT_ASYNC("POST", "/uploadFile", MultipartUpload) {
    ENDPOINT_ASYNC_INIT(MultipartUpload)
    std::shared_ptr<multipart::PartList> m_multipart;
    std::shared_ptr<oatpp::data::stream::BufferOutputStream> m_bufferStream =
      std::make_shared<oatpp::data::stream::BufferOutputStream>();
    String tmpFilePath;
    Action act() override {
      tmpFilePath = controller->config->upAndDownPath+TMPFILENAME;
      m_multipart = std::make_shared<multipart::PartList>(request->getHeaders());
      auto multipartReader = std::make_shared<multipart::AsyncReader>(m_multipart);
      multipartReader->setPartReader("file", 
        multipart::createAsyncFilePartReader(tmpFilePath));
      multipartReader->setDefaultPartReader(
        multipart::createAsyncInMemoryPartReader(16 * 1024 * 1024/* max-data-size */));
      return request->transferBodyAsync(multipartReader)
        .next(yieldTo(&MultipartUpload::onUploaded));
    }

    Action onUploaded() {
      String filename, data;
      const auto& file = m_multipart->getNamedPart("file");
      if(file && (filename=file->getFilename())){
          filename = controller->config->upAndDownPath+filename;
          OATPP_LOGI("onUploaded"," upload filename: [%s]", filename->c_str());
          rename(tmpFilePath->c_str(),filename->c_str());
      } else {
        return _return(allowCors(controller->createDtoResponse(Status::CODE_200, 
          oatpp::Fields<oatpp::Any>({
            {"code", oatpp::Int32(404)},
            {"message", oatpp::String("upload file not found!")}
          }))));
      }
      
      return _return(allowCors(controller->createDtoResponse(Status::CODE_200, 
        oatpp::Fields<oatpp::Any>({
          {"code", oatpp::Int32(200)},
          {"message", oatpp::String("OK")},
          {"file-name", filename /* no file data */}
        }))));
    }
  };
  // TODO Insert Your endpoints here !!!
};

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- End codegen

