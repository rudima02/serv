CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Itables -Itables/dto -Itables/orm
LDFLAGS = -lodb -lodb-pgsql -lpq -pthread

# Основные исходники
MAIN_SRCS = main.cpp

# Маршруты
ROUTE_SRCS = $(wildcard routes/*.cpp)

# ODB сгенерированные файлы
ORM_SRCS = $(wildcard tables/orm/*.cxx)

# Все исходники
ALL_SRCS = $(MAIN_SRCS) $(ROUTE_SRCS) $(ORM_SRCS)

# Объектные файлы
OBJS = $(ALL_SRCS:.cpp=.o)
OBJS := $(OBJS:.cxx=.o)

all: server

server: $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o server

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) server1

run: server1
	./server1

test: server1
	curl http://localhost:8080

watch:
	while true; do \
		inotifywait -e modify -r --include='\.(cpp|h|cxx)$' .; \
		make; \
		echo "Rebuilt at $$(date)"; \
	done

.PHONY: all clean run test watch
