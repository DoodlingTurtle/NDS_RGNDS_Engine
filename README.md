# RGNDS::Engine
An engine/framework for the development of LibNDS homebrew, without having to actually deal with all the technicalitys of the Nintendo DS (Like VRam bank management, ellpased time handling, etc ...)

The engine is inspired by the OneLoneCoder/olcPixelGameEngine, as well as the "Scene" system, that can be found in engines like Unity or Godot

This framework will continue to expand (and hopefully improve)

## For now it provides the following functions.

- It sets up both screens, so that both can be drawn to via the 3D engine of the Nintendo DS (at 30 FPS)

    - both screens are handled as one large screen (256x384 px in size).
    - everything drawn on the Y-coordinate > 192 appears on the bottom screen

- Multiple RGNDS::Engine classes can be treated like, for example, scenes in other engines.
    - but there can only be one running RGNDS::Engine - Instance at any time.
    - See example Application below


- Provide access to the 3D drawing functions of the Nintendo DS (via RGNDS::GL2D)

    - The provided functions are an extension of the original "gl2d", that was included in devkitpro / libnds.
    - Drawing of sprites via the original glImage structure (via RGNDS::GL2D::glSprite)
  
    - Drawing strings directly to the screen (via RGNDS::GL2D::glText)
        - for now only in all CAPS.
    
    - Drawing single pixels to the screen (via RGNDS::GL2D::glPixel)

    - Drawing of 2D shapes, generated from a list of 2D coordinates, using one of the many 3D drawing modes provided by the DS (via RGNDS::GL2D::glShape)

    - The engine also includes a class to predefine polygon based 2D shapes, that can be reused. (via RGNDS::GL2D::PolyShape)

    - take a look into gl2d.h and gl2d_PolyShape.h for more details on the functions.
    
- It handles all the timing of logic and drawing updates.

- provides easy access for loading data from the NitroFS
    - Use void* RGNDS::Files::loadNitroFS(int filesize, const char* filename) to load binary files

---
# TODO
- Implement music and sfx handling.
- Implement a 60 FPS mode, that only uses 3D on one screen and sprites and backgrounds on the Other
- Allow access to the background and sprite systems of the Nintendo DS (Right now, these are used as framebuffer for the 3D Engine)
- Add support for lowercase text
- Implement filesystem access for gamesaves.

---
# Engine Macros
The Framework provides multiple small utility macros

```c
#define PI 3.1415926535897932384626433832793                                                                     

#define PI2 6.283185307179586476925286766558                                    

#define Engine_Log() // See next section for more details

#define Engine_RandF() //returns a random float value between 0.0f and 1.0f

#define Engine_Color16(alpha_1bit, red_5bit, green_5bit, blue_5bit) // assembles a 16bit abgr color definition, that can be used in RGNDS::GL2D
```

---
# Debug and stderr output
Modern emulators support the output of messages to the command shell, that they were started from.

with Engine_Log(message) the game can write to these outputs.
Each call creates one new line of text.

Example:
```c
Engine_Log("this is a message");

// you can also concatinate multiple values in one message
// different types of variables can be mixed
Engine_Log("this is a message with a " << concatincated << " part ");
```

For this to work, you need to add the following to your compiler flags.
```
-DTARGET_DEBUG
```

If this compiler flag is not set, all uses of Engine_Log will be skipped by the compiler.
This will improve the memory usage and Performance.

So you should use this flag only for debug builds, and remove it for release builds.


---
# Engine components
The framework also contains additional components.
These are mainly used in RGNDS::GL2D, but can also be applyed to other parts of your game, by them selfs.

### RGNDS::Point<?>
A structure, holding two values of what ever numeric type is definded as '?'
It can be used to store 2D Vect`ors and do math with them.

Example:
```c

RGNDS::Point<float> directionVector= {0.0f, -1.0f};
RGNDS::Point<float> position =  10.0f, 10.0f }

