#pragma once

#include <string>

namespace irc {
  class IRCController {
   public:
    IRCController();
    ~IRCController();

    std::string ProcessCommand(const std::string& command);

   private:
    
  };
}
