add_library (libs
    STATIC

    MyLib/Assert/print_error.cpp
    MyLib/Logger/logging.cpp
    MyLib/My_stdio/my_stdio.cpp

    Stack/stack.cpp
    Stack/hash.cpp
)

target_include_directories (libs
    PUBLIC
        .
        Stack
        MyLib
        MyLib/Assert
        MyLib/Logger
        MyLib/My_stdio
)

target_link_libraries(libs
    flags_SPU
)

# cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++
# cmake --build build
