#include <Poco/ConsoleChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/Logger.h>
#include <Poco/Message.h>
#include <Poco/PatternFormatter.h>
#include <Poco/SplitterChannel.h>
#include <Poco/Thread.h>
#include <Poco/Util/Application.h>

#include <iostream>

struct TestLogger {
  static std::string _logPath;

  static void test1() {

    Poco::SplitterChannel *pSplitterChannel = new Poco::SplitterChannel();
    pSplitterChannel->addChannel(new Poco::ConsoleChannel());

    std::string logFilename("test1.log");
    Poco::FileChannel *pFileChannel = new Poco::FileChannel;
    pFileChannel->setProperty("path",
                              Poco ::format("%s/%s", _logPath, logFilename));

    pSplitterChannel->addChannel(pFileChannel);

    Poco::Logger::create(
        "test1",
        new Poco::FormattingChannel(
            new Poco::PatternFormatter("%L%H-%M-%S.%i %t"), pSplitterChannel),
        Poco::Message::PRIO_TRACE);

    Poco::Logger &logger = Poco::Logger::get("test1");
    logger.trace("console log and file log");
  }

  static void test2() {
    std::string logFilename("test2.log");
    Poco::FileChannel *pFileChannel = new Poco::FileChannel;
    // the file is rotated when its size exceeds 5 bytes.
    pFileChannel->setProperty("rotation", "5");
    pFileChannel->setProperty("archive", "timestamp");
    pFileChannel->setProperty("path",
                              Poco ::format("%s/%s", _logPath, logFilename));
    Poco::Logger &logger = Poco::Logger::create(
        "test2",
        new Poco::FormattingChannel(
            new Poco::PatternFormatter("%L%H-%M-%S.%i %t"), pFileChannel),
        Poco::Message::PRIO_TRACE);

    for (int i = 0; i < 3; ++i)
      logger.trace("rotation log");
  }

  static void test3() {
    std::string logFilename("test3.log");

    Poco::Logger &logger = Poco::Logger::create(
        "test3",
        new Poco::FormattingChannel(
            new Poco::PatternFormatter("%L%H:%M:%S.%i [%P][%p] %t"),
            new Poco::ConsoleChannel),
        Poco::Message::PRIO_TRACE);

    logger.trace("PatternFormatter logging");
  }
};

std::string TestLogger::_logPath = "./log";

class MyApp : public Poco::Util::Application {
private:
  void initialize(Poco::Util::Application &self) {

    this->loadConfiguration();

    Poco::Util::Application::initialize(self);
  }

  void uninitialize() { Poco::Util::Application::uninitialize(); }

  int main(const std::vector<std::string> &arguments) {
    TestLogger::test1();
    TestLogger::test2();
    TestLogger::test3();
    return EXIT_OK;
  }
};

POCO_APP_MAIN(MyApp)
