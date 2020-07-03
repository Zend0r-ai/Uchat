NAME = uchat

INC = client.h

FILE = *

FINC = $(addprefix inc/, $(INC))

SRC = $(addsuffix .c, $(FILE))

FSRC = $(addprefix src/, $(SRC)) 

OBJ = $(addsuffix .o, $(FILE))

FOBJ = $(addprefix obj/, $(OBJ))

CFLAG = -std=c11 -Wall -Wextra -Wpedantic

OFLAG = $(CFLAG) -c $(SRC)

BFLAG = $(CFLAG) $(FOBJ) -o $(NAME)

all: install clean

install:
	@cp $(FSRC) .
	@cp $(FINC) .
	@clang $(OFLAG) $(INC)
	@mkdir -p obj
	@mv $(OBJ) ./obj
	@clang $(BFLAG)

clean:
	@rm -rf $(INC)
	@rm -rf $(SRC)
	@rm -rf $(addsuffix .gch, $(FINC))
	@rm -rf $(addsuffix .gch, $(INC))

uninstall : clean
	@rm -rf $(NAME)