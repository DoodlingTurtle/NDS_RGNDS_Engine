#ifndef __RGNDS_BROADCAST_H__
#define __RGNDS_BROADCAST_H__

#include <stdlib.h>
#include <vector>
#include <functional>

namespace RGNDS
{
    class Broadcast {
    public:
        void subscribe(std::function<void(int, void*)>*);
        void unsubscribe(std::function<void(int, void*)>*);
        void transmit(int, void*);

    protected:
        std::vector<std::function<void(int, void*)>*> listeners;
    };

} // namespace RGNDS

#endif