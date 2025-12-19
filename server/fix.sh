#!/bin/bash
# fix_routes.sh - исправляем все db->load вызовы

echo "Исправление маршрутов..."

# 1. pc_routes.cpp
sed -i 's/std::shared_ptr<Department> department = db->load<Department>/Department* dept_ptr = db->load<Department>/g' routes/pc_routes.cpp
sed -i 's/if (!department)/if (!dept_ptr)/g' routes/pc_routes.cpp
sed -i '/if (!dept_ptr)/a\            std::shared_ptr<Department> department(dept_ptr);' routes/pc_routes.cpp

# 2. po_routes.cpp  
sed -i 's/std::shared_ptr<Category> category = db->load<Category>/Category* cat_ptr = db->load<Category>/g' routes/po_routes.cpp
sed -i 's/if (!category)/if (!cat_ptr)/g' routes/po_routes.cpp
sed -i '/if (!cat_ptr)/a\            std::shared_ptr<Category> category(cat_ptr);' routes/po_routes.cpp

# 3. semi_routes.cpp
sed -i 's/std::shared_ptr<PO> po = db->load<PO>/PO* po_ptr = db->load<PO>/g' routes/semi_routes.cpp
sed -i 's/std::shared_ptr<PC> pc = db->load<PC>/PC* pc_ptr = db->load<PC>/g' routes/semi_routes.cpp
sed -i 's/if (!po || !pc)/if (!po_ptr || !pc_ptr)/g' routes/semi_routes.cpp
sed -i '/if (!po_ptr || !pc_ptr)/a\            std::shared_ptr<PO> po(po_ptr);\n            std::shared_ptr<PC> pc(pc_ptr);' routes/semi_routes.cpp

# 4. versionpo_routes.cpp
sed -i 's/std::shared_ptr<PO> po = db->load<PO>/PO* po_ptr = db->load<PO>/g' routes/versionpo_routes.cpp
sed -i 's/if (!po)/if (!po_ptr)/g' routes/versionpo_routes.cpp
sed -i '/if (!po_ptr)/a\            std::shared_ptr<PO> po(po_ptr);' routes/versionpo_routes.cpp

echo "Готово!"
