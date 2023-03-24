//
// Created by Patrick Bu on 3/15/23.
//

#include <sstream>
#include "config.h"

int modifyConfig(char** argv){
    ifstream inputFile("config.json");
    if (!inputFile.is_open()) {
        std::cerr << "config.json cannot be found!" << std::endl;
        return 1;
    }
    json confData;
    try{
        inputFile >> confData;
    }catch (const exception &e){
        cerr << "config.json cannot be parsed" <<endl;
        return 1;
    }
    size_t argSeek = 0;
    while (argv[argSeek] != nullptr) {
        vector<string> tokens = tokenizeKeyValue(argv[argSeek]);
        if (tokens.size() != 2) {
            cerr << "Usage: mygpt[-c] key=value. For example, mygpt -c api_key=myapikey" << endl;
            return 1;
        }
        if (find(CONF_KEYS.begin(), CONF_KEYS.end(), tokens[0]) == CONF_KEYS.end()) {
            cerr << "Confuguration key is not valid" << endl;
            return 1;
        }
        confData[tokens[0]] = tokens[1];
    }
    inputFile.close();

    std::ofstream outputFile("conf.json");
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return 1;
    }

    outputFile << confData;

    if (truncate("conf.json", outputFile.tellp()) != 0) {
        std::cerr << "Error truncating file: " << strerror(errno) << std::endl;
        return 1;
    }
    return 0;
}

vector<string> tokenizeKeyValue(const char* str){
    stringstream ss(str);
    vector<string> tokens;
    string temp_str;
    while(getline(ss, temp_str, '=')){
        tokens.push_back(temp_str);
    }
    return tokens;
}