#include "users_routes.h"
#include "../tables/orm/all_tables_single-odb.hxx"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

void registerUsersRoutes(
    httplib::Server& server,
    std::shared_ptr<odb::pgsql::database> db
) {
    // ======================
    // GET /users
    // ======================
    server.Get("/users", [db](const httplib::Request&, httplib::Response& res) {
        try {
            odb::transaction t(db->begin());

            odb::result<Users> users = db->query<Users>();
            std::vector<UsersDTO> dtos;

            for (const auto& u : users) {
                UsersDTO dto;
                dto.id = u.getID();
                dto.name = u.getName();

                if (u.getDepartment())
                    dto.department_id = u.getDepartment()->getID();

                if (u.getRole())
                    dto.role_id = u.getRole()->getID();

                dtos.push_back(dto);
            }

            t.commit();
            res.status = 200;
            res.set_content(json(dtos).dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "GET /users error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Internal server error"})", "application/json");
        }
    });

    // ======================
    // POST /users
    // ======================
    server.Post("/users", [db](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            std::string name = j["name"];
            unsigned long department_id = j["department_id"];
            unsigned long role_id = j["role_id"];
    
            odb::transaction t(db->begin());
    
            auto dep_res = db->query<Department>(odb::query<Department>::id == department_id);
            auto role_res = db->query<Roles>(odb::query<Roles>::id == role_id);
    
            if (dep_res.empty() || role_res.empty()) {
                res.status = 404;
                res.set_content(R"({"error":"Department or Role not found"})", "application/json");
                return;
            }
    
            std::shared_ptr<Department> dept = std::make_shared<Department>(*dep_res.begin());
            std::shared_ptr<Roles> role = std::make_shared<Roles>(*role_res.begin());
    
            Users user(name, dept, role);
            db->persist(user);
            t.commit();
    
            res.status = 201;
            res.set_content(json{
                {"id", user.getID()},
                {"name", user.getName()},
                {"department_id", department_id},
                {"role_id", role_id}
            }.dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "POST /users error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });
        // DELETE /users/{id}
    server.Delete(R"(/users/(\d+))", [db](const httplib::Request& req, httplib::Response& res) {
        try {
            unsigned long user_id = std::stoul(req.matches[1]);
            
            odb::transaction t(db->begin());
            
            // Проверяем существование
            std::unique_ptr<Users> user(db->load<Users>(user_id));
            if (!user) {
                res.status = 404;
                res.set_content(R"({"error":"User not found"})", "application/json");
                return;
            }
            
            // Удаляем
            db->erase<Users>(user_id);
            
            t.commit();
            
            res.status = 200;
            res.set_content(R"({"success":true,"message":"User deleted"})", "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "DELETE /users error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });
    
}
