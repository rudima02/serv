#pragma once
#include <nlohmann/json.hpp>
#include <string>

class UpdatePOCountDTO {
public:
    unsigned long po_id;
    std::string po_name;

    UpdatePOCountDTO() = default;
};

inline void to_json(nlohmann::json& j, const UpdatePOCountDTO& dto) {
    j = {
        {"po_id", dto.po_id},
        {"po_name", dto.po_name}
    };
}

inline void from_json(const nlohmann::json& j, UpdatePOCountDTO& dto) {
    j.at("po_id").get_to(dto.po_id);
    j.at("po_name").get_to(dto.po_name);
}
