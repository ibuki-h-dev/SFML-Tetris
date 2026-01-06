cmake_minimum_required(VERSION 3.16)
project(Tetris VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ソースファイル
# CMakeLists.txt の SOURCES セクションを修正
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Board.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Piece.cpp
)

add_executable(Tetris ${SOURCES})

# SFML 3.0を探す
# vcpkg経由の場合、CONFIGモードで探すのが一般的です
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System CONFIG)

target_link_libraries(Tetris PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

# アセットコピー
add_custom_command(
    TARGET Tetris POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:Tetris>/assets"
)

if (MSVC)
    target_compile_options(Tetris PRIVATE /W4)
else()
    target_compile_options(Tetris PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # ローカル開発用：絶対パスを渡すことで、どこから実行してもフォントが見つかる
    target_compile_definitions(Tetris PRIVATE ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
else()
    # 配布用：実行ファイルからの相対パスを渡す
    target_compile_definitions(Tetris PRIVATE ASSET_DIR="assets/")
endif()