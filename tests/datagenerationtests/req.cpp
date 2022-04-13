#include <cpr/cpr.h>
#include <iostream>
#include <cpr/ssl_options.h>
#include <curl/curl.h>
#include<string>
int main(int argc, char** argv) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://localhost:8443/hello");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    std::cout << readBuffer << std::endl;
  }
    return 0;                         
}
