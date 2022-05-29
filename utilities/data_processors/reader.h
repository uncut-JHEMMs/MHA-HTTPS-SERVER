#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

class FileReader
{
    private:
        long int offset_from;
        long int offset_to;
        std::ifstream file;
        std::string line;

    public:
        FileReader(long int from , long int to , std::string file_path) : offset_to(to) , offset_from(from)
        {
            file.open(file_path);
            file.seekg(offset_from);
        }

        bool next_line()
        {
            if (offset_from <= offset_to)
            {
                std::getline(file , line);
                offset_from = file.tellg();
                if (line == "")
                    return false;
                return true;

            }
            else
            {
                return false;
            }
        }

        std::string get_user_id()
        {
            std::string id;
            std::stringstream ss(line);
            std::getline(ss , id , ',');
            return id;
        }

        std::string get_card_number()
        {
            std::string card_number;
            std::stringstream ss(line);
            std::getline(ss , card_number , ',');
            std::getline(ss , card_number , ',');
            return card_number;
        }

        std::string get_year()
        {
            std::string year;
            std::stringstream ss(line);
            for (int i=0; i<3; i++)
            {
                std::getline(ss , year , ',');
            }
            return year;
        }

        std::string get_month()
        {
            std::string month;
            std::stringstream ss(line);
            for (int i=0; i<4; i++)
            {
                std::getline(ss , month , ',');
            }
            return month;
        }
        
        std::string get_date()
        {
            std::string date;
            std::stringstream ss(line);
            for (int i=0; i<5; i++)
            {
                std::getline(ss , date , ',');
            }
            return date;
        }

        std::string get_time()
        {
            std::string time;
            std::stringstream ss(line);
            for (int i=0; i< 6; i++)
            {
                std::getline(ss , time , ',');
            }
            return time;
        }

        std::string get_amount()
        {
            std::string amount;
            std::stringstream ss(line);
            for(int i=0; i<7; i++)
            {
                std::getline(ss , amount , ',');
            }
            return amount;
        }

        std::string get_transaction_type()
        {
            std::string transaction_type;
            std::stringstream ss(line);
            for (int i=0; i<8; i++)
            {
                std::getline(ss , transaction_type , ',');
            }
            return transaction_type;
        }

        std::string get_merchant_id()
        {
            std::string merchant_id;
            std::stringstream ss(line);
            for (int i=0; i<9; i++)
            {
                std::getline(ss , merchant_id , ',');
            }
            return merchant_id;
        }

        std::string get_merchant_city()
        {
            std::string merchant_city;
            std::stringstream ss(line);
            for (int i=0; i<10; i++)
            {
                std::getline(ss , merchant_city , ',');
            }
            return merchant_city;
        }

        std::string get_merchant_state()
        {
            std::string merchant_state;
            std::stringstream ss(line);
            for (int i=0; i<11; i++)
            {
                std::getline(ss , merchant_state , ',');
            }
            return merchant_state;
        }

        std::string get_zip()
        {
            std::string zip;
            std::stringstream ss(line);
            for (int i=0; i<12; i++)
            {
                std::getline(ss , zip , ',');
            }
            return zip;
        }

        std::string get_mcc()
        {
            std::string mcc;
            std::stringstream ss(line);
            for (int i=0; i<13; i++)
            {
                std::getline(ss , mcc , ',');
            }
            return mcc;
        }

        std::string get_errors()
        {
            std::string errors;
            std::stringstream ss(line);
            for (int i=0; i<14; i++)
            {
                std::getline(ss , errors , ',');
            }
            if (errors == "")
            {
                return "";
            }
            errors += ","; 
            std::string temp = "";
            std::getline(ss , temp , '"');
            return errors + temp + '"';
        }

        std::string get_fraud()
        {
            std::string fraud;
            std::stringstream ss(line);
            while(ss.good())
            {
                std::getline(ss , fraud , ',');
            }
            return fraud;
        }





};
