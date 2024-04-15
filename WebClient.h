//
//  WebClient.h
//  Cplusplus
//
//  Created by Ektishaf Ahwaz on 13/04/2024.

#include <curl/curl.h>
#include <string>
#include <map>


void Init();
void Deint();
void CreateRequest(std::string url, std::vector<std::string> headers, std::string body);
void Perform();
