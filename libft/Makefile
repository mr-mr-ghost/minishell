CFILES = ft_atoi.c ft_bzero.c ft_calloc.c \
		ft_isalnum.c ft_isalpha.c ft_isascii.c \
		ft_isdigit.c ft_isprint.c ft_itoa.c \
		ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c \
		ft_lstdelone.c ft_lstiter.c ft_lstlast.c \
		ft_lstmap.c ft_lstnew.c ft_lstsize.c \
		ft_memchr.c ft_memcmp.c ft_memcpy.c \
		ft_memmove.c ft_memset.c ft_putchar_fd.c \
		ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c \
		ft_split.c ft_strchr.c ft_strdup.c \
		ft_striteri.c ft_strjoin.c ft_strlcat.c \
		ft_strlcpy.c ft_strlen.c ft_strmapi.c \
		ft_strncmp.c ft_strnstr.c ft_strrchr.c \
		ft_strtrim.c ft_substr.c ft_tolower.c \
		ft_toupper.c ft_printf.c ft_putaddress.c \
		ft_putchar.c ft_puthexa.c ft_putnbr.c \
		ft_putstr.c ft_putunsigned.c

OBJ_DIR = obj

OFILES = $(addprefix $(OBJ_DIR)/,$(CFILES:.c=.o))

CC = cc

CFLAGS = -Wall -Werror -Wextra

NAME = libft.a

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
	@ar rcs $(NAME) $(OFILES)
	@echo "${YELLOW}[COMPLETED]${RESET}	${GREEN}Created library${RESET} $(NAME)"

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "${YELLOW}[Libft]${RESET}		${RED}Deleted directory${RESET} $(OBJ_DIR) ${RED}containing${RESET} $(CFILES:.c=.o)"

fclean:
	@rm -f $(NAME)
	@echo "${YELLOW}[Libft]${RESET}		${RED}Deleted library${RESET} $(NAME)"

re: fclean $(NAME)

.PHONY: all clean fclean re
