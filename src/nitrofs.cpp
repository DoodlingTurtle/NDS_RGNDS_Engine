#include "../nitrofs.h"

namespace RGNDS {
    namespace Files {
        
        void* loadNitroFS(int size, const char* filename) {
            FILE* f = fopen(filename, "rb");
            void* data = (void*)valloc(size);
            fread(data, size, 1, f);
            fclose(f);

            return data;
        }
    }
}
