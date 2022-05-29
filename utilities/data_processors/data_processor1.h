#pragma once
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<map>
#include<thread>
#include<vector>
#include<mutex>
#include<set>
#include "../../xmldatabase/xml_controller_temp.h"
#include "reader.h"


struct largest_transactions
{
    std::string uid;
    std::string merchnat_id;
    double amount;

    void operator =(largest_transactions other)
    {
        this->uid = other.uid;
        this->merchnat_id = other.merchnat_id;
        this->amount = other.amount;
    }
};
class data_processor
{
    private:
        std::mutex mtx;
        std::map<std::string , int> mp;
        std::set<std::string> st;
        std::map<std::string , std::map<std::string , int>> mp2;
        std::vector<std::string> vec;
        std::map<std::string , int> gpset;
        std::map<std::string , double> mpamount;
    public:

        data_processor()
        {}
        void get_index_info(std::vector<long int> &indices)
        {
            std::ifstream index_file;
            index_file.open("./utilities/data_processors/index_info.txt");
            indices.push_back(0);
            while (index_file.good())
            {
                long int temp;
                index_file >> temp;
                // std::cout << "pushing temp" << temp << std::endl;
                indices.push_back(temp);
            }
        }

        bool insufficient_balance_occured(std::string errors , std::string id)
        {
            if (errors == "")
            {
                return false;
            }
            errors = errors.erase(errors.size() - 1);
            errors = errors.erase(0 , 1);
            
            std::stringstream errors_stream(errors);
            while(errors_stream.good())
            {
                std::string error;
                std::getline(errors_stream , error , ',');
                
                if (error == "Insufficient Balance")
                {
                    return true;
                }
            }
            return false;
        }
        void set_map_for_users_with_insufficient(long int from , long int to)
        {
            std::string transaction , id , errors;
            std::ifstream flat_file;
            flat_file.open("../card_transaction.v1.csv");
            if(flat_file.is_open())
            {
                std::cout << "flat file is open" << std::endl;
            }
            flat_file.seekg(from, flat_file.beg);
            if(from ==0)
            {
                std::getline(flat_file, transaction);
            }
            while(flat_file.tellg() != to)
            {

                std::getline(flat_file, transaction);
                std::stringstream ss(transaction);
                if(transaction == "")
                {
                    return;
                }    
                //got the id from transaction line
                std::getline(ss , id , ',');
                
                for (int i =0; i<13;i++)
                {
                    std::getline(ss , errors , ',');
                }
                if (errors[0] == '"')
                {
                    std::string temp;
                    std::getline(ss , temp , '"');
                    errors += ',';
                    errors += temp;
                    errors += '"';
                }
                
                //checking if insufficient balance occured
                
                if (insufficient_balance_occured(errors , id))
                {
                    // std::cout << "check passed" << std::endl;
                    std::map<std::string,int>::iterator it; // will store pointer to id if present in map
                    it  = mp.find(id);
                    if (it != mp.end())
                    {
                        std::unique_lock<std::mutex> lk(mtx);
                        it->second += 1;
                        lk.unlock();
                    }
                    else
                    {
                        std::unique_lock<std::mutex> lk(mtx);
                        mp.insert({id , 1});
                        lk.unlock();
                    }
                }
            }
            return ;
        }


        //fruad detection by year

        void set_map_for_fraud_year(long int from , long int to)
        {
            std::string transaction , year , fraud;
            std::ifstream flat_file;
            flat_file.open("card_transaction.v1.csv");
            flat_file.seekg(from, flat_file.beg);
            if(from ==0)
            {
                std::getline(flat_file, transaction);
            }
            while(flat_file.tellg() != to)
            {

                std::getline(flat_file, transaction);
                std::stringstream ss(transaction);
                if(transaction == "")
                {
                    return;
                }    
                //got the because 3rd index is year from transaction line
                std::getline(ss , year , ',');
                std::getline(ss , year , ',');
                std::getline(ss , year , ',');

                std::string temp = "";
                while(true){
                    std::getline(ss , fraud , ',');
                    if (fraud == "No" || fraud == "Yes")
                    {
                        break;
                    }
                }

                // std::cout << fraud << "  ,   " << year << std::endl;
                
                std::map<std::string,std::map<std::string , int>>::iterator it;

                it = mp2.find(year);
                if (it == mp2.end())
                {
                    
                }


            }
        }



