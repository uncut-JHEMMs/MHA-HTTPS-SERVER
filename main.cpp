#include <httpserver.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iomanip>



//logger
#include "./loggers/PerformanceLogger.h"
//resources
#include "./resources/digest_resource.h"
#include "./resources/hello_world_resource.h"

using namespace httpserver;



// printing logs to terminal
void custom_access_log(const std::string& url) {
    std::cout << "ACCESSING: " << url << std::endl;
    
        {
            std::lock_guard<std::mutex> lk(PerformanceLogger::m);
            PerformanceLogger::sig = true;
        }
        PerformanceLogger::cv.notify_one();
}

// if resource not found
const std::shared_ptr<http_response> not_found_custom(const http_request& req) 
{
      return std::shared_ptr<string_response>(new string_response("Path Not Found", 404, "text/plain"));
}


int main(int argc, char** argv) 
{
    
    webserver ws = create_webserver(8443)
        .start_method(http::http_utils::INTERNAL_SELECT).max_threads(8)
        .max_connections(10)
        .connection_timeout(60)
        .per_IP_connection_limit(2)
        .use_dual_stack()
        .use_ssl()
        .https_mem_trust("./certs/newcert.crt")
        .https_mem_key("./certs/newkey.pem")
        .https_mem_cert("./certs/newcert.crt")
        .log_access(custom_access_log)
        .debug()
        .not_found_resource(not_found_custom);
    

    std::thread t(PerformanceLogger::calculateResponseTime);
    
    hello_world_resource hwr;
    ws.register_resource("/hello", &hwr);
    
    digest_resource dr;
    ws.register_resource("/auth" , &dr);


    ws.start(true);
    
    t.join();
    return 0;
}
