#pragma once
#include "../tables/all_tables_single.h"
#include "../tables/dto/semiDTO.h"

inline Semi toEntity(
    const SemiDTO& dto,
    const std::shared_ptr<PO>& po,
    const std::shared_ptr<PC>& pc
) {
    return Semi(po, pc);
}
