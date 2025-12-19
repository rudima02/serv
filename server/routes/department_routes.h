#pragma once
#include <httplib.h>
#include <memory>
#include <odb/pgsql/database.hxx>
#include "../tables/dto/departmentDTO.h"
#include "../tables/dto/pcDTO.h"
#include "../tables/mappers/read/department_mapper.h"
#include "../tables/mappers/write/department_mapper.h"

void registerDepartmentRoutes(httplib::Server& server, std::shared_ptr<odb::pgsql::database> db);
