#pragma once
#include <httplib.h>
#include <memory>
#include <odb/pgsql/database.hxx>
#include "../tables/dto/pcDTO.h"
#include "../tables/mappers/read/pc_mapper.h"
#include "../tables/mappers/write/pc_mapper.h"

void registerPCRoutes(httplib::Server& server, std::shared_ptr<odb::pgsql::database> db);
