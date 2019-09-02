/*
   Include standard libraries
*/
#include <sys/time.h>
#include <stdio.h>

#ifndef __TIMER_H_
#define __TIMER_H_

/*
   A class for measuring and printing execution time information
*/
class Timer{
	private:
      
      /*
         The start time of the execution
      */
      struct timeval startTime;

      /*
         The actual time at a measurement point
      */
      struct timeval now;

   public:		
      
      /*
         Starts a measurement by setting the startTime to the actual time
      */
      void start(){
         gettimeofday(&startTime, NULL);
      }

      /*
         @return  The elapsed time since calling start() in seconds
      */
      double elapsedTime(){
         long sec, usec, mtime;

         gettimeofday(&now, NULL);

         sec = now.tv_sec - startTime.tv_sec;
         usec = now.tv_usec - startTime.tv_usec;

         mtime = (sec * 1000 + usec/1000);

         return (double)mtime / 1000.0;
      }

      /*
         Prints the elapsed time since calling start() in seconds
      */
      void printElapsedTime(){
         double elapsed;

         elapsed = elapsedTime();

         printf("%f", elapsed);
      }

};

#endif