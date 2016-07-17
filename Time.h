//
// Created by neo on 5/7/16.
//

#ifndef TESTDATABASE_TIME_H
#define TESTDATABASE_TIME_H
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

// thanks to  http://stackoverflow.com/questions/459691/best-timing-method-in-c
// HDevejian
int timeval_subtract(struct timeval *result,
                     struct timeval end,
                     struct timeval start)
{
    if (start.tv_usec < end.tv_usec) {
        int nsec = (end.tv_usec - start.tv_usec) / 1000000 + 1;
        end.tv_usec -= 1000000 * nsec;
        end.tv_sec += nsec;
    }
    if (start.tv_usec - end.tv_usec > 1000000) {
        int nsec = (end.tv_usec - start.tv_usec) / 1000000;
        end.tv_usec += 1000000 * nsec;
        end.tv_sec -= nsec;
    }

    result->tv_sec = end.tv_sec - start.tv_sec;
    result->tv_usec = end.tv_usec - start.tv_usec;

    return end.tv_sec < start.tv_sec;
}

void set_exec_time(int end)
{
    static struct timeval time_start;
    struct timeval time_end;
    struct timeval time_diff;

    if (end) {
        gettimeofday(&time_end, NULL);
        if (timeval_subtract(&time_diff, time_end, time_start) == 0) {
            if (end == 1)
                printf("\nexec time: %1.20fs\n",
                       time_diff.tv_sec + (time_diff.tv_usec / 1000000.0f));
            else if (end == 2)
                printf("%1.20fs",
                       time_diff.tv_sec + (time_diff.tv_usec / 1000000.0f));
        }
        return;
    }
    gettimeofday(&time_start, NULL);
}

void start_exec_timer()
{
    set_exec_time(0);
}

void print_exec_timer()
{
    set_exec_time(1);
}
#endif //TESTDATABASE_TIME_H
