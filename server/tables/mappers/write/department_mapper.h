#pragma once
#include "../tables/all_tables_single.h"
#include "../tables/dto/departmentDTO.h"

inline Department toEntity(const DepartmentDTO& dto) {
    return Department(dto.name);
}
