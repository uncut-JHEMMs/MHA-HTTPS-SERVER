#include<chrono>
#include<vector>
#include<iostream>
#include<string>
#include<sstream>
#include<thread>
#include<mutex>
#include<fstream>
#include<set>

using namespace std::chrono;

std::mutex mut;
std::mutex mut2;
std::set<std::string> ids_set;

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
    //mut2.unlock(); //unlocks the lock in the function above
    std::string arr[3];
    std::string id;


    for(int i=0; i<chunk_size; i++)
    {

        if (lines[i] == "")
        {
            std::cout << "breaking" << std::endl;
            break;
        }
        std::stringstream s_stream(lines[i]);
        for(int j =0; j<9; j++)
            {std::getline(s_stream , id , ',');} // loop to the line to reach merchant id culomn
        //std::cout << lines[i] << std::endl;
        if(ids_set.find(id) != ids_set.end())
        {
            continue;
        }
        
        else
        {
            mut.lock();
            std::getline(names , arr[0] , ',');
            std::getline(names , arr[1] , '\n');

            std::cout << id << " , " << arr[0] << " , " << arr[1] <<std::endl;
            file << id << '\t';
            file << arr[0] << '\t';
            file << arr[1] << '\n';
            ids_set.insert(id);
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

    fake_read.open("./MHA-HTTPS-SERVER/data_generation/merchants.csv");
    id_read.open("card_transaction.v1.csv");
    user_write.open("merchants_table.csv");
    
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
