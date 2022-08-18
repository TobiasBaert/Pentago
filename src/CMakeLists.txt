target_sources(Pentago PRIVATE
        BitBoardRepresentation.cpp
        BitBoardRepresentation.h
        EnumBoard.cpp
        EnumBoard.h
        Game.cpp
        Game.h
        IBoard.cpp
        IBoard.h
        RoundedRectangleShape.hpp
        Utilities.h
        IState.h
        PlacementState.cpp
        PlacementState.h
        RotationState.cpp
        RotationState.h)

target_include_directories(Pentago PRIVATE
        )