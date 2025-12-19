#pragma once
#include <httplib.h>
#include <memory>
#include <odb/pgsql/database.hxx>
#include "../tables/dto/versionPODTO.h"
#include "../tables/mappers/read/versionpo_mapper.h"
#include "../tables/mappers/write/versionpo_mapper.h"

void registerVersionPORoutes(httplib::Server& server, std::shared_ptr<odb::pgsql::database> db);
