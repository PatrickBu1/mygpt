//
// Created by Patrick Bu on 3/15/23.
//

#include "api.h"


ApiSession::ApiSession(const string &addr, const string &token, const string &model_name, int max_tokens, float temperature){
    this->addr = addr;
    this->token = token;
    this->model_name = model_name;
    this->max_tokens = max_tokens;
    this->temperature = temperature;
    conn = curl_easy_init();
    if (!conn) {
        throw runtime_error("cURL cannot be initiated");
    }

}

json ApiSession::call(const string &prompt){
    // Set headers
    curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("Authorization: Bearer " + token).c_str());
    curl_easy_setopt(conn, CURLOPT_URL, addr.c_str());  // Set HTTP address
    curl_easy_setopt(conn, CURLOPT_HTTPPOST, 1L);        // Set HTTP method
    curl_easy_setopt(conn, CURLOPT_HTTPHEADER, headers);    // Add HTTP header fields

    // Set body
    json requestBody;
    requestBody["model"] = model_name;
    requestBody["messages"] = {{{"role", "user"},{"content", prompt}}};
    if (temperature != 0) requestBody["temperature"] = temperature;
    if (max_tokens != 0) requestBody["max_tokens"] = max_tokens;
    string json_string = requestBody.dump();

    std::string response_body;
    curl_easy_setopt(conn, CURLOPT_POSTFIELDS, json_string.c_str());
    curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(conn, CURLOPT_WRITEDATA, &response_body);

    CURLcode res = curl_easy_perform(conn);
    if (res != CURLE_OK) {
        throw runtime_error("http not ok");
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(conn);
    json jsonResponse = json::parse(response_body);
    return jsonResponse;
}

size_t ApiSession::write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t num_bytes = size * nmemb;
    auto *response = reinterpret_cast<string *>(userdata);
    response->append(ptr, num_bytes);
    return num_bytes;
}


