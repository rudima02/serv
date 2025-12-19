#pragma once
#include <string>
#include <vector>
#include "poDTO.h"
#include <nlohmann/json.hpp>

class CategoryDTO {
public:
    unsigned long id;
    std::string name;
    std::vector<PODTO> programs;

    CategoryDTO() = default;
};

inline void to_json(nlohmann::json& j, const CategoryDTO& c) {
    j = {
        {"id", c.id},
        {"name", c.name},
        {"programs", c.programs}
    };
}

inline void from_json(const nlohmann::json& j, CategoryDTO& c) {
    if (j.contains("id")) j.at("id").get_to(c.id);
    j.at("name").get_to(c.name);
    if (j.contains("programs")) j.at("programs").get_to(c.programs);
}
