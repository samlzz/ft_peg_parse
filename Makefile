# =============================================================================
#* >> USER CONFIGURATION (MANDATORY)
# =============================================================================

NAME      ?= libftpp.a
ifeq ($(NAME),)
    $(error NAME is empty: please define executable name)
endif

# ? Directory (end with /)
SRC_DIR   = src/lib/
OBJ_DIR   = build/
BIN_DIR   =

### UFILES_START ###
FILES =	 \
		ast/AstNode.cpp \
		ast/AstNodeDebug.cpp \
		packrat/PackratParser.cpp \
		packrat/PackratParserDebug.cpp \
		peg/PegLexer.cpp \
		peg/PegParser.cpp \
		peg/debug/ExprDebug.cpp \
		peg/debug/GrammarDebug.cpp \
		peg/debug/StatsVisitor.cpp \
		peg/debug/TokenDebug.cpp \
		peg/debug/TreeVisitor.cpp \
		peg/grammar/Grammar.cpp \
		peg/syntax/ExprContainer.cpp \
		peg/syntax/ExprLeaf.cpp \
		peg/syntax/ExprUnary.cpp \
		peg/syntax/RuleRef.cpp \
		utils/DebugLogger.cpp \
		utils/Diag.cpp \
		utils/Input.cpp \
		utils/StringUtils.cpp
### END ###
ifeq ($(FILES),)
    $(error FILES is empty: please define source files)
endif

# =============================================================================
#* >> USER CONFIGURATION (OPTIONAL)
# =============================================================================

AR        = ar rcs

CC        = cc
CFLAGS    = -Wall -Wextra -Werror

CXX       = c++
CXXFLAGS  = -Wall -Wextra -Werror -std=c++98 -g3
DEBUG     ?= 0

INCL_DIRS = include
# ? Directories & Libraries to link against
LIB_DIRS  =
LIB_FILES =

RM = rm -f
MD = mkdir -p

# =============================================================================
# >> COLORS
# =============================================================================

ESC = \033[
NC = $(ESC)0;39m

GRAY = $(ESC)0;90m
RED = $(ESC)0;91m
GREEN = $(ESC)0;92m
YELLOW = $(ESC)0;93m
BLUE = $(ESC)0;94m
MAGENTA = $(ESC)0;95m
CYAN = $(ESC)0;96m
UNDERLINE = $(ESC)4m

define clr_print
	printf "$(1)$(2)$(NC)\n"
endef

# =============================================================================
# >> AUTOMATIC VARIABLES
# =============================================================================
C_FILES   := $(filter %.c,   $(FILES))
C_SRCS    := $(addprefix $(SRC_DIR), $(C_FILES))
C_OBJS    := $(patsubst $(SRC_DIR)%.c,   $(OBJ_DIR)%.o, $(C_SRCS))

CPP_FILES  := $(filter %.cpp, $(FILES))
CPP_SRCS   := $(addprefix $(SRC_DIR), $(CPP_FILES))
CPP_OBJS   := $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(CPP_SRCS))
COMPILE.cpp = 

SRCS      := $(C_SRCS) $(CPP_SRCS)
OBJS      := $(C_OBJS) $(CPP_OBJS)
DEPS      = $(OBJS:.o=.d)
O_DIRS     = $(sort $(dir $(OBJS)))

CPPFLAGS   = $(addprefix -I, $(INCL_DIRS))
LDFLAGS    = $(addprefix -L, $(LIB_DIRS))
LDLIBS     = $(addprefix -l, $(LIB_FILES))

# ? Determin linker
ifeq ($(suffix $(NAME)), .a)
	LD = $(AR)
else ifneq ($(CPP_FILES),)
	LD = $(CXX)
else ifneq ($(C_FILES),)
	LD = $(CC)
