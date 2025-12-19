#pragma once
#include <httplib.h>
#include <memory>
#include <odb/pgsql/database.hxx>
#include "../tables/dto/categoryDTO.h"
#include "../tables/mappers/read/category_mapper.h"
#include "../tables/mappers/write/category_mapper.h"

void registerCategoryRoutes(httplib::Server& server, std::shared_ptr<odb::pgsql::database> db);
