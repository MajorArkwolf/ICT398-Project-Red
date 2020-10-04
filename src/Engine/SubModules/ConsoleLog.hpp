#pragma once
#include <string>
#include <tuple>
#include <vector>
#include <ctime>
#include <fstream>

/**
* @class ConsoleLog
* @brief A log console for logging things
*/
class ConsoleLog {
   public:
    enum class LogType {
        Engine,
        Collision,
        Resolution,
        Error,
        Warning,
        Json
    };
    using LogLine = std::tuple<LogType, std::string, int, std::string, std::string>;

    ConsoleLog() = default;
    ~ConsoleLog() = default;

    /**
    * @brief Adds a new log to the log console and file output
    * @param type The type of log that it is
    * @param text The text displayed in the log output
    * @param line_number The Line number of the code that the log is added, use the macro __LINE__
    * @param file The file that the log was added from , use the macro __FILE__
    */
    void AddLog(LogType type, const std::string& text, int line_number = -1, const std::string& file = "");

    /**
    * @brief returns the log file so things that need to search through it can
    */
    const std::vector<LogLine>& GetLogFile();
    /**
    * Starts the log generation by creating the log file
    * @param the base_path to create the logs from  e.g.  base_path/logs/log.txt
    */
    void StartLog(const std::string& base_path);

    ConsoleLog(const ConsoleLog&) = delete;
    ConsoleLog& operator=(const ConsoleLog&) = delete;
    ConsoleLog(ConsoleLog&&) = delete;
    ConsoleLog& operator=(ConsoleLog&&) = delete;

   private:
    std::string file_path = " ";
    std::vector<LogLine> log_repo_;
    std::ofstream outfile;
};