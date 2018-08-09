#include <Poco/Process.h>
#include <Poco/Runnable.h>
#include <Poco/SignalHandler.h>
#include <Poco/Thread.h>
#include <Poco/Util/Application.h>

#include <iostream>

using Poco::Util::Application;

class HelloRunnable : public Poco::Runnable {
public:
  virtual ~HelloRunnable() { std::cout << "thread exit" << std::endl; }
  virtual void run() {
    while (true) {
      Poco::Thread::sleep(1000);
      std::cout << "[" << Poco::Process::id() << "] "
                << "hello, world!->" << Poco::Thread::currentTid() << std::endl;
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
    try {
      poco_throw_on_signal;

      HelloRunnable runnable;

      Poco::Thread thread;

      thread.start(runnable);
      thread.join();
    } catch (Poco::Exception &ex) {
      std::cerr << ex.message() << std::endl;
    }

    std::cout << "exit main" << std::endl;
    Poco::Thread::sleep(5000);

    return EXIT_OK;
  }
};

POCO_APP_MAIN(MyApp)
