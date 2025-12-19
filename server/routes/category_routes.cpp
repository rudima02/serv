// category_routes.cpp - УПРОЩЁННАЯ ВЕРСИЯ
#include "category_routes.h"
#include "../tables/orm/all_tables_single-odb.hxx"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

void registerCategoryRoutes(
    httplib::Server& server,
    std::shared_ptr<odb::pgsql::database> db
) {
    server.Get("/categories", [db](const httplib::Request&, httplib::Response& res) {
        try {
            odb::transaction t(db->begin());
    
            // Получаем категории
            odb::result<Category> categories = db->query<Category>();
    
            std::vector<CategoryDTO> dtos;
    
            for (const auto& cat : categories) {
                CategoryDTO dto;
                dto.id = cat.getID();
                dto.name = cat.getName();
    
                // Получаем PO для этой категории
                odb::result<PO> pos = db->query<PO>(odb::query<PO>::category->id == cat.getID());
                for (const auto& po : pos) {
                    PODTO po_dto;
                    po_dto.id = po.getID();
                    po_dto.name = po.getName();
                    po_dto.category_id = cat.getID();
                    dto.programs.push_back(po_dto);
                }
    
                dtos.push_back(dto);
            }
    
            t.commit();
    
            res.set_content(nlohmann::json(dtos).dump(), "application/json");
            res.status = 200;
        }
        catch (const std::exception& e) {
            std::cerr << "GET /categories error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Internal server error"})", "application/json");
        }
    });
    
    
    server.Post("/categories", [db](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            std::string name = j["name"].get<std::string>();
            
            Category category(name);
            
            odb::transaction t(db->begin());
            db->persist(category);
            t.commit();
            
            json response;
            response["id"] = category.getID();
            response["name"] = category.getName();
            
            res.status = 201;
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "POST /categories error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });
        // DELETE /categories/{id}
        // DELETE /categories/{id} (с каскадным удалением)
        // DELETE /categories/{id} (полное каскадное удаление)
    server.Delete(R"(/categories/(\d+))", [db](const httplib::Request& req, httplib::Response& res) {
        try {
            unsigned long category_id = std::stoul(req.matches[1]);
            
            odb::transaction t(db->begin());
            
            // Проверяем существование
            std::unique_ptr<Category> category(db->load<Category>(category_id));
            if (!category) {
                res.status = 404;
                res.set_content(R"({"error":"Category not found"})", "application/json");
                return;
            }
            
            // Получаем все программы этой категории
            odb::result<PO> programs = db->query<PO>(odb::query<PO>::category->id == category_id);
            std::vector<unsigned long> program_ids;
            
            for (const auto& program : programs) {
                program_ids.push_back(program.getID());
            }
            
            // Для каждой программы удаляем версии
            for (unsigned long po_id : program_ids) {
                odb::result<VersionPO> versions = db->query<VersionPO>(odb::query<VersionPO>::po->id == po_id);
                for (const auto& version : versions) {
                    db->erase<VersionPO>(version.getID());
                }
                
                // Удаляем установки (Semi) для этой программы
                odb::result<Semi> installations = db->query<Semi>(odb::query<Semi>::po->id == po_id);
                for (const auto& install : installations) {
                    db->erase<Semi>(install.getID());
                }
                
                // Удаляем саму программу
                db->erase<PO>(po_id);
            }
            
            // Удаляем категорию
            db->erase<Category>(category_id);
            
            t.commit();
            
            res.status = 200;
            res.set_content(R"({"success":true,"message":"Category and all related data deleted"})", "application/json");
        }
        catch (const odb::exception& e) {
            std::cerr << "DELETE /categories database error: " << e.what() << std::endl;
            res.status = 500;
            json error_response;
            error_response["error"] = "Database error";
            error_response["details"] = e.what();
            res.set_content(error_response.dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "DELETE /categories error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Internal server error"})", "application/json");
        }
    });
}