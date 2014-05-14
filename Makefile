# Makefile created by N.Richard
# Cours : Intelligence Artificielle
# Projet : Reconnaissance de caractères via un réseau de Hopfield
# Date de création : 4 janvier 2006
# Date de version : 10 mai 2014
# Version 2.1

CC := g++	
PREFIX := .
SRCDIR := $(PREFIX)/src
INCDIR := $(PREFIX)/lib
#un répertoire pour le binaire n'est pas intéressant car les chemins (relatif) posent souvent problèmes
EXEC := carac_recognize
SRC := $(wildcard $(SRCDIR)/*.cpp)
OBJ := $(SRC:.cpp=.o)
DEPENDS := $(SRC:.cpp=.d)
#NDEBUG = RELEASE
#-Wfatal-errors : le compilateur s'arrêtera à la 1ère erreur rencontrée (ici un simple warning)
CXX_RELEASE_FLAGS := -O3 -pipe -fopenmp -DNDEBUG -march=native -fstack-protector --param=ssp-buffer-size=4 
CXX_DEBUG_FLAGS := -O3 -pipe -fopenmp -fdiagnostics-color=auto -pedantic -Wall -Wno-narrowing -Wextra -Woverloaded-virtual \
	-Wwrite-strings -Wno-variadic-macros -Wno-unused-parameter -Wvolatile-register-var -Wunsafe-loop-optimizations -Wcast-qual \
	-Wunknown-pragmas -Wmissing-include-dirs -Winline -Wstack-protector -Wfloat-equal -Wstrict-null-sentinel -Winvalid-pch \
	-Wpointer-arith -Wredundant-decls -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlong-long -Werror -Wconversion \
	-std=c++11 -Weffc++ -Wold-style-cast -Wcast-align -Wdouble-promotion -Wlogical-op -Wfatal-errors

#CXXFLAGS += 
#CFLAGS += -O3 -pedantic -Wall -Wextra -fopenmp $(shell pkg-config --cflags sdl SDL_image) -lSDL_image
LDFLAGS += -fopenmp #-lm -lpthread -D_REENTRANT $(shell pkg-config --libs sdl) -lSDL_image 

.PHONY : all debug release run clean mrproper

all: release

debug: CXXFLAGS += $(CXX_DEBUG_FLAGS) 
debug: $(EXEC)

release: CXXFLAGS += $(CXX_RELEASE_FLAGS) 
release: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS) 
	@printf " \033[1;32mCompilation success !\033[0m\n"

$(DEPENDS): $(SRC)
	@$(CC) -MM $< >> $@ 

-include $(DEPENDS)

run: 
	@$(MAKE) && ./$(EXEC)

clean:
	@printf "  \033[1;31m\033[4mRemoved files\033[0m\033[31m :\n"
	@printf "\t*~ a.out core *.d \n\t$(OBJ)\033[0m\n"
	@$(RM) -rf *~ a.out core $(DEPENDS) $(OBJ)

mrproper: clean
	@printf "\033[31m\t$(EXEC)\033[0m\n"
	@$(RM) -rf $(EXEC)
