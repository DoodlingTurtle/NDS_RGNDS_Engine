#include "textmenu.h"

#include <cstdio>
#include "../modules/RGNDS_Engine/engine.h"
namespace RGNDS {

    TextMenu::TextMenu() {
        cursor = '*';
        iSelected = 0;
    }
    TextMenu::~TextMenu() {
        for(auto o : options)
            free(o);

        options.clear();
    }

    void TextMenu::addOption(char* txt) {
        
        int size = 0;
        char* ptr = &(*txt);

        while(*ptr != 0) {
            size++;
            ptr++;
        }
        size++;
        char* buffer = (char*)malloc(sizeof(char)*size);

        memcpy(buffer, txt, size-1);
      
        if(size > maxlength)
            maxlength = size;

        buffer[size-1] = 0;

        options.push_back(buffer);
    }

    void TextMenu::draw() {
        char buffer[maxlength+2];

        char c;
        int i = 0;

        for( char* txt : options ) {
            c = (i==iSelected) ? cursor : ' ';
            i++;
            
            sprintf(buffer, "%c %s", c, txt); 
            RGNDS::GL2D::glText(buffer, 0xffff, &transform, 31, 2);
            transform.pos.y += 10*transform.scale;
        }
        transform.pos.y -= 10 * options.size() * transform.scale;
    }

    void TextMenu::selectNext() {
        int s = iSelected;
        s++;
        if(s >= (int)options.size())
            s = 0;

        iSelected = s;
    }

    void TextMenu::selectPrev() {
        int s = iSelected;
        int si = options.size();
        if(si == 0) return ;

        s--;
        if(s < 0)
            s = si-1;

        iSelected = s;
    }

    int TextMenu::selected() { return iSelected; }
}
