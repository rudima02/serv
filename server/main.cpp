// main.cpp - УПРОЩЁННАЯ ВЕРСИЯ
#include <iostream>
#include "db.hpp"
#include "routes/category_routes.h"
#include "routes/department_routes.h"
#include "routes/pc_routes.h"
#include "routes/po_routes.h"
#include "routes/semi_routes.h"
#include "routes/users_routes.h"
#include "routes/versionpo_routes.h"
#include "routes/roles_routes.h"
#include "routes/update_po_count_routes.h"
#include "include/httplib.h"

int main() {
    try {
        auto db = createDB("rudima02", "QwE21gg9", "coursework", "postgres");
        
        httplib::Server server;
        
        // ТОЛЬКО категории
        registerUpdatePOCountRoutes(server, db);
        registerCategoryRoutes(server, db);
        registerDepartmentRoutes(server, db);
        registerPCRoutes(server, db);
        registerPORoutes(server, db);
        registerRolesRoutes(server, db);
        registerSemiRoutes(server, db);
        registerUsersRoutes(server, db);
        registerVersionPORoutes(server, db);
        
        std::cout << "Server started at http://localhost:8081\n";
        server.listen("0.0.0.0", 8081);
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}