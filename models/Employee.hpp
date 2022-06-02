#include<string>

#include "./Person.hpp"

class Employee: public Person
{
    private:
        std::string id;
        std::string type;
    public:

        Employee(){}

        //setters start here*****************
        void set_id(std::string eid)
        {
            this->id = eid;
        }
        
        void set_type(std::string etype)
        {
            this->type = etype;
        }
        //setter ends here ****************
        

        //getter start here**************
        std::string get_id()
        {
            return this->id;
        }
        std::string get_type()
        {
            return this->type;
        }
        //getters ends here****************

        
};
