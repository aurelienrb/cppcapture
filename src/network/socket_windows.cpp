#include "socket_windows.h"
#include "utils.h"

namespace raven {
    void InitWinsock() {
        // don't use a static initialization object that calls WSAStartup() in its constructor because
        // it can make the code hang on VC++ if this function is itself called from another static object constructor
        // it seems there's a deadlock in atexit, not really sure why / if it's related to what does WSAStartup
        // under the hood: we just avoid to do that
        // Note: we don't cleanup Winsock on purpose since it's not really useful
        static bool s_initDone = false;
        if (!s_initDone) {
            s_initDone = true;
            LogDebug("initializing winsock");
            WSADATA WSAData;
            if (int err = WSAStartup(MAKEWORD(2, 0), &WSAData)) {
                LogError("failed to init Winsock, err=", err);
            }
        }
    }
} // namespace raven
