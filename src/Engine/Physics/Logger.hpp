#pragma once
#include "reactphysics3d/reactphysics3d.h"

/**
* @class Logger
* @brief This class is used to log things from the engine
* inherits from the react  physics 3d logger class to grab errors from reactphysics3d
*/
class Logger : public reactphysics3d::Logger {

    /**
* @brief Logs a particular text string
* @param level The level of error that has occured
*/
    void log(Level level, const std::string &physicsWorldName, Category category,
             const std::string &message, const char *filename, int lineNumber) override;
};