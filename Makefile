# Makefile created by N.Richard
# Cours : Intelligence Artificielle
# Projet : Reconnaissance de caractères via un réseau de Hopfield
# Date de création : 4 janvier 2006
# Date de version : 8 mai 2014
# Version 1.9

CC=g++	
PREFIX = .
SRCDIR = $(PREFIX)/src
INCDIR = $(PREFIX)
#un répertoire pour le binaire n'est pas intéressant car les chemins (relatif) posent souvent problèmes
EXEC = carac_recognize
SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)
DEPENDS = $(SRC:.cpp=.d)

#CFLAGS += `pkg-config --cflags sdl SDL_image` -fopenmp -O3 -pedantic -Wall -Wextra
#NDEBUG = RELEASE
#-Wfatal-errors : le compilateur s'arrêtera à la 1ère erreur rencontrée (ici un simple warning)
CXXFLAGS += -O3 -fopenmp -DNDEBUG -fdiagnostics-color=auto -pedantic -Wall -Wno-narrowing -Wextra -Woverloaded-virtual \
-Wwrite-strings -Wno-variadic-macros -Wno-unused-parameter -Wvolatile-register-var -Wunsafe-loop-optimizations -Wcast-qual \
-Wunknown-pragmas -Wmissing-include-dirs -Winline -Wstack-protector -Wfloat-equal -Wstrict-null-sentinel -Winvalid-pch \
-Wpointer-arith -Wredundant-decls -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlong-long -Werror -Wconversion \
-std=c++11 -Weffc++ -Wold-style-cast -Wcast-align -Wdouble-promotion -Wlogical-op -Wfatal-errors
LDFLAGS += -fopenmp #-lm -lpthread -D_REENTRANT

.PHONY : run clean mrproper

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS) 
	@printf " \033[1;32mCompilation success !\033[0m\n"

%.d: %.cpp
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