// Move character by 10 px in direction
position += directionVector * 10.0f;
```

### RGNDS::Transform
This Component represents a transformation in the 2D space.

It has the following properties:
- float scale  => defines how much a translated point is moved towards or away from 0,0.
- RGNDS::Point<float> pos   => Position, that the given point is translated to.
                               (The translation is applyed after rotation and scale)

- setAngle(float radiants)    => sets the angle to the given value
- setAngleRel(float radiants) => moves the angle by the given radiants
- directionFromPositionVector()  => takes the direction between 0,0 and pos as the new direction vector/angle
- moveInDirection(float distance) => moves position acording to currentAngle by the given distance.

RGNDS::Transform will also provide a _default instance.
It can be accessed via.
```c
RGNDS::Tranform::_default
```

This one is used, by various functions of RGNDS::GL2D as a default value. 
If you dont give them any transformation instance during the function call.

---
# Examples

### RGNDS::Engine (Main component)
```cpp

    class GameStateMainGame : public RGNDS::Engine {
        // Will exit, once the player wins or looses

    public: 
        bool wasGameWon() { return hasWon; } 

    protected:
        // this is an override
        int onStart() {

            hasWon = false;
            // Other stuff required for your game to start            

            ...

            return 0; // <<-- this function must return 0, otherwise the Console will be stopped 
                     //and the returned number is displayed as an error code 
        }

        // this is an override
        void onEnd() {
            // If you have used any "new" or alloc-functions in onStart, then you can use there respective "delete" or "free" calls here.

            ...
        }

        // this is an override
        void onDraw(RGNDS::Engine::Screen screen) {
            // This function is called 60 Times per second.

            // every odd call will be for one screen every other call for the other screen.

            // if you need to know what screen is currently drawn, you can use the give "screen" Parameter            
            // Draw everything you need in this function.

            // If you have trouble with performance, you can use the given screen parameter to determin, if something needs to be drawn at all.

            if(screen == ENGINE_SCREEN_BOTTOM) {
                // draw stuff, that only needs to be seen, if it is on the bottom screen
                ...
            }

            if(screen == ENGINE_SCREEN_TOP) {
                // draw stuff, that only needs to be seen, if it is on the top screen
                ...
            }

            // Everything else is draw on both screens 
            ...

        }

        // this is an override
        void onUpdate(float deltaTime) {
            // this function is called 30 Times per second.

            // All updates to your game objects and logic should be done here.
            // For example: moving sprites, processing inputs, etc.

            // here is also, where you can stop the instance from running (via exit()) 

            // all actions, that require a specific timing, can use the "deltaTime" multiplyer.
            // "deltaTime" * 1000.0f = the number of milliseconds, that have passed, since the last call of onUpdate.
            // 
            // application example: The player should move with a speed of 10 PX per second.
            //   this->playerPosX += 10 * deltaTime;


            if(/* "player wins" condition */) {
                hasWon = true;
                this->exit();
            }
            else if(/* "player lose" condition */) {
                hasWon = false;
                this->exit();
            }
            else {
                // Running game logic
                ...
            }
        }

    private:
        bool hasWon;
    }

    class GameStateGameOver : public RGNDS::Engine {
        // Will end itself, as soon as a key was pressed
        ...
    }

    class GameStateTitleScreen : public RGNDS::Engine {
        // Will end itself, as soon as a key was pressed
        ...
    }

    
    void main() {

        // The Engine has to be initialized, before any instances of it can be used
        RGNDS::Engine::init();

        auto mainGame = new GameStateMainGame();
        auto gameOver = new GameStateGameOver();
        auto titleScreen = new GameStateTitleScreen();

        // NDS Applications are not meant to end by them selfs, so, this entire thing must be encased in an infinit loop.
        while(1) {
           
            titleScreen.run();

            // The main game will restart until the player loses
            while(1) {
                mainGame.run();                
                if(!mainGame.wasGameWon())     
                    break;
            } 
ld
            // after the player loses, show the "Game over" screen
            gameOver.run();
        }
    }
```

### Hello World
```cpp

class HelloWorldExample : public RGNDS::Engine {
    
    protected:
        int onStart() { return 0; }

        void onDraw(RGNDS::Engine::Screen scr) {


            if(scr == ENGINE_SCREEN_TOP) {

                Transform textpos(5, 5);
            
                RGNDS::GL2D::glText("Hello World!!!", Engine_Color16(1, 31, 15, 0), &textpos);

                // Reduce Size of the text to 50%;
                textpos.scale = 0.5f;
                textpos.pos.y += 20;

                RGNDS::GL2D::glText("Hello small World!", Engine_color(1, 15, 31, 0), &textpos);

            }
        }
}

void main() {
    RGNDS::Engine::Init();

    HelloWorldExample app;
    app.run();
}

```
