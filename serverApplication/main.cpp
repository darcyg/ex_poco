#include <Poco/Foundation.h>
#include <Poco/Util/ServerApplication.h>

#include <iostream>

using Poco::Util::ServerApplication;
using Poco::Util::Application;

class MyApp : public ServerApplication {
protected:
  void initialize(Application &application) {
    this->loadConfiguration(
        "/root/project/ex_poco/serverApplication/test.json");
    this->loadConfiguration(
        "/root/project/ex_poco/serverApplication/test2.json");
    ServerApplication::initialize(application);
  }

  void uninitialize() { ServerApplication::uninitialize(); }

  int main(const std::vector<std::string> &arguments) {
    if (config().getBool("application.runAsDaemon", false)) {
      std::cout << "Hello My Daemon App!!" << std::endl;
    } else {
      std::cout << "Hello My App!!" << std::endl;
      std::cout << config().getString("test.prop4") << std::endl;
      std::cout << config().getString("test2.prop4") << std::endl;
      waitForTerminationRequest();
    }
    return EXIT_OK;
  }
};

POCO_SERVER_MAIN(MyApp)
