SOURCE_FILES = $(wildcard *.cpp)
HEADER_FILES = $(wildcard *.hpp)
OBJECT_FILES = $(addprefix object_files/,$(SOURCE_FILES:.cpp=.o)) 
CC = c++
# CPPFLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -rf

NAME = ircserv  

all: $(NAME)

$(NAME): $(OBJECT_FILES)
	$(CC) $(CPPFLAGS) $^ -o $(NAME)
	
object_files/%.o: %.cpp 
	mkdir -p object_files
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) object_files

fclean: clean
	$(RM) $(NAME) 

re: fclean all