NAME        := irc
CXX         := c++
CXXFLAGS    := -std=c++98 -Wall -Wextra -Werror #-fsanitize=address
SRCS        := $(wildcard *.cpp)
OBJS        := $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re