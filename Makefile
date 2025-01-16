# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: codespace <codespace@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/04 21:44:18 by iassil            #+#    #+#              #
#    Updated: 2025/01/16 13:00:24 by codespace        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CPP				=	c++
CPP				+=	-Wall -Wextra -Werror -std=c++98
CPP				+=	-fsanitize=address -g
RM				=	rm -f
NAME			=	irc_server
HEADER			=	headers/Channel.hpp  headers/Client.hpp  headers/ClientManager.hpp  headers/Responses.hpp  headers/bits.hpp

FLD_NAME		=	._object_files

##### SOURCE FILES #######################################################################
MAIN_FILE	=	main.cpp
SRC_FILES	=	Channel.cpp  Client.cpp  ClientManager.cpp  utils.cpp
CMD_FILES	=	join.cpp  kick.cpp  nick.cpp  part.cpp  privmsg.cpp  quit.cpp  topic.cpp

##########################################################################################

MAIN_SRC	=	$(addprefix /,$(MAIN_FILE))
SRC_SRC		=	$(addprefix srcs/,$(SRC_FILES))
CMD_SRC		=	$(addprefix cmds/,$(CMD_FILES))

MAIN_OBJ	=	$(addprefix $(FLD_NAME)/,$(MAIN_FILE:.cpp=.o))
SRC_OBJ		=	$(addprefix $(FLD_NAME)/srcs/,$(SRC_FILES:.cpp=.o))
CMD_OBJ 	=	$(addprefix $(FLD_NAME)/cmds/,$(CMD_FILES:.cpp=.o))

OBJ 		=	$(MAIN_OBJ) $(SRC_OBJ) $(CMD_OBJ)

########### Goal Target
all: $(NAME)

run: $(NAME) art
	@./$(NAME)

$(FLD_NAME)/%.o: ./%.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CPP) -c $< -o $@

$(FLD_NAME)/srcs/%.o: ./srcs/%.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CPP) -c $< -o $@

$(FLD_NAME)/cmds/%.o: ./cmds/%.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CPP) -c $< -o $@

######################################################
$(NAME): $(OBJ)
	@echo "$(YELLOW)[ ~ ] Compilation of the Objects files...$(RESET)"
	@$(CPP) $^ -o $@
	@echo "$(GREEN)[ ✓ ] Executable file Compiled Successfully!$(RESET)"

clean:
	@echo "$(YELLOW)[ ~ ] Removing Object files $(RESET)"
	@$(RM) $(OBJ)
	@$(RM) -rf $(FLD_NAME)
	@echo "$(GREEN)[ ✓ ] Object files removed successfully!$(RESET)"
	
fclean: clean
	@echo "$(YELLOW)[ ~ ] Removing Executable...$(RESET)"
	@$(RM) $(NAME)
	@echo "$(GREEN)[ ✓ ] Executable files removed successfully!$(RESET)"

re: fclean all

.PHONY: all fclean clean re

###################################################
###################################################
TERM_WIDTH := $(shell tput cols)

PADDING := $(shell expr $(TERM_WIDTH) - 74)
ifeq ($(shell test $(PADDING) -lt 0; echo $$?),0)
    PADDING := 0
endif
PADDING := $(shell expr $(PADDING) / 2)
PAD := $(shell printf '%*s' $(PADDING) '')
###################################################
###################################################

art:
	@echo ""
	@echo "$(PAD)$(LRD)██╗██████╗  ██████╗    ███████╗███████╗██████╗ ██╗   ██╗███████╗██████╗ $(RESER)"
	@echo "$(PAD)$(LRD)██║██╔══██╗██╔════╝    ██╔════╝██╔════╝██╔══██╗██║   ██║██╔════╝██╔══██╗$(RESER)"
	@echo "$(PAD)$(RED)██║██████╔╝██║         ███████╗█████╗  ██████╔╝██║   ██║█████╗  ██████╔╝$(RESER)"
	@echo "$(PAD)$(RED)██║██╔══██╗██║         ╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝██╔══╝  ██╔══██╗$(RESER)"
	@echo "$(PAD)$(RED)██║██║  ██║╚██████╗    ███████║███████╗██║  ██║ ╚████╔╝ ███████╗██║  ██║$(RESER)"
	@echo "$(PAD)$(RED)╚═╝╚═╝  ╚═╝ ╚═════╝    ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝$(RESER)"
	@echo "$(RESET)"                                                                                     

########## Define ANSI escape codes for colors
GREEN	:=	\033[1;32m
YELLOW	:=	\033[33m
LRD		:=	\033[91m
RED		:=	\033[31m
DRD		:=	\033[2;31m
RESET	:=	\033[0m