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
#include "src/broadcast.cpp"

namespace RGNDS {

    int     Engine::FontTilesTextureID;
    bool    Engine::initialized = false;

    Engine* Engine::_currentInstance = nullptr;
    std::vector<Engine*> Engine::_instances = std::vector<Engine*>();

    void Engine::onVBlank() {
        if(_currentInstance == nullptr || _currentInstance->renderdone) return;

        while(REG_DISPCAPCNT & DCAP_ENABLE);

        lcdMainOnBottom();
        vramSetBankC(VRAM_C_LCD);
        vramSetBankD(VRAM_D_SUB_SPRITE);
        REG_DISPCAPCNT = DCAP_BANK(2) | DCAP_ENABLE | DCAP_SIZE(3);
        RGNDS::GL2D::glBegin2D();
        _currentInstance->onDraw(ENGINE_SCREEN_TOP );
        RGNDS::GL2D::glEnd2D();
        glFlush(0);

        while(REG_DISPCAPCNT & DCAP_ENABLE);

        lcdMainOnTop();
        vramSetBankD(VRAM_D_LCD);
        vramSetBankC(VRAM_C_SUB_BG);
        REG_DISPCAPCNT = DCAP_BANK(3) | DCAP_ENABLE | DCAP_SIZE(3);
        RGNDS::GL2D::glBegin2D(SCREEN_HEIGHT);
        _currentInstance->onDraw(ENGINE_SCREEN_BOTTOM );
        RGNDS::GL2D::glEnd2D();
        glFlush(0);

        _currentInstance->renderdone = true;
    }


    void Engine::init() {
        if(initialized) return;

//        irqInit();   // <--- bad on new versions of devkitpro/libnds
        irqSet(IRQ_VBLANK, onVBlank);
        irqEnable(IRQ_VBLANK);

    // Setup VRAM for displaying textures
        videoSetMode(MODE_5_3D);        // Top-Screen will always output gl2d
        videoSetModeSub(MODE_5_2D);     // The bottom screen needs some trickery to output gl2d to

        vramSetBankA(VRAM_A_TEXTURE);   // VRAM Bank A and B will give you 256KB of texture space for gl2d sprites
        vramSetBankB(VRAM_B_TEXTURE);   // - 4K for the default font 
        // VRAM Bank C and D will be used for outputing to the Screens
        vramSetBankF(VRAM_F_TEX_PALETTE); // will hold the paletts for the sprites

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
        this->renderdone = false;
    }
    Engine::~Engine(){
    }

    void Engine::error(const char* msg, int code) {
		consoleDemoInit();
		printf("Error %d: %s", code, msg);
		while(true) {
			swiWaitForVBlank();
		}
    }

    void Engine::exit() {
        this->keepRunning = false;
    }

    void Engine::run() {
        
        keepRunning = true;
        int tmp = onStart();
        if(tmp > 0)
            this->error("init failed: ", tmp);

        if(_currentInstance != nullptr)
            _instances.push_back(_currentInstance);

        _currentInstance = this;

        float deltaTime = 0.0f;
        while(keepRunning) {

            RGNDS::Timer::nextCycle();
            deltaTime = RGNDS::Timer::getDeltaTime() / 1000.0f;
            this->onUpdate( deltaTime );

            if(!keepRunning) break;

            renderdone = false;
            swiWaitForVBlank();

        }

        _currentInstance = nullptr;
        if(_instances.size() > 0) {
            _currentInstance = _instances.at(_instances.size()-1);
            _instances.pop_back();
        }

        this->onEnd();
    }
}
