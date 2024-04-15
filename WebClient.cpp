//
//  WebClient.cpp
//  Cplusplus
//
//  Created by Ektishaf Ahwaz on 13/04/2024.
//

#include "WebClient.h"
#include <iostream>
#include "Request.h"

CURLM* CURLM_HNDL;
std::vector<Request> handles;
int still_running = 1;

void Init()
{
    if(curl_global_init(CURL_GLOBAL_ALL))
    {
        std::cout << "curl_global_init wasn't successful";
        return;
    }
    
    CURLM_HNDL = curl_multi_init();
    if(!CURLM_HNDL)
    {
        std::cout << "curl_multi_init wasn't successful";
        return;
    }
}

void CreateRequest(std::string url, std::vector<std::string> headers, std::string body)
{
    printf("Request:\nurl: %s\nheaders: %lu\nbody: %s", url.c_str(), headers.size(), body.c_str());
    Request req(url, headers, body);
    curl_multi_add_handle(CURLM_HNDL, req.Handle);
    handles.push_back(req);
}

void Perform()
{
    while(still_running)
    {
        CURLMcode mc = curl_multi_perform(CURLM_HNDL, &still_running);
        if(!mc)
        {
            mc = curl_multi_poll(CURLM_HNDL, NULL, 0, 1000, NULL);
        }
        
        if(mc)
        {
            break;
        }
        
        CURLMsg* msg;
        int queued;
        do
        {
            msg = curl_multi_info_read(CURLM_HNDL, &queued);
            if(msg)
            {
                if(msg->msg == CURLMSG_DONE)
                {
                    std::cout << msg->msg << "CURLMSG_DONE";
                }
                else
                {
                    std::cout << "CURLMSG_ERROR";
                }
            }
            
        } while (msg);
    }
}

void Deint()
{
    for(auto& req : handles)
    {
        curl_multi_remove_handle(CURLM_HNDL, req.Handle);
        req.CleanUp();
    }
    handles.clear();
    curl_multi_cleanup(CURLM_HNDL);
    curl_global_cleanup();
}
