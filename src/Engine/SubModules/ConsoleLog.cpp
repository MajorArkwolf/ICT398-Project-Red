#include "ConsoleLog.hpp"
#include <sstream>
#include <filesystem>


void ConsoleLog::AddLog(LogType type, const std::string& text, int line_number, const std::string& file) {
    time_t ttime = time(nullptr);
    tm* local_time = localtime(&ttime);
    char buffer[99];

    strftime(buffer, 99, "%d/%m/%y %X", local_time);

    LogLine line(type, text, line_number, file, buffer);
    log_repo_.push_back(line);

    outfile.open(file_path, std::ios::app);
    if (outfile.is_open()) {
        outfile << "[" << buffer << "] " << text << " [File Name: " << file << ", Line Number: " << line_number << ']' << std::endl;
        outfile.close();
    } else {
        std::stringstream error;
        LogLine error_line(LogType::Error, "Failed to write log to file", __LINE__, __FILE__, buffer);
        log_repo_.push_back(error_line);
    }

}

const std::vector<ConsoleLog::LogLine>& ConsoleLog::GetLogFile() {
    return log_repo_;
}

void ConsoleLog::StartLog(const std::string& base_path) {
    std::stringstream stream_file_path = std::stringstream();

    time_t ttime = time(nullptr);
    tm* local_time = localtime(&ttime);
    char buffer[99];
    strftime(buffer, 99, "%d-%m-%y_%H-%M-%S_RedEngineLog.txt", local_time);

    stream_file_path << base_path << "logs/" << buffer;
    std::filesystem::create_directory("logs");
    //mkdir("logs");
    file_path = stream_file_path.str();
    outfile.open(file_path.c_str());
    outfile.close();
}
