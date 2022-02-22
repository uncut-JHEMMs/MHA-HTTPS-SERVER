#include<httpserver.hpp>

#define MY_OPAQUE "11733b200778ce33060f31c9af70a870ba96ddd4"


using namespace httpserver;


class digest_resource : public httpserver::http_resource
{
    public:
        const std::shared_ptr<http_response> render_GET(const http_request &req)
        {
            if(req.get_digested_user() != "afridi")
            {
                return std::shared_ptr<digest_auth_fail_response>(new digest_auth_fail_response("FAIL" , "test@example.com" , MY_OPAQUE , true));
            }
           /* else
            {
                bool reload_nonce = false;
                bool *p = reload_nonce;
                if(!req.check_digest_auth("test@example.com", "mypass" , 300 , p))
                {
                    return std::shared_ptr<digest_auth_fail_response>(new digest_auth_fail_response("FAIL" , "test@example.com" , MY_OPAQUE , reload_nonce));
                }
            }*/
            return std::shared_ptr<string_response>(new string_response("success" , 200 , "text/plain"));
        }
};
