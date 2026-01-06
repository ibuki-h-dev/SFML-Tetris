cmake_minimum_required(VERSION 3.10)
project(TetrisWasm)

# Web（Emscripten）ビルド用の設定
if(EMSCRIPTEN)
    include(FetchContent)
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1  # Webで最も安定している2.6系を使います
    )
    # Web用に不要な機能をオフにする
    set(SFML_OS_EMSCRIPTEN TRUE CACHE BOOL "" FORCE)
    set(SFML_BUILD_AUDIO FALSE CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK FALSE CACHE BOOL "" FORCE)
    
    FetchContent_MakeAvailable(SFML)
else()
    # PCでビルドするとき用（今まで通り）
    find_package(SFML 3.0.2 COMPONENTS Graphics Window System REQUIRED)
endif()

# ソースファイルの指定（あなたのファイル名に合わせて調整してください）
add_executable(index 
    src/main.cpp 
    src/Board.cpp 
    src/Piece.cpp 
    src/Game.cpp
)

if(EMSCRIPTEN)
    # SFMLのリンク設定
    target_link_libraries(index PRIVATE sfml-graphics sfml-window sfml-system)
    
    # Emscripten専用の出力設定（HTMLを出す）
    set_target_properties(index PROPERTIES SUFFIX ".html")
    target_link_options(index PRIVATE 
        "-sALLOW_MEMORY_GROWTH=1"
        "-sUSE_PTHREADS=0"
        "--preload-file" "src/assets@src/assets"  # フォントなどの読み込み
        "-sSINGLE_FILE=1" # 全てを1つのHTMLにまとめる（公開が楽になります）
    )
else()
    target_link_libraries(index PRIVATE SFML::Graphics SFML::Window SFML::System)
endif()

target_include_directories(index PRIVATE src)