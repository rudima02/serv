#pragma once
#include <httplib.h>
#include <memory>
#include <odb/pgsql/database.hxx>
#include "../tables/dto/usersDTO.h"
#include "../tables/mappers/read/users_mapper.h"
#include "../tables/mappers/write/users_mapper.h"

void registerUsersRoutes(httplib::Server& server, std::shared_ptr<odb::pgsql::database> db);
