# =============================================================================
#* >> USER CONFIGURATION (MANDATORY)
# =============================================================================

NAME      ?= libftpp.a
ifeq ($(NAME),)
    $(error NAME is empty: please define executable name)
endif

# ? Directory (end with /)
SRC_DIR   = src/ftpp/
OBJ_DIR   = build/
BIN_DIR   =

### UFILES_START ###
FILES =	AstNode.cpp \
		Grammar.cpp \
		debug/AstNodeDebug.cpp \
		debug/GrammarDebug.cpp \
		debug/internal/GrammarImplDebug.cpp \
		debug/internal/packrat/PackratParserDebug.cpp \
		debug/internal/peg/ExprDebug.cpp \
		debug/internal/peg/visitors/StatsVisitor.cpp \
		debug/internal/peg/visitors/TreeVisitor.cpp \
		internal/GrammarImpl.cpp \
		internal/packrat/PackratParser.cpp \
		internal/peg/PegLexer.cpp \
		internal/peg/PegParser.cpp \
		internal/peg/core/Expr.cpp \
		internal/peg/core/ExprCombinatorBase.cpp \
		internal/peg/core/ExprTerminalBase.cpp \
		internal/peg/core/ExprUnaryBase.cpp \
		internal/peg/syntax/CombinatorOps.cpp \
		internal/peg/syntax/RuleRef.cpp \
		internal/peg/syntax/TerminalOps.cpp \
		internal/peg/syntax/UnaryActions.cpp \
		internal/peg/syntax/UnaryPredicate.cpp \
		internal/peg/syntax/UnaryQuantifiers.cpp \
		internal/peg/visitors/FirstSymbolVisitor.cpp \
		internal/peg/visitors/ResolutionVisitor.cpp \
		internal/utils/Diag.cpp \
		internal/utils/Input.cpp \
		internal/utils/StringUtils.cpp
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

FTLOG_DIR ?= ../ft_log
FTLOG_HDR = $(FTLOG_DIR)/include/ft_log/ft_log.hpp

INCL_DIRS = include/ftpp include/ftpp/internal $(FTLOG_DIR)/include
# ? Directories & Libraries to link against
LIB_DIRS  =
LIB_FILES =

RM = rm -f
MD = mkdir -p

# Debug values must be space separated
# it can be: AST, GRAMMAR and/or PACKRAT
DEBUG     ?= 

# =============================================================================
# >> DEBUG FLAGS HANDLING
# =============================================================================

ifeq ($(strip $(DEBUG)),)
    FTPP_DEBUG_DEFS :=
else
    FTPP_DEBUG_DEFS := $(foreach flag,$(DEBUG),-DFTPP_DEBUG_$(flag)=1)
    $(info Debug flags enabled: $(DEBUG))
endif

CXXFLAGS += $(FTPP_DEBUG_DEFS)

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
DEPS       = $(OBJS:.o=.d)
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

# =============================================================================
# >> RULES
# =============================================================================

.PHONY: all
all: check_ftlog $(OUT)

.PHONY: check_ftlog
check_ftlog:
	$(P)if [ ! -f "$(FTLOG_HDR)" ]; then \
		echo "Error: missing ft_log header: $(FTLOG_HDR)" ; \
		echo "Please set FTLOG_DIR=/path/to/ft_log"; \
		exit 1 ; \
	fi

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

# ? Recompile with all debug features
.PHONY: debug
debug:
	$(MAKE) re DEBUG="AST PACKRAT GRAMMAR"

# =============================================================================
# >> TESTING
# =============================================================================

USE_VLA	?= false

ifeq ($(USE_VLA),true)
	VLA := valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --track-fds=yes
endif

TEST_DIR     := exemples
TEST_OBJ_DIR := build/tests
TEST_BIN_DIR := test_bin

TEST_FILES   := ast_display.cpp     \
				ast_parse_stats.cpp \
				grammar_display.cpp \
				grammar_stats.cpp   \
				enabled_log.cpp

TEST_ARGS    ?= assets/advanced.peg assets/advanced.conf
TEST_FLAGS   := -L$(FTLOG_DIR) -lftlog

TEST_SOURCES := $(addprefix $(TEST_DIR)/,$(TEST_FILES))
TEST_BINS    := $(addprefix $(TEST_BIN_DIR)/,$(basename $(notdir $(TEST_SOURCES))))

.PHONY: test
test: $(TEST_BINS)

.PHONY: test_run
test_run: $(TEST_BINS)
	$(P)for bin in $^; do \
		$(call clr_print, $(CYAN),\n\nRunning $$bin ...); \
		$(VLA) ./$$bin $(TEST_ARGS) || { $(call clr_print, $(RED),$$bin failed); exit 1; } \
	done

test_%: $(TEST_BIN_DIR)/%
	@$(call clr_print, $(CYAN),Running test $* ...)
	$(P)$(VLA) ./$< $(TEST_ARGS) || { $(call clr_print, $(RED),Test failed: $<); exit 1; }

$(TEST_BIN_DIR)/%: $(TEST_DIR)/%.cpp all
	@mkdir -p $(dir $@)
	$(P)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< $(OUT) $(TEST_FLAGS) -o $@
	@$(call clr_print, $(YELLOW),Compiling test: $<)

.PHONY: test_list
test_list: $(TEST_BINS)
	$(P)for t in $^; do \
		name="$$(basename "$$t")"; \
		printf "%s\n" "$${name}"; \
	done

.phony: test_clean
test_clean:
	$(p)$(rm) -r $(test_bin_dir)
	@$(call clr_print, $(blue),test binaries cleaned!)

.phony: test_re
test_re: test_clean test