#pragma once

#include <string>
#include <unordered_set>

namespace irc {
  typedef int client_id;

  class IRCControllerDelegate;

  class IRCController {
   public:
    IRCController(IRCControllerDelegate* delegate_);
    ~IRCController();

    // Process a new client connected action
    // and associate a client with him.
    client_id ClientConnected();

    // Process a client disconnect event.
    // No network writes shall be done to this client,
    // while reads are permitted.
    void ClientDisconnected(client_id client);
    void ProcessCommand(client_id client, const std::string& command);

   private:
    int free_client_id_ = 0;
    std::unordered_set<int> clients_;
    IRCControllerDelegate* delegate_;
  };

  class IRCControllerDelegate {
   public:
    virtual void DropClient(client_id client) = 0;
    virtual void SendMessage(client_id client, const std::string& message) = 0;
  };
}
