#include "../broadcast.h"

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
        for(std::function<void(int, void*)>* l : listeners) 
            ((std::function<void(int, void*)>)*l)(event, data);
    }

}