#ifndef BaseControllerTest_hpp
#define BaseControllerTest_hpp

#include "oatpp-test/UnitTest.hpp"

class BaseControllerTest : public oatpp::test::UnitTest {
public:

  BaseControllerTest() : UnitTest("TEST[BaseControllerTest]"){}
  void onRun() override;

};

#endif // BaseControllerTest_hpp
