CPP				=	c++
CPP				+=	-Wall -Wextra -Werror -std=c++98
CPP				+=	-fsanitize=address -g
RM				=	rm -f
NAME			=	ircserv
NAME_BONUS		=	bot
HEADER			=	headers/Channel.hpp  headers/Client.hpp  headers/ClientManager.hpp  headers/Responses.hpp  \
					headers/bits.hpp headers/Server.hpp

FLD_NAME		=	._object_files

##### SOURCE FILES #######################################################################
MAIN_FILE	=	main.cpp
SRC_FILES	=	Channel.cpp		Client.cpp	ClientManager.cpp	utils.cpp	Server.cpp
CMD_FILES	=	invite.cpp	join.cpp	kick.cpp			\
				mode.cpp	nick.cpp	privmsg.cpp			\
				topic.cpp	ping.cpp	pong.cpp			\
				quit.cpp	part.cpp
BOT_FILES	=	Bot.cpp		main.cpp

##########################################################################################

MAIN_SRC	=	$(addprefix /,$(MAIN_FILE))
SRC_SRC		=	$(addprefix srcs/,$(SRC_FILES))
CMD_SRC		=	$(addprefix cmds/,$(CMD_FILES))
BOT_SRC		=	$(addprefix bonus/,$(BOT_FILES))

MAIN_OBJ	=	$(addprefix $(FLD_NAME)/,$(MAIN_FILE:.cpp=.o))
SRC_OBJ		=	$(addprefix $(FLD_NAME)/srcs/,$(SRC_FILES:.cpp=.o))
CMD_OBJ 	=	$(addprefix $(FLD_NAME)/cmds/,$(CMD_FILES:.cpp=.o))
BOT_OBJ		=	$(addprefix $(FLD_NAME)/bonus/,$(BOT_FILES:.cpp=.o))

OBJ 		=	$(MAIN_OBJ) $(SRC_OBJ) $(CMD_OBJ)

########### Goal Target
all: $(NAME)

run: $(NAME) art
	@./$(NAME) 6667 hehe

run_bonus: bonus bonus_art
	@./bot

$(FLD_NAME)/bonus/%.o: ./bonus/%.cpp bonus/Bot.hpp headers/bits.hpp headers/Responses.hpp
	@mkdir -p $(dir $@)
	@$(CPP) -c $< -o $@

$(FLD_NAME)/%.o: ./%.cpp headers/Server.hpp headers/Responses.hpp
	@mkdir -p $(dir $@)
	@$(CPP) -c $< -o $@

$(FLD_NAME)/srcs/Channel.o: ./srcs/Channel.cpp headers/Channel.hpp headers/Client.hpp headers/Responses.hpp
	@mkdir -p $(dir $@)
	@$(CPP) -c $< -o $@

$(FLD_NAME)/srcs/Client.o: ./srcs/Client.cpp headers/Client.hpp headers/bits.hpp headers/Responses.hpp
	@mkdir -p $(dir $@)
	@$(CPP) -c $< -o $@

$(FLD_NAME)/srcs/ClientManager.o: ./srcs/ClientManager.cpp headers/ClientManager.hpp headers/Client.hpp headers/Channel.hpp headers/bits.hpp headers/Responses.hpp
	@mkdir -p $(dir $@)
	@$(CPP) -c $< -o $@

$(FLD_NAME)/srcs/Server.o: ./srcs/Server.cpp headers/Server.hpp headers/ClientManager.hpp headers/bits.hpp headers/Responses.hpp
	@mkdir -p $(dir $@)
	@$(CPP) -c $< -o $@

$(FLD_NAME)/srcs/utils.o: ./srcs/utils.cpp headers/bits.hpp headers/Responses.hpp
	@mkdir -p $(dir $@)
	@$(CPP) -c $< -o $@

$(FLD_NAME)/cmds/%.o: ./cmds/%.cpp headers/ClientManager.hpp headers/Responses.hpp
	@mkdir -p $(dir $@)
	@$(CPP) -c $< -o $@

######################################################
$(NAME): $(OBJ)
	@echo "$(YELLOW)[ ~ ] Compilation of the Objects files...$(RESET)"
	@$(CPP) $^ -o $@
	@echo "$(GREEN)[ ✓ ] Executable file Compiled Successfully!$(RESET)"

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(BOT_OBJ) 
	@echo "$(YELLOW)[ ~ ] Compilation of the Objects files...$(RESET)"
	@$(CPP) $^ -o $(NAME_BONUS)
	@echo "$(GREEN)[ ✓ ] Executable file Compiled Successfully!$(RESET)"
######################################################

clean:
	@echo "$(YELLOW)[ ~ ] Removing Object files $(RESET)"
	@$(RM) $(OBJ) $(BOT_OBJ)
	@$(RM) -rf $(FLD_NAME) $(NAME_BONUS)
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
	@echo "$(PAD)$(LRD)██╗██████╗  ██████╗    ███████╗███████╗██████╗ ██╗   ██╗███████╗██████╗ $(RESET)"
	@echo "$(PAD)$(LRD)██║██╔══██╗██╔════╝    ██╔════╝██╔════╝██╔══██╗██║   ██║██╔════╝██╔══██╗$(RESET)"
	@echo "$(PAD)$(RED)██║██████╔╝██║         ███████╗█████╗  ██████╔╝██║   ██║█████╗  ██████╔╝$(RESET)"
	@echo "$(PAD)$(RED)██║██╔══██╗██║         ╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝██╔══╝  ██╔══██╗$(RESET)"
	@echo "$(PAD)$(RED)██║██║  ██║╚██████╗    ███████║███████╗██║  ██║ ╚████╔╝ ███████╗██║  ██║$(RESET)"
	@echo "$(PAD)$(RED)╚═╝╚═╝  ╚═╝ ╚═════╝    ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝$(RESET)"
	@echo ""

bonus_art:
	@echo ""
	@echo "$(PAD)$(LRD)███████╗ █████╗  ██████╗████████╗███████╗    ██████╗  ██████╗ ████████╗$(RESET)"
	@echo "$(PAD)$(LRD)██╔════╝██╔══██╗██╔════╝╚══██╔══╝██╔════╝    ██╔══██╗██╔═══██╗╚══██╔══╝$(RESET)"
	@echo "$(PAD)$(RED)█████╗  ███████║██║        ██║   ███████╗    ██████╔╝██║   ██║   ██║   $(RESET)"
	@echo "$(PAD)$(RED)██╔══╝  ██╔══██║██║        ██║   ╚════██║    ██╔══██╗██║   ██║   ██║   $(RESET)"
	@echo "$(PAD)$(RED)██║     ██║  ██║╚██████╗   ██║   ███████║    ██████╔╝╚██████╔╝   ██║   $(RESET)"
	@echo "$(PAD)$(RED)╚═╝     ╚═╝  ╚═╝ ╚═════╝   ╚═╝   ╚══════╝    ╚═════╝  ╚═════╝    ╚═╝   $(RESET)"
	@echo ""


########## Define ANSI escape codes for colors
GREEN	:=	\033[1;32m
YELLOW	:=	\033[33m
LRD		:=	\033[91m
RED		:=	\033[31m
DRD		:=	\033[2;31m
RESET	:=	\033[0m