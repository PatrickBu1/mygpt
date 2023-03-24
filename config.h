//
// Created by Patrick Bu on 3/15/23.
//

#ifndef MYGPT_CONFIG_H
#define MYGPT_CONFIG_H

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

const vector<string> CONF_KEYS({"api_key", "model"});

int modifyConfig(char**);
vector<string> tokenizeKeyValue(const char*);

#endif