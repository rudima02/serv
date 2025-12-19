#pragma once
#include "../tables/all_tables_single.h"
#include "../tables/dto/categoryDTO.h"

inline Category toEntity(const CategoryDTO& dto) {
    return Category(dto.name);
}
