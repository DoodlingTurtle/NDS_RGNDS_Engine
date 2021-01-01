#ifndef __TextMenu_H__
#define __TextMenu_H__

#include <vector>
#include <string>
#include "../../transform.h"
#include "../../gl2d.h"

namespace RGNDS {
    enum TextMenuType {
        TEXTMENU_LIST,
        TEXTMENU_LINE,
        TEXTMENU_SINGLE_HORIZONTAL,
        TEXTMENU_SINGLE_VERTICAL
    };

    class TextMenu {
    public:
        TextMenu(TextMenuType textMenuType=TEXTMENU_LIST);
        virtual~TextMenu();
        
        void addOption(const std::string txt);
      
        void selectNext();
        void selectPrev();
        int selected();

        void setSelection(int index);

        int getPXWidth();

        void draw();
        RGNDS::Transform transform;

        std::string selectedText();

    protected:
        std::vector<std::string> options;
        char cursor; 
        int iSelected;

    private:
        int maxlength;
        TextMenuType type;
    };
}
#endif
