#include <httpserver.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iomanip>

//resources
#include "./resources/digest_resource.h"

using namespace httpserver;


//creating global binary semaphore
std::mutex m;
std::condition_variable cv;
bool sig = false;

void performanceLogger()
{
    while(true){
    
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk , []{return sig;});
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


// printing logs to terminal
void custom_access_log(const std::string& url) {
    std::cout << "ACCESSING: " << url << std::endl;
    
        {
            std::lock_guard<std::mutex> lk(m);
            sig = true;
        }
        cv.notify_one();
}

// if resource not found
const std::shared_ptr<http_response> not_found_custom(const http_request& req) 
{
      return std::shared_ptr<string_response>(new string_response("Path Not Found", 404, "text/plain"));
}

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
            std::lock_guard<std::mutex> lk(m);
            sig = true;
        }
        cv.notify_one();
        return std::shared_ptr<http_response>(new string_response("Hello, World!" + data));

    }
};


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
    

    std::thread t(performanceLogger);
    
    hello_world_resource hwr;
    ws.register_resource("/hello", &hwr);
    
    digest_resource dr;
    ws.register_resource("/auth" , &dr);


    ws.start(true);
    
    t.join();
    return 0;
}
