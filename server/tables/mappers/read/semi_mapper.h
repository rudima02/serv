#pragma once
#include "../tables/all_tables_single.h"
#include "../tables/dto/semiDTO.h"

inline SemiDTO toDTO(const Semi& s) {
    SemiDTO dto;
    dto.id = s.getID();
    if (s.getPO()) dto.po_id = s.getPO()->getID();
    if (s.getPC()) dto.pc_id = s.getPC()->getID();
    return dto;
}
