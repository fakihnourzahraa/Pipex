# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nfakih <nfakih@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/27 22:32:07 by nfakih            #+#    #+#              #
#    Updated: 2025/08/27 22:32:47 by nfakih           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = pipex

SRC = ft_split.c ft_strjoin.c libft.c ft_strdup.c main.c help.c help_two.c

OBJ     = $(SRC:.c=.o)

CC      = cc
CFLAGS  =  -Wall -Werror -Wextra -g

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "🏂 done making"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@echo "💄 done cleaning"

fclean: clean
	@rm -f $(NAME)
	@echo "👺 done fcleaning"

re: fclean all

.PHONY: all clean fclean re
