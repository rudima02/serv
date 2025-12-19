#pragma once
#include "../tables/all_tables_single.h"
#include "../tables/dto/usersDTO.h"

inline UsersDTO toDTO(const Users& u) {
    UsersDTO dto;
    dto.id = u.getID();
    dto.name = u.getName();
    if (u.getDepartment()) dto.department_id = u.getDepartment()->getID();
    if (u.getRole()) dto.role_id = u.getRole()->getID();
    return dto;
}
