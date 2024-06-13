_BOLD=$'\033[1m
_UNDER=$'\033[4m
_REV=$'\033[7m
_END=$'\033[0m

# Colors
_GREY=$'\033[30m
_RED=$'\033[31m
_GREEN=$'\033[32m
_YELLOW=$'\033[33m
_BLUE=$'\033[34m
_PURPLE=$'\033[35m
_CYAN=$'\033[36m
_WHITE=$'\033[37m

# Inverted, i.e. colored backgrounds
_IGREY=$'\033[40m
_IRED=$'\033[41m
_IGREEN=$'\033[42m
_IYELLOW=$'\033[43m
_IBLUE=$'\033[44m
_IPURPLE=$'\033[45m
_ICYAN=$'\033[46m
_IWHITE=$'\033[47m

SRC = \
	src/main.cpp \
	src/init.cpp \
	src/manageClient.cpp \
	src/startServer.cpp \
	src/Server.cpp \
	src/handleMessage.cpp \
	src/server/handleChannel.cpp \

INCLUDES = \
	-I ./include/ \

OBJ = $(SRC:%.cpp=$(BUILD_DIR)%.o)
BUILD_DIR = ./build/
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98
NAME = ircserv
BOOL_EXEC = 0
EXEC = ./$(NAME)
IS_COMPILING = 0

all: $(NAME)

$(NAME): $(OBJ)
	@echo
	@echo "$(_BOLD)$(_CYAN)Compiling ($(_RED)$@$(_CYAN))$(_END)"
	@echo
	@echo " $(_BOLD)$(_RED)$(NAME) : $(_END)$(_CYAN) $(CXX) $(OBJ) $(_END)"
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)
	@echo
	@ \
	if [ $(BOOL_EXEC) -eq 1 ]; then \
		@echo "$(_BOLD)$(_CYAN)Executing...$(_END)"; \
		@echo; \
		$(EXEC); \
	fi

$(BUILD_DIR)%.o: %.cpp
	@ \
	mkdir -p $(@D); \
	if [ $(IS_COMPILING) -eq 0 ]; then \
		echo; \
		echo "$(_BOLD)$(_CYAN)Entering ($(_RED)./src$(_CYAN))$(_END)"; \
		echo; \
	fi
	@$(eval IS_COMPILING = 1)
	@echo "$(_BOLD)$(_RED) $< : $(_END) $(_CYAN) $(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ $(_END)"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "$(_BOLD)$(_CYAN)Cleaning...$(_END)"
	@rm -rf $(BUILD_DIR)
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re valgrind
