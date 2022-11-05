cmake_minimum_required(VERSION 3.8.0)
project (ingest-data)
add_executable(main main.cpp)

# RESTinio dependencies:
# 1. ASIO or Boost::ASIO (goes as headers, vcpkg knows where)
# 2. HTTP parser
find_package(unofficial-http-parser REQUIRED)
# 3. fmtlib
find_package(fmt REQUIRED)
# RESTinio itself
find_package(restinio REQUIRED)

target_link_libraries(main mysqlcppconn)
target_link_libraries(main pthread)
# Make your project dependent on restinio,
# and let cmake deal with all the headers paths and linked libs.
target_link_libraries(main restinio::restinio)

