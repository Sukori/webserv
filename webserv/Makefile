NAME		= webServ_0-1

CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror

CXX			+= -std=c++98


ODIR		= obj/

WEBDIR		= core/server/
CNFDIR		= config/
CLIDIR		= core/client/
HTPDIR		= core/http/
CGIDIR		= core/cgi/

HDRS		= $(WEBDIR)WebServer \
			$(CNFDIR)Configuration \
			$(CNFDIR)helperConfig \
			$(CNFDIR)Parser \
			$(CLIDIR)Client \
			$(HTPDIR)Http \
			$(CGIDIR)Cgi \

SRCS		= $(WEBDIR)WebServer \
			$(CNFDIR)Configuration \
			$(CNFDIR)helperConfig \
			$(CNFDIR)Parser \
			$(CLIDIR)Client \
			$(HTPDIR)Http \
			$(CGIDIR)Cgi \
			run \

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
