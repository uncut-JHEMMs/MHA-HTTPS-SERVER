#pragma once

#include<iostream>
#include<string>
#include<tinyxml.h>


#include "../models/Customer.hpp"
#include "../models/CreditCard.hpp"
#include "../models/Merchant.hpp"

class MyXmlClass
{
    private:
        TiXmlDocument doc;
        TiXmlDeclaration *decl;
        std::string file_name;
    public:

        MyXmlClass(std::string user_id)
        {
               file_name = user_id;
               decl = new TiXmlDeclaration("1.0" , "" ,"");
        }

        bool add_user(Customer &user)
        {
            TiXmlElement *person_element = new TiXmlElement("Person");
            person_element->SetAttribute("id" , "ssn");

            TiXmlElement *customer_element = new TiXmlElement("Cutomer");
            customer_element->SetAttribute("uid" , user.get_uid());
            person_element->LinkEndChild(customer_element);


            TiXmlElement *first_name_element = new TiXmlElement("First Name");
            first_name_element->LinkEndChild(new TiXmlText(user.get_first_name()));
            customer_element->LinkEndChild(first_name_element);

            TiXmlElement *last_name_element = new TiXmlElement("Last Name");
            TiXmlElement *email_element = new TiXmlElement("Email");

            last_name_element->LinkEndChild(new TiXmlText(user.get_last_name()));
            email_element->LinkEndChild(new TiXmlText(user.get_email()));
            customer_element->LinkEndChild(last_name_element);
            customer_element->LinkEndChild(email_element);  

            doc.LinkEndChild(decl);
            doc.LinkEndChild(person_element);
            file_name = "xmldatabase/Users/"+file_name+".xml";
            doc.SaveFile(file_name.c_str());
            return true;
        }

        bool add_card(CreditCard &crd)
        {
            TiXmlElement *card = new TiXmlElement("CreditCard");
            card->SetAttribute("holder_id" , crd.get_holder_id());

            TiXmlElement *card_number = new TiXmlElement("Number");
            card_number->LinkEndChild(new TiXmlText(crd.get_card_number()));
            card->LinkEndChild(card_number);

            TiXmlElement *card_type = new TiXmlElement("Type");
            card_type->LinkEndChild(new TiXmlText("default"));
            card->LinkEndChild(card_type);

            doc.LinkEndChild(decl);
            doc.LinkEndChild(card);
            file_name = "xmldatabase/CreditCards/"+file_name+".xml";
            doc.SaveFile(file_name);
            return true;
        }

        bool add_merchant(Merchant &crd)
        {
            TiXmlElement *card = new TiXmlElement("Merchant");
            card->SetAttribute("Merchant_id" , crd.get_merchant_id());

            TiXmlElement *merchant_name = new TiXmlElement("Merchant Name");
            merchant_name->LinkEndChild(new TiXmlText(crd.get_merchant_name()));
            card->LinkEndChild(merchant_name);

            TiXmlElement *merchant_type = new TiXmlElement("Type");
            merchant_type->LinkEndChild(new TiXmlText(crd.get_merchant_type()));
            card->LinkEndChild(merchant_type);

            doc.LinkEndChild(decl);
            doc.LinkEndChild(card);
            file_name = "xmldatabase/Merchants/"+file_name+".xml";
            doc.SaveFile(file_name);
            return true;
        }
};
