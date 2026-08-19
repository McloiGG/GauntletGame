NAME = game

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c++17 -g

SRC_DIR = src
SRC_FILES = main.cpp

ENGINE_DIR = engine
ENGINE_FILES = VirtualScreen.cpp

GAME_DIR = game
GAME_FILES =

MODULES = ENGINE GAME

$(foreach M,$(MODULES),\
	$(eval $(M)_SRCS = $(addprefix $($(M)_DIR)/,$($(M)_FILES))))

SRCS = $(addprefix $(SRC_DIR)/,\
	$(SRC_FILES) $(foreach M,$(MODULES),$($(M)_SRCS)))

OBJ_DIR = build
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS = $(OBJS:.o=.d)
DEPFLAGS = -MMD -MP -MF $(@:.o=.d) -MT $@

INCLUDE_DIRS = $(SRC_DIR) include $(RAYLIB_DIR)
IFLAGS = $(addprefix -I,$(INCLUDE_DIRS))

RAYLIB_DIR = vendor/raylib/src
RAYLIB = $(RAYLIB_DIR)/libraylib.a
LFLAGS = $(RAYLIB) -lGL -lm -lpthread -ldl -lrt -lX11

COUNT_FILE = $(OBJ_DIR)/.build_count
TOTAL_FILE = $(OBJ_DIR)/.build_total
BAR_WIDTH = 30
PROGRESS ?= 1

RESET = \033[0m
BOLD = \033[1m
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
ORANGE = \033[38;5;208m
YELLOW_GREEN = \033[38;2;150;255;25m

.DEFAULT_GOAL = all
.DELETE_ON_ERROR:

ifeq ($(PROGRESS),1)
.NOTPARALLEL:
endif

all: $(NAME)
	@rm -f $(COUNT_FILE) $(TOTAL_FILE)

$(NAME): $(OBJS) $(RAYLIB)
	@printf "$(GREEN)Linking $(NAME)...$(RESET)\n"
	@$(CXX) $(OBJS) -o $(NAME) $(LFLAGS)
	@printf "$(BOLD)$(GREEN)Build complete$(RESET)\n"

$(RAYLIB):
	@printf "$(YELLOW)Building raylib (first build only)...$(RESET)\n"
	@$(MAKE) --no-print-directory -s -C $(RAYLIB_DIR) \
		PLATFORM=PLATFORM_DESKTOP RAYLIB_BUILD_MODE=RELEASE

ifeq ($(PROGRESS),1)
$(OBJS): | progress-start
endif

progress-start:
	@mkdir -p $(OBJ_DIR)
	@needed=0; \
	for obj in $(OBJS); do \
		$(MAKE) --no-print-directory PROGRESS=0 -q "$$obj" \
			>/dev/null 2>&1; \
		status=$$?; \
		if [ $$status -eq 1 ]; then \
			needed=$$((needed + 1)); \
		elif [ $$status -eq 2 ]; then \
			needed=$(words $(OBJS)); \
			break; \
		fi; \
	done; \
	if [ $$needed -eq 0 ]; then needed=1; fi; \
	printf "0\n" > $(COUNT_FILE); \
	printf "%s\n" "$$needed" > $(TOTAL_FILE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(IFLAGS) $(DEPFLAGS) -c $< -o $@
ifeq ($(PROGRESS),1)
	@read count < $(COUNT_FILE); \
	read total < $(TOTAL_FILE); \
	count=$$((count + 1)); \
	printf "%s\n" "$$count" > $(COUNT_FILE); \
	percent=$$((count * 100 / total)); \
	filled=$$((count * $(BAR_WIDTH) / total)); \
	empty=$$(($(BAR_WIDTH) - filled)); \
	bar=""; background=""; \
	i=0; \
	while [ $$i -lt $$filled ]; do \
		bar="$${bar}#"; \
		i=$$((i + 1)); \
	done; \
	i=0; \
	while [ $$i -lt $$empty ]; do \
		background="$${background}-"; \
		i=$$((i + 1)); \
	done; \
	if [ $$percent -lt 25 ]; then color="$(RED)"; \
	elif [ $$percent -lt 50 ]; then color="$(ORANGE)"; \
	elif [ $$percent -lt 75 ]; then color="$(YELLOW)"; \
	elif [ $$percent -lt 100 ]; then color="$(YELLOW_GREEN)"; \
	else color="$(GREEN)"; fi; \
	printf "\r$(BOLD)Compiling $(NAME): $$color[%s%s] %3d%%$(RESET) %-34s" \
		"$$bar" "$$background" "$$percent" "$(notdir $<)"; \
	if [ $$count -eq $$total ]; then printf "\n"; fi
else
	@printf "Compiled %s\n" "$(notdir $<)"
endif

run: all
	@./$(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@printf "$(RED)Objects removed$(RESET)\n"

fclean: clean
	@rm -f $(NAME)
	@printf "$(RED)Executable removed$(RESET)\n"

re: fclean
	@$(MAKE) --no-print-directory all

.PHONY: all clean fclean re run progress-start

-include $(DEPS)
