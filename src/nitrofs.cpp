#include "../nitrofs.h"

namespace RGNDS {
    namespace Files {
        void* loadNitroFS(const char* filename, int size) {
            FILE* f = fopen(filename, "rb");
            if(size <= 0) {
                fseek(f, 0, SEEK_END);
                size = ftell(f);
                fseek(f, 0, SEEK_SET);
            }

            char* data = (char*)valloc(size+1);
            fread(data, size, 1, f);
            data[size] = 0;
            fclose(f);

            return (void*)data;
        }
    }
}
