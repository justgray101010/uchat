NAME = uchat_server

#строку 3 я взял из твоего мейка клиента,
#так как при компиляции возникала проблема линков
CFLG = -std=c11 $(addprefix -W, all extra error pedantic)\
		$(addprefix -Wno, -unused-command-line-argument)\
		-pthread -lsqlite3 -g # -fsanitize=address

SRCD = src
INCD = inc
OBJD = obj
LMXD = libmx
SQLITE3 = sqlite3_lib.a
LMXA :=	$(LMXD)/libmx.a
LMXI :=	$(LMXD)/$(INCD)
SQL = sql
INC	= server.h
INCS = $(addprefix $(INCD)/, $(INC))
SRC = server.c \
	  server_func.c \
	  sociable_loop.c \
	  mx_db.c \
	  mx_db_chat.c \
	  mx_db_friend.c \
	  mx_db_subs.c \
	  mx_db_tag.c \
	  server_loop_sub.c \
	  server_loop_sub2.c \
	  server_loop_sub3.c \
	  server_func_sub.c

SRCS = $(addprefix $(SRCD)/, $(SRC))
OBJS = $(addprefix $(OBJD)/, $(SRC:%.c=%.o))

all: $(LMXD) $(NAME) clean

$(FILE:a/%=%)

$(NAME): $(LMXA) $(OBJS)
	@rm -f sql/data.db 	#при каждой сборке проекта мы уаляем базу
	@touch sql/data.db 	#а затем создаём новую
	@clang $(CFLG) $(OBJS) -L$(LMXD) -L$(SQL) -lmx -o $@
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"

$(OBJD)/%.o: $(SRCD)/%.c $(INCS)
	@clang $(CFLG) -c $< -o $@ -I$(INCD) -I$(LMXI)
	@printf "\r\33[2K$(NAME) \033[33;1mcompile \033[0m$(<:$(SRCD)/%.c=%) "

$(OBJS): | $(OBJD)

$(OBJD):
	@mkdir -p $@

$(LMXD): $(LMXA)

$(LMXA):
	@make -sC sql
	@make -sC $(LMXD)

clean:
	@make -sC $(LMXD) $@
	@rm -rf $(OBJD)

uninstall: clean
	@make -sC $(LMXD) $@
	@rm -rf $(NAME)
	@printf "$(NAME) \033[31;1muninstalled\033[0m\n"

reinstall: uninstall all$
