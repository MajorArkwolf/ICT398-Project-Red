#pragma once
#include <string>
#include <tuple>
#include <vector>
#include <ctime>
#include <fstream>

class ConsoleLog {
   public:
    enum class LogType {
        Engine,
        Collision,
        Resolution,
        Error,
        Warning
    };
    using LogLine = std::tuple<LogType, std::string, int, std::string, std::string>;

    ConsoleLog();
    ~ConsoleLog() = default;

    void AddLog(LogType type, const std::string& text, int line_number = -1, std::string file = "");
    const std::vector<LogLine>& GetLogFile();
    void SetBasePath(std::string base_path);
    void StartLog();

    ConsoleLog(const ConsoleLog&) = delete;
    ConsoleLog& operator=(const ConsoleLog&) = delete;
    ConsoleLog(ConsoleLog&&) = delete;
    ConsoleLog& operator=(ConsoleLog&&) = delete;

   private:
    std::string file_path = " ";
    std::vector<LogLine> log_repo_;
    std::ofstream outfile;
};