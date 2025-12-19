#!/bin/bash
# create.sh

echo "=== Создание тестовых данных ==="

# 1. Уже есть категории (id: 1,2,3)
# 2. Уже есть отделы (id: 1,2,3,4) 
# 3. Уже есть роли (id: 1,2)

# 4. Создаем ПК (нужен department_id)
echo "Создаем ПК..."
curl -X POST http://localhost:8080/pcs \
  -H "Content-Type: application/json" \
  -d '{"name_pc": "WS-001", "department_id": 1}' | python3 -m json.tool

curl -X POST http://localhost:8080/pcs \
  -H "Content-Type: application/json" \
  -d '{"name_pc": "WS-002", "department_id": 2}' | python3 -m json.tool

# 5. Создаем ПО (нужен category_id)
echo -e "\nСоздаем ПО..."
curl -X POST http://localhost:8080/pos \
  -H "Content-Type: application/json" \
  -d '{"name_po": "Windows 10", "category_id": 1}' | python3 -m json.tool

curl -X POST http://localhost:8080/pos \
  -H "Content-Type: application/json" \
  -d '{"name_po": "Microsoft Office", "category_id": 2}' | python3 -m json.tool

curl -X POST http://localhost:8080/pos \
  -H "Content-Type: application/json" \
  -d '{"name_po": "1C Бухгалтерия", "category_id": 3}' | python3 -m json.tool

# 6. Создаем пользователей (нужны department_id и role_id)
echo -e "\nСоздаем пользователей..."
curl -X POST http://localhost:8080/users \
  -H "Content-Type: application/json" \
  -d '{"name_user": "Иван Иванов", "department_id": 1, "role_id": 1}' | python3 -m json.tool

curl -X POST http://localhost:8080/users \
  -H "Content-Type: application/json" \
  -d '{"name_user": "Петр Петров", "department_id": 2, "role_id": 2}' | python3 -m json.tool

# 7. Создаем связи ПО-ПК (нужны po_id и pc_id)
echo -e "\nСоздаем связи ПО-ПК..."
curl -X POST http://localhost:8080/semis \
  -H "Content-Type: application/json" \
  -d '{"po_id": 1, "pc_id": 1}' | python3 -m json.tool

curl -X POST http://localhost:8080/semis \
  -H "Content-Type: application/json" \
  -d '{"po_id": 2, "pc_id": 2}' | python3 -m json.tool

# 8. Создаем версии ПО (нужен po_id)
echo -e "\nСоздаем версии ПО..."
curl -X POST http://localhost:8080/versionpos \
  -H "Content-Type: application/json" \
  -d '{"version": "21H2", "date_version": "2023-01-15", "po_id": 1}' | python3 -m json.tool

curl -X POST http://localhost:8080/versionpos \
  -H "Content-Type: application/json" \
  -d '{"version": "2021", "date_version": "2023-06-20", "po_id": 2}' | python3 -m json.tool

echo -e "\n=== Данные созданы ==="
