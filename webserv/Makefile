NAME		= webServ_0-1

CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror

CXX			+= -std=c++98

ODIR		= obj/

WEBDIR		= core/server/
CNFDIR		= config/
CLIDIR		= core/client/

HDRS		= $(WEBDIR)WebServer \
			$(WEBDIR)helperWebServer \
			$(CNFDIR)Configuration \
			$(CNFDIR)helperConfig \
			$(CNFDIR)helperParser \
			$(CNFDIR)Parser \
			$(CNFDIR)validatorConfig \
			$(CNFDIR)errPages \
			$(CLIDIR)Client

SRCS		= $(WEBDIR)WebServer \
			$(WEBDIR)helperWebServer \
			$(WEBDIR)memberFunctionsWebServer \
			$(WEBDIR)mainLoopWebServer \
			$(CNFDIR)Configuration \
			$(CNFDIR)ConfigurationStreamOut \
			$(CNFDIR)helperConfig \
			$(CNFDIR)helperParser \
			$(CNFDIR)Parser \
			$(CNFDIR)ParserLocation \
			$(CNFDIR)ParserServer \
			$(CNFDIR)validatorConfig \
			$(CNFDIR)errPages \
			$(CLIDIR)Client \
			run

HDR			= $(addsuffix .hpp, $(HDRS))
SRC			= $(addsuffix .cpp, $(SRCS))
OBJ			= $(addprefix $(ODIR), $(notdir $(addsuffix .o, $(SRCS))))

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)%.o: $(WEBDIR)%.cpp $(HDR) | $(ODIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ODIR)%.o: $(CNFDIR)%.cpp $(HDR) | $(ODIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ODIR)%.o: $(CLIDIR)%.cpp $(HDR) | $(ODIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ODIR)%.o: %.cpp $(HDR) | $(ODIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(ODIR)

fclean: clean	
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
