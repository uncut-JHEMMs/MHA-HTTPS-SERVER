#pragma once
#include<iostream>
#include<string>

class Person
{
    private:
        int id;
        std::string first_name;
        std::string last_name;
        std::string email;
    public:

        Person(){}


        //setters start here*************************
        void set_first_name(std::string fname)
        {
            this->first_name = fname;
        }
        void set_last_name(std::string lname)
        {
            this->last_name = lname;
        }
        void set_email(std::string email)
        {
            this->email = email;
        }
        void set_id(int id)
        {
            this->id = id;
        }
        
        //setter ends here ***************************
        

        //getters start here
        std::string get_first_name()
        {
            return this->first_name;
        }
        std::string get_last_name()
        {
            return this->last_name;
        }
        std::string get_email()
        {
            return this->email;
        }
        int get_id()
        {
            return this->id;
        }
        
        //getters ends here ***************************
};
