#pragma once
#include<string>

#include "./Person.hpp"

class Customer: public Person
{
    private:
        std::string uid;
    public:

        Customer(){}

        //setters start here*****************
        void set_uid(std::string cid)
        {
            this->uid = cid;
        }
        
        //setter ends here ****************
        

        //getter start here**************
        std::string get_uid()
        {
            return this->uid;
        }
        //getters ends here**************** 
};
