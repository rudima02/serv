#include "roles_routes.h"
#include "../tables/orm/all_tables_single-odb.hxx"
#include <nlohmann/json.hpp>
#include <iostream>
#include <memory>

using json = nlohmann::json;

void registerRolesRoutes(
    httplib::Server& server,
    std::shared_ptr<odb::pgsql::database> db
) {
    // GET /roles
    server.Get("/roles", [db](const httplib::Request&, httplib::Response& res) {
        try {
            odb::transaction t(db->begin());

            odb::result<Roles> roles = db->query<Roles>();
            std::vector<RolesDTO> dtos;

            for (const auto& r : roles) {
                RolesDTO dto;
                dto.id = r.getID();
                dto.name = r.getName();
                dtos.push_back(dto);
            }

            t.commit();
            res.status = 200;
            res.set_content(json(dtos).dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "GET /roles error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });

    // POST /roles
    server.Post("/roles", [db](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            std::string name = j["name"].get<std::string>();

            Roles role(name);

            odb::transaction t(db->begin());
            db->persist(role);
            t.commit();

            json response;
            response["id"] = role.getID();
            response["name"] = role.getName();

            res.status = 201;
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "POST /roles error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });
        // DELETE /roles/{id}
    server.Delete(R"(/roles/(\d+))", [db](const httplib::Request& req, httplib::Response& res) {
        try {
            unsigned long role_id = std::stoul(req.matches[1]);
            
            odb::transaction t(db->begin());
            
            // Проверяем существование
            std::unique_ptr<Roles> role(db->load<Roles>(role_id));
            if (!role) {
                res.status = 404;
                res.set_content(R"({"error":"Role not found"})", "application/json");
                return;
            }
            
            // Удаляем
            db->erase<Roles>(role_id);
            
            t.commit();
            
            res.status = 200;
            res.set_content(R"({"success":true,"message":"Role deleted"})", "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "DELETE /roles error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });
}
