add_executable (disassembler
    main_disassembler.cpp

    disassembler.cpp
)

target_link_libraries(disassembler
    libs
    flags_SPU
)

target_include_directories (disassembler
    PRIVATE
        .
)

# cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++
# cmake --build build
