#pragma once
#include "../tables/all_tables_single.h"
#include "../tables/dto/pcDTO.h"

inline PCDTO toDTO(const PC& pc) {
    PCDTO dto;
    dto.id = pc.getID();
    dto.name = pc.getName();
    if (pc.getDepartment())
        dto.department_id = pc.getDepartment()->getID();
    return dto;
}