        int get_users_percentage_with_insufficient_more_then_once(int num_of_times)
        {
            std::ifstream index_file;
            index_file.open("./utilities/data_processors/index_info.txt");
            std::vector<long int> indices;  //to store index information
            std::vector<std::thread> vector_of_threads; // for threads

            //getting index info
            indices.push_back(0);
            while (index_file.good())
            {
                long int temp;
                index_file >> temp;
                // std::cout << "pushing temp" << temp << std::endl;
                indices.push_back(temp);
            }

            for (unsigned int i = 1; i < indices.size(); i++)
            {
                vector_of_threads.push_back(std::thread (&data_processor::set_map_for_users_with_insufficient ,this , indices[i-1] , indices[i]));
            }

            for (unsigned int i = 0; i < indices.size()-1; i++)
            {
                vector_of_threads[i].join();
            }

            int count = 0;
            for (auto &i : mp)
            {
                if (i.second > num_of_times)
                {
                    count++;
                }
            }
            return count *100 / 2000;

        }
        void set_set_for_unique_merchant(long int from , long int to)
        {

            std::string transaction , merchant_id;
            std::ifstream flat_file;
            flat_file.open("../card_transaction.v1.csv");
            if(flat_file.is_open())
            {
                std::cout << "flat file is open" << std::endl;
            }
            flat_file.seekg(from, std::ios::beg);
            if(from == 0)
            {
                std::getline(flat_file, transaction);
            }

            while(flat_file.tellg() != to)
            {
                std::getline(flat_file, transaction);
                if(transaction == "")
                {
                    return;
                }
                std::stringstream ss(transaction);
                for(int j =0; j<9; j++)
                    {std::getline(ss , merchant_id , ',');}

                std::unique_lock<std::mutex> lk(mtx);
                st.insert(merchant_id);
                lk.unlock(); 
            }            
        }
        int total_number_of_unique_merchants()
        {
            std::ifstream index_file;
            index_file.open("./utilities/data_processors/index_info.txt");
            std::vector<long int> indices;  //to store index information
            std::vector<std::thread> vector_of_threads; // for threads
            get_index_info(indices);

            for (unsigned int i = 1; i < indices.size(); i++)
            {
                vector_of_threads.push_back(std::thread (&data_processor::set_set_for_unique_merchant ,this , indices[i-1] , indices[i]));
            }

            for (unsigned int i = 0; i < indices.size()-1; i++)
            {
                vector_of_threads[i].join();
            }

            return st.size();
        }

        int percentage_of_fraud_by_year()
        {
            set_map_for_fraud_year(0 , 470925384);
            std::cout << mp.size() << std::endl;
            return 0;
        }



        //Identify all types of transactions
        bool check_already_vector(std::string type)
        {
            for (auto &i : vec)
            {
                if (i == type)
                {
                    return true;
                }
            }
            return false;
        }
        void identify_all_types_of_transactions_setup(long int from , long int to)
        {
            std::string transaction , type;
            std::ifstream flat_file;
            flat_file.open("../card_transaction.v1.csv" );
            if (flat_file.is_open())
            {
                std::cout << "file is open" << std::endl;
            }
            flat_file.seekg(from);

            if(from == 0)
            {
                std::getline(flat_file, transaction);
            }
            while(flat_file.tellg() != to)
            {
                std::getline(flat_file, transaction);
                if(transaction == "")
                {
                    return;
                }
                std::stringstream ss(transaction);
                for(int i=0; i< 8; i++)
                {
                    std::getline(ss , type , ',');
                }
                
                if (!check_already_vector(type))
                {
                    std::lock_guard<std::mutex> lk(mtx);
                    
                    vec.push_back(type);
                }
            }
        }

