NAME = minishell

CFILES = minishell.c utils.c

OBJ_DIR = obj

OFILES = $(addprefix $(OBJ_DIR)/,$(CFILES:.c=.o))

CC = cc

CFLAGS = -Wall -Werror -Wextra -g

LIBRARY = libft/libft.a -lreadline

RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
RESET = \033[0m

TOTAL_FILES = $(words $(CFILES))
COMPILED_FILES = 0

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(eval COMPILED_FILES=$(shell echo $$(($(COMPILED_FILES)+1))))
	@echo "${YELLOW}[$$(($(COMPILED_FILES)*100/$(TOTAL_FILES)))%]${RESET}		${GREEN}Compiled${RESET} $< ${GREEN}with flags${RESET} $(CFLAGS)"

$(NAME): $(OFILES)
	@echo "${YELLOW}[LIBFT]${RESET}		${GREEN}Compiling library${RESET} libft.a"
	@$(MAKE) -s -C ./libft
	@$(CC) $(CFLAGS) $(OFILES) $(LIBRARY) -o $(NAME)
	@echo "${YELLOW}[COMPLETED]${RESET}	${GREEN}Created executable${RESET} $(NAME) ${GREEN}using libraries${RESET} $(LIBRARY)"

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -s -C ./libft clean
	@echo "${RED}Deleted directory${RESET} $(OBJ_DIR) ${RED}containing${RESET} $(CFILES:.c=.o)"

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -s -C ./libft fclean
	@echo "${RED}Deleted executable${RESET} $(NAME)"

re: fclean $(NAME)

.PHONY: all clean fclean re
