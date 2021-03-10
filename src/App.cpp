#include "controller/BaseController.hpp"
#include "controller/UserController.hpp"
#include "controller/ArticleController.hpp"
#include "controller/CommentController.hpp"
#include "AppComponent.hpp"
#include "DatabaseComponent.hpp"

#include "oatpp/network/Server.hpp"
#include <iostream>
#include "bcrypt/BCrypt.hpp"
#include "utils/UserAuth.hpp"

//--------------------------------------------------------------------------
int test_jwt(){
  UserAuth ua {UserAuth::newExp(),23,"abcdef"};
  string en=ua.toToken()->std_str();
  cout << en << endl;
  auto ub = UserAuth::fromToken(en.c_str());
  cout << ub.exp << " " << ub.id << " "<< ub.username->c_str() << endl;
  return 0;
}

int test_bcrypt(){
	std::string right_password = "right_password";
	std::string wrong_password = "wrong_password";

	std::cout << "generate hash... " << std::flush;
	std::string hash = BCrypt::generateHash(right_password, 12);
	std::cout << "done. hash:" << hash << std::endl;

	std::cout << "checking right password: " << std::flush
			  << BCrypt::validatePassword(right_password,hash) << std::endl;

	std::cout << "checking wrong password: " << std::flush
			  << BCrypt::validatePassword(wrong_password,hash) << std::endl;

	return 0;
}
//--------------------------------------------------------------------------
void run() {

  /* Register Components in scope of run() method */
  AppComponent components;
  DatabaseComponent dbcomponents;

  // test_jwt();

  // components.readConfig("./config.json");
  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  /* Create BaseController and add all of its endpoints to router */
  auto baseController = std::make_shared<BaseController>();
  baseController->addEndpointsToRouter(router);
  auto userController = std::make_shared<UserController>();
  userController->addEndpointsToRouter(router);
  auto articleController = std::make_shared<ArticleController>();
  articleController->addEndpointsToRouter(router);
  auto commentController = std::make_shared<CommentController>();
  commentController->addEndpointsToRouter(router);

  /* Get connection handler component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

  /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
  oatpp::network::Server server(connectionProvider, connectionHandler);

  /* Priny info about server port */
  OATPP_LOGI("MyApp", "Server running on port %s", connectionProvider->getProperty("port").getData());

  /* Run server */
  server.run();

}

int main(int argc, const char * argv[]) {
  // test_bcrypt();

  oatpp::base::Environment::init();

  auto logger = std::static_pointer_cast<oatpp::base::DefaultLogger>(oatpp::base::Environment::getLogger());
  // logger->disablePriority(oatpp::base::DefaultLogger::PRIORITY_D);

  run();

  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";

  oatpp::base::Environment::destroy();
  
  return 0;
}
