#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "bridge.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace KeyRemap4MacBook_client {
    enum {
      TIMEOUT_SECOND = 0,
      TIMEOUT_MICROSECOND = 100 * 1000, // 100ms
    };
    void initialize(void);
    void terminate(void);

    void refreshSockAddr(void);
    int sendmsg(KeyRemap4MacBook_bridge::RequestType type, void* request, uint32_t requestsize, void* reply, uint32_t replysize);
  }
}

#endif
