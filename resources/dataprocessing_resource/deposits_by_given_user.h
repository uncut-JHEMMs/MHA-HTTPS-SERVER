#include<httpserver.hpp>
#include"../../utilities/data_processors/data_processor1.h"
#include<string>

using namespace httpserver;


class deposits_by_given_user : public httpserver::http_resource
{
    public:
        const std::shared_ptr<http_response> render_GET(const http_request &req)
        {
            data_processor dp;
            dp.list_all_payments_made_by_user(req.get_arg("data"));

            return std::shared_ptr<file_response>(new file_response("xmldatabase/temp_files/temp.xml" , 200 , "text/plain"));
        }
};