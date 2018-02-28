#include "irc_controller.hpp"

#include <iostream>
#include <vector>

#include <cstring>

namespace irc {
  IRCController::IRCController() = default;
  IRCController::~IRCController() = default;

  struct IRCCommand {
    IRCCommand() = delete;
    IRCCommand(const IRCCommand&) = default;
    IRCCommand(const std::string& message); 
    std::string prefix;
    std::string command;
    std::vector<std::string> arguments;
  };

  IRCCommand::IRCCommand(const std::string& message) {
    char* command_copy = strdup(message.c_str());
    char* command_ptr = command_copy;
    if (command_copy[0] == ':') {
      prefix = strsep(&command_copy, " ") + 1;
    }
    command = strsep(&command_copy, " ");
    while(command_copy) {
      if (command_copy[0] == ':') {
        arguments.emplace_back(command_copy);
        break;
      } else {
        arguments.emplace_back(strsep(&command_copy, " "));
      }
    }
    free(command_ptr);
  }

  std::ostream& operator << (std::ostream& stream, const IRCCommand& cmd) {
    stream << "IRCCommand(";
    if (!cmd.prefix.empty()) {
      stream << "prefix: " << cmd.prefix << ", ";
    }
    stream << "command: " << cmd.command;
    if (!cmd.arguments.empty()) {
      stream << ", arguments: ";
    }
    for (const auto& arg : cmd.arguments) {
      stream << arg << ", ";
    }
    return stream << ")";
  }

  std::string IRCController::ProcessCommand(const std::string& command) {
    std::cout << IRCCommand(command) << std::endl;
    return "";
  }
}
