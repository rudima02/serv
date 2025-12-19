#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class PCDTO {
public:
    unsigned long id = 0;
    std::string name;
    unsigned long department_id = 0;
    std::vector<unsigned long> installed_po_ids;

    PCDTO() = default;
};

inline void to_json(nlohmann::json& j, const PCDTO& p) {
    j = {
        {"id", p.id},
        {"name", p.name},
        {"department_id", p.department_id},
        {"installed_po_ids", p.installed_po_ids}
    };
}

inline void from_json(const nlohmann::json& j, PCDTO& p) {
    if (j.contains("id")) j.at("id").get_to(p.id);
    j.at("name").get_to(p.name);
    if (j.contains("department_id")) j.at("department_id").get_to(p.department_id);
    if (j.contains("installed_po_ids")) j.at("installed_po_ids").get_to(p.installed_po_ids);
}