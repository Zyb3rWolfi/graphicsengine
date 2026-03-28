//
// Created by Zyb3r on 28/03/2026.
//

#include "Helper.h"

void Helper::Log(const std::string &message, LogLevel level) {
    if (!Panel::debugList) {
        Panel::debugList = std::make_shared<std::vector<LogMessage>>();
    }

    Panel::debugList->push_back({level, message});

    // Keep the console from using too much memory
    if (Panel::debugList->size() > 100) {
        Panel::debugList->erase(Panel::debugList->begin());
    }
}
