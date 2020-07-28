NAME	=	uchat

CFLG	=	-std=c11 $(addprefix -W, all extra error pedantic) -g

SRCD	=	src
INCD	=	inc
OBJD	=	obj
GLDD	=	gld

SDIR	=	uchat.dSYM


INC		=	client.h
INCS	=	$(addprefix $(INCD)/, $(INC))

SRC		=	add_message.c chat.c delete.c edit.c extra.c free.c gtk_extra.c \
			gtk_history.c history.c login.c main.c mx_report_tls.c history_2.c\
			reconnecting.c server_handler.c sign_up.c start.c style.c


GLD 	= 	start.glade\
			login.glade\
			sign_up.glade\
			chat.glade\

SGLD	=	$(addprefix $(GLDD)/, $(GLD))

SRCS	=	$(addprefix $(SRCD)/, $(SRC))

OBJS	=	$(addprefix $(OBJD)/, $(SRC:%.c=%.o))

JSONA = 	../libjson/libjsonc.a
JSOND = 	../libjson
JSONI = 	../libjson/inc

LIBMXA = 	../libmx/libmx.a
LIBMXD = 	../libmx
LIBMXI = 	../libmx/inc

LIBRESSL_A = ../libressl-3.2.0/tls/.libs/libtls.a \
			 ../libressl-3.2.0/ssl/.libs/libssl.a \
			 ../libressl-3.2.0/crypto/.libs/libcrypto.a

LIBRESSL_H = \
			-I ../libressl/include/tls.h \
			-I ../libressl/include/openssl \
			-I ../libressl/include/pqueue.h \
			-I ../libressl/tls \
			-I ../libressl/ssl \
			-I ../libressl/crypto

all: install_json install_libmx install clean

install:
	@cp $(SGLD) ./
	@clang $(CFLG) $(SRCS) $(JSONA) $(LIBMXA) `pkg-config --cflags --libs gtk+-3.0` -o $(NAME) -I$(INCS) -I$(INCD) -I$(JSONI) -I$(LIBMXI) $(LIBRESSL_H) $(LIBRESSL_A) -g -fsanitize=address,undefined
	@printf "\r\33[2K$(NAME) \033[32;1mcreated\033[0m\n"

install_libmx:
	@make -sC $(LIBMXD) debug

install_json:
	@make -sC $(JSOND)

clean:
	@rm -rf ./src/resources.c $(GLD)
	@make -sC $(JSOND) $@
	@rm -rf $(OBJD)
	@printf "$(OBJD)\t   \033[31;1mdeleted\033[0m\n"

uninstall: clean
	@make -sC $(JSOND) $@
	@rm -rf $(SDIR)
	@rm -rf $(NAME)
	@printf "$(NAME) \033[31;1muninstalled\033[0m\n"

reinstall: uninstall all