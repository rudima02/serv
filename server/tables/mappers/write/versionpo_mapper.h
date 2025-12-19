#pragma once
#include "../tables/all_tables_single.h"
#include "../tables/dto/versionPODTO.h"

inline VersionPO toEntity(
    const VersionPODTO& dto,
    const std::shared_ptr<PO>& po
) {
    return VersionPO(dto.version, dto.date_version, po);
}
