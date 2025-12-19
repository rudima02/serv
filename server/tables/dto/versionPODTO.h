#pragma once
#include <string>
#include <nlohmann/json.hpp>

class VersionPODTO {
public:
    unsigned long id = 0;
    std::string version;
    std::string date_version;
    unsigned long po_id = 0;  // связь с PO

    VersionPODTO() = default;
};

inline void to_json(nlohmann::json& j, const VersionPODTO& v) {
    j = {
        {"id", v.id},
        {"version", v.version},
        {"date_version", v.date_version},
        {"po_id", v.po_id}
    };
}

inline void from_json(const nlohmann::json& j, VersionPODTO& v) {
    if (j.contains("id")) j.at("id").get_to(v.id);
    j.at("version").get_to(v.version);
    j.at("date_version").get_to(v.date_version);
    if (j.contains("po_id")) j.at("po_id").get_to(v.po_id);
}
