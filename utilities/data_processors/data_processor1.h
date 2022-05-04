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
class data_processor
{
    private:
        std::mutex mtx;
        std::map<std::string , int> mp;
        std::set<std::string> st;
        std::map<std::string , std::map<std::string , int>> mp2;
        std::vector<std::string> vec;
        std::map<std::string , int> gpset;
    public:

        data_processor()
        {}
        void get_index_info(std::vector<long int> &indices)
        {
            std::ifstream index_file;
            index_file.open("index_info.txt");
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
            std::string transaction , fid , payment;
            std::ifstream flat_file;
            flat_file.open("../card_transaction.v1.csv" );
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
                std::getline(ss , fid , ',');
                for(int i=0; i< 6; i++)
                {
                    std::getline(ss , payment , ',');
                }
                
                
                if (fid == id)
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    vec.push_back(payment);
                }
                else{
                    continue;
                }             
            }
        }


        std::string list_all_payments_made_by_user(std::string id)
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

            std::string buffer;
            for (auto &i : vec)
            {
                buffer += i + '\n';
            }   
            return buffer;
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


};