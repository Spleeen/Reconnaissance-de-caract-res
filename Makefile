# Makefile created by N.Richard
# Cours : Intelligence Artificielle
# Projet : Reconnaissance de caractères via un réseau de Hopfield
# Date de création : 4 janvier 2006
# Date de version : 5 mai 2014
# Version 1.8

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
CXXFLAGS += -DNDEBUG -fopenmp -O3 -Wall -Wno-narrowing -pedantic -Wextra -Woverloaded-virtual -Wfloat-equal -Wpointer-arith -Wredundant-decls -Winit-self -Wswitch-default -Wundef -Wlong-long -Werror -Wconversion -Wextra -std=c++11 #-Weffc++ # -Wshadow 
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
