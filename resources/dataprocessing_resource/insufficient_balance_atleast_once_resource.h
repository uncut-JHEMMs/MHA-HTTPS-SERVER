
#include<httpserver.hpp>
#include"../../utilities/data_processors/data_processor1.h"
#include<string>

using namespace httpserver;


class insufficient_balance_atleast_once_resource : public httpserver::http_resource
{
    public:
        const std::shared_ptr<http_response> render_GET(const http_request &req)
        {
            data_processor dp;
            std::string val = std::to_string(dp.get_users_percentage_with_insufficient_more_then_once(0));

            return std::shared_ptr<string_response>(new string_response(val , 200 , "text/plain"));
        }
};