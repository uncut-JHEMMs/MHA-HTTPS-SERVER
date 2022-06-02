#include <httpserver.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iomanip>



//logger
#include "./loggers/PerformanceLogger.h"
//resources
#include "./resources/digest_resource.h"
#include "./resources/hello_world_resource.h"
#include "./resources/create_user_resource.h"
#include "./resources/create_credit_cards_resource.h"
#include "./resources/add_merchant.h"

//data_processor resources here**************
#include "./resources/dataprocessing_resource/insufficient_balance_more_than_once_resource.h"
#include "./resources/dataprocessing_resource/insufficient_balance_atleast_once_resource.h"
#include "./resources/dataprocessing_resource/Identify_all_type_transactions_resource.h"
#include "./resources/dataprocessing_resource/deposits_by_given_user.h"
#include "./resources/dataprocessing_resource/top_five_group_by_cities_with_total_number_of_transactions_resource.h"
#include "./resources/dataprocessing_resource/top_five_recurring_transactions_groupby+merchants_resource.h"
#include "./resources/dataprocessing_resource/t5_groupby_zipcodes_amount_resource.h"
#include "./resources/dataprocessing_resource/t10_largest_transactions_resource.h"
#include "./resources/dataprocessing_resource/transactions_without_fraud_by_states_resource.h"
#include "./resources/dataprocessing_resource/transactions_above_100_after_8_resource.h"
#include "./resources/dataprocessing_resource/t5_merchants_with_isufficient_resource.h"
#include "./resources/dataprocessing_resource/bot5_month_with_insuffcient_resource.h"
#include "./resources/dataprocessing_resource/groupby_merchants_online_city_resource.h"


using namespace httpserver;



// printing logs to terminal
void custom_access_log(const std::string& url) {
    std::cout << "ACCESSING: " << url << std::endl;
    
        //{
        //    std::lock_guard<std::mutex> lk(PerformanceLogger::m);
        //    PerformanceLogger::sig = true;
        //}
        //PerformanceLogger::cv.notify_one();
}

// if resource not found
const std::shared_ptr<http_response> not_found_custom(const http_request& req) 
{
      return std::shared_ptr<string_response>(new string_response("Path Not Found", 404, "text/plain"));
}


int main(int argc, char** argv) 
{
    
    webserver ws = create_webserver(8443)
        .start_method(http::http_utils::INTERNAL_SELECT).max_threads(8)
        .max_connections(10)
        .connection_timeout(60)
        //.per_IP_connection_limit(2)
        .use_dual_stack()
        .use_ssl()
        .https_mem_trust("./certs/newcert.crt")
        .https_mem_key("./certs/newkey.pem")
        .https_mem_cert("./certs/newcert.crt")
        .log_access(custom_access_log)
        .debug()
        .not_found_resource(not_found_custom);
    

//    std::thread t(PerformanceLogger::calculateResponseTime);
    
    hello_world_resource hwr;
    ws.register_resource("/hello", &hwr);
    
    digest_resource dr;
    ws.register_resource("/auth" , &dr);

    create_user_resource cur;
    ws.register_resource("/addUser" , &cur);
    
    create_merchants_resource mrt;
    ws.register_resource("/addMerchant" , &mrt);

    create_credit_card_resource crt;
    ws.register_resource("/addCard" , &crt);

    insufficient_balance_more_than_once_resource insuf_balance;
    ws.register_resource("/insufficient_balance_more_then_one" , &insuf_balance);

    insufficient_balance_atleast_once_resource insuf_balance_once;
    ws.register_resource("/insufficient_balance_atleast_once" , &insuf_balance_once);

    identify_all_types_of_transactions_resource all_trans_types;
    ws.register_resource("/identify_all_type_of_transactions" , &all_trans_types);

    deposits_by_given_user dpu;
    ws.register_resource("/deposits" , &dpu);

    top_five_gorup_by_cities_with_total_number_of_transactions tpc;
    ws.register_resource("/top_five_gorup_by_cities" , &tpc);

    top_five_recurring_transactions_groupby_merchants tpm;
    ws.register_resource("/top_five_recurring_transactions" , &tpm);

    t5_groupby_zipcodes_amount_resource tgz;
    ws.register_resource("t5_by_code_by_amount" , &tgz);

    t10_largest_transactions_resource tlt;
    ws.register_resource("t10_largest_transactions" , &tlt);

    transactions_without_fraud_by_states_resource tfs;
    ws.register_resource("transactions_without_fraud" , &tfs);

    transactions_above_100_after_8_resource ta8;
    ws.register_resource("transactions_above_100_after_8" , &ta8);

    t5_merchants_with_insufficient_resource tins;
    ws.register_resource("/t5_merchants_with_insuff" , &tins);

    bot5_month_with_insufficient_resource  bmi;
    ws.register_resource("bot5_with_insuff" , &bmi);

    groupby_merchants_online_city_resource gmoc;
    ws.register_resource("groupby_merchants_online_city" , &gmoc);


    //std::thread t (PerformanceLogger::calculateResponseTime());

    ws.start(true);
    
//    t.join();
    return 0;
}
