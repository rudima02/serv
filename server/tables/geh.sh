#!/bin/bash

# Путь к odb
ODB=odb

# Стандарт C++
STD=c++11

# База данных
DB=pgsql   # <-- PostgreSQL

# Все заголовочные файлы
FILES="category.h department.h pc.h po.h semi.h users.h roles.h version_po.h"

for f in $FILES; do
    echo "Generating ODB for $f..."
    $ODB --database $DB --std $STD --generate-query --generate-schema --generate-session --generate-prepared $f
done

echo "ODB generation done."
