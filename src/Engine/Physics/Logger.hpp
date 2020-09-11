#pragma once
#include "reactphysics3d/reactphysics3d.h"

class Logger : public reactphysics3d::Logger {
    void log(Level level, const std::string &physicsWorldName, Category category,
             const std::string &message, const char *filename, int lineNumber) override;
};