NAME = uchat

CFLG = -std=c11  $(addprefix -W, all extra error pedantic) \
		 	$(addprefix -Wno, -unused-command-line-argument) \
			 -g \
		 	`pkg-config --cflags --libs gtk+-3.0` \
			# -fsanitize=address \

SRCD = src
INCD = inc
OBJD = obj
LMXD = libmx

LMXA :=	$(LMXD)/libmx.a
LMXI :=	$(LMXD)/$(INCD)

INC	= uchat.h
INCS = $(addprefix $(INCD)/, $(INC))
SRC= main.c \
	server_func.c \
	login.c \
	register.c \
	settings.c \
	uchat_window.c \
	server.c \
	
SRCS = $(addprefix $(SRCD)/, $(SRC))
OBJS = $(addprefix $(OBJD)/, $(SRC:%.c=%.o))

all: $(LMXD) $(NAME) clean

$(FILE:a/%=%)

$(NAME): $(LMXA) $(OBJS)
	@clang $(CFLG) $(OBJS) -L$(LMXD) -lmx -o $@ `pkg-config gtk+-3.0 --libs`
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"

$(OBJD)/%.o: $(SRCD)/%.c $(INCS)
	@clang $(CFLG) -c $< -o $@ -I$(INCD) -I$(LMXI) `pkg-config gtk+-3.0 --libs`
	@printf "\r\33[2K$(NAME) \033[33;1mcompile \033[0m$(<:$(SRCD)/%.c=%) "

$(OBJS): | $(OBJD)

$(OBJD):
	@mkdir -p $@

$(LMXD): $(LMXA)

$(LMXA):
	@make -sC $(LMXD)

clean:
	@make -sC $(LMXD) $@
	@rm -rf $(OBJD)

uninstall: clean
	@make -sC $(LMXD) $@
	@rm -rf $(NAME)
	@printf "$(NAME) \033[31;1muninstalled\033[0m\n"

reinstall: uninstall all