NAME = philo

CC = gcc

CFLAGS = -Wall -Werror -Wextra -g

Y = "\033[33m"
R = "\033[31m"
G = "\033[32m"
B = "\033[34m"
X = "\033[0m"
UP = "\033[A"
CUT = "\033[K"

CFILES = src/main.c src/utils/init.c src/utils/utils.c \
			src/utils/threads.c src/utils/actions.c \

OBJECTS = $(CFILES:.c=.o)

all: libraries $(NAME)

%.o : %.c
	@echo $(Y)Compiling [$<]...$(X)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@printf $(UP)$(CUT)

libraries:
#	@echo $(B)
#	make -C $(LIBFT_PATH) all
#	@echo $(B)
#	make -C $(PRINTF_PATH) all

$(NAME): $(OBJECTS)
	@echo $(Y)Compiling [$(CFILES)]...$(X)
	@echo $(G)Finished [$(CFILES)]$(X)
	@echo
	@echo $(Y)Compiling [$(NAME)]...$(X)
	@$(CC) $(CFLAGS) $(LIBFT_LIB) $(PRINTF_LIB) $(OBJECTS) -o $(NAME)
	@echo $(G)Finished [$(NAME)]$(X)

clean:
#	@make -C $(LIBFT_PATH) clean
#	@make -C $(PRINTF_PATH) clean
	@rm -f $(OBJECTS)
	@echo $(R)Removed [$(OBJECTS)]$(X)
	@echo $(R)Removed libraries.o$(X)

fclean: clean
#	@make -C $(LIBFT_PATH) fclean
#	@make -C $(PRINTF_PATH) fclean
	@rm -f $(NAME)
	@echo $(R)Removed [$(NAME)]$(X)

re: fclean all

norm:
	norminette ft_printf utils checker

#run:	all

.PHONY: all clean fclean re norm