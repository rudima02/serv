#pragma once
#include <httplib.h>
#include <memory>
#include <odb/pgsql/database.hxx>
#include "../tables/dto/semiDTO.h"
#include "../tables/mappers/read/semi_mapper.h"
#include "../tables/mappers/write/semi_mapper.h"

void registerSemiRoutes(httplib::Server& server, std::shared_ptr<odb::pgsql::database> db);
