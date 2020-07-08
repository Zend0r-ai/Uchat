NAME	=	server

CFLG	=	-std=c11 -lsqlite3 -Wall -W -Wextra -ansi -pedantic -o uchat_server

SRCD	=	src
INCD	=	inc
OBJD	=	obj

INC		=	server.h
INCS	=	$(addprefix $(INCD)/, $(INC))

SRC		= main.c

SRCS	=	$(addprefix $(SRCD)/, $(SRC))
OBJS	=	$(addprefix $(OBJD)/, $(SRC:%.c=%.o))

all:
	clang $(CFLG) src/*.c -I inc/ -I
