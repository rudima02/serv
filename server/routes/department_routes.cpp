#include "department_routes.h"
#include "../tables/orm/all_tables_single-odb.hxx"
#include <nlohmann/json.hpp>
#include <iostream>
#include <memory>

using json = nlohmann::json;

void registerDepartmentRoutes(
    httplib::Server& server,
    std::shared_ptr<odb::pgsql::database> db
) {
    // GET /departments
    server.Get("/departments", [db](const httplib::Request&, httplib::Response& res) {
        try {
            odb::transaction t(db->begin());

            odb::result<Department> departments = db->query<Department>();
            std::vector<DepartmentDTO> dtos;

            for (const auto& dept : departments) {
                DepartmentDTO dto;
                dto.id = dept.getID();
                dto.name = dept.getName();

                // Можно, если нужно, добавить ПК этой департамента
                odb::result<PC> pcs = db->query<PC>(odb::query<PC>::department->id == dept.getID());
                for (const auto& pc : pcs) {
                    PCDTO pc_dto;
                    pc_dto.id = pc.getID();
                    pc_dto.name = pc.getName();
                    pc_dto.department_id = dept.getID();
                    dto.pcs.push_back(pc_dto);
                }

                dtos.push_back(dto);
            }

            t.commit();

            res.status = 200;
            res.set_content(json(dtos).dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "GET /departments error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });

    // POST /departments
    server.Post("/departments", [db](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            std::string name = j["name"].get<std::string>();

            Department department(name);

            odb::transaction t(db->begin());
            db->persist(department);
            t.commit();

            json response;
            response["id"] = department.getID();
            response["name"] = department.getName();

            res.status = 201;
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "POST /departments error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });
        // DELETE /departments/{id}
    server.Delete(R"(/departments/(\d+))", [db](const httplib::Request& req, httplib::Response& res) {
        try {
            unsigned long department_id = std::stoul(req.matches[1]);
            
            odb::transaction t(db->begin());
            
            // Проверяем существование
            std::unique_ptr<Department> department(db->load<Department>(department_id));
            if (!department) {
                res.status = 404;
                res.set_content(R"({"error":"Department not found"})", "application/json");
                return;
            }
            
            // Проверяем, есть ли связанные компьютеры
            odb::result<PC> pcs = db->query<PC>(odb::query<PC>::department->id == department_id);
            std::vector<unsigned long> pc_ids;
            
            for (const auto& pc : pcs) {
                pc_ids.push_back(pc.getID());
            }
            
            // Если есть компьютеры, удаляем их каскадно
            if (!pc_ids.empty()) {
                // Для каждого компьютера удаляем установки (Semi)
                for (unsigned long pc_id : pc_ids) {
                    odb::result<Semi> installations = db->query<Semi>(odb::query<Semi>::pc->id == pc_id);
                    for (const auto& install : installations) {
                        db->erase<Semi>(install.getID());
                    }
                    
                    // Удаляем компьютер
                    db->erase<PC>(pc_id);
                }
            }
            
            // Проверяем, есть ли связанные пользователи
            odb::result<Users> users = db->query<Users>(odb::query<Users>::department->id == department_id);
            for (const auto& user : users) {
                db->erase<Users>(user.getID());
            }
            
            // Удаляем отдел
            db->erase<Department>(department_id);
            
            t.commit();
            
            res.status = 200;
            res.set_content(R"({"success":true,"message":"Department and all related data deleted"})", "application/json");
        }
        catch (const odb::exception& e) {
            std::cerr << "DELETE /departments database error: " << e.what() << std::endl;
            res.status = 500;
            json error_response;
            error_response["error"] = "Database error";
            error_response["details"] = e.what();
            res.set_content(error_response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "DELETE /departments error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Internal server error"})", "application/json");
        }
    });
}
