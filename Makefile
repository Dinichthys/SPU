CXXFLAGS = -D _DEBUG  -ggdb -g3 -D_FORTIFY_SOURCES=3 -std=c++17 -Og -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=81920 -Wstack-usage=81920 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

CXX = g++

WARNINGS = -Wall -Wextra
# -Wswitch-enum
ifeq ($(CXX), clang++)
  WARNINGS += -pedantic -Wconversion -Wdangling -Wdeprecated -Wdocumentation -Wformat -Wfortify-source -Wgcc-compat -Wgnu -Wignored-attributes -Wignored-pragmas -Wimplicit -Wmost -Wshadow-all -Wthread-safety -Wuninitialized -Wunused -Wformat
  WARNINGS += -Wargument-outside-range -Wassign-enum -Wbitwise-instead-of-logical -Wc23-extensions -Wc11-extensions -Wcast-align -Wcast-function-type -Wcast-qual -Wcomma -Wcomment -Wcompound-token-split -Wconditional-uninitialized -Wduplicate-decl-specifier -Wduplicate-enum -Wduplicate-method-arg -Wduplicate-method-match -Wempty-body -Wempty-init-stmt -Wenum-compare -Wenum-constexpr-conversion -Wextra-tokens -Wfixed-enum-extension -Wfloat-equal -Wloop-analysis -Wframe-address -Wheader-guard -Winfinite-recursion -Wno-gnu-binary-literal -Wint-conversion -Wint-in-bool-context -Wmain -Wmisleading-indentation -Wmissing-braces -Wmissing-prototypes -Wover-aligned -Wundef -Wvla
endif
ifeq ($(CXX), cc)
  WARNINGS += -pedantic -Waggressive-loop-optimizations -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wswitch-default -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -Wstack-usage=8192 -Wstack-protector
endif
ifeq ($(CXX), g++)
  WARNINGS += -pedantic -Waggressive-loop-optimizations -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wswitch-default -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -Wstack-usage=8192 -Wstack-protector
endif


all: compiler processor disassembler

stack_rebuild: clean stack

debug: CXXFLAGS+=-D DEBUG -D CANARY_PROT -D HASH_PROT -lm
debug: stack processor

debug_stack: CXXFLAGS+=-D DEBUG -D CANARY_PROT -D HASH_PROT -lm
debug_stack: stack

release_stack: CXXFLAGS= -D NDEBUG -lm
release_stack: stack

compiler: build my_stdio.o main_assembler.o assembler.o logging.o print_error.o
	@$(CXX) $(CXXFLAGS) build/main_assembler.o build/assembler.o build/my_stdio.o build/logging.o build/print_error.o -o compiler

processor: build stack main_spu.o spu.o logging.o print_error.o
	@$(CXX) $(CXXFLAGS) $(SFMLFLAGS) build/main_spu.o build/spu.o build/stack.o build/hash.o build/logging.o build/print_error.o -o processor

disassembler: build my_stdio.o main_disassembler.o disassembler.o logging.o print_error.o
	@$(CXX) $(CXXFLAGS) build/my_stdio.o build/main_disassembler.o build/disassembler.o build/logging.o build/print_error.o -o disassembler

build:
	mkdir build

stack: stack.o hash.o
	@

stack.o: Stack/stack.cpp
	@$(CXX) $(CXXFLAGS) -c Stack/stack.cpp -o build/stack.o

hash.o: Stack/hash.cpp
	@$(CXX) $(CXXFLAGS) -c Stack/hash.cpp -o build/hash.o


my_stdio.o: My_lib/My_stdio/my_stdio.cpp
	@$(CXX) $(CXXFLAGS) -c My_lib/My_stdio/my_stdio.cpp -o build/my_stdio.o


main_assembler.o: Assembler/main_assembler.cpp
	@$(CXX) $(CXXFLAGS) -c Assembler/main_assembler.cpp -o build/main_assembler.o

assembler.o: Assembler/assembler.cpp
	@$(CXX) $(CXXFLAGS) -c Assembler/assembler.cpp -o build/assembler.o


main_spu.o: SPU/main_spu.cpp
	@$(CXX) $(CXXFLAGS) -c SPU/main_spu.cpp -o build/main_spu.o

spu.o: SPU/spu.cpp
	@$(CXX) $(CXXFLAGS) -c SPU/spu.cpp -o build/spu.o


main_disassembler.o: Disassembler/main_disassembler.cpp
	@$(CXX) $(CXXFLAGS) -c Disassembler/main_disassembler.cpp -o build/main_disassembler.o

disassembler.o: Disassembler/disassembler.cpp
	@$(CXX) $(CXXFLAGS) -c Disassembler/disassembler.cpp -o build/disassembler.o


logging.o: My_lib/Logger/logging.cpp
	@$(CXX) $(CXXFLAGS) -c My_lib/Logger/logging.cpp -o build/logging.o

print_error.o: My_lib/Assert/print_error.cpp
	@$(CXX) $(CXXFLAGS) -c My_lib/Assert/print_error.cpp -o build/print_error.o

clean: rmdir_build
	rm disassembler compiler processor

rmdir_build: clean_build
	rmdir build

clean_build:
	rm build/*

clean_compiler:
	rm build/main_assembler.o build/assembler.o compiler

clean_processor:
	rm build/main_spu.o build/spu.o processor

clean_disassembler:
	rm build/main_disassembler.o build/disassembler.o disassembler

clean_stack:
	rm hash.o stack.o

