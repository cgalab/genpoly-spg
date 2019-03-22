#include <sys/time.h>
#include <stdio.h>

#ifndef __TIMER_H_
#define __TIMER_H_

class Timer{
	private:
      struct timeval startTime, now;

   public:		
      void start(){
         gettimeofday(&startTime, NULL);
      }

      double elapsedTime(){
         long sec, usec, mtime;

         gettimeofday(&now, NULL);

         sec = now.tv_sec - startTime.tv_sec;
         usec = now.tv_usec - startTime.tv_usec;

         mtime = (sec * 1000 + usec/1000);

         return (double)mtime / 1000.0;
      }

      void printElapsedTime(){
         double elapsed;

         elapsed = elapsedTime();

         printf("%f", elapsed);
      }

};

#endif