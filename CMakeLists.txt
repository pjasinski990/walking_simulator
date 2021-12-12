CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project("game")
set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp)

# mrld
set(MRLD_BASE ../mrld_engine)

list(APPEND SOURCES src/game.cpp)
list(APPEND SOURCES src/game_loop.cpp)
list(APPEND SOURCES src/world.cpp)
list(APPEND SOURCES src/player.cpp)
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

include_directories(${PROJECT_NAME} PRIVATE ${MRLD_BASE}/include)
add_subdirectory(${MRLD_BASE} mrld)

target_link_libraries(${PROJECT_NAME} mrld)
