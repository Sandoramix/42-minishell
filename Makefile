NAME = minishell
PNAME = $(shell echo -n ${NAME} | tr 'a-z' 'A-Z')

CC = cc
INCLUDES = -Isrc/includes -Isrc/libftx/includes
CFLAGS = -Wall -Wextra -Werror $(INCLUDES) $(DEBUGFLAGS)

RM = rm -rf

SRC = ./main.c \
	./src/builtin/ms_cd.c \
	./src/builtin/ms_env.c \
	./src/builtin/ms_exit.c \
	./src/builtin/ms_export.c \
	./src/builtin/ms_history.c \
	./src/builtin/ms_pwd.c \
	./src/builtin/ms_unset.c \
	./src/ms_init.c \
	./src/ms_prompt.c \
	./src/ms_run_builtin.c \
	./src/parsing/args_expand_clear.c \
	./src/parsing/cmd_parse.c \
	./src/parsing/str_clearquotes.c \
	./src/parsing/str_env_variable.c \
	./src/sys/sys_update_cwd.c \
	./src/utils/history_utils.c \
	./src/utils/ms_cleanup.c

# ----RULES-----
all: $(NAME)

$(NAME): $(SRC)
	$(MAKE) -C src/libftx
	$(CC) $(CFLAGS) $(SRC) -pthread -o $(NAME) -Lsrc/libftx -lft -lreadline
	@echo "$(GREEN)[$(PNAME)]:\tPROGRAM CREATED$(R)"
	[ -z "$(strip $(DEBUGFLAGS))" ] || echo "$(RED)[$(PNAME)]:\tDEBUG MODE ENABLED$(R)"

clean:
	$(MAKE) -C src/libftx clean

fclean: clean
	$(MAKE) -C src/libftx fclean
	@$(RM) $(NAME)
	@echo "$(BLUE)[$(PNAME)]:\tPROGRAM DELETED$(R)"

re: fclean all

# ----UTILS-----

VALGRIND=@valgrind --suppressions=src/readline.supp --leak-check=full --show-leak-kinds=all --track-origins=yes --quiet --tool=memcheck --keep-debuginfo=yes
valgrind: debug-log
	clear
	$(VALGRIND) ./$(NAME)


re-debug-log: fclean debug-log
re-debug: fclean debug

debug:
	$(MAKE) DEBUGFLAGS="-g"
debug-log:
	$(MAKE) DEBUGFLAGS="-g -DDEBUG=true"
# --------------
.PHONY: all clean fclean re
.SILENT:

# ----COLORS----
GREEN=\033[0;32m
RED=\033[0;31m
BLUE=\033[0;34m
R=\033[0m
