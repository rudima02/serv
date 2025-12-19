#include "pc_routes.h"
#include "../tables/orm/all_tables_single-odb.hxx"
#include <nlohmann/json.hpp>
#include <iostream>
#include <memory>

using json = nlohmann::json;

void registerPCRoutes(
    httplib::Server& server,
    std::shared_ptr<odb::pgsql::database> db
) {
    // GET /pcs
    server.Get("/pc", [db](const httplib::Request&, httplib::Response& res) {
        try {
            odb::transaction t(db->begin());

            odb::result<PC> pcs = db->query<PC>();
            std::vector<PCDTO> dtos;

            for (const auto& pc : pcs) {
                PCDTO dto;
                dto.id = pc.getID();
                dto.name = pc.getName();
                dto.department_id = pc.getDepartment() ? pc.getDepartment()->getID() : 0;
                dtos.push_back(dto);
            }

            t.commit();
            res.status = 200;
            res.set_content(json(dtos).dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "GET /pcs error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });

    // POST /pcs
    server.Post("/pc", [db](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            std::string name = j["name"].get<std::string>();
            unsigned long dept_id = j["department_id"].get<unsigned long>();

            odb::transaction t(db->begin());

            // Загружаем отдел через сырую ссылку
            Department* dept_raw = db->load<Department>(dept_id);
            if (!dept_raw) throw std::runtime_error("Department not found");

            // Создаём shared_ptr с пустым deleter
            std::shared_ptr<Department> dept(dept_raw, [](Department*) {});

            PC pc(name, dept);
            db->persist(pc);

            t.commit();

            json response;
            response["id"] = pc.getID();
            response["name"] = pc.getName();
            response["department_id"] = pc.getDepartment()->getID();

            res.status = 201;
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "POST /pcs error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });
        // DELETE /pc/{id}
    server.Delete(R"(/pc/(\d+))", [db](const httplib::Request& req, httplib::Response& res) {
        try {
            unsigned long pc_id = std::stoul(req.matches[1]);
            
            odb::transaction t(db->begin());
            
            // Проверяем существование
            std::unique_ptr<PC> pc(db->load<PC>(pc_id));
            if (!pc) {
                res.status = 404;
                res.set_content(R"({"error":"PC not found"})", "application/json");
                return;
            }
            
            // Удаляем
            db->erase<PC>(pc_id);
            
            t.commit();
            
            res.status = 200;
            res.set_content(R"({"success":true,"message":"PC deleted"})", "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "DELETE /pc error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });
}
