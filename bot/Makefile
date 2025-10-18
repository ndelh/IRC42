# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: doley <doley@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/17 13:31:00 by ndelhota          #+#    #+#              #
#    Updated: 2025/10/18 14:09:01 by doley            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := bot

SRC := main.cpp \
       Bot.cpp \

CC := c++

CFLAGS := -g -Wall -Werror -Wextra -std=c++98

OBJDIR := obj
OBJ := $(SRC:.cpp=.o)
OBJ := $(addprefix $(OBJDIR)/, $(OBJ))

all: $(OBJ) $(NAME)

$(NAME)	: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -rf $(OBJDIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re

