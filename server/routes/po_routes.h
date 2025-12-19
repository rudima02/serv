#pragma once
#include <httplib.h>
#include <memory>
#include <odb/pgsql/database.hxx>
#include "../tables/dto/poDTO.h"
#include "../tables/mappers/read/po_mapper.h"
#include "../tables/mappers/write/po_mapper.h"

void registerPORoutes(httplib::Server& server, std::shared_ptr<odb::pgsql::database> db);
