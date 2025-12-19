#pragma once
#include <nlohmann/json.hpp>

class SemiDTO {
public:
    unsigned long id;
    unsigned long po_id;
    unsigned long pc_id;

    SemiDTO() = default;
};

inline void to_json(nlohmann::json& j, const SemiDTO& s) {
    j = {
        {"id", s.id},
        {"po_id", s.po_id},
        {"pc_id", s.pc_id}
    };
}

inline void from_json(const nlohmann::json& j, SemiDTO& s) {
    if (j.contains("id")) j.at("id").get_to(s.id);
    j.at("po_id").get_to(s.po_id);
    j.at("pc_id").get_to(s.pc_id);
}
