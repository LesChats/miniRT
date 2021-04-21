# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abaudot <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/19 20:59:34 by abaudot           #+#    #+#              #
#    Updated: 2021/04/21 13:38:40 by abaudot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT
CC = gcc

NUM_THREADS = $(shell nproc --all)
alias = 1
ANTI = $$(( $(alias) * $(alias) ))

CFLAGS = -Wall -Wextra -Werror -D NUM_THREADS=$(NUM_THREADS) 
CFLAGS += -D ANTIALIASING=$(alias) -D ANTI2=$(ANTI)
CFLAGS += -Iincludes -Imlx_linux -Ofast
LDFLAGS = -Lmlx_linux -lmlx -lXext -lX11 -lm -lbsd -lpthread

OBJS_DIR = objs
#DEPS_DIR = $(BUILD_DIR)/deps

#PRECOMPILE = mkdir -p $(dir $@)
PRECOMPILE = mkdir -p $(dir $(OBJS_DIR)/$*)

DEF = \033[0m
RED			= \033[31;1m
GRN			= \033[32;1m
YEL			= \033[33;1m
BLU			= \033[34;1m

include make_deps/src.mk
include make_deps/inc.mk

OBJS = $(patsubst src/%.c, $(OBJS_DIR)/%.o, $(SRCS))

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

clean:
	@rm -rf objs
	@echo "$(GRN)clean !"

fclean: clean
	@rm -f $(NAME)
	@echo "$(GRN)Everything's clean !"

re: fclean all

.PHONY: all clean fclean re test
