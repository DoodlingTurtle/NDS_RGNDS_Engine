#ifndef __RGNDS_ENGINE_H__
#define __RGNDS_ENGINE_H__ 1

#include <vector>
#include <nds.h>

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

            enum Screen {
                ENGINE_SCREEN_TOP,
                ENGINE_SCREEN_BOTTOM
            };

            static void init();

            /** Default constructor */
            Engine();
            /** Default destructor */
            virtual ~Engine();

            /** \brief this will start the Engine until "exit" is called
             *         from within onUpdate or onDraw
             *
             *         you can call the "run" function of another engine from
             *         within the onUpdate function of another engine
             *
             *         but there can only be one active engine at any time
             */
            void run();


            void exit();

            /** \brief This function is called 60 times per second.
             *         The rendering of the top and bottom screen are switched on every call.
             *         Make sure all draw calles and logic in this function are finished withing
             *         1/60 of a second, otherwise the screens will start to flicker
             *         //TODO: (DoTu) Fix that
             * 
             *          both screens share one coordinate system. 
             *          Everything above y = 191 will be drawn to the bottom screen
             *
             * \param screen - defines, what screen is currently drawing
             */
            virtual void onDraw(Screen screen){};

            /** \brief Can be called to stop what ever the program is doing, and
             *         and print a message to the screen 
             *
             *  \param error  - the message string to display
             *  \param iCode  - an error-code, that can make reading the error easyer
             */
            static void error(const char* error, int iCode);

        protected:

            /* \brief should only be called once
             *        code, like the initial setup of variables, should be done here.
             * \return - any value > 0 will not start the engine and is treated as an error-code
             */
            virtual int onStart(){ return 2; }

            /* \brief only called once, when the Application should be closed (after exit has been called)
             *        use this function to free or delete all allocated Memory or created objects
             */
            virtual void onEnd(){}

            /* \brief called 30 times per second 
             *
             * \param deltaTime - multiplyer for the time that passed since the last call of onUpdate
             *                    (example: 1 = 1 Second, 0.5 = halfe a second, etc. ...)
             */
            virtual void onUpdate( float deltaTime ){};


        private:
            bool renderdone;

            static Engine* _currentInstance;
            static std::vector<Engine*> _instances;

            static void onVBlank();

            bool keepRunning;           // set to false, if the Main-Loop should stop to repeat itself

            static bool initialized;    // tells if the Engine has been initialized
										// TODO: (DoTu) Implement
            static int FontTilesTextureID;

    };
}

#endif // __RGNDS_ENGINE_H__

