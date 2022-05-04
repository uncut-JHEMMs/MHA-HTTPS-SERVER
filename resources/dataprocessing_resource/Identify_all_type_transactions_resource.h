#include<httpserver.hpp>
#include"../../utilities/data_processors/data_processor1.h"
#include<string>

using namespace httpserver;


class identify_all_types_of_transactions_resource : public httpserver::http_resource
{
    public:
        const std::shared_ptr<http_response> render_GET(const http_request &req)
        {
            data_processor dp;
            std::string vals = dp.indentify_all_types_of_transactions();
            
            return std::shared_ptr<string_response>(new string_response(vals , 200 , "text/plain"));
        }
};