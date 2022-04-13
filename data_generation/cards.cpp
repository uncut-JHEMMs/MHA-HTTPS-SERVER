#include<iostream>
#include<fstream>
#include<string>
#include<set>
#include<sstream>
#include<mutex>
#include<chrono>
#include<vector>
#include<thread>

using namespace std::chrono;


std::mutex mut;
std::mutex mut2;
std::set<std::pair<std::string , std::string>> ids_set;

bool checkLuhn(std::string &cardNo)
{
    int nDigits = cardNo.length();
 
    int nSum = 0, isSecond = false;
    for (int i = nDigits - 1; i >= 0; i--) {
 
        int d = cardNo[i] - '0';
 
        if (isSecond == true)
            d = d * 2;
        nSum += d / 10;
        nSum += d % 10;
 
        isSecond = !isSecond;
    }
    return (nSum % 10 == 0);
}

std::string* bring_data(std::ifstream &id_read , int chunk_size)
{
    mut2.lock();
    std::string *arr = new std::string[chunk_size];
    for(int i = 0; i<chunk_size; i++)
    {
        std::getline(id_read , arr[i]);
    }
    mut2.unlock();
    return arr;

}

void writer(std::ifstream &names , std::ifstream &writer , std::ofstream &file , int chunk_size)
{
    while(!writer.eof()){
    std::string *lines = bring_data(writer , chunk_size);

    std::string card_number;
    std::pair<std::string, std::string> pr;

    for(int i=0; i<chunk_size; i++)
    {

        if (lines[i] == "")
        {
            std::cout << "breaking" << std::endl;
            break;
        }
        std::stringstream s_stream(lines[i]);
        std::getline(s_stream , pr.first , ',');
        std::getline(s_stream , pr.second , ',');

        if(ids_set.find(pr) != ids_set.end())
        {
            continue;
        }
        
        else
        {
            mut.lock();
            std::getline(names , card_number , '\n');
            if(!checkLuhn(card_number))
            {
                continue;
            }

            std::cout << pr.first << " , " << pr.second << " , " << card_number <<std::endl;
            file << pr.first << '\t';
            file << pr.second << '\t';
            file << card_number << '\n';
            ids_set.insert(pr);
            mut.unlock();
        }   
    }//for loop closing
    }//while loop closing
}

int main()
{
    auto start = high_resolution_clock::now();
    std::ifstream fake_read , id_read;
    std::ofstream user_write;

    fake_read.open("./MHA-HTTPS-SERVER/data_generation/fake_data/credit_Cards.csv");
    id_read.open("card_transaction.v1.csv");
    user_write.open("credit_cards_table.csv");
    
    {
        std::string st;
        std::getline(id_read , st);
    }
   //writer(fake_read , id_read , user_write , 1);  
    std::vector<std::thread> thrds;
    for (int i =0; i< 4; i++)
    {
        thrds.push_back(std::thread([&fake_read , &id_read , &user_write] { writer(fake_read , id_read , user_write , 2000); }));//3048363
    }

    for (int i =0; i<4; i++)
    {
        thrds[i].join();
    }
    std::cout << "closing files" << std::endl;
    fake_read.close();
    id_read.close();
    user_write.close();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    std::cout << duration.count() << std::endl;
    return 0;
    
}