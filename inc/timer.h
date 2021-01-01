#ifndef __RGNDS_TIMER_H__
#define __RGNDS_TIMER_H__ 1

namespace RGNDS {

    namespace Timer {
        /* \brief Will define hardware Timers 0 and 1 to count CPU-Cycles
         */
        void init();

        /* \brief Updateds the instance based on CPU-Cycles
         *        Call once per Main-Loop-Cycle
         */
        void nextCycle();

        /* \brief gets the milliseconds, that passed, since the
         *        last Timer::nextCycle Call
         *
         * \return the ellapsed milliseconds
         */
        int getDeltaTime();

        /* \brief Pauses the timer
         *        If the Timer is paused, then Timer::getDeltaTime == 0
         */
        void pause();

        /* \brief Resumes a paused timer after the next Timer::nextCycle() call
         */
        void unpause();

        extern int elapsedTime;
        extern int lastRead;
        extern byte running;
    }

}

#endif // __RGNDS_TIMER_H__
