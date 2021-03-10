#include "BaseControllerTest.hpp"

#include "controller/BaseController.hpp"

#include "app/MyApiTestClient.hpp"
#include "app/TestComponent.hpp"

#include "oatpp/web/client/HttpRequestExecutor.hpp"

#include "oatpp-test/web/ClientServerTestRunner.hpp"

void BaseControllerTest::onRun() {

  /* Register test components */
  TestComponent component;

  /* Create client-server test runner */
  oatpp::test::web::ClientServerTestRunner runner;

  /* Add BaseController endpoints to the router of the test server */
  runner.addController(std::make_shared<BaseController>());

  /* Run test */
  runner.run([this, &runner] {

    /* Get client connection provider for Api Client */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, clientConnectionProvider);

    /* Get object mapper component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

    /* Create http request executor for Api Client */
    auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(clientConnectionProvider);

    /* Create Test API client */
    auto client = MyApiTestClient::createShared(requestExecutor, objectMapper);

    /* Call server API */
    /* Call hello endpoint of BaseController */
    auto response = client->getHello();

    /* Assert that server responds with 200 */
    OATPP_ASSERT(response->getStatusCode() == 200);

    /* Read response body as MessageDto */
    auto message = response->readBodyToDto<oatpp::Object<MessageDto>>(objectMapper.get());

    /* Assert that received message is as expected */
    OATPP_ASSERT(message);
    OATPP_ASSERT(message->statusCode == 200);
    OATPP_ASSERT(message->message == "Hello World Async!");

  }, std::chrono::minutes(10) /* test timeout */);

  /* Wait Executor finished */
  OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor);
  executor->waitTasksFinished();

  /* stop the executor */
  executor->stop();
  /* You must join or detach Executor threads */
  executor->join();

}
