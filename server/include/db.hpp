#pragma once
#include <memory>
#include <odb/pgsql/database.hxx>

inline std::shared_ptr<odb::pgsql::database> createDB(
    const std::string& user,
    const std::string& password,
    const std::string& host,
    const std::string& dbname,
    unsigned int port = 5432
) {
    return std::make_shared<odb::pgsql::database>(
        user, password, host, dbname, port
    );
}
