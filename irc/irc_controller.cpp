#include "irc_controller.hpp"

#include <iostream>
#include <vector>

#include <cstring>

namespace irc {
  IRCController::IRCController(IRCControllerDelegate* delegate) : delegate_(delegate) {}
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

  client_id IRCController::ClientConnected() {
    clients_.insert(free_client_id_);
    return free_client_id_++;
  }

  void IRCController::ClientDisconnected(client_id client) {
    clients_.erase(client);
  }

  void IRCController::ProcessCommand(client_id client, const std::string& command) {
    std::cout << IRCCommand(command) << std::endl;
    delegate_->SendMessage(client, "DROPPED LUL");
    delegate_->DropClient(client);
  }
}
