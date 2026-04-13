// Multiple pimpl inheritance with pimpl_type

#include <estd/idiom/pimpl.h>
#include <iostream>
#include <string>

// ============================================
// logger.h
// ============================================
class Logger : public es::idiom::pimpl<Logger, 64> {
public:
  explicit Logger(const std::string& name);

  void log(const std::string& message);
};

// ============================================
// service.h
// ============================================
class Service : public es::idiom::pimpl<Service, 64>, public Logger {
public:
  explicit Service(const std::string& name);

  void run();
};

// ============================================
// logger.cpp
// ============================================
struct LoggerImpl {
  std::string name;
  int logCount = 0;

  LoggerImpl(const std::string& n) : name(n) {}

  void log(const std::string& message) {
    ++logCount;
    std::cout << "[" << name << "] " << message << " (log #" << logCount
              << ")\n";
  }
};

template <>
struct es::idiom::pimpl_traits<Logger> {
  using impl_type = LoggerImpl;
};

Logger::Logger(const std::string& name) : pimpl(name) {}

void Logger::log(const std::string& message) { pimpl_cast(this)->log(message); }

// ============================================
// service.cpp
// ============================================
struct ServiceImpl {
  std::string serviceName;

  ServiceImpl(const std::string& n) : serviceName(n) {}
};

template <>
struct es::idiom::pimpl_traits<Service> {
  using impl_type = ServiceImpl;
  using pimpl_type = es::idiom::pimpl<Service, 64>;
};

Service::Service(const std::string& name)
    : es::idiom::pimpl<Service, 64>(name), Logger(name) {}

void Service::run() {
  auto* impl = pimpl_cast(this);
  std::cout << "Service '" << impl->serviceName << "' running...\n";

  log("Starting service");
  log("Service is active");
}

// ============================================
// main.cpp
// ============================================
int main() {
  std::cout << "=== Multiple pimpl Inheritance ===\n\n";

  std::cout << "sizeof(Logger) = " << sizeof(Logger) << " bytes\n";
  std::cout << "sizeof(Service) = " << sizeof(Service) << " bytes\n\n";

  Service svc("MyService");
  svc.run();

  std::cout << "\nNote: Service inherits from pimpl<Service> and Logger\n";
  std::cout << "pimpl_type = pimpl<Service, 64> routes pimpl_cast through "
               "Service's pimpl base\n";

  return 0;
}