else
	$(error Can't determine which linker to use. Please set LD manually.)
endif

OUT := $(if $(BIN_DIR),$(BIN_DIR),./)$(NAME)

export VERBOSE    ?= false
export P := @
ifeq ($(VERBOSE),true)
	P :=
endif

# ? Tests and Debug adjustement
ifneq (,$(filter test%,$(MAKECMDGOALS)))
	INCL_DIRS += src/test
	ifeq ($(DEBUG),0)
		DEBUG=1
	endif
	CXXFLAGS += -DPEG_DEBUG_LEVEL=$(DEBUG)
endif

ifneq (DEBUG,0)
	CXXFLAGS += -DPEG_DEBUG_LEVEL=$(DEBUG)
endif

# =============================================================================
# >> RULES
# =============================================================================

.PHONY: all
all: $(OUT)

$(OUT): $(O_DIRS) $(OBJS)
	$(P)printf "$(GRAY)"
ifneq ($(suffix $(NAME)), .a)
	$(LD) $(LDFLAGS) $(OBJS) -o $(OUT) $(LDLIBS)
else
	$(LD) $(OUT) $(OBJS)
endif
	@$(call clr_print,$(GREEN)$(UNDERLINE),$(NAME) compiled !)

$(O_DIRS):
	$(P)$(MD) $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(P)$(CC) $(CFLAGS) -MMD $(CPPFLAGS) -c $< -o $@
	@$(call clr_print, $(YELLOW),Compiling C: $<)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(P)$(CXX) $(CXXFLAGS) -MMD $(CPPFLAGS) -c $< -o $@
	@$(call clr_print, $(YELLOW),Compiling C++: $<)

-include $(DEPS)

.PHONY: clean
clean:
	$(P)$(RM) $(OBJS) $(DEPS)
	$(P)$(RM) -r $(OBJ_DIR)
	@$(call clr_print,$(BLUE),$(NAME) object files cleaned!)

.PHONY: fclean
fclean: clean
	$(P)$(RM) $(OUT)
	@$(call clr_print,$(CYAN),executables files cleaned!)

.PHONY: re
re: fclean all

.PHONY: run
run: $(OUT)
ifneq ($(suffix $(NAME)), .a)
	$(OUT)
else
	@echo "Nothing to run for a static library: $(OUT)"
endif

# =============================================================================
# >> TESTING
# =============================================================================

USE_VLA	?= false

ifeq ($(USE_VLA),true)
	VLA := valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --track-fds=yes
endif


TEST_DIR := src/test
TEST_OBJ_DIR := build/test
TEST_BIN_DIR := test_bin
TEST_FILES := $(wildcard $(TEST_DIR)/*/*/*_main.cpp $(TEST_DIR)/*/*_main.cpp $(TEST_DIR)/*_main.cpp)

TEST_BINS := $(patsubst %_main.cpp,$(TEST_BIN_DIR)/%,$(notdir $(TEST_FILES)))

.PHONY: test
test: run_all_tests

test_%: $(TEST_BIN_DIR)/_%
	@$(call clr_print, $(CYAN),Running test $* ...)
	$(P)$(VLA) ./$< || { $(call clr_print, $(RED),❌ Test failed: $<); exit 1; }

.PHONY: run_all_tests
run_all_tests: $(TEST_BINS)
	$(P)for bin in $^; do \
		$(call clr_print, $(CYAN),\n\nRunning $$bin ...); \
		$(VLA) ./$$bin || { $(call clr_print, $(RED),❌ $$bin failed); exit 1; } \
	done

$(TEST_BIN_DIR)/%: $(TEST_FILES) $(OUT)
	@mkdir -p $(dir $@)
	$(P)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -o $@ $(OUT)
	@$(call clr_print, $(YELLOW),Compiling test: $<)

.PHONY: test_list
test_list:
	$(P)for t in $(TEST_BIN_DIR)/*; do \
		name="$$(basename "$$t")"; \
		printf "%s\n" "$${name/_/}"; \
	done

.PHONY: test_clean
test_clean:
	$(P)$(RM) -r $(TEST_BIN_DIR)
	@$(call clr_print, $(BLUE),Test binaries cleaned!)
