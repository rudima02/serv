#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class UsersDTO {
public:
    unsigned long id = 0;
    std::string name;
    unsigned long department_id = 0;
    unsigned long role_id = 0;
    std::vector<unsigned long> pc_ids;
    std::vector<unsigned long> installed_po_ids;

    UsersDTO() = default;
};

inline void to_json(nlohmann::json& j, const UsersDTO& u) {
    j = {
        {"id", u.id},
        {"name", u.name},
        {"department_id", u.department_id},
        {"role_id", u.role_id},
        {"pc_ids", u.pc_ids},
        {"installed_po_ids", u.installed_po_ids}
    };
}

inline void from_json(const nlohmann::json& j, UsersDTO& u) {
    if (j.contains("id")) j.at("id").get_to(u.id);
    j.at("name").get_to(u.name);
    if (j.contains("department_id")) j.at("department_id").get_to(u.department_id);
    if (j.contains("role_id")) j.at("role_id").get_to(u.role_id);
    if (j.contains("pc_ids")) j.at("pc_ids").get_to(u.pc_ids);
    if (j.contains("installed_po_ids")) j.at("installed_po_ids").get_to(u.installed_po_ids);
}