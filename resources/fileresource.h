#include<httpserver.hpp>


using namespace httpserver;


class file_resource : public httpserver::http_resource
{
    public:
        const std::shared_ptr<http_response> render_GET(const http_request &req)
        {   
            return std::shared_ptr<file_response>(new file_response("./resources/credit_card/transactions/card_transaction.v1.csv" , 200 , "text/plain"));
        }
};
