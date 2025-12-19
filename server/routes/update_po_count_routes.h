#pragma once
#include <httplib.h>
#include <memory>
#include "../tables/orm/all_tables_single-odb.hxx"
#include "../tables/dto/update_po_countDTO.h"
#include <odb/pgsql/database.hxx>

void registerUpdatePOCountRoutes(httplib::Server& server, std::shared_ptr<odb::pgsql::database> db);
