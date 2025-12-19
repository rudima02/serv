#pragma once
#include "../tables/all_tables_single.h"
#include "../tables/dto/usersDTO.h"

inline Users toEntity(
    const UsersDTO& dto,
    const std::shared_ptr<Department>& department,
    const std::shared_ptr<Roles>& role
) {
    return Users(dto.name, department, role);
}
