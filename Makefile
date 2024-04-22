NAME = minishell
PNAME = $(shell echo -n ${NAME} | tr 'a-z' 'A-Z')

CC = cc
INCLUDES = -Isrc/includes -Isrc/libftx/includes
CFLAGS = -Wall -Wextra -Werror $(INCLUDES) $(DEBUGFLAGS)

RM = rm -rf

SRC= main.c \
	./src/builtin/ms_exit.c \
	./src/ms_init.c \
	./src/ms_prompt.c \
	./src/builtin/ms_unset.c \
	./src/builtin/ms_env.c \
	./src/ms_cleanup.c 
# ----RULES-----
all: $(NAME)

$(NAME): $(SRC)
	$(MAKE) -C src/libftx
	$(CC) $(CFLAGS) $(SRC) -pthread -o $(NAME) -Lsrc/libftx -lft -lreadline
	@echo "$(GREEN)[$(PNAME)]:\tPROGRAM CREATED$(R)"
	[ -z "$(strip $(DEBUGFLAGS))" ] || echo "$(RED)[$(PNAME)]:\tDEBUG MODE ENABLED$(R)"

clean:

fclean: clean
	@$(RM) $(NAME)
	@echo "$(BLUE)[$(PNAME)]:\tPROGRAM DELETED$(R)"

re: fclean all

# ----UTILS-----
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