        std::string indentify_all_types_of_transactions()
        {
            std::ifstream index_file;
            index_file.open("./utilities/data_processors/index_info.txt");
            std::vector<long int> indices;  //to store index information
            std::vector<std::thread> vector_of_threads; // for threads
           // get_index_info(indices);

            //getting index info
            indices.push_back(0);
            while (index_file.good())
            {
                long int temp;
                index_file >> temp;
                // std::cout << "pushing temp" << temp << std::endl;
                indices.push_back(temp);
            }

            for (unsigned int i = 1; i < indices.size(); i++)
            {
                vector_of_threads.push_back(std::thread (&data_processor::identify_all_types_of_transactions_setup ,this , indices[i-1] , indices[i]));
            }

            for (unsigned int i = 0; i < indices.size()-1; i++)
            {
                vector_of_threads[i].join();
            }

            std::string buffer;
            for (auto &i : vec)
            {
                buffer += i + '\n';
            }
            return buffer;
        }


        void get_list_of_payments_for_given_user_from_flat_file(long int from , long int to , std::string id)
        {
            std::string transaction , fid , payment , month , errors;
            std::ifstream flat_file;
            flat_file.open("../card_transaction.v1.csv" );
            flat_file.seekg(from);
            if(from == 0)
            {
                std::getline(flat_file, transaction);
            }
            bool prev_error = false;
            int prev_month =13;

            while(flat_file.tellg() != to)
            {
                std::getline(flat_file, transaction);
                if(transaction == "")
                {
                    return;
                }

                std::stringstream ss(transaction);
                std::getline(ss , fid , ',');
                if (fid == id)
                {
                    std::getline(ss , month , ',');
                    std::getline(ss , month , ',');
                    std::getline(ss , month , ',');

                    for(int i=0; i< 3; i++)
                    {
                        std::getline(ss , payment , ',');
                    }
                    for(int i=0; i< 7; i++)
                    {
                        std::getline(ss , errors , ',');
                    }
                    if (errors[0] == '"')
                    {
                        std::string temp;
                        std::getline(ss , temp , '"');
                        errors += ',';
                        errors += temp;
                        errors += '"';
                    }
                    bool flag = insufficient_balance_occured(errors , fid);
                    if (flag == true && prev_error == false)
                    {
                        prev_error = flag;
                    }
                    if (flag == false && prev_error == true)
                    {
                        vec.push_back(payment);
                        prev_error = false;
                        continue;
                    }

                    if (prev_month < std::stoi(month))
                    {
                        vec.push_back(payment);
                        prev_month = std::stoi(month);
                        continue;
                    }
                    if (prev_month > std::stoi(month))
                    {
                        prev_month = std::stoi(month);
                        continue;
                    }
                } 
                else if (fid  < id){
                    continue;
                }
                else{
                    return;
                }             
            }
        }


        void list_all_payments_made_by_user(std::string id)
        {
            std::ifstream index_file;
            index_file.open("./utilities/data_processors/index_info.txt");
            std::vector<long int> indices;  //to store index information
            std::vector<std::thread> vector_of_threads; // for threads
            //get_index_info(indices);
             //getting index info
            indices.push_back(0);
            while (index_file.good())
            {
                long int temp;
                index_file >> temp;
                // std::cout << "pushing temp" << temp << std::endl;
                indices.push_back(temp);
            }

            for (unsigned int i = 1; i < indices.size(); i++)
            {
                vector_of_threads.push_back(std::thread (&data_processor::get_list_of_payments_for_given_user_from_flat_file ,this , indices[i-1] , indices[i] , id));
            }

            for (unsigned int i = 0; i < indices.size()-1; i++)
            {
                vector_of_threads[i].join();
            }

            std::vector<std::pair<std::string , std::vector<std::string>>> xmvect;
            std::string user_data[3];
            get_user_name(id , user_data);
            for (auto &i : vec)
            {
                std::vector<std::string> internalvect;
                internalvect.push_back(user_data[0]);
                internalvect.push_back(user_data[1]);
                internalvect.push_back(user_data[2]);
                xmvect.push_back(std::pair<std::string , std::vector<std::string>>({i ,  internalvect}));
            }
            TempXml xm;
            xm.create_xml(xmvect , "Amount" , {"First Name" , "Last Name" , "Email"});

        }

