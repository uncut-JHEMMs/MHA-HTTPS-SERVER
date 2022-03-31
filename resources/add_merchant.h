#pragma once
#include "../xmldatabase/xml_controller.h"
#include<httpserver.hpp>
#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>

//models
#include "../models/Merchant.hpp"

using namespace httpserver;


class create_merchants_resource : public http_resource
{
    public:

        const std::shared_ptr<http_response> render_POST(const http_request& req)
        {
            std::cout << "reached here" << std::endl;
            Merchant mrt;
            mrt.set_merchant_id(req.get_arg("id"));
            mrt.set_merchant_name( req.get_arg("merchant_name"));
            mrt.set_merchant_type(req.get_arg("type"));
            MyXmlClass xmlobj(mrt.get_merchant_id());
            xmlobj.add_merchant(mrt);
            return std::shared_ptr<http_response>(new string_response("Success fully added the credit card boo ah"));
        }
};
