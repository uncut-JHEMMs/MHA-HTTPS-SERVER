#include<httpserver.hpp>
#include <mutex>
//#ifndef _PerformanceLogger_
#include "../loggers/PerformanceLogger.h"
//#endif


using namespace httpserver;



// /hello path
class hello_world_resource : public http_resource 
{
public:
    const std::shared_ptr<http_response> render(const http_request& req) 
    {
        //std::cout << "request recieved" << std::endl;
        std::string data = req.get_arg("data");
        std::cout << "request recieved with data : " << data <<  std::endl;
        {
            std::lock_guard<std::mutex> lk(PerformanceLogger::m);
            PerformanceLogger::sig = true;
        }
        PerformanceLogger::cv.notify_one();
        return std::shared_ptr<http_response>(new string_response("Hello, World!" + data));

    }
};
