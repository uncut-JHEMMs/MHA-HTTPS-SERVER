#include "../xmldatabase/xml_controller.h"
#include<httpserver.hpp>
#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>

//models
#include "../models/Customer.hpp"

using namespace httpserver;


class create_user_resource : public http_resource
{
    public:

        const std::shared_ptr<http_response> render_POST(const http_request& req)
        {
            std::cout << "reached here" << std::endl;
            Customer user;
            user.set_uid(req.get_arg("id"));
            user.set_first_name( req.get_arg("first__name"));
            user.set_last_name(req.get_arg("last_name"));
            user.set_email(req.get_arg("email"));

            MyXmlClass xmlobj(user.get_uid());
            xmlobj.add_user(user);
            return std::shared_ptr<http_response>(new string_response("Success fully added the user boo ah"));
        }
};
