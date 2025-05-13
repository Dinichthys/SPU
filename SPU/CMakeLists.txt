add_executable (spu
    main_spu.cpp

    spu.cpp
)

target_link_libraries(spu
    libs
    flags_SPU

    sfml-graphics
    sfml-window
    sfml-system
)

target_include_directories (spu
    PRIVATE
        .
)

# cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++
# cmake --build build
