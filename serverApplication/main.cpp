#include <Poco/Foundation.h>
#include <Poco/Util/ServerApplication.h>

#include <iostream>

using Poco::Util::ServerApplication;
using Poco::Util::Application;

class MyApp : public ServerApplication {
protected:
  void initialize(Application &application) {
    this->loadConfiguration();
    ServerApplication::initialize(application);
  }

  void uninitialize() { ServerApplication::uninitialize(); }

  int main(const std::vector<std::string> &arguments) {
    if (config().getBool("application.runAsDaemon", true)) {
      std::cout << "Hello My App!!" << std::endl;
      Poco::Thread::sleep(1000);
      waitForTerminationRequest();
    }
    return EXIT_OK;
  }
};

POCO_SERVER_MAIN(MyApp)
