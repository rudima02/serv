#pragma once
#include "../tables/all_tables_single.h"
#include "../tables/dto/rolesDTO.h"

inline RolesDTO toDTO(const Roles& r) {
    RolesDTO dto;
    dto.id = r.getID();
    dto.name = r.getName();
    return dto;
}
