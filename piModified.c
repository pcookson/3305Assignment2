/* Program to compute Pi using Monte Carlo methods */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include<sys/time.h>
#include <sched.h>
#define SEED 35791246

main(int argc, char **argv)
{
    int niter;
    double x,y;
    int i,count=0; /* # of points in the 1st quadrant of unit circle */
    double z;
    double pi;
    int processes;
    int index;



    if (argc != 3)
    {
        perror("incorrect arguments");
        exit(0);
    }

    niter = atoi(argv[1]);
    processes = atoi(argv[2]);



    pid_t pids[processes];

    //if index is even, then SCHED_OTHER, else SCHED_FIFO
    for(index = 0; index < processes; index++)
    {
        if((pids[index] = fork()) < 0)
        {
            perror("fork error");
            exit(1);
        }
        else if(pids[index] == 0)
        {
            struct sched_param param;
            struct timeval timeBeforeStruct;
            struct timeval timeAfterStruct;
            long int total;



            if(index % 2 == 0)
            {
                param.sched_priority = sched_get_priority_max(SCHED_OTHER);
            }
            else
            {
                param.sched_priority = sched_get_priority_max(SCHED_FIFO);
            }
            gettimeofday(&timeBeforeStruct, NULL);

            /* initialize random numbers */
            srand(SEED);
            count=0;
            for ( i=0; i<niter; i++)
            {
                x = (double)rand()/RAND_MAX;
                y = (double)rand()/RAND_MAX;
                z = x*x+y*y;
                if (z<=1) count++;
            }
            pi=(double)count/niter*4;
            gettimeofday(&timeAfterStruct, NULL);

            total = ((timeAfterStruct.tv_sec - timeBeforeStruct.tv_sec) * 1000000L + timeAfterStruct.tv_usec) - timeBeforeStruct.tv_usec;

            printf("process # = %d, elapsed microseconds = %ld\n", index, total);


            exit(0);
        }
    }

    int status;
    pid_t pid;
    while(index > 0)
    {
        pid = wait(&status);
        --index;
    }

}
