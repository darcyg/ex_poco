#include <Poco/AsyncChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/Exception.h>
#include <Poco/FileChannel.h>
#include <Poco/Format.h>
#include <Poco/FormattingChannel.h>
#include <Poco/Logger.h>
#include <Poco/Message.h>
#include <Poco/PatternFormatter.h>
#include <Poco/Runnable.h>
#include <Poco/SplitterChannel.h>
#include <Poco/Thread.h>
#include <Poco/Util/Application.h>

#include <iostream>
#include <memory>

class MassiveLogger : public Poco::Runnable {
private:
  int _num;
  std::string _name;

public:
  MassiveLogger() : _num(0), _name("") {}
  MassiveLogger(const std::string &name) : _num(0), _name(name) {}

  void setName(const std::string &name) { this->_name = name; }
  virtual void run() {
    Poco::Logger &logger = Poco::Logger::get("async_logger");
    for (int i = 0; i < 100000; ++i) {
      logger.trace(
          Poco::format("[%s]massive log->%d", this->_name, this->_num++));
    }
  }
};

class MyApp : public Poco::Util::Application {
private:
  void initialize(Poco::Util::Application &self) {

    this->loadConfiguration();

    Poco::Util::Application::initialize(self);
  }

  void uninitialize() { Poco::Util::Application::uninitialize(); }

  int main(const std::vector<std::string> &arguments) {
    try {
      std::string logPath("./"), logFilename("massive.log");
      Poco::FileChannel *pFileChannel = new Poco::FileChannel;
      pFileChannel->setProperty("rotation", "100 M");
      pFileChannel->setProperty("archive", "timestamp");
      pFileChannel->setProperty("path",
                                Poco ::format("%s/%s", logPath, logFilename));

      Poco::AsyncChannel *pAsyncChannel = new Poco::AsyncChannel(pFileChannel);

      Poco::Logger::create(
          "async_logger",
          new Poco::FormattingChannel(
              new Poco::PatternFormatter("%L%H:%M:%S.%i [%P][%T] %p %t"),
              pAsyncChannel),
          Poco::Message::PRIO_TRACE);

      /*
          Poco::ThreadPool::defaultPool().start(massiveLogger, "worker1");
          Poco::ThreadPool::defaultPool().start(massiveLogger, "worker2");
          Poco::ThreadPool::defaultPool().start(massiveLogger, "worker3");
          Poco::ThreadPool::defaultPool().start(massiveLogger, "worker4");
          Poco::ThreadPool::defaultPool().start(massiveLogger, "worker5");
          Poco::ThreadPool::defaultPool().start(massiveLogger, "worker6");
          Poco::ThreadPool::defaultPool().start(massiveLogger, "worker7");
          Poco::ThreadPool::defaultPool().start(massiveLogger, "worker8");
          Poco::ThreadPool::defaultPool().start(massiveLogger, "worker9");
          Poco::ThreadPool::defaultPool().start(massiveLogger, "worker10");

          Poco::ThreadPool::defaultPool().joinAll();
      */
      auto p = std::make_shared<MassiveLogger>("hasoo");

      Poco::ThreadPool *threadPool = new Poco::ThreadPool(2, 5);
      while (true) {
        try {
          std::cout << "used count:" << threadPool->used() << std::endl;
          while (true) {
            int n = 0;
            threadPool->start(*p, Poco::format("worker%d", ++n));
            Poco::Thread::sleep(100);
          }
        } catch (Poco::NoThreadAvailableException &ex) {
          std::cerr << ex.what() << std::endl;
        }
      }
    } catch (Poco::Exception &ex) {
      std::cerr << ex.what() << std::endl;
    }

    return EXIT_OK;
  }
};

POCO_APP_MAIN(MyApp)
