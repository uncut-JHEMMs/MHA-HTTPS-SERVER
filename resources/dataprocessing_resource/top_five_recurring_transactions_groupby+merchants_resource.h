#include<httpserver.hpp>
#include"../../utilities/data_processors/data_processor1.h"
#include<string>

using namespace httpserver;


class top_five_recurring_transactions_groupby_merchants : public httpserver::http_resource
{
    public:
        const std::shared_ptr<http_response> render_GET(const http_request &req)
        {
            data_processor dp;
            std::string val = dp.top_five_recurring_transactions_merchant();

            return std::shared_ptr<string_response>(new string_response(val , 200 , "text/plain"));
        }
};