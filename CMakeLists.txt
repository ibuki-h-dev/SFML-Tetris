cmake_minimum_required(VERSION 3.10)
project(TetrisWasm)
if(EMSCRIPTEN)
    # Actionsでダウンロード済みのソースがある場合、それを使う
    if(EXISTS "${SFML_SOURCE_DIR}/CMakeLists.txt")
        add_subdirectory("${SFML_SOURCE_DIR}" sfml)
    else()
        # ローカル等でソースがない場合のフォールバック（本家URL）
        include(FetchContent)
        FetchContent_Declare(
            SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 3.0.0
        )
        FetchContent_MakeAvailable(SFML)
    endif()
    
    # Emscripten用のビルド設定
    set(SFML_OS_EMSCRIPTEN TRUE CACHE BOOL "" FORCE)
    set(SFML_BUILD_AUDIO FALSE CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK FALSE CACHE BOOL "" FORCE)
else()
    # PC用
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
        "--preload-file" "${CMAKE_CURRENT_SOURCE_DIR}/src/assets@assets"
        "-sSINGLE_FILE=1"
    )
else()
    # PC版のリンク
    target_link_libraries(index PRIVATE SFML::Graphics SFML::Window SFML::System)
    
    # 【自動コピー設定】ビルド成功時にassetsフォルダをDebugフォルダへコピーする
    add_custom_command(
        TARGET index POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:index>/assets"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/src/assets"
        "$<TARGET_FILE_DIR:index>/assets"
        COMMENT "Copying assets to build directory..."
    )
endif()

target_include_directories(index PRIVATE src)