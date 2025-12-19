#pragma once
#include <httplib.h>
#include <memory>
#include <odb/pgsql/database.hxx>
#include "../tables/dto/rolesDTO.h"
#include "../tables/mappers/read/roles_mapper.h"
#include "../tables/mappers/write/roles_mapper.h"

void registerRolesRoutes(httplib::Server& server, std::shared_ptr<odb::pgsql::database> db);
