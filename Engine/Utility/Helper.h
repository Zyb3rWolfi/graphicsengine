//
// Created by Zyb3r on 28/03/2026.
//

#ifndef HELPER_H
#define HELPER_H
#include <string>

#include "Engine/UI/Panel.h"


class Helper {
    public:
      static void Log(const std::string& message, LogLevel level = LogLevel::INFO);
};



#endif //HELPER_H
