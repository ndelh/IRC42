# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: doley <doley@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/28 15:13:32 by ndelhota          #+#    #+#              #
#    Updated: 2025/10/08 16:24:44 by doley            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := IRC

SRC := main.cpp \
       Server.cpp \
       Client.cpp \
       Channel.cpp \
       Action.cpp \
       NICK.cpp \
	   USER.cpp \
	   JOIN.cpp \
	   PASS.cpp \
	   PrivateMsg.cpp \

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
