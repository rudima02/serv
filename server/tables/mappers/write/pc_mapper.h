#pragma once
#include "../tables/all_tables_single.h"
#include "../tables/dto/pcDTO.h"

inline PC toEntity(
    const PCDTO& dto,
    const std::shared_ptr<Department>& department
) {
    return PC(dto.name, department);
}
