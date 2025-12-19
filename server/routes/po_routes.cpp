#include "po_routes.h"
#include "../tables/orm/all_tables_single-odb.hxx"
#include <nlohmann/json.hpp>
#include <iostream>
#include <memory>

using json = nlohmann::json;

void registerPORoutes(
    httplib::Server& server,
    std::shared_ptr<odb::pgsql::database> db
) {
    // GET /pos
    server.Get("/po", [db](const httplib::Request&, httplib::Response& res) {
        try {
            odb::transaction t(db->begin());

            odb::result<PO> pos = db->query<PO>();
            std::vector<PODTO> dtos;

            for (const auto& po : pos) {
                PODTO dto;
                dto.id = po.getID();
                dto.name = po.getName();
                if (po.getCategory())
                    dto.category_id = po.getCategory()->getID();
                dtos.push_back(dto);
            }

            t.commit();
            res.status = 200;
            res.set_content(json(dtos).dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "GET /pos error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });

        // POST /pos
    server.Post("/po", [db](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            std::string name = j["name"].get<std::string>();
            unsigned long category_id = j["category_id"].get<unsigned long>();

            odb::transaction t(db->begin());
            
            // Проверяем существование категории
            std::unique_ptr<Category> category(db->load<Category>(category_id));
            if (!category) {
                res.status = 404;
                res.set_content(R"({"error":"Category not found"})", "application/json");
                t.rollback();
                return;
            }
            
            // Создаем shared_ptr для категории
            std::shared_ptr<Category> category_ptr = std::shared_ptr<Category>(
                db->load<Category>(category_id), 
                [](Category*) {}  // Пустой deleter
            );
            
            PO po(name, category_ptr);
            db->persist(po);
            t.commit();

            json response;
            response["id"] = po.getID();
            response["name"] = po.getName();
            response["category_id"] = category_id;

            res.status = 201;
            res.set_content(response.dump(), "application/json");
        }
        catch (const json::exception& e) {
            std::cerr << "POST /po JSON error: " << e.what() << std::endl;
            res.status = 400;
            res.set_content(R"({"error":"Invalid JSON"})", "application/json");
        }
        catch (const odb::exception& e) {
            std::cerr << "POST /po database error: " << e.what() << std::endl;
            res.status = 500;
            json error_response;
            error_response["error"] = "Database error";
            error_response["details"] = e.what();
            res.set_content(error_response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "POST /po error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Internal server error"})", "application/json");
        }
    });
        // DELETE /po/{id}
    server.Delete(R"(/po/(\d+))", [db](const httplib::Request& req, httplib::Response& res) {
        try {
            unsigned long po_id = std::stoul(req.matches[1]);
            
            odb::transaction t(db->begin());
            
            // Проверяем существование
            std::unique_ptr<PO> po(db->load<PO>(po_id));
            if (!po) {
                res.status = 404;
                res.set_content(R"({"error":"PO not found"})", "application/json");
                return;
            }
            
            // Удаляем все версии этой программы
            odb::result<VersionPO> versions = db->query<VersionPO>(odb::query<VersionPO>::po->id == po_id);
            for (const auto& version : versions) {
                db->erase<VersionPO>(version.getID());
            }
            
            // Удаляем все установки (Semi) этой программы
            odb::result<Semi> installations = db->query<Semi>(odb::query<Semi>::po->id == po_id);
            for (const auto& install : installations) {
                db->erase<Semi>(install.getID());
            }
            
            // Удаляем саму программу
            db->erase<PO>(po_id);
            
            t.commit();
            
            res.status = 200;
            res.set_content(R"({"success":true,"message":"PO and all related data deleted"})", "application/json");
        }
        catch (const odb::exception& e) {
            std::cerr << "DELETE /po database error: " << e.what() << std::endl;
            res.status = 500;
            json error_response;
            error_response["error"] = "Database error";
            error_response["details"] = e.what();
            res.set_content(error_response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "DELETE /po error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Internal server error"})", "application/json");
        }
    });
}
