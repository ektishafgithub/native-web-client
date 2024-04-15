//
//  Request.h
//  Cplusplus
//
//  Created by Ektishaf Ahwaz on 13/04/2024.
//

#ifndef Request_h
#define Request_h

#include <stdio.h>
#include <cstring>
#include <format>
#include <string>
#include <vector>
#include <map>
#include <curl/curl.h>

struct Request
{
    CURL* Handle;
    CURLcode Result;
    
    std::vector<std::string> Headers;
    struct curl_slist* RequestHeaders = NULL;
    std::string ResponseHeaders;
    
    std::string URL;
    std::string Body;
    std::string Response;
    
    static size_t WriteFunction(char* contents, size_t size, size_t nmemb, std::string* userdata)
    {
        userdata->append(contents, size * nmemb);
        printf("%s\n", contents);
        return size * nmemb;
    }
    
    Request(std::string url, std::vector<std::string> headers, std::string body)
    {
        URL = url;
        Headers = headers;
        Body = body;
        RequestHeaders = NULL;
        
        Handle = curl_easy_init();
        if(!Handle)
        {
            std::cout << "Handle can not be created for request " << url.c_str() << std::endl;
            return;
        }
        
        if(headers.size() > 0)
        {
            for(const auto& field : headers)
            {
                RequestHeaders = curl_slist_append(RequestHeaders, field.c_str());
            }
            curl_easy_setopt(Handle, CURLOPT_HEADER, 1);
            curl_easy_setopt(Handle, CURLOPT_HTTPHEADER, RequestHeaders);
        }
        
        curl_easy_setopt(Handle, CURLOPT_HEADERDATA, &ResponseHeaders);
        curl_easy_setopt(Handle, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
        curl_easy_setopt(Handle, CURLOPT_TIMEOUT, 10);
        curl_easy_setopt(Handle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(Handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        curl_easy_setopt(Handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(Handle, CURLOPT_WRITEFUNCTION, WriteFunction);
        curl_easy_setopt(Handle, CURLOPT_WRITEDATA, &Response);
        
        if(!body.empty())
        {
            char* json = strcpy(new char[body.length() + 1], body.c_str());
            curl_easy_setopt(Handle, CURLOPT_POSTFIELDS, json);
        }
    }
    
    void Perform()
    {
        Result = curl_easy_perform(Handle);
        //printf("Response Headers: %s\n", ResponseHeaders.c_str());
        if(Result != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(Result));
        }
        else
        {
            //printf("Response: %s\n", Response.c_str());
        }
    }
    
    void CleanUp()
    {
        //free(WriteData);
        if(Handle != NULL)
        {
            curl_easy_cleanup(Handle);
            Handle = NULL;
        }
        if(RequestHeaders != NULL)
        {
            curl_slist_free_all(RequestHeaders);
            RequestHeaders = NULL;
        }
    }
};

#endif /* Request_h */
