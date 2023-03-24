//
// Created by Patrick Bu on 3/15/23.
//

#ifndef MYGPT_API_H
#define MYGPT_API_H

#include <curl/curl.h>
#include <exception>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

class ApiSession{
private:
    CURL* conn;
    string addr;
    string token;
    string model_name;
    int max_tokens;
    float temperature;

public:
    ApiSession(const string &addr, const string &token, const string &model_name, int max_tokens, float temperature);

    json call(const string &prompt);

    static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

};
#endif //MYGPT_API_H
