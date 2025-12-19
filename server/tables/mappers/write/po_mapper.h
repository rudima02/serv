#pragma once
#include "../tables/all_tables_single.h"
#include "../tables/dto/poDTO.h"

inline PO toEntity(
    const PODTO& dto,
    const std::shared_ptr<Category>& category
) {
    return PO(dto.name, category);
}
