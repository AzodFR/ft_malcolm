NAME = ft_malcolm

INCLUDES = includes/

SRC = srcs/

FLAGS = -Wall -Wextra -Werror -g3

CC = gcc $(FLAGS)


FILES = ft_malcolm.c utils.c

RM = rm -f

OBJ = $(addprefix $(SRC), $(FILES))

OBJS = $(OBJ:.c=.o)

%.o: %.c $(INCLUDES)/ft_malcolm.h
	@gcc -Wall -Wextra -Werror -g3 -o $@ -c $< -I$(INCLUDES)
	@printf "\e[JCreated \e[92m$(notdir $@)\e[0m\n"

$(NAME): $(INCLUDES)/ft_malcolm.h $(OBJS)
	@$(CC) $(OBJS) -o $@ -I$(INCLUDES)
	@printf "\n\e[36mft_\e[33mmalcolm \e[92mis ready !\e[0m\e[?25h\n"


all: $(NAME)

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(OBJS)
	@$(RM) $(NAME)
	@printf "$(NAME) [\033[31mX\033[0m]\n"

re: fclean all

.PHONY: all clean fclean re ft_malcolm