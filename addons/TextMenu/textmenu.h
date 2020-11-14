#ifndef __TextMenu_H__
#define __TextMenu_H__

#include <vector>
#include "../modules/RGNDS_Engine/transform.h"
#include "../modules/RGNDS_Engine/gl2d.h"

namespace RGNDS {
    class TextMenu {
    public:
        TextMenu();
        virtual~TextMenu();
        
        void addOption(char* txt);
      
        void selectNext();
        void selectPrev();
        int selected();

        void draw();

        RGNDS::Transform transform;

    protected:
        std::vector<char*> options;
        char cursor; 
        int iSelected;

    private:
        int maxlength;
    };
}
#endif
