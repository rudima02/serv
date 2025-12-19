#pragma once
#include "../tables/all_tables_single.h"
#include "../tables/dto/rolesDTO.h"

inline Roles toEntity(const RolesDTO& dto) {
    return Roles(dto.name);
}
