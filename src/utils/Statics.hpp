/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi, <lganzzzo@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************************/

#ifndef static_files_hpp
#define static_files_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/data/stream/ChunkedBuffer.hpp"
#include "oatpp/core/data/stream/FileStream.hpp"

#include <unordered_map>

class Statics {
private:
  oatpp::String m_resDir;
  std::unordered_map<oatpp::String, oatpp::String> m_resources;
  bool m_streaming;
private:
  oatpp::String loadFromFile(const char* fileName);
  void cacheFile(const char* fileName);

  class ReadCallback : public oatpp::data::stream::ReadCallback {
   private:
    oatpp::String m_file;
    oatpp::data::stream::FileInputStream m_stream;

   public:

    ReadCallback(const oatpp::String& file);
    oatpp::v_io_size read(void *buffer, v_buff_size count, oatpp::async::Action& action) override;

  };

public:
  /**
   * Constructor.
   * @param resDir - directory containing static file resources.
   */
  Statics(const oatpp::String& resDir, bool streaming = false);
public:

  /**
   * Load and cache Swagger-UI resources.
   * @param resDir - directory containing static resources.
   * @return - `std::shared_ptr` to Statics.
   */
  static std::shared_ptr<Statics> cacheFiles(const oatpp::String& resDir) {
    auto res = std::make_shared<Statics>(resDir);

    res->cacheFile("test.html");
    
    return res;
  }

  /**
   * Stream static file directly from disk.
   * @param resDir - directory containing static resources.
   * @return - `std::shared_ptr` to Resources.
   */
  static std::shared_ptr<Statics> streamFiles(const oatpp::String& resDir) {
    auto res = std::make_shared<Statics>(resDir, true);

    return res;
  }

  /**
   * Get cached resource by filename.
   * @param filename - name of the resource file.
   * @return - &id:oatpp::String; containing resource binary data.
   */
  oatpp::String getFile(const oatpp::String& filename);

  /**
   * Get streamed resource by filename.
   * @param filename - name of the resource file.
   * @return - `std::shared_ptr` to &id:oatpp::data::stream::ReadCallback; containing resource binary data stream."
   */
  std::shared_ptr<ReadCallback> getFileStream(const oatpp::String& filename);

  /**
   * Returns true if this is a streaming ressource instance.
   * @return
   */
  bool isStreaming() {
    return m_streaming;
  }
};

#endif /* static_file_hpp */