        //Top 5 Group BY Cities with total number of transactions
        void top_five_cities_withtotal_number_of_transactions_setup(long int from , long int to)
        {
            std::map<std::string, int>::iterator it;
            std::string transaction , city;
            std::ifstream flat_file;
            flat_file.open("../card_transaction.v1.csv");
            flat_file.seekg(from);

            if(from == 0)
            {
                std::getline(flat_file, transaction);
            }

            while(flat_file.tellg() != to)
            {
                std::getline(flat_file, transaction);
                if(transaction == "")
                {
                    return;
                }
                std::stringstream ss(transaction);

                for (int i =0; i<10; i++)
                {
                    std::getline(ss , city , ',');
                }

                it = gpset.find(city);
                if(it == gpset.end())
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    gpset.insert({city , 1});
                }
                else{
                    std::lock_guard<std::mutex> lock(mtx);
                    it->second++;
                }

            }
        }
        std::string top_five_cities_withtotal_number_of_transactions()
        {
            std::ifstream index_file;
            index_file.open("./utilities/data_processors/index_info.txt");
            std::vector<long int> indices;  //to store index information
            std::vector<std::thread> vector_of_threads; // for threads
            indices.push_back(0);
            while (index_file.good())
            {
                long int temp;
                index_file >> temp;
                // std::cout << "pushing temp" << temp << std::endl;
                indices.push_back(temp);
            }

            for (unsigned int i = 1; i < indices.size(); i++)
            {
                vector_of_threads.push_back(std::thread (&data_processor::top_five_cities_withtotal_number_of_transactions_setup ,this , indices[i-1] , indices[i]));
            }

            for (unsigned int i = 0; i < indices.size()-1; i++)
            {
                vector_of_threads[i].join();
            }

            std::vector<std::pair<std::string , int>> vect;
            vect.push_back({"ciry" , 1});
            vect.push_back({"ciry" , 1});
            vect.push_back({"ciry" , 1});
            vect.push_back({"ciry" , 1});
            vect.push_back({"ciry" , 1});

            for (auto &i : gpset)
            {
                for(int j =0; j<5; j++)
                {
                    if(vect[j].second < i.second)
                    {
                        vect[j].first = i.first;
                        vect[j].second = i.second;
                        break;
                    }
                }
            }
            std::string buffer = "";
            for (auto &i: vect)
            {
                buffer += i.first + "  ,  " + std::to_string(i.second) + "\n";
            }
            return buffer;

            
        }

        void top_five_recurring_transactions_merchant_setup(long int from , long int to)
        {
            std::map<std::string, int>::iterator it;
            std::string transaction , amount , merchant;
            std::ifstream flat_file;
            flat_file.open("../card_transaction.v1.csv");
            flat_file.seekg(from);

            if(from == 0)
            {
                std::getline(flat_file, transaction);
            }

            while(flat_file.tellg() != to)
            {
                std::getline(flat_file, transaction);
                if(transaction == "")
                {
                    return;
                }
                std::stringstream ss(transaction);

                for (int i =0; i<7; i++)
                {
                    std::getline(ss , amount , ',');
                }

                for (int i =0; i<2; i++)
                {
                    std::getline(ss , merchant , ',');
                }

                std::map<std::string , int >::iterator it;

                it = gpset.find(merchant);

                if(it == gpset.end())
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    gpset.insert({merchant , 1});
                }
                else
                {
                    it->second += 1;
                }
            }
        }

        std::string top_five_recurring_transactions_merchant()
        {
            std::ifstream index_file;
            index_file.open("./utilities/data_processors/index_info.txt");
            std::vector<long int> indices;  //to store index information
            std::vector<std::thread> vector_of_threads; // for threads
            indices.push_back(0);
            while (index_file.good())
            {
                long int temp;
                index_file >> temp;
                // std::cout << "pushing temp" << temp << std::endl;
                indices.push_back(temp);
            }

            for (unsigned int i = 1; i < indices.size(); i++)
            {
                vector_of_threads.push_back(std::thread (&data_processor::top_five_recurring_transactions_merchant_setup ,this , indices[i-1] , indices[i]));
            }

            for (unsigned int i = 0; i < indices.size()-1; i++)
            {
                vector_of_threads[i].join();
            }
            for (auto &i : mp2)
            {
                for(auto &j : i.second)
                {
                    std::cout << i.first << "  ,   " << j.first << "  ,  " << j.second << std::endl;
                }
            }
            std::vector<std::pair<std::string , int>> vect;
            vect.push_back({"ciry" , 1});
            vect.push_back({"ciry" , 1});
            vect.push_back({"ciry" , 1});
            vect.push_back({"ciry" , 1});
            vect.push_back({"ciry" , 1});
            for (auto &i : gpset)
            {
                for(int j =0; j<5; j++)
                {
                    if(vect[j].second < i.second)
                    {
                        vect[j].first = i.first;
                        vect[j].second = i.second;
                        break;
                    }
                }
            }
            std::string buffer = "";
            for (auto &i: vect)
            {
                buffer += i.first + "  ,  " + std::to_string(i.second) + "\n";
            }
            return buffer;
        }

        void t5_total_amount_transactions_groupby_zipcodes_setup(long int from , long int to)
        {
            std::string transaction , amount , zipcode;
            std::ifstream flat_file;
            flat_file.open("../card_transaction.v1.csv");
            flat_file.seekg(from);

            if(from == 0)
            {
                std::getline(flat_file, transaction);
            }

            while(flat_file.tellg() != to)
            {
                std::getline(flat_file, transaction);
                std::stringstream ss(transaction);
                for(int i=0; i<7; i++)
                {
                    std::getline(ss , amount , ',');
                }
                for(int i=0; i<6; i++)
                {
                    std::getline(ss , zipcode , ',');
                }

                amount.erase(0,1);
                double amnt = std::stod(amount);

                std::map<std::string, double>::iterator it;
                it = mpamount.find(zipcode);

                if(it == mpamount.end())
                {
                    mpamount.insert({zipcode , amnt});
                }
                else
                {
                    it->second += amnt;
                }
            }
        }

        void t5_total_amount_transactions_groupby_zipcodes()
        {
            std::ifstream index_file;
            index_file.open("./utilities/data_processors/index_info.txt");
            if (index_file.is_open())
                std::cout << "index file is open" << std::endl;
            std::vector<long int> indices;  //to store index information
            std::vector<std::thread> vector_of_threads; // for threads
            while (index_file.good())
            {
                long int temp;
                index_file >> temp;
                // std::cout << "pushing temp" << temp << std::endl;
                indices.push_back(temp);
            }

            for (unsigned int i = 1; i < indices.size(); i++)
            {
                vector_of_threads.push_back(std::thread (&data_processor::t5_total_amount_transactions_groupby_zipcodes_setup ,this , indices[i-1] , indices[i]));
            }

            for (unsigned int i = 0; i < indices.size()-1; i++)
            {
                vector_of_threads[i].join();
            }
            std::vector<std::pair<std::string , double>> vect;
            vect.push_back({"ciry" , 1});
            vect.push_back({"ciry" , 1});
            vect.push_back({"ciry" , 1});
            vect.push_back({"ciry" , 1});
            vect.push_back({"ciry" , 1});
            for (auto &i : mpamount)
            {
                for(int j =0; j<5; j++)
                {
                    if(vect[j].second < i.second)
                    {
                        vect[j].first = i.first;
                        vect[j].second = i.second;
                        break;
                    }
                }
            }
            index_file.close();
            //prepping for xml function
            std::vector<std::pair<std::string , std::vector<std::string>>> xmvect;
            std::vector<std::string> internalvect;
            std::ifstream mydata;
            for (auto &i : vect)
            {
                internalvect.push_back(std::to_string(i.second));
                std::ifstream mydata;
                mydata.open("data_generation/final_data/states.csv");
                
                //briging in data for states according to zipcode
                while(!mydata.eof())
                {
                    std::string state, abb;
                    int start , end;
                    std::string states_data;
                    std::getline(mydata , states_data);
                    std::stringstream ss(states_data);
                    while(ss.good())
                    {
                        std::getline(ss , state , '\t');
                        std::getline(ss , state , '\t');
                        std::getline(ss , abb , '\t');
                        ss >> start >> end;
                    }
                    if (state != "")
                    {
                        if ( std::stoi(i.first) > start && std::stoi(i.first) < end)
                        {
                            internalvect.push_back(state);
                            internalvect.push_back(abb);
                        }
                    }
                }
                xmvect.push_back(std::pair<std::string , std::vector<std::string>>({i.first , internalvect}));
                internalvect.clear();
            }
            
            TempXml xm;
            xm.create_xml(xmvect , "zipcode" , {"Amount" , "Sate" , "Abbrevation"});
        }

        void t10_largest_transactions_setup(long int from , long int to , std::vector<largest_transactions> &vect)
        {
            std::string transaction , amnt;
            largest_transactions temp;
            
            FileReader reader(from , to , "../card_transaction.v1.csv");

            if(from == 0)
            {
                reader.next_line();
            }
            while(reader.next_line())
            {
                temp.uid = reader.get_user_id();
                amnt = reader.get_amount();
                amnt.erase(0,1);
                temp.amount = std::stod(amnt);
                
                temp.merchnat_id = reader.get_merchant_id();
                
                for (auto &i : vect)
                {
                    if (i.amount < temp.amount)
                    {
                        mtx.lock();
                        i.amount = temp.amount;
                        i.merchnat_id = temp.merchnat_id;
                        i.uid = temp.uid;
                        mtx.unlock();
                        break;
                    }
                }
            }
        }

        void t10_largest_transactions()
        {
            std::vector<largest_transactions> my_vect;
            largest_transactions temp;
            temp.amount = 0;
            temp.merchnat_id = "-1";
            temp.uid = "-1";
            for (int i =0; i<10; i++)
            {
                my_vect.push_back(temp);
            }
            std::vector<long int> indices;  //to store index information
            std::vector<std::thread> vector_of_threads; // for threads
            get_index_info(indices);
            
            for (unsigned int i = 1; i < indices.size(); i++)
            {
                vector_of_threads.push_back(std::thread (&data_processor::t10_largest_transactions_setup ,this , indices[i-1] , indices[i] , std::ref(my_vect)));
            }

            for (unsigned int i = 0; i < indices.size()-1; i++)
            {
                vector_of_threads[i].join();
            }

            std::vector<std::pair<std::string , std::vector<std::string>>> xmvect;
            for (auto &i : my_vect)
            {
                
                std::vector<std::string> internal_vector;
                std::string merchant_data = get_merchant_name(i.merchnat_id);
                std::string user_data[3];
                get_user_name(i.uid , user_data);
                internal_vector.push_back(merchant_data);
                internal_vector.push_back(user_data[0]);
                internal_vector.push_back(user_data[1]);
                internal_vector.push_back(user_data[2]);

                xmvect.push_back(std::pair<std::string , std::vector<std::string>>({std::to_string(i.amount) , internal_vector}));
            }
            TempXml xm;
            xm.create_xml(xmvect , "Amount" , {"Merchant" , "Client First Name" , "Client Last Name" , "Client Email"});

        }

        std::string get_merchant_name(std::string id)
        {
            std::string merchant , mid , name , type;
            std::ifstream mydata;
            mydata.open("data_generation/final_data/merchants_table.csv");
            while(!mydata.eof())
            {
                std::getline(mydata , merchant);
                std::stringstream ss(merchant);
                while(ss.good())
                {
                    std::getline(ss , mid , '\t');
                    std::getline(ss , name , '\t');
                    std::getline(ss , type , '\n');  
                }
                if (mid == id)
                {
                    return name + " " + type;
                }
            }
            return "None";
            
        }
        std::string * get_user_name(std::string id , std::string arr[] )
        {
            std::string merchant , uid;
            std::ifstream mydata;
            mydata.open("data_generation/final_data/users_table.csv");
            while(!mydata.eof())
            {
                std::getline(mydata , merchant);
                std::stringstream ss(merchant);
                while(ss.good())
                {
                    std::getline(ss , uid , '\t');
                    std::getline(ss , arr[0] , '\t');
                    std::getline(ss , arr[1] , '\t');
                    std::getline(ss , arr[2] , '\n');  
                }
                if (uid == id)
                {
                    return arr;
                }
            }
            arr[0] = "none";
            arr[1] = "none";
            arr[2] = "none";
            return arr;
        }

        std:: string get_sate(std::string zipcode)
        {
            std::string state_data , state_name , state_abb , zip_start , zip_end;
            std::ifstream my_data;
             my_data.open("data_generation/final_data/states.csv");
            if (zipcode == "online")
            {
                return "online";
            }
           
            while(my_data.eof())
            {
                std::getline(my_data , state_data);
                std::stringstream ss(state_data);
                std::getline(ss , state_name , '\t');
                std::getline(ss , state_name , '\t');
                std::getline(ss , state_abb , '\t');
                std::getline(ss , zip_start , '\t');
                std::getline(ss , zip_end , '\n');
                
                if (std::stoi(zipcode) > std::stoi(zip_start) && std::stoi(zipcode) < std::stoi(zip_end))
                {
                    return state_name + " " + state_abb;
                }
            }
            return "other";
        }

        void transactions_without_fraud_by_states_setup(long int from , long int to)
        {
            FileReader reader(from , to , "../card_transaction.v1.csv");
            std::string transaction , state , fraud;

            if(from == 0)
            {
                reader.next_line();
            }
            while(reader.next_line())
            {
                state = reader.get_merchant_state();
                fraud = reader.get_fraud();

                if (fraud == "No")
                {      
                    std::map<std::string , int>::iterator it;
                    it = mp.find(state);
                    if (it == mp.end())
                    {
                        mp.insert({state , 1});
                    }
                    else
                    {
                        it->second += 1;
                    }
                }
            }
        }

        void transactions_without_fraud_by_states()
        {
            std::vector<largest_transactions> my_vect;
            largest_transactions temp;
            temp.amount = 0;
            temp.merchnat_id = "-1";
            temp.uid = "-1";
            for (int i =0; i<10; i++)
            {
                my_vect.push_back(temp);
            }
            std::vector<long int> indices;  //to store index information
            std::vector<std::thread> vector_of_threads; // for threads
            get_index_info(indices);
            
            for (unsigned int i = 1; i < indices.size(); i++)
            {
                vector_of_threads.push_back(std::thread (&data_processor::transactions_without_fraud_by_states_setup, this , indices[i-1] , indices[i]));
            }

            for (unsigned int i = 0; i < indices.size()-1; i++)
            {
                vector_of_threads[i].join();
            }
            std::cout << "fault here " << std::endl;

            std::vector<std::pair<std::string , std::vector<std::string>>> xmvect;
            for (auto &i : mp)
            {
                std::vector<std::string> internalvect;
                internalvect.push_back(std::to_string(i.second));
                xmvect.push_back(std::pair<std::string , std::vector<std::string>>({i.first , internalvect}));
            }
            TempXml xm;
            xm.create_xml(xmvect , "State" , {"Number of transactions"});
        }


        void transactions_above_100_after_8_setup(long int from , long int to)
        {
            std::string transaction , time , amount , zipcode;
            FileReader reader(from , to , "../card_transaction.v1.csv");
            int tm , amnt;
        
            if(from == 0)
            {
                reader.next_line();
            }
            while(reader.next_line())
            {
                time = reader.get_time();
                amount = reader.get_amount();
                zipcode = reader.get_zip();

                if (zipcode == "")
                {
                    zipcode = "online";
                }

                amount.erase(0,1);
                tm = std::stoi(time);
                amnt = std::stoi(amount);

                //populating vector
                if (tm >= 8 && amnt > 100)
                {
                    std::map<std::string , int>::iterator it;
                    it = mp.find(zipcode);
                    if (it == mp.end())
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        mp.insert({zipcode , 1});
                    }
                    else
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        it->second += 1;
                    }
                }
            }
        }

        void transactions_above_100_after_8()
        {
            std::vector<long int> indices;  //to store index information
            std::vector<std::thread> vector_of_threads; // for threads
            get_index_info(indices);
            
            for (unsigned int i = 1; i < indices.size(); i++)
            {
                vector_of_threads.push_back(std::thread (&data_processor::transactions_above_100_after_8_setup , this , indices[i-1] , indices[i]));
            }

            for (unsigned int i = 0; i < indices.size()-1; i++)
            {
                vector_of_threads[i].join();
            }
            
            std::vector<std::pair<std::string , std::vector<std::string>>> xmvect;

            for (auto &i : mp)
            {
                std::vector<std::string> internalvect;
                internalvect.push_back(get_sate(i.first));
                internalvect.push_back(std::to_string(i.second));
                xmvect.push_back(std::pair<std::string , std::vector<std::string>>({i.first , internalvect}));
            }

            TempXml xm;
            xm.create_xml(xmvect , "Zipcode" , {"State Name","Number of transactions"});
        }

        void string_to_vect(std::string str  , std::vector<std::string> &vect)
        {
            if(str == ""){
                return;
            }
            std::stringstream ss(str);
            std::string temp;
            while(ss.good())
            {
                std::getline(ss , temp ,  ',');
                vect.push_back(temp);
            }
        }
        void t5_merchants_with_insuff_only_setup(long int from , long int to)
        {
            std::string merchant , errors;
            FileReader reader(from , to , "../card_transaction.v1.csv");
            if(from == 0)
            {
                reader.next_line();
            }
            while(reader.next_line())
            {
                std::vector<std::string> errors_vect; //store all the errors in transaction
                merchant = reader.get_merchant_id();
                errors = reader.get_errors();
                string_to_vect(errors , errors_vect);

                if(errors_vect.empty())
                {
                    continue;
                }
                if (std::find(errors_vect.begin() , errors_vect.end() , "Insufficient Balance") != errors_vect.end())
                {
                    std::map<std::string , int>::iterator it;
                    it = gpset.find(merchant);
                    if (it == gpset.end()) //merchant is not in the map
                    {
                        gpset.insert({merchant , 1});
                    }
                    else
                    {
                        it->second += 1;
                    }
                }
                
            }            
        }
        void t5_merchants_with_insuff_only()
        {
            std::vector<long int> indices;  //to store index information
            std::vector<std::thread> vector_of_threads; // for threads
            get_index_info(indices);
            
            for (unsigned int i = 1; i < indices.size(); i++)
            {
                vector_of_threads.push_back(std::thread (&data_processor::t5_merchants_with_insuff_only_setup , this , indices[i-1] , indices[i]));
            }

            for (unsigned int i = 0; i < indices.size()-1; i++)
            {
                vector_of_threads[i].join();
            }
            std::vector<std::pair<std::string , int>> vect;
            for (int i = 0; i<5; i++)
            {
                auto pr = std::max_element(gpset.begin(), gpset.end(), [](const auto &x, const auto &y) {
                    return x.second < y.second;
                });
                vect.push_back({pr->first, pr->second});
                gpset.erase(pr->first);
            }
            gpset.clear();   //empty  the map to save memory


            std::vector<std::pair<std::string , std::vector<std::string>>> xmvect;
            for (auto &i : vect)
            {
                std::vector<std::string> internalvect;
                internalvect.push_back(get_merchant_name(i.first));
                internalvect.push_back(std::to_string(i.second));
                xmvect.push_back(std::pair<std::string , std::vector<std::string>>({i.first , internalvect}));
            }

            TempXml xm;
            xm.create_xml(xmvect , "MerchantID" , {"MerchantName" , "InsufficientBalannceCount"});

        }
};