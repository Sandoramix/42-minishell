NAME = minishell
PNAME = $(shell echo -n ${NAME} | tr 'a-z' 'A-Z')

CC = cc
INCLUDES = -Isrc/includes -Isrc/libftx/includes
CFLAGS = -Wall -Wextra -Werror $(INCLUDES) $(DEBUGFLAGS)

RM = rm -rf

SRC= main.c

# ----RULES-----
all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -pthread -o $(NAME)

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