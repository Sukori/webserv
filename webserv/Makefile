NAME		= webServ_0-1

CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror

CXX			+= -std=c++98


ODIR		= obj/

WEBDIR		= core/server/
CNFDIR		= config/
CLIDIR		= core/client/

HDRS		= $(WEBDIR)WebServer \
			$(CNFDIR)Configuration \
			$(CNFDIR)helperConfig \
			$(CLIDIR)Client

SRCS		= $(WEBDIR)WebServer \
			$(CNFDIR)Configuration \
			$(CNFDIR)helperConfig \
			$(CLIDIR)Client \
			run

HDR			= $(addsuffix .hpp, $(HDRS))
SRC			= $(addsuffix .cpp, $(SRCS))
OBJ			= $(addsuffix .o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(ODIR)%.o: $(WEBDIR)%.cpp $(CNFDIR)%.cpp $(CLIDIR)%.cpp $(HDR) | $(ODIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ODIR):
	mkdir -p $(ODIR)

clean:
	rm -rf $(ODIR)

fclean: clean	
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
