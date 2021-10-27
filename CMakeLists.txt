cmake_minimum_required(VERSION 3.1)

set(project_name realworld-oatpp) ## rename your project here

project(${project_name})

# brew install openssl
set(OPENSSL_ROOT_DIR /usr/local/opt/openssl)
#set(OPENSSL_LIBRARIES /usr/local/opt/openssl/lib)

# mac: brew install nlohmann-json
# ubuntu: sudo apt install nlohmann-json-dev
# centos: sudo yum install json-devel

# setting JWT CMakeLists.txt file:
#option(CPP_JWT_BUILD_EXAMPLES "build examples" OFF)
#option(CPP_JWT_BUILD_TESTS "build tests" OFF)
#option(CPP_JWT_USE_VENDORED_NLOHMANN_JSON "use vendored json header" ON)       #linux
#option(CPP_JWT_USE_VENDORED_NLOHMANN_JSON "use vendored json header" OFF)       #mac

set(CMAKE_CXX_STANDARD 11)

find_package(oatpp 1.3.0 REQUIRED)
find_package(oatpp-zlib 1.3.0 REQUIRED)
find_package(oatpp-postgresql   1.3.0 REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(src)
include_directories(${OpenCV_INCLUDE_DIRS})

add_subdirectory(3rd/libbcrypt)
include_directories(3rd/libbcrypt/include)
add_subdirectory(3rd/cpp-jwt)
include_directories(3rd/cpp-jwt/include)

# .cpp file must add, .hpp file can not add here if already #include
add_library(${project_name}-lib
        src/utils/Statics.cpp
        src/utils/deunicode.cpp
        3rd/MimeTypes.cpp
)

## link libs
link_directories(${OpenCV_LIBRARY_DIRS})

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test
        PUBLIC oatpp::oatpp-zlib
        PUBLIC oatpp::oatpp-postgresql
        PUBLIC ZLIB::ZLIB
        bcrypt
        cpp-jwt::cpp-jwt
)
#target_include_directories(${project_name}-lib PUBLIC src)

## add executables

add_executable(${project_name}
        src/App.cpp
)

target_link_libraries(${project_name} ${project_name}-lib)
add_dependencies(${project_name} ${project_name}-lib)

#add_executable(${project_name}-test
#        test/tests.cpp
#        test/app/TestComponent.hpp
#        test/app/MyApiTestClient.hpp
#        test/BaseControllerTest.cpp
#        test/BaseControllerTest.hpp
#)
#target_link_libraries(${project_name}-test ${project_name}-lib)
#add_dependencies(${project_name}-test ${project_name}-lib)

#set_target_properties(${project_name}-lib ${project_name} ${project_name}-test PROPERTIES
set_target_properties(${project_name}-lib ${project_name} PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
        LINKER_LANGUAGE CXX
)

#enable_testing()
#add_test(project-tests ${project_name}-test)
