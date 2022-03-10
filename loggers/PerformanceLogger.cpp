#include "PerformanceLogger.h"
#include <iostream>
#include <iomanip>

bool PerformanceLogger::sig = false;
std::condition_variable PerformanceLogger::cv = std::condition_variable();
std::mutex PerformanceLogger::m;

void PerformanceLogger::calculateResponseTime()
{
    while(true)
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk , [] {return sig;});
        auto start = std::chrono::high_resolution_clock::now();
        sig = false;
        lk.unlock();

        lk.lock();
        cv.wait(lk , [] {return sig;});
        auto end = std::chrono::high_resolution_clock::now();
        auto time_taken  = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        time_taken *= 1e-9;

        std::cout << "Time taken to process request is : " << time_taken << std::setprecision(9)<<std::endl;
        sig = false;
        lk.unlock();
    }
} 

