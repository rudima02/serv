#pragma once
#include <vector>
#include <string>
#include <nlohmann/json.hpp> 

template<typename T>
std::string serializeVector(const std::vector<T>& vec) {
    nlohmann::json j = vec;
    return j.dump();
}

template<typename T>
T parseDTO(const std::string& body) {
    nlohmann::json j = nlohmann::json::parse(body);
    return j.get<T>();
}
