#include <any>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <gtest/gtest.h>

TEST(any, int) {
  std::any v = 1;
  ASSERT_EQ(std::any_cast<int>(v), 1);
}

TEST(any, float) {
  std::any v = 1.0f;
  ASSERT_EQ(std::any_cast<float>(v), 1.0);
}

TEST(any, string) {
  std::any v = std::string("hello");
  ASSERT_EQ(*std::any_cast<std::string>(&v), "hello");
}

TEST(any, vector) {
  std::vector<std::any> v = {1, 2.0f, std::string("hello")};
  ASSERT_EQ(v.size(), 3);
  ASSERT_EQ(std::any_cast<int>(v[0]), 1);
  ASSERT_EQ(std::any_cast<float>(v[1]), 2.0f);
  ASSERT_EQ(*std::any_cast<std::string>(&v[2]), "hello");
}

class CommandLine {
public:
  CommandLine(const CommandLine&) = delete;
  CommandLine(CommandLine&&) = delete;
  CommandLine &operator=(const CommandLine&) = delete;
  CommandLine &operator=(CommandLine&&) = delete;

  void addCommand(const std::string& name, std::any option) {
    commands[name] = std::move(option);
  }

  void addCommand(const std::string& option) {
    commands[option] = std::any(true);
  }

  template <typename T>
  T get(const std::string& key, T&& defRet) {
    auto it = commands.find(key);
    if (it == commands.end())
      return defRet;
    return std::any_cast<T>(it->second);
  }

  bool any(const std::string& key) {
    auto it = commands.find(key);
    if (it == commands.end())
      return false;
    return std::any_cast<bool>(it->second);
  }

  static CommandLine& ins() {
    static CommandLine cmd;
    return cmd;
  }

private:
  CommandLine() = default;

private:
  std::map<std::string, std::any> commands;
};

enum class CPPVersion {
  kCPP98,
  kCPP11,
  kCPP14,
  kCPP17
};

TEST(CommandLine, test) {
  auto& cmdl = CommandLine::ins();
  cmdl.addCommand("std", CPPVersion::kCPP17);
  cmdl.addCommand("O2");
  cmdl.addCommand("o", std::string("filename"));

  ASSERT_TRUE(cmdl.any("O2"));
  ASSERT_FALSE(cmdl.any("O0"));

  ASSERT_EQ(cmdl.get<CPPVersion>("std", CPPVersion::kCPP98), CPPVersion::kCPP17);
  ASSERT_EQ(cmdl.get<std::string>("o", "a.out"), "filename");
}
