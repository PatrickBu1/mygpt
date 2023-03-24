
#include <iostream>
#include <cstring>
#include <thread>
#include <future>
#include <chrono>
#include "config.h"
#include "api.h"

using namespace std;

//string combine_prompt(const vector<string>&, const string&);

int main(int argc, char* argv[]) {
    if ((argc > 2 && strcmp(argv[1], "-c")!=0) ||
            (argc == 2 && strcmp(argv[1], "-s")!=0)){
        cerr << "Usage: mygpt [-s] or mygpt[-c] key=value" <<endl;
        return 1;
    }
    if (strcmp(argv[2], "-c") == 0) {
        return modifyConfig(argv + 2);
    }

    ifstream inputFile("config.json");
    if (!inputFile.is_open()) {
        std::cerr << "config.json cannot be opened!" << std::endl;
        return 1;
    }

    json confData;
    inputFile >> confData;
    string addr = confData["address"];
    string token = confData["api_key"];
    string model = confData["model"];

    ApiSession conn(addr, token, model, 0, 0);

    cout << "ChatGPT model " << model << ". Type \"quit\" to quit"<< endl;

    // bool stateful = strcmp(argv[2], "-s") == 0;
    // vector<string> stateful_history;

    while(true){
        string prompt;
        string user(getenv("USER"));
        cout << user << " >> ";
        getline(cin, prompt);
        if (prompt == "quit"){
            return 0;
        }
//        if (stateful) {
//            prompt = combine_prompt(stateful_history, prompt);
//        }

        std::promise<json> result_promise;
        std::future<json> result_future = result_promise.get_future();
        std::thread api_thread([&result_promise, prompt, &conn]{
            try{
                json response = conn.call(prompt);
                result_promise.set_value(response);
            }catch(const exception &e){
                cerr << "Error: Network Error"<<endl;
            }
        });

        cout << "ChatGPT >> ";
        int dots = 0;
        while (result_future.wait_for(chrono::milliseconds(200)) != future_status::ready) {
            cout << ".";
            dots++;
            if (dots > 5) {
                dots = 0;
                cout << "\rChatGPT >> ";
            }
            cout.flush();
        }

        cout << "\rChatGPT >> ";
        json response = result_future.get();
        api_thread.join();
        string res_text = string(response["choices"][0]["message"]["content"]).substr(2);
        cout << res_text <<endl;
    }


}

//string combine_prompt(const vector<string> &history, const string &prompt){
//
//}

