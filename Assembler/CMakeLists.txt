add_executable (assembler
    main_assembler.cpp

    assembler.cpp
)

target_link_libraries(assembler
    libs
    flags_SPU
)

target_include_directories (assembler
    PRIVATE
        .
)

# cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++
# cmake --build build
