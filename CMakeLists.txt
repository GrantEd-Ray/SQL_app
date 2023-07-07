cmake_minimum_required(VERSION 3.24)
project(SQLite_app)

set(CMAKE_CXX_STANDARD 17)

include_directories("/ext/")

add_executable(SQLite_app main.cpp ext/sqlite3/sqlite3.c)

file(COPY dt.db DESTINATION "/")
