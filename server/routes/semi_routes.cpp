#include "semi_routes.h"
#include "../tables/orm/all_tables_single-odb.hxx"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

void registerSemiRoutes(
    httplib::Server& server,
    std::shared_ptr<odb::pgsql::database> db
) {
    // ======================
    // GET /semis
    // ======================
    server.Get("/semis", [db](const httplib::Request&, httplib::Response& res) {
        try {
            odb::transaction t(db->begin());

            odb::result<Semi> semis = db->query<Semi>();
            std::vector<SemiDTO> dtos;

            for (const auto& s : semis) {
                SemiDTO dto;
                dto.id = s.getID();

                if (s.getPO())
                    dto.po_id = s.getPO()->getID();

                if (s.getPC())
                    dto.pc_id = s.getPC()->getID();

                dtos.push_back(dto);
            }

            t.commit();
            res.status = 200;
            res.set_content(json(dtos).dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "GET /semis error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Internal server error"})", "application/json");
        }
    });

    // ======================
    // POST /semis
    // ======================
    server.Post("/semis", [db](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            unsigned long po_id = j["po_id"];
            unsigned long pc_id = j["pc_id"];
    
            odb::transaction t(db->begin());
    
            auto po_res = db->query<PO>(odb::query<PO>::id == po_id);
            auto pc_res = db->query<PC>(odb::query<PC>::id == pc_id);
    
            if (po_res.empty() || pc_res.empty()) {
                res.status = 404;
                res.set_content(R"({"error":"PO or PC not found"})", "application/json");
                return;
            }
    
            std::shared_ptr<PO> po = std::make_shared<PO>(*po_res.begin());
            std::shared_ptr<PC> pc = std::make_shared<PC>(*pc_res.begin());
    
            Semi semi(po, pc);
            db->persist(semi);
            t.commit();
    
            res.status = 201;
            res.set_content(json{
                {"id", semi.getID()},
                {"po_id", po_id},
                {"pc_id", pc_id}
            }.dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "POST /semis error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });
        // DELETE /semis/{id}
    server.Delete(R"(/semis/(\d+))", [db](const httplib::Request& req, httplib::Response& res) {
        try {
            unsigned long semi_id = std::stoul(req.matches[1]);
            
            odb::transaction t(db->begin());
            
            // Проверяем существование
            std::unique_ptr<Semi> semi(db->load<Semi>(semi_id));
            if (!semi) {
                res.status = 404;
                res.set_content(R"({"error":"Semi not found"})", "application/json");
                return;
            }
            
            // Удаляем
            db->erase<Semi>(semi_id);
            
            t.commit();
            
            res.status = 200;
            res.set_content(R"({"success":true,"message":"Semi deleted"})", "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "DELETE /semis error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });
    
}
