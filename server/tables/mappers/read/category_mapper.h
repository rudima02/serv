#pragma once
#include "../tables/all_tables_single.h"
#include "../tables/dto/categoryDTO.h"

inline CategoryDTO toDTO(const Category& c) {
    CategoryDTO dto;
    dto.id = c.getID();
    dto.name = c.getName();
    return dto;
}
