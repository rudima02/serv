#!/bin/bash
# test.sh

echo "=== Тестирование всех API эндпоинтов ==="
echo ""

# 1. Категории
echo "1. Категории:"
echo "GET /categories:"
curl -s http://localhost:8080/categories | python3 -m json.tool
echo ""

# 2. Отделы
echo "2. Отделы:"
echo "GET /departments:"
curl -s http://localhost:8080/departments | python3 -m json.tool
echo ""

# 3. Роли
echo "3. Роли:"
echo "GET /roles:"
curl -s http://localhost:8080/roles | python3 -m json.tool
echo ""

# 4. ПК
echo "4. ПК:"
echo "GET /pcs:"
curl -s http://localhost:8080/pcs | python3 -m json.tool
echo ""

# 5. ПО
echo "5. ПО:"
echo "GET /pos:"
curl -s http://localhost:8080/pos | python3 -m json.tool
echo ""

# 6. Пользователи
echo "6. Пользователи:"
echo "GET /users:"
curl -s http://localhost:8080/users | python3 -m json.tool
echo ""

# 7. Связи
echo "7. Связи:"
echo "GET /semis:"
curl -s http://localhost:8080/semis | python3 -m json.tool
echo ""

# 8. Версии ПО
echo "8. Версии ПО:"
echo "GET /versionpos:"
curl -s http://localhost:8080/versionpos | python3 -m json.tool
echo ""

echo "=== Тестирование завершено ==="
