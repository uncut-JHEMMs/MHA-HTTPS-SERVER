#pragma once

#include<iostream>
#include<string>
#include<tinyxml.h>
#include<vector>

//signing class
#include "../utilities/signer.hpp"

class TempXml
{
    public:
        TempXml()
        {}

        void create_xml(std::vector<std::pair<std::string , std::vector<std::string>>> vect , std::string outer , std::vector<std::string> inner)
        {
            Signer sig("../../ca.key" , "../../ca-pub.pem");
            std::string signing_buffer;
            for (auto &i : vect )
            {
                signing_buffer += i.first;
                for (auto &j : i.second)
                {
                    signing_buffer += j;
                }
            }
            TiXmlElement *signature_element = new TiXmlElement("Signature");
            signature_element->SetAttribute("key_info" , "RSA");
            TiXmlElement *key_element = new TiXmlElement("Certificate");
            key_element->LinkEndChild(new TiXmlText(sig.get_certificate()));

            TiXmlElement *digest_element = new TiXmlElement("Digest Value");
            digest_element->LinkEndChild(new TiXmlText(sig.signMessage(signing_buffer)));
            signature_element->LinkEndChild(key_element);
            signature_element->LinkEndChild(digest_element);


            TiXmlDocument doc;
            TiXmlDeclaration *decl;
            decl = new TiXmlDeclaration("1.0" , "" ,"");
            TiXmlElement *data = new TiXmlElement("data");
            
            for (auto &i : vect)
            {
                TiXmlElement *outer_element = new TiXmlElement(outer.c_str());
                outer_element->SetAttribute("field" , i.first.c_str());
                data->LinkEndChild(outer_element);

                for(unsigned j =0; j < inner.size(); j++)
                {
                    TiXmlElement *inner_element = new TiXmlElement(inner[j].c_str());
                    inner_element->LinkEndChild(new TiXmlText(i.second[j].c_str()));
                    outer_element->LinkEndChild(inner_element);
                }
            }
            doc.LinkEndChild(decl);
            doc.LinkEndChild(signature_element);
            doc.LinkEndChild(data);
            doc.SaveFile("./xmldatabase/temp_files/temp.xml");

        }
};