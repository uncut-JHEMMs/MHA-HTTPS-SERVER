#pragma once
#include "../xmldatabase/xml_controller.h"
#include<httpserver.hpp>
#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>

//models
#include "../models/CreditCard.hpp"

using namespace httpserver;


class create_credit_card_resource : public http_resource
{
    public:

        const std::shared_ptr<http_response> render_POST(const http_request& req)
        {
            std::cout << "reached here" << std::endl;
           
            CreditCard crd;
            crd.set_holder_id(req.get_arg("id"));
            crd.set_card_number( req.get_arg("card_number"));
            crd.set_card_type(req.get_arg("card_type"));

            MyXmlClass xmlobj(crd.get_holder_id());
            xmlobj.add_card(crd);
            return std::shared_ptr<file_response>(new file_response("../xmldatabase/CreditCards/"+crd.get_holder_id() , 200 , "text/plain"));
        }
};
