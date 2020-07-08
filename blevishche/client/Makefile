NAME	=	uchat

CFLG	=	`pkg-config --cflags --libs gtk+-3.0` -std=c11 -lsqlite3 -Wall -W -Wextra -ansi -pedantic -o uchat

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