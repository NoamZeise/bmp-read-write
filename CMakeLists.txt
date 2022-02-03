cmake_minimum_required(VERSION 3.10)
set(ProjectName bmprw)

# set the project name
project(${ProjectName} C)


file(GLOB SOURCES src/*)

add_executable(${ProjectName} ${SOURCES})

target_link_libraries(${ProjectName} m) #link C math library


target_include_directories(${ProjectName} PUBLIC
                           ${PROJECT_BINARY_DIR}
                           )
