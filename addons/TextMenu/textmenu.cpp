#include "textmenu.h"

#include <cstdio>
#include <math.h>

#include "../modules/RGNDS_Engine/engine.h"

namespace RGNDS {

    TextMenu::TextMenu(TextMenuType menuType) {
        cursor = '*';
        iSelected = 0;
        type = menuType;
        maxlength = 0;
        zDepth = 2;
    }
    TextMenu::~TextMenu() {
        options.clear();
    }

    void TextMenu::addOption(std::string txt) {

        if(txt.length() > maxlength)
            maxlength = txt.length();

        options.push_back(txt);
    }

    void TextMenu::draw() {
        char buffer[maxlength+6];

        char c;
        int i = 0;

        switch(type) {
            case TEXTMENU_SINGLE_HORIZONTAL:
                sprintf(buffer, "< %s >", options.at(iSelected).c_str()); 
                RGNDS::GL2D::glText(buffer, 0xffff, &transform, 31, zDepth);
                break;

            default:
            for( auto txt : options ) {
                c = (i==iSelected) ? cursor : ' ';
                i++;
                
                sprintf(buffer, "%c %s", c, txt.c_str()); 
                RGNDS::GL2D::glText(buffer, 0xffff, &transform, 31, zDepth);
                transform.pos.y += 10*transform.scale;
            }
            transform.pos.y -= 10 * options.size() * transform.scale;
        }
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

    void TextMenu::clearOptions() {
        options.clear();
        iSelected = 0;
    }

    void TextMenu::setSelection(int index) {
        if(index < 0 || index >= options.size())
            return;
        
        iSelected = index;
    }
    int TextMenu::getPXWidth() {
        int width = maxlength;
        switch(type) {
            case TEXTMENU_SINGLE_HORIZONTAL:
                width += 4;
                break;
        }

        return width * RGNDS::GL2D::defaultFont[0].width * transform.scale;
    }

    std::string TextMenu::selectedText() {
        return options.at(iSelected);
    }
}
