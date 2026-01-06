cmake_minimum_required(VERSION 3.10)
project(TetrisWasm)

if(EMSCRIPTEN)
    include(FetchContent)
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY "https://github.com/Blaatand213/sfml-wasm.git"
        GIT_TAG master
        GIT_SHALLOW TRUE
    )
    
    set(SFML_OS_EMSCRIPTEN TRUE CACHE BOOL "" FORCE)
    set(SFML_BUILD_AUDIO FALSE CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK FALSE CACHE BOOL "" FORCE)
    set(SFML_BUILD_EXAMPLES FALSE CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(SFML)
else()
    # PC用（SFML 3.xを想定）
    find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)
endif()

# 実行ファイルの設定
add_executable(index 
    src/main.cpp 
    src/Board.cpp 
    src/Piece.cpp 
    src/Game.cpp
)

if(EMSCRIPTEN)
    target_link_libraries(index PRIVATE sfml-graphics sfml-window sfml-system)
    set_target_properties(index PROPERTIES SUFFIX ".html")
    target_link_options(index PRIVATE 
        "-sALLOW_MEMORY_GROWTH=1"
        "-sUSE_PTHREADS=0"
        # Web上では "assets/font.ttf" として読み込めるように設定
        "--preload-file" "${CMAKE_CURRENT_SOURCE_DIR}/src/assets@assets"
        "-sSINGLE_FILE=1"
    )
else()
    target_link_libraries(index PRIVATE SFML::Graphics SFML::Window SFML::System)
    
    # よりシンプルで確実なコピー命令
    add_custom_command(
        TARGET index POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:index>/assets"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/src/assets"
        "$<TARGET_FILE_DIR:index>/assets"
        COMMENT "Copying assets..."
    )
endif()

target_include_directories(index PRIVATE src)