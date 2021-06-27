# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abaudot <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/19 20:59:34 by abaudot           #+#    #+#              #
#    Updated: 2021/06/27 16:43:20 by abaudot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT
CC = gcc

MLX_DIR = mlx_linux


UNAME := $(shell uname)

ifeq ($(UNAME),Darwin)
	NUM_THREADS = $(shell sysctl -n hw.ncpu)
	LDFLAGS = -Lmlx_mac -lmlx -framework OpenGL -framework AppKit
	CFLAGS = -Iincludes -Imlx_mac -Ofast -D MACOS
endif

ifeq ($(UNAME),Linux)
	NUM_THREADS = $(shell nproc --all)
	LDFLAGS = -Lmlx_linux -lmlx -lXext -lX11 -lm -lbsd -lpthread
	CFLAGS = -Iincludes -Imlx_linux -Ofast -D LINUX
endif

alias = 1
ANTI = $$(( $(alias) * $(alias) ))

CFLAGS += -Wall -Wextra -Werror -D NUM_THREADS=$(NUM_THREADS) 
CFLAGS += -D ANTIALIASING=$(alias) -D ANTI2=$(ANTI)
OBJS_DIR = objs

PRECOMPILE = mkdir -p $(dir $(OBJS_DIR)/$*)

DEF = \033[0m
RED			= \033[31;1m
GRN			= \033[32;1m
YEL			= \033[33;1m
BLU			= \033[34;1m


include assets/src.mk
include assets/inc.mk
include assets/src_bonus.mk

OBJS = $(patsubst src/%.c, $(OBJS_DIR)/%.o, $(SRCS))
OBJS_BONUS = $(patsubst src/%.c, $(OBJS_DIR)/%.o, $(SRCS_BONUS))

all: $(NAME)

$(OBJS_DIR)/%.o: src/%.c $(INCLUDES)
	@$(PRECOMPILE)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo "$(BLU)Created: $(@:%=%) $(DEF)"

$(NAME): $(OBJS)
	@$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	@echo "$(YEL)\n ******************* Everything's done ! ************************$(DEF)"
	@echo "$(YEL)\$(DEF)"
	@echo "$(YEL) Compiled with: $(BLU)$(NUM_THREADS) threads $(YEL)| $(GRN)$(alias)x$(alias) antialiasing$(YEL) and.. $(RED)LOVE <3\t$(DEF)"
	@echo "$(YEL) Usage : ./miniRT scenes/myscene.rt [skymaps/skymapDir/] [--save]\n$(DEF)"
	@echo "$(RED) Note : To modify aliasing x factor type: $(GRN)make $(BLU)alias=x $(GRN)re \n$(DEF)"

bonus: $(OBJS_BONUS)
	@$(CC) -o miniRT $^ $(CFLAGS) $(LDFLAGS)
	@echo "$(YEL)\n ******************* Everything's done ! ************************$(DEF)"
	@echo "$(YEL)\$(DEF)"
	@echo "$(YEL) Compiled with: $(BLU)$(NUM_THREADS) threads $(YEL)| $(GRN)$(alias)x$(alias) antialiasing$(YEL) and.. $(RED)LOVE <3\t$(DEF)"
	@echo "$(YEL) Usage : ./miniRT scenes/myscene.rt [skymaps/skymapDir/] [--save]\n$(DEF)"
	@echo "$(RED) Note : To modify aliasing x factor type: $(GRN)make $(BLU)alias=x $(GRN)re \n$(DEF)"

install :
	#sudo apt-get update && apt-get install libxext-dev libbsd-dev
	@$(foreach path, $(MLX_DIR), make -C $(path);)

check_leaks: re
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./miniRT scenes/sphere1.rt

clean:
	@rm -rf objs
	@echo "$(GRN)clean !"

fclean: clean
	@rm -f $(NAME)
	@echo "$(GRN)Everything's clean !"

re: fclean all

.PHONY: all clean fclean re test
