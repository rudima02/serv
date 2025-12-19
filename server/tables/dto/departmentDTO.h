#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "pcDTO.h"   // <- вот этот include

class DepartmentDTO {
public:
    unsigned long id;
    std::string name;
    std::vector<PCDTO> pcs;
    std::vector<unsigned long> users;

    DepartmentDTO() = default;
};

inline void to_json(nlohmann::json& j, const DepartmentDTO& d) {
    j = {
        {"id", d.id},
        {"name", d.name},
        {"pcs", d.pcs},
        {"users", d.users}
    };
}

inline void from_json(const nlohmann::json& j, DepartmentDTO& d) {
    if (j.contains("id")) j.at("id").get_to(d.id);
    j.at("name").get_to(d.name);
    if (j.contains("pcs")) j.at("pcs").get_to(d.pcs);
    if (j.contains("users")) j.at("users").get_to(d.users);
}
