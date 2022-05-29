#include<httpserver.hpp>
#include"../../utilities/data_processors/data_processor1.h"
#include<string>

using namespace httpserver;


class t5_merchants_with_insufficient_resource : public httpserver::http_resource
{
    public:
        const std::shared_ptr<http_response> render_GET(const http_request& req)
        {
            data_processor dp;
            dp.t5_merchants_with_insuff_only();
            return std::shared_ptr<file_response>(new file_response("xmldatabase/temp_files/temp.xml" , 200 , "text/plain"));
        }
};