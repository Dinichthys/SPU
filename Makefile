CXXFLAGS = -D _DEBUG  -ggdb -g3 -D_FORTIFY_SOURCES=3 -std=c++17 -Og -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=81920 -Wstack-usage=81920 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

all: compiler processor

stack_rebuild: clean stack

debug: CXXFLAGS+=-D DEBUG -D CANARY_PROT -D HASH_PROT
debug: stack processor

debug_stack: CXXFLAGS+=-D DEBUG -D CANARY_PROT -D HASH_PROT
debug_stack: stack

release_stack: CXXFLAGS= -D NDEBUG -lm
release_stack: stack

compiler: assembler.o
	@g++ $(CXXFLAGS) assembler.o -o compiler

processor: stack spu.o
	@g++ $(CXXFLAGS) spu.o stack.o hash.o -o processor

stack: main.o stack.o hash.o
	@g++ $(CXXFLAGS) main.o hash.o stack.o -o stack

main.o: main.cpp
	@g++ $(CXXFLAGS) -c main.cpp

stack.o: Stack/stack.cpp
	@g++ $(CXXFLAGS) -c Stack/stack.cpp

hash.o: Stack/hash.cpp
	@g++ $(CXXFLAGS) -c Stack/hash.cpp

assembler.o: assembler.cpp
	@g++ $(CXXFLAGS) -c assembler.cpp

spu.o: spu.cpp
	@g++ $(CXXFLAGS) -c spu.cpp

clean: clean_compiler clean_processor
	rm -rf *.o

clean_compiler:
	rm assembler.o compiler

clean_processor:
	rm spu.o processor

clean_stack:
	rm hash.o stack.o stack
