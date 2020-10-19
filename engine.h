#ifndef __RGNDS_ENGINE_H__
#define __RGNDS_ENGINE_H__ 1

#include <nds.h>
#include <nds/arm9/videoGL.h>

#ifndef PI
#define PI 3.141592653589793238462643383279
#endif // PI

#ifndef PI2
#define PI2 6.283185307179586476925286766558
#endif // PI2

#ifdef TARGET_DEBUG
#include <iostream>
#define Engine_Debug_Init() consoleDebugInit(DebugDevice_NOCASH)
#define Engine_Log(x)       std::cerr << x << std::endl
#else
#define Engine_Debug_Init()
#define Engine_Log(x) /**/
#endif // NDEBUG

#define Engine_RandF() ((float)rand() / (float)RAND_MAX)
#define Engine_Color16(a, r, g, b) ((r&31)|((g&31)<<5)|((b&31)<<10)|((a&1)<<15))

#include "inc/timer.h"
#include "inc/point.h"

namespace RGNDS {

    class Engine {
        public:



            static void init();

            /** Default constructor */
            Engine();
            /** Default destructor */
            virtual ~Engine();

            void run();

            void error(const char* error, int iCode);

            void exit();

            static void drawLine(Point<int> startPoint, Point<int> endPoint, unsigned short color);
            static void drawText(int x, int y, const char* txt, u16 color=0xFFFF);
            static void drawTriangle(const Point<int> p1, const Point<int> p2, const Point<int>p3, unsigned short c, bool filled);
            static void drawCircle(const Point<int>& pos, float radius, byte resolution, unsigned short color);

        protected:

            /* \brief should only be called once
             *        code like initial setup of variables should be done here
             * \return - any value > 0 will not start the engine and is treated as an error-code
             */
            virtual int onStart(){ return 2; }

            /* \brief only called once, when the Application should be closed
             *        use this function to free or delete all allocated Memory or created objects
             */
            virtual void onEnd(){}

            virtual void onUpdate( float deltaTime ){};

            virtual void onDraw(float deltaTime, int screenYOffset){};



        private:
            bool keepRunning;           // set to false, if the Main-Loop should stop to repeat itself

            const char* lastError;
            int   errorCode;

            static bool initialized;    //
            static int FontTilesTextureID;

    };
}

#endif // __RGNDS_ENGINE_H__

