#include <Poco/Runnable.h>
#include <Poco/Thread.h>
#include <Poco/Util/Application.h>

#include <iostream>

using Poco::Util::Application;

class HelloRunnable : public Poco::Runnable {
  virtual void run() {
    while (true) {
      Poco::Thread::sleep(1000);
      std::cout << "hello, world!->" << Poco::Thread::currentTid() << std::endl;
    }
  }
};

class MyApp : public Application {
protected:
  void initialize(Application &self) {

    this->loadConfiguration();
    Application::initialize(self);
  }

  void uninitialize() { Application::uninitialize(); }

  int main(const std::vector<std::string> &arguments) {
    HelloRunnable runnable;

    Poco::Thread thread;
    thread.start(runnable);
    std::cout << thread.tid() << std::endl;
    std::cout << thread.currentTid() << std::endl;
    thread.join();
    return EXIT_OK;
  }
};

POCO_APP_MAIN(MyApp)
