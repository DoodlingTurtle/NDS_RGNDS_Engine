#include "../broadcast.h"
#include "../engine.h"

#include <map>

namespace RGNDS {


    void Broadcast::subscribe(std::function<void(int, void*)>* fnc) {
        listeners.push_back( fnc );
        Engine_Log("Broadcast: Subscribe listener " << listeners.size());
    }

    void Broadcast::unsubscribe(std::function<void(int, void*)>* fnc) {
        for(int a = listeners.size()-1; a >= 0; a--) {
            if(listeners.at(a) == fnc) {
                listeners.erase(listeners.begin()+a);
            }
        }
        
        Engine_Log("Broadcast: Unsubscribe listener " << listeners.size());

    }

    void Broadcast::transmit(int event, void* data) {

        std::map<std::function<void(int, void*)>*, bool> processed;
        std::function<void(int, void*)>* l;

        for(int a = listeners.size()-1; a >= 0; a--) {
            l = listeners.at(a);

            if(processed.find(l) != processed.end()) continue;

            ((std::function<void(int, void*)>)*l)(event, data);

            processed.insert(processed.begin(), std::pair<std::function<void(int, void*)>*, bool>(l, true));
        }
    }

}