#include "update_po_count_routes.h"
#include "../tables/dto/update_po_countDTO.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

void registerUpdatePOCountRoutes(
    httplib::Server& server,
    std::shared_ptr<odb::pgsql::database> db
) {
    // GET /update_po_count - получить список всех ПО для выбора
    server.Get("/update_po_count", [db](const httplib::Request&, httplib::Response& res) {
        try {
            odb::transaction t(db->begin());
            odb::result<PO> pos = db->query<PO>();

            std::vector<UpdatePOCountDTO> dtos;
            for (const auto& po : pos) {
                UpdatePOCountDTO dto;
                dto.po_id = po.getID();
                dto.po_name = po.getName();
                dtos.push_back(dto);
            }

            t.commit();
            res.status = 200;
            res.set_content(json(dtos).dump(), "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "GET /update_po_count error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });

    // POST /update_po_count - изменить количество ПК с выбранным ПО
    server.Post("/update_po_count", [db](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            unsigned long po_id = j["po_id"].get<unsigned long>();
            unsigned long new_count = j["count"].get<unsigned long>();

            odb::transaction t(db->begin());

            // Проверяем существование PO
            odb::result<PO> pos = db->query<PO>(odb::query<PO>::id == po_id);
            if (pos.empty()) {
                res.status = 404;
                res.set_content(R"({"error":"PO not found"})", "application/json");
                t.rollback();
                return;
            }

            // Получаем все ПК и отделы
            odb::result<Department> depts = db->query<Department>();

            for (const auto& dept : depts) {
                odb::result<PC> pcs = db->query<PC>(
                    odb::query<PC>::department->id == dept.getID()
                );

                unsigned long current_count = 0;
                for (auto& pc : pcs) {
                    odb::result<Semi> semis = db->query<Semi>(
                        odb::query<Semi>::pc->id == pc.getID() &&
                        odb::query<Semi>::po->id == po_id
                    );
                    current_count += semis.size();
                }

                if (current_count < new_count) {
                    // добавляем недостающие Semi
                    unsigned long to_add = new_count - current_count;
                    auto pc_it = pcs.begin();
                    for (unsigned long i = 0; i < to_add && pc_it != pcs.end(); ++i, ++pc_it) {
                        Semi s(std::make_shared<PO>(*pos.begin()), std::make_shared<PC>(*pc_it));
                        db->persist(s);
                    }
                }
                else if (current_count > new_count) {
                    // удаляем лишние Semi
                    unsigned long to_remove = current_count - new_count;
                    for (auto& pc : pcs) {
                        odb::result<Semi> semis = db->query<Semi>(
                            odb::query<Semi>::pc->id == pc.getID() &&
                            odb::query<Semi>::po->id == po_id
                        );

                        auto it = semis.begin();
                        for (unsigned long i = 0; i < to_remove && it != semis.end(); ++i) {
                            db->erase<Semi>(it->getID());
                            ++it;
                        }
                        to_remove = (to_remove > semis.size()) ? to_remove - semis.size() : 0;
                        if (to_remove == 0) break;
                    }
                }
            }

            t.commit();
            res.status = 200;
            res.set_content(R"({"status":"updated"})", "application/json");
        }
        catch (const std::exception& e) {
            std::cerr << "POST /update_po_count error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Database error"})", "application/json");
        }
    });

    // GET /po_counts/:po_id - количество ПК с этим ПО по отделам
server.Get(R"(/po_counts/(\d+))", [db](const httplib::Request& req, httplib::Response& res) {
    try {
        unsigned long po_id = std::stoul(req.matches[1]);

        odb::transaction t(db->begin());

        // Проверяем существование ПО
        odb::result<PO> pos = db->query<PO>(odb::query<PO>::id == po_id);
        if (pos.empty()) {
            res.status = 404;
            res.set_content(R"({"error":"PO not found"})", "application/json");
            t.rollback();
            return;
        }

        odb::result<Department> depts = db->query<Department>();
        json result = json::array();

        for (const auto& dept : depts) {
            odb::result<PC> pcs = db->query<PC>(odb::query<PC>::department->id == dept.getID());
            unsigned long count = 0;

            for (const auto& pc : pcs) {
                odb::result<Semi> semis = db->query<Semi>(
                    odb::query<Semi>::pc->id == pc.getID() &&
                    odb::query<Semi>::po->id == po_id
                );
                count += semis.size();
            }

            json entry;
            entry["department_id"] = dept.getID();
            entry["department_name"] = dept.getName();
            entry["pc_count"] = count;
            result.push_back(entry);
        }

        t.commit();
        res.status = 200;
        res.set_content(result.dump(), "application/json");
    }
    catch (const std::exception& e) {
        std::cerr << "GET /po_counts error: " << e.what() << std::endl;
        res.status = 500;
        res.set_content(R"({"error":"Database error"})", "application/json");
    }
});

}
