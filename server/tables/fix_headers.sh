#!/bin/bash

# Функция для исправления одного файла
fix_file() {
    local file=$1
    
    # Создаем временный файл
    cp "$file" "${file}.tmp"
    
    # Исправляем pointer(std::shared_ptr) на pointer(std::shared_ptr<TYPE>)
    # Нужно ВРУЧНУЮ определить TYPE для каждого файла
    
    echo "Исправляем $file..."
}

# Исправляем каждый файл
for f in *.h; do
    if [[ "$f" != "all_classes.h" ]]; then
        echo "=== Исправление $f ==="
        
        # Определяем тип для pointer
        if [[ "$f" == "category.h" ]]; then
            sed -i 's/pointer(std::shared_ptr)/pointer(std::shared_ptr<PO>)/g' "$f"
        elif [[ "$f" == "department.h" ]]; then
            sed -i 's/pointer(std::shared_ptr)/pointer(std::shared_ptr<PC>)/g; s/pointer(std::shared_ptr)/pointer(std::shared_ptr<Users>)/g' "$f"
        elif [[ "$f" == "pc.h" ]]; then
            sed -i 's/pointer(std::shared_ptr)/pointer(std::shared_ptr<Department>)/g; s/pointer(std::shared_ptr)/pointer(std::shared_ptr<Semi>)/g' "$f"
        elif [[ "$f" == "po.h" ]]; then
            sed -i 's/pointer(std::shared_ptr)/pointer(std::shared_ptr<Category>)/g; s/pointer(std::shared_ptr)/pointer(std::shared_ptr<VersionPO>)/g; s/pointer(std::shared_ptr)/pointer(std::shared_ptr<Semi>)/g' "$f"
        elif [[ "$f" == "roles.h" ]]; then
            sed -i 's/pointer(std::shared_ptr)/pointer(std::shared_ptr<Users>)/g' "$f"
        elif [[ "$f" == "users.h" ]]; then
            sed -i 's/pointer(std::shared_ptr)/pointer(std::shared_ptr<Department>)/g; s/pointer(std::shared_ptr)/pointer(std::shared_ptr<Roles>)/g' "$f"
        elif [[ "$f" == "version_po.h" ]]; then
            sed -i 's/pointer(std::shared_ptr)/pointer(std::shared_ptr<PO>)/g' "$f"
        elif [[ "$f" == "semi.h" ]]; then
            sed -i 's/pointer(std::shared_ptr)/pointer(std::shared_ptr<PO>)/g; s/pointer(std::shared_ptr)/pointer(std::shared_ptr<PC>)/g' "$f"
        fi
        
        echo "✓ $f исправлен"
    fi
done

echo "Все файлы исправлены!"
