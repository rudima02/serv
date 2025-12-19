#pragma once
#include "../tables/all_tables_single.h"
#include "../tables/dto/departmentDTO.h"

inline DepartmentDTO toDTO(const Department& d) {
    DepartmentDTO dto;
    dto.id = d.getID();
    dto.name = d.getName();
    return dto;
}
