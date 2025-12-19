#include "versionpo_routes.h"
#include "../tables/orm/all_tables_single-odb.hxx"
#include <nlohmann/json.hpp>
#include <iostream>
#include <memory>

using json = nlohmann::json;

void registerVersionPORoutes(
    httplib::Server& server,
    std::shared_ptr<odb::pgsql::database> db
) {
    // GET /versionpos - получить все версии ПО
    server.Get("/versionpos", [db](const httplib::Request&, httplib::Response& res) {
        try {
            odb::transaction t(db->begin());
            odb::result<VersionPO> versions = db->query<VersionPO>();

            std::vector<VersionPODTO> dtos;
            for (const auto& v : versions) {
                VersionPODTO dto;
                dto.id = v.getID();
                dto.version = v.getVersion();
                dto.date_version = v.getDate();
                if (v.getPO()) dto.po_id = v.getPO()->getID();
                dtos.push_back(dto);
            }

            t.commit();
            res.status = 200;
            res.set_content(json(dtos).dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "GET /versionpos error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });

    // POST /versionpos - создать новую версию ПО
    server.Post("/versionpos", [db](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            std::string version = j["version"].get<std::string>();
            std::string date_version = j["date_version"].get<std::string>();
            unsigned long po_id = j["po_id"].get<unsigned long>();

            odb::transaction t(db->begin());

            auto query = odb::query<PO>::id == po_id;
            odb::result<PO> pos = db->query<PO>(query);

            if (pos.empty()) {
                t.rollback();
                res.status = 404;
                res.set_content(R"({"error":"PO not found"})", "application/json");
                return;
            }

            auto it = pos.begin();
            std::shared_ptr<PO> po = std::make_shared<PO>(*it);

            VersionPO vpo(version, date_version, po);

            db->persist(vpo);
            t.commit();

            VersionPODTO dto;
            dto.id = vpo.getID();
            dto.version = vpo.getVersion();
            dto.date_version = vpo.getDate();
            dto.po_id = po_id;

            res.status = 201;
            res.set_content(json(dto).dump(), "application/json");
        }
        catch (const json::exception& e) {
            std::cerr << "POST /versionpos JSON parsing error: " << e.what() << std::endl;
            res.status = 400;
            res.set_content(R"({"error":"Invalid JSON format"})", "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "POST /versionpos error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });
        // DELETE /versionpos/{id}
    server.Delete(R"(/versionpos/(\d+))", [db](const httplib::Request& req, httplib::Response& res) {
        try {
            unsigned long version_id = std::stoul(req.matches[1]);
            
            odb::transaction t(db->begin());
            
            // Проверяем существование
            std::unique_ptr<VersionPO> version(db->load<VersionPO>(version_id));
            if (!version) {
                res.status = 404;
                res.set_content(R"({"error":"VersionPO not found"})", "application/json");
                return;
            }
            
            // Удаляем
            db->erase<VersionPO>(version_id);
            
            t.commit();
            
            res.status = 200;
            res.set_content(R"({"success":true,"message":"VersionPO deleted"})", "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "DELETE /versionpos error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });
}
