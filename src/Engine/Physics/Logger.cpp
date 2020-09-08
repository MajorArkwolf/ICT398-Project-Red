#include "Logger.hpp"

#include <sstream>

#include "Engine/Engine.hpp"

void Logger::log(Level level, const std::string& physicsWorldName, Category category, const std::string& message, const char* filename, int lineNumber) {
    auto& engine = redengine::Engine::get();
    std::string cat = std::invoke([&]() {
        switch (category) {
            case Category::PhysicCommon: {
                return "PhysicsCommon";
            }
            case Category::Body: {
                return "Body";
            }
            case Category::Collider: {
                return "Collider";
            }
            case Category::World: {
                return "World";
            }
            case Category::Joint: {
                return "Joint";
            }
        }
    });

    std::stringstream log_stream;
    log_stream << "[Category]: " << cat << " [Message]: " << message ;
    
    engine.GetLog().AddLog(ConsoleLog::LogType::Collision, log_stream.str(), lineNumber, filename);
}
