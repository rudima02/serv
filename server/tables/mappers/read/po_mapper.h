#pragma once
#include "../tables/all_tables_single.h"
#include "../tables/dto/poDTO.h"

inline PODTO toDTO(const PO& po) {
    PODTO dto;
    dto.id = po.getID();
    dto.name = po.getName();
    if (po.getCategory())
        dto.category_id = po.getCategory()->getID();
    return dto;
}
