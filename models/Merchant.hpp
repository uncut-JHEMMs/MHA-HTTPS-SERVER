#pragma once
#include<string>


class Merchant
{
    private:
        std::string mid;
        std::string merchant_name;
        std::string merchant_type;

    public:
        Merchant(){}

        //setters
        void set_merchant_id(std::string id)
        {
            this->mid = id;
        }

        void set_merchant_name(std::string name)
        {
            this->merchant_name = name;
        }

        void set_merchant_type(std::string mt)
        {
            this->merchant_type = mt;
        }
        
        //getters
        std::string get_merchant_id()
        {
            return this->mid;
        }

        std::string get_merchant_name()
        {
            return this->merchant_name;
        }

        std::string get_merchant_type()
        {
            return this->merchant_type;
        }
};
