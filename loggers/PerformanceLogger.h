#ifndef _PERFORMANCELOGGER_

#define _PERFORMANCELOGGER_


#include<mutex>
#include<condition_variable>
#include<chrono>


//#pragma once

class PerformanceLogger
{
    public:
        static std::mutex m;
        static std::condition_variable cv;
        static bool sig;

        PerformanceLogger() {sig = false;}
        static void calculateResponseTime();
};

#endif
