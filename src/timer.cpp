#ifndef __RGNDS_TIMER_CPP__
#define __RGNDS_TIMER_CPP__ 1

namespace RGNDS {
    namespace Timer {

        int elapsedTime = 0;
        int lastRead = 0;
        byte running = 1;

        void init() {
            cpuStartTiming(0);
        }

        void nextCycle() {
            int curMilliseconds = cpuGetTiming() / (BUS_CLOCK/1000);

            elapsedTime = (curMilliseconds - lastRead)
                * running
                * (lastRead > 0);

            lastRead = curMilliseconds;
        }

        int getDeltaTime() {
            return elapsedTime;
        }

        void pause() {
            running = 0;
        }
        void unpause() {
            running = 1;
        }
    }
}
#endif // __RGNDS_TIMER_CPP__
