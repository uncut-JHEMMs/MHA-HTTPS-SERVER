#include<httpserver.hpp>
#include"../../utilities/data_processors/data_processor1.h"
#include<string>

using namespace httpserver;


class t10_largest_transactions_resource : public httpserver::http_resource
{
    public:
        const std::shared_ptr<http_response> render_GET(const http_request& req)
        {
            data_processor dp;
            dp.t10_largest_transactions();
            return std::shared_ptr<file_response>(new file_response("xmldatabase/temp_files/temp.xml" , 200 , "text/plain"));
        }
};