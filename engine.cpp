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
        videoSetMode(MODE_5_3D);        // Top-Screen will always output gl2d
        videoSetModeSub(MODE_5_2D);     // The bottom screen needs some trickery to output gl2d to

        vramSetBankA(VRAM_A_TEXTURE);       // VRAM Bank A and B will give you 256KB of texture space for gl2d sprites
        vramSetBankB(VRAM_B_TEXTURE);
        // VRAM Bank C and D will be used for outputing to the Screens
        vramSetBankF(VRAM_F_TEX_PALETTE);

    // For the Bottom Screen, The OAM / Sprite system is used to act as a buffer to display the render of gl2d
        oamInit(&oamSub, SpriteMapping_Bmp_2D_256, false);

    // arrange sprites, so they fill out the entire bottom screen
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

        RGNDS::GL2D::glScreen2D();  // Initialize GL2D Engine

        Timer::init();              // Setup timer for the Engine

        Engine_Debug_Init();        // Initalize the Debug output

        Engine::initialized = true;
    }

    Engine::Engine() {
        this->keepRunning = true;
    }
    Engine::~Engine(){
    }

    void Engine::error(const char* msg, int code) {
        this->keepRunning = false;
        if(this->errorCode != 0) {
            consoleDemoInit();
            printf("Error %d: %s", code, msg);
            while(true) {
                swiWaitForVBlank();
            }
        }
        this->lastError = msg;
        this->errorCode = code ? code : -1;
    }

    void Engine::exit() {
        this->keepRunning = false;
    }

    void Engine::run() {

        keepRunning = true;
        int tmp = onStart();
        if(tmp > 0)
            this->error("init failed: ", tmp);

        byte frame = 1;
        float deltaTime = 0.0f;
        while(this->keepRunning) {
            frame++;

            // update logic every second frAME
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
                this->onDraw( deltaTime, ENGINE_SCREEN_TOP );
                RGNDS::GL2D::glEnd2D();
            }
            else {
                lcdMainOnTop();
                vramSetBankD(VRAM_D_LCD);
                vramSetBankC(VRAM_C_SUB_BG);
            	REG_DISPCAPCNT = DCAP_BANK(3) | DCAP_ENABLE | DCAP_SIZE(3);
            	RGNDS::GL2D::glBegin2D(SCREEN_HEIGHT);
                this->onDraw( deltaTime, ENGINE_SCREEN_BOTTOM );
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
}
