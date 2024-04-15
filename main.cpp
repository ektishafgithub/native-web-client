//
//  main.cpp
//  Cplusplus
//
//  Created by Ektishaf Ahwaz on 11/04/2024.
//

#include <iostream>
#include "WebClient.h"
#include <vector>


int main(int argc, const char * argv[]) {
    std::cout << "Begin ..." << std::endl;
    Init();
    std::vector<std::string> headers = 
    {
        "Content-Type: application/json",
    };
    
    std::string body = std::string("{\"title\":\"8kt\",\"body\":\"h8w\",\"userId\":8}");
    CreateRequest(std::string("https://jsonplaceholder.typicode.com/posts"), headers, body);
    //CreateRequest(std::string("https://jsonplaceholder.typicode.com/posts"), {}, "");
    Perform();
    //Deint();
    std::cout << "End ..." << std::endl;
    return 0;
}


