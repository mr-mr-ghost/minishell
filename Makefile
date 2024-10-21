NAME = minishell

CFILES = minishell.c main/prompt.c main/utils.c main/libft_utils.c \
		enviro/env_init.c enviro/env_utils.c enviro/env_list_utils.c \
		tokens/tokens_handling.c tokens/tokens_utils.c tokens/tokens_split_utils.c \
		tokens/tokens_split_dollar.c tokens/tokens_list_utils.c main/signals.c \
		execution/execution.c execution/exec_utils.c execution/exec_arr_utils.c \
		builtin/builtin_utils.c builtin/builtin_pwd.c builtin/builtin_export.c \
		builtin/builtin_env.c builtin/builtin_echo.c builtin/builtin_unset.c \
		builtin/builtin_cd.c builtin/builtin_exit.c builtin/builtin_declaration.c \
		nonbuiltin/nonbuiltin_handling.c nonbuiltin/nonbuiltin_path_utils.c \
		nonbuiltin/nonbuiltin_processes.c redirection/redirection_utils.c redirection/piping.c \
		redirection/redirect_heredoc.c redirection/redirect_heredoc_utils.c redirection/piping_utils.c \

OBJ_DIR = obj/

MAIN = main/
BUILTINS = builtin/
ENVIRO = enviro/
TOKENS = tokens/
EXECUTION = execution/
NONBUILTIN = nonbuiltin/
REDIR = redirection/

OFILES = $(addprefix $(OBJ_DIR), $(notdir $(patsubst %.c, %.o, $(CFILES))))

CC = cc

CFLAGS = -Wall -Werror -Wextra -g

LIBRARY = libft/libft.a -lreadline

RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
RESET = \033[0m

TOTAL_FILES = $(words $(CFILES))
COMPILED_FILES = 0

$(OBJ_DIR)%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(eval COMPILED_FILES=$(shell echo $$(($(COMPILED_FILES)+1))))
	@echo "${YELLOW}[$$(($(COMPILED_FILES)*100/$(TOTAL_FILES)))%]${RESET}		${GREEN}Compiled${RESET} ${notdir $<} ${GREEN}with flags${RESET} $(CFLAGS)"

$(OBJ_DIR)%.o: $(MAIN)%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(eval COMPILED_FILES=$(shell echo $$(($(COMPILED_FILES)+1))))
	@echo "${YELLOW}[$$(($(COMPILED_FILES)*100/$(TOTAL_FILES)))%]${RESET}		${GREEN}Compiled${RESET} ${notdir $<} ${GREEN}with flags${RESET} $(CFLAGS)"

$(OBJ_DIR)%.o: $(BUILTINS)%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(eval COMPILED_FILES=$(shell echo $$(($(COMPILED_FILES)+1))))
	@echo "${YELLOW}[$$(($(COMPILED_FILES)*100/$(TOTAL_FILES)))%]${RESET}		${GREEN}Compiled${RESET} ${notdir $<} ${GREEN}with flags${RESET} $(CFLAGS)"

$(OBJ_DIR)%.o: $(ENVIRO)%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(eval COMPILED_FILES=$(shell echo $$(($(COMPILED_FILES)+1))))
	@echo "${YELLOW}[$$(($(COMPILED_FILES)*100/$(TOTAL_FILES)))%]${RESET}		${GREEN}Compiled${RESET} ${notdir $<} ${GREEN}with flags${RESET} $(CFLAGS)"

$(OBJ_DIR)%.o: $(TOKENS)%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(eval COMPILED_FILES=$(shell echo $$(($(COMPILED_FILES)+1))))
	@echo "${YELLOW}[$$(($(COMPILED_FILES)*100/$(TOTAL_FILES)))%]${RESET}		${GREEN}Compiled${RESET} ${notdir $<} ${GREEN}with flags${RESET} $(CFLAGS)"

$(OBJ_DIR)%.o: $(EXECUTION)%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(eval COMPILED_FILES=$(shell echo $$(($(COMPILED_FILES)+1))))
	@echo "${YELLOW}[$$(($(COMPILED_FILES)*100/$(TOTAL_FILES)))%]${RESET}		${GREEN}Compiled${RESET} ${notdir $<} ${GREEN}with flags${RESET} $(CFLAGS)"

$(OBJ_DIR)%.o: $(NONBUILTIN)%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(eval COMPILED_FILES=$(shell echo $$(($(COMPILED_FILES)+1))))
	@echo "${YELLOW}[$$(($(COMPILED_FILES)*100/$(TOTAL_FILES)))%]${RESET}		${GREEN}Compiled${RESET} ${notdir $<} ${GREEN}with flags${RESET} $(CFLAGS)"

$(OBJ_DIR)%.o: $(REDIR)%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(eval COMPILED_FILES=$(shell echo $$(($(COMPILED_FILES)+1))))
	@echo "${YELLOW}[$$(($(COMPILED_FILES)*100/$(TOTAL_FILES)))%]${RESET}		${GREEN}Compiled${RESET} ${notdir $<} ${GREEN}with flags${RESET} $(CFLAGS)"

$(NAME): $(OFILES)
	@echo "${YELLOW}[LIBFT]${RESET}		${GREEN}Compiling library${RESET} libft.a"
	@$(MAKE) -s -C ./libft
	@$(CC) $(CFLAGS) $(OFILES) $(LIBRARY) -o $(NAME)
	@echo "${YELLOW}[COMPLETED]${RESET}	${GREEN}Created executable${RESET} $(NAME) ${GREEN}using libraries${RESET} $(notdir $(LIBRARY))"

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -s -C ./libft clean
	@echo "${YELLOW}[Minishell]${RESET}	${RED}Deleted directory${RESET} $(OBJ_DIR) ${RED}containing${RESET} $(notdir $(patsubst %.c, %.o, $(CFILES)))"

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -s -C ./libft fclean
	@echo "${YELLOW}[Minishell]${RESET}	${RED}Deleted executable${RESET} $(NAME)"

re: fclean $(NAME)

.PHONY: all clean fclean re
