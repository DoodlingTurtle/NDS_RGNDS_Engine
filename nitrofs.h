#ifndef __NitroFS_H__
#define __NitroFS_H__

#include <nds.h>
#include <filesystem.h>

namespace RGNDS {
    namespace Files {

        void* loadNitroFS(int size, const char* filename);

    } 
}
#endif
