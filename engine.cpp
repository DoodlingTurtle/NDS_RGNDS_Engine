#include "engine.h"

#include <stdio.h>
#include <math.h>

#include <nds.h>
#include "inc/fonts_res.h"


// Including C/C++ Files into another compiled file is bad practice, but this
// will allow me to enable / disable what parts of the engine get compiled, depending on the included headers
// (vFor this to work, 'engine.cpp' must be the last file in your makefiles list of files to compile )

#include "src/timer.cpp"
#include "src/gl2d.cpp"
#include "src/transform.cpp"
#include "src/polyshape.cpp"


namespace RGNDS {

    int     Engine::FontTilesTextureID;
    bool    Engine::initialized = false;

    void Engine::init() {
        if(initialized) return;

    // Setup VRAM for displaying textures
        // BANK C and D are used as display-buffer for gl2d
        videoSetMode(MODE_5_3D);        // Top-Screen will always output gl2d
        videoSetModeSub(MODE_5_2D);     // The bottom screen needs some trickery to output gl2d to

        vramSetBankA(VRAM_A_TEXTURE);       // VRAM Bank A and B will give you 256KB of texture space for sprites
        vramSetBankB(VRAM_B_TEXTURE);
        vramSetBankF(VRAM_F_TEX_PALETTE);

    // For the Bottom Screen, The OAM / Sprite system is used to act as a buffer to display gl2d
        oamInit(&oamSub, SpriteMapping_Bmp_2D_256, false);

        int id = 0, y = 0, x = 0;

        for(y = 0; y < 3; y++)
            for(x = 0; x < 4; x++)
            {
                oamSub.oamMemory[id].attribute[0] = ATTR0_BMP | ATTR0_SQUARE | (64 * y);
                oamSub.oamMemory[id].attribute[1] = ATTR1_SIZE_64 | (64 * x);
                oamSub.oamMemory[id].attribute[2] = ATTR2_ALPHA(1) | (8 * 32 * y) | (8 * x);
                id++;
            }

        swiWaitForVBlank();
        oamUpdate(&oamSub);

        bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

        RGNDS::GL2D::glScreen2D();           // Initialize GL2D Engine

        Timer::init();   // Setup timer for the Engine

        Engine_Debug_Init();

        Engine::initialized = true;
    }

    Engine::Engine() {
        this->keepRunning = true;
    }
    Engine::~Engine(){
    }

    void Engine::error(const char* msg, int code) {
        this->keepRunning = false;
        this->lastError = msg;
        this->errorCode = code;
    }

    void Engine::run() {

        int tmp = onStart();
        if(tmp > 0)
            this->error("init failed: ", tmp);

        byte frame = 1;
        float deltaTime = 0.0f;
        while(this->keepRunning) {
            frame++;

            if(frame&1) {
                RGNDS::Timer::nextCycle();
                deltaTime = RGNDS::Timer::getDeltaTime() / 1000.0f;
                this->onUpdate( deltaTime );
            }

    // wait for capture unit to be ready
            while(REG_DISPCAPCNT & DCAP_ENABLE);

            if((frame&1) == 0) {
                lcdMainOnBottom();
                vramSetBankC(VRAM_C_LCD);
                vramSetBankD(VRAM_D_SUB_SPRITE);
                REG_DISPCAPCNT = DCAP_BANK(2) | DCAP_ENABLE | DCAP_SIZE(3);
                RGNDS::GL2D::glBegin2D();
                this->onDraw( deltaTime, 0 );
                RGNDS::GL2D::glEnd2D();
            }
            else {
                lcdMainOnTop();
                vramSetBankD(VRAM_D_LCD);
                vramSetBankC(VRAM_C_SUB_BG);
            	REG_DISPCAPCNT = DCAP_BANK(3) | DCAP_ENABLE | DCAP_SIZE(3);
            	RGNDS::GL2D::glBegin2D();
                this->onDraw( deltaTime, SCREEN_HEIGHT );
                RGNDS::GL2D::glEnd2D();
            }
            glFlush(0);
            swiWaitForVBlank();

        }

        if(this->errorCode != 0) {
            consoleDemoInit();
            printf("Error %d: %s", this->errorCode, this->lastError);
            while(true) {
                swiWaitForVBlank();
            }
        }

        this->onEnd();
    }

/*
    void Engine::drawText(int x, int y, const char* txt, u16 color) {
        byte* ptr = (byte*)txt;
        byte c = *ptr;

        glColor( color );

        int newX = x;
        int newY = y;
        while(c != 0) {

        // Handle special cases
            if((char)c == '\n') {
                newX = x;
                newY += 8;

                ptr++;
                c = *ptr;
                continue;
            }


        // convert lowercase to uppercase letters
            if(c > 96 && c < 173)
                c -= 32;

        // make sure only the supported characters are displayed
            if(c <32 || c > 95)
                c = 32;

        // Translate Char to TileIndex
            EngineGL2D::glSprite(newX, newY, EngineGL2D::GL_FLIP_NONE, &(font[c-32]));

            newX+=8;
            ptr++;
            c = *ptr;
        }

        glColor(0xFFFF);

    }
    */
}
