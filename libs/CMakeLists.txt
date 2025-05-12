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

target_compile_features (libs
    STATIC
        cxx_std_17
)

target_compile_options (libs
    STATIC
        -fdiagnostics-color=always

        -Wall
        -Wextra
        -fstack-protector-strong
        -fcheck-new
        -fstrict-overflow

        -march=native

        -g

        $<$<CONFIG:Debug>:
            -Og
            -g3
            -DDEBUG
            -ggdb
            -fsanitize=address,leak,undefined
        >

        $<$<CONFIG:Release>:
            -O2
            -flto
            -DNDEBUG
        >
)

target_link_options (libs
    STATIC
        -march=native

        -Wl,-q
        -g

        $<$<CONFIG:Debug>:
            -Og
            -g3
            -DDEBUG
            -ggdb
            -fsanitize=address,leak,undefined
        >

        $<$<CONFIG:Release>:
            -O2
            -flto
            -DNDEBUG
        >
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # to generate compile_commands.json

# cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++
# cmake --build build
