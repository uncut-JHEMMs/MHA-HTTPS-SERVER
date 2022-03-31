#pragma once

#include<string>

class CreditCard
{
    private:
        std::string holder_id;
        std::string card_number;
        std::string card_type;
    public:
        CreditCard() {}
        
        //setters
        void set_holder_id(std::string hid)
        {
            this->holder_id = hid;
        }
        void set_card_number(std::string cn)
        {
            this->card_number = cn;
        }
        void set_card_type(std::string ct)
        {
            this->card_type = ct;
        }
        //getters
        std::string get_holder_id()
        {
            return this->holder_id;
        }
        std::string get_card_number()
        {
            return this->card_number;
        }
        std::string get_card_type()
        {
            return this->card_type;
        }
};
