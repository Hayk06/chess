cmake_minimum_required(VERSION 3.20)

project(chess LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(APP_ICON ${CMAKE_CURRENT_SOURCE_DIR}/resources/chess.icns)
set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

add_executable(chess MACOSX_BUNDLE
        ${APP_ICON}
        main.cpp
        piece/PieceType.h
        shared/Color.h
        piece/Piece.h
        piece/Piece.cpp
        board/Board.cpp
        board/Board.h
        board/Cell.h
        board/BoardNameMap.h
        pieces/Pawn.h
        pieces/Pawn.cpp
        pieces/Rook.h
        pieces/Rook.cpp
        pieces/Knight.h
        pieces/Knight.cpp
        pieces/Bishop.h
        pieces/Bishop.cpp
        pieces/Queen.h
        pieces/Queen.cpp
        pieces/King.h
        pieces/King.cpp
        game/Game.cpp
        game/Timer.h
        game/Side.h
        game/Side.cpp
        game/Game.h
        board/Position.h
        ui/MainWindow.cpp
        ui/MainWindow.h
        ui/GameView.cpp
        ui/GameView.h
        ui/PlayerPanel.h
        ui/MoveList.h
        ui/MoveList.cpp
        ui/GameOverDialog.cpp
        ui/GameOverDialog.h
        ui/GamePage.cpp
        ui/GamePage.h
        ui/PromotionDialog.cpp
        ui/PromotionDialog.h
        pieces/PieceFactory.cpp
        pieces/PieceFactory.h
        ui/PlayerPanel.cpp
        game/MoveHistory.h
)

set_target_properties(chess PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "Chess"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.haykderdzyan.chess"
        MACOSX_BUNDLE_ICON_FILE "chess.icns"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
        MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
)

target_link_libraries(chess PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)
