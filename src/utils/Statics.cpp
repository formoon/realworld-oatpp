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

#include "Statics.hpp"
#include <stdio.h>
#include <fstream>

Statics::Statics(const oatpp::String& resDir, bool streaming) {
  
  // if(!resDir || resDir->getSize() == 0) {
  if(!resDir || resDir->length() == 0) {
    throw std::runtime_error("[Statics::Statics()]: Invalid resDir path. Please specify full path to STATICS folder");
  }
  
  m_resDir = resDir;
  // if(m_resDir->getData()[m_resDir->getSize() - 1] != '/') {
  if(m_resDir->at(m_resDir->length() - 1) != '/') {
    m_resDir = m_resDir + "/";
  }

  m_streaming = streaming;

}
  
void Statics::cacheFile(const char* fileName) {
  m_resources[fileName] = loadFromFile(fileName);
}
  
oatpp::String Statics::loadFromFile(const char* fileName) {
  
  auto fullFilename = m_resDir + fileName;
  /*
  std::ifstream file (fullFilename->c_str(), std::ios::in|std::ios::binary|std::ios::ate);
  
  if (file.is_open()) {
    
    auto result = oatpp::String((v_int32) file.tellg());
    file.seekg(0, std::ios::beg);
    file.read((char*)result->getData(), result->getSize());
    file.close();
    return result;
    
  }*/
  oatpp::String rs = oatpp::String::loadFromFile(fullFilename->c_str());
  if (rs)
    return rs;

  OATPP_LOGE("Statics::loadFromFile()", "Can't load file '%s'", fullFilename->c_str());
  throw std::runtime_error("[Statics::loadFromFile(...)]: Can't load file. Please make sure you specified full path to STATIC folder");  
}
  
oatpp::String Statics::getFile(const oatpp::String& filename) {

  auto it = m_resources.find(filename);
  if(it != m_resources.end()) {
    // m_resources is a map, .first==key,.second==value
    return it->second;
  }
  throw std::runtime_error("[Statics::getFile(...)]: static file not in cache. ");
}

std::shared_ptr<Statics::ReadCallback> Statics::getFileStream(const oatpp::String &filename) {
  try {
    return std::make_shared<ReadCallback>(m_resDir + filename);
  } catch(std::runtime_error &e) {
    throw std::runtime_error("[Statics::getFileStream(...)]: Static file not found. ");
  }
}

Statics::ReadCallback::ReadCallback(const oatpp::String &file) : m_file(file), m_stream(file->c_str())
{}

oatpp::v_io_size Statics::ReadCallback::read(void *buffer, v_buff_size count, oatpp::async::Action& action) {
  return m_stream.read(buffer, count, action);
}
