# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/05/16 11:42:25 by charles           #+#    #+#              #
#    Updated: 2020/05/16 11:52:38 by charles          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RM = rm -f
MAKE = make --no-print-directory

LIBFT_DIR = libft

SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

CC = gcc
OFLAG ?= -O0
CCFLAGS = $(OFLAG) -I$(INC_DIR) -I$(LIBFT_DIR)/include \
		  -Wall -Wextra #-Werror
LDFLAGS = -L$(LIBFT_DIR) -lft

NAME = ft_ls

INC = $(shell find $(INC_DIR) -type f -name '*.h')
SRC = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: prebuild libft_all $(NAME)

prebuild:
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ)
	@echo "Linking: $@"
	@$(CC) -o $@ $(OBJ) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC)
	@echo "Compiling: $@"
	@$(CC) $(CCFLAGS) -c -o $@ $<

cleanloc:
	@echo "Removing objects"
	@$(RM) $(OBJ)

fcleanloc: cleanloc
	@echo "Removing $(NAME)"
	@$(RM) $(NAME)

reloc: fcleanloc all

clean: cleanloc libft_clean
fclean: fcleanloc libft_fclean

re: fclean libft_fclean all

libft_all:
	@echo "Making libft all"
	@$(MAKE) -C $(LIBFT_DIR) all

libft_clean:
	@echo "Making libft clean"
	@$(MAKE) -C $(LIBFT_DIR) clean

libft_fclean:
	@echo "Making libft fclean"
	@$(MAKE) -C $(LIBFT_DIR) fclean
