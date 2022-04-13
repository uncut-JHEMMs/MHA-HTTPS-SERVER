#include<chrono>
#include<vector>
#include<iostream>
#include<string>
#include<sstream>
#include<thread>
#include<mutex>
#include<fstream>
#include<set>


std::set<std::pair<std::string , std::string> > ids_set;

int main()
{
    std::string states[50][3];
    std::ifstream states_data;

    std::ofstream write_states;
    write_states.open("states_table.csv");
    states_data.open("states.csv");

    for(int i =0; i<50; i++)
    {
        std::string temp;
        std::getline(states_data , temp);

        std::stringstream ss(temp);

        while(ss.good())
        {
            std::getline(ss , states[i][0] , ',');
            std::getline(ss , states[i][1] , ',');
            std::getline(ss , states[i][2] , ',');
        }
    }

    std::string arr;
    std::pair<std::string, std::string> pr;
    std::ifstream transactions;
    transactions.open("trans.csv");
    while(transactions.good())
    {
        std::string temp;
        
        std::getline(transactions , temp);

        std::stringstream ss(temp);

        std::getline(ss , arr , ',');
        std::getline(ss , arr , ',');
        std::getline(ss , arr , ',');
        std::getline(ss , arr , ',');
        std::getline(ss , arr , ',');
        std::getline(ss , arr , ',');
        std::getline(ss , arr , ',');
        std::getline(ss , arr , ',');
        std::getline(ss , arr , ',');
        std::getline(ss , pr.first , ',');
        std::getline(ss , arr , ',');
        std::getline(ss , pr.second , ',');

        if(ids_set.find(pr) != ids_set.end())
        {
            continue;
        }
        else
        {
            for (int i =0; i<50; i++)
            {
                if(states[i][0] == arr)
                {
                    write_states << pr.first << '\t';
                    write_states << states[i][2] << '\t';
                }
            }
            write_states << pr.first << '\t';
            write_states << pr.second << '\n';
        }
    }
}