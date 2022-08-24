target_sources(Pentago PRIVATE
        BoardRepresentation/BitBoardRepresentation.cpp
        BoardRepresentation/BitBoardRepresentation.h
        BoardRepresentation/EnumBoard.cpp
        BoardRepresentation/EnumBoard.h
        BoardRepresentation/IBoard.cpp
        BoardRepresentation/IBoard.h
        GameStates/IState.cpp
        GameStates/IState.h
        GameStates/PlacementState.cpp
        GameStates/PlacementState.h
        GameStates/RotationState.cpp
        GameStates/RotationState.h
        Game.cpp
        Game.h
        RoundedRectangleShape.hpp
        Utilities.h)

target_include_directories(Pentago PRIVATE
        BoardRepresentation
        GameStates)