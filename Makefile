NAME = minishell

# -----$(NAME) but uppercase----------------------------------------------------

PNAME = $(shell echo -n ${NAME} | tr 'a-z' 'A-Z')

# -----VARIABLES-DECLARATIONS-+-OVVERRIDES--------------------------------------

DEBUG_VALUE=0

LIBFTX_DIR=src/libftx

CC = cc
INCLUDES = -Iincludes -I$(LIBFTX_DIR)/includes
CFLAGS = -Wall -Wextra -Werror -g $(INCLUDES) -DDEBUG=$(DEBUG_VALUE)

RM = rm -rf

# ----SOURCE-FILES--------------------------------------------------------------

SRC = ./main.c \
	./src/builtin/ms_cd.c \
	./src/builtin/ms_echo.c \
	./src/builtin/ms_env.c \
	./src/builtin/ms_exit.c \
	./src/builtin/ms_export.c \
	./src/builtin/ms_history.c \
	./src/builtin/ms_pwd.c \
	./src/builtin/ms_unset.c \
	./src/execute/ms_exec.c \
	./src/execute/ms_heredoc.c \
	./src/execute/ms_redirect.c \
	./src/execute/ms_wrap_commands.c \
	./src/ms_init.c \
	./src/ms_prompt.c \
	./src/parsing/args_expand_clear.c \
	./src/parsing/cmd_parse.c \
	./src/parsing/cmd_pre_parse.c \
	./src/parsing/str_clearquotes.c \
	./src/parsing/str_env_variable.c \
	./src/utils/command_split_utils.c \
	./src/utils/expansion_utils.c \
	./src/utils/global_utils.c \
	./src/utils/heredoc_utils.c \
	./src/utils/history_utils.c \
	./src/utils/ms_cleanup.c \
	./src/utils/ms_convert_utils.c \
	./src/utils/ms_update_cwd.c \
	./src/utils/parsing_utils.c


# ----RULES---------------------------------------------------------------------

all: $(NAME)

debug:
	$(MAKE) DEBUG_VALUE=1

$(NAME): $(SRC)
	$(MAKE) -C $(LIBFTX_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME) -L$(LIBFTX_DIR) -lft -lreadline -pthread
	@echo "$(GREEN)[$(PNAME)]:\tPROGRAM CREATED$(R)"
	[ "$(strip $(DEBUG_VALUE))" = "0" ] || echo "$(RED)[$(PNAME)]:\tDEBUG MODE ENABLED$(R)"

clean:
	$(MAKE) -C $(LIBFTX_DIR) clean

fclean: clean
	$(MAKE) -C $(LIBFTX_DIR) fclean
	@$(RM) $(NAME)
	@echo "$(BLUE)[$(PNAME)]:\tPROGRAM DELETED$(R)"

re: fclean all
re-debug: fclean debug

# ----UTILS---------------------------------------------------------------------

VALGRIND=@valgrind --suppressions=src/readline.supp --leak-check=full --show-leak-kinds=all --track-origins=yes --quiet --tool=memcheck --keep-debuginfo=yes
# FOR FD 		TRACKING: --track-fds=yes
# FOR CHILDREN	TRACKING: --trace-children=yes
valgrind: debug
	clear
	$(VALGRIND) ./$(NAME)

# ------------------------------------------------------------------------------

.PHONY: all clean fclean re re-debug debug
.SILENT:

# ----COLORS--------------------------------------------------------------------
GREEN=\033[0;32m
RED=\033[0;31m
BLUE=\033[0;34m
R=\033[0m
