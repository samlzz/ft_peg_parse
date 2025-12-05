# ftpp — PEG Parser Framework for C++98

`ftpp` is a **generic, modular PEG parser framework** written in **strict C++98**, originally developed for the 42 **webserv** project.
It loads a `.peg` grammar, resolves rule references, parses configuration files or strings, and returns a structured **AST** with precise error reporting.

It is lightweight, dependency-free (except for logging via `ft_log`), and cleanly separates its **public API** from an internal **PEG + packrat engine**.
*Note: the packrat memoization table is not implemented yet; full linear-time parsing is planned.*

## Table of Contents

* [Features](#features)
* [Installation](#installation)

  * [Option 1 — One-line install](#option-1--one-line-install-recommended)
  * [Option 2 — Manual clone](#option-2--manual-clone)
* [Build](#build)

  * [Mandatory configuration](#mandatory-configuration-inside-the-makefile)
  * [Optional configuration](#optional-configuration)
  * [Main targets](#main-build-targets)
  * [Examples](#compiling-and-running-example-programs)
* [Minimal Usage Example](#minimal-usage-example)
* [API Overview](#api-overview)

  * [Grammar](#grammar)
  * [AstNode](#astnode)
  * [Exceptions](#exceptions)
  * [Debug & Logging](#debug--logging)
* [Architecture Overview](#architecture-overview)
* [Roadmap](#roadmap)
* [License](#license)

---

## Features

* ✔ **PEG grammar loader** (`.peg`) with full rule resolution
* ✔ **Recursive-descent parser** with packrat structure
* ✔ **AST generation** with node type, attributes, children, and input spans
* ✔ **Accurate error diagnostics** via typed exceptions
* ✔ **Parse from file or string**
* ✔ **Optional debug modes**: AST, grammar, packrat stats
* ✔ **Built-in logging categories** (lexer, parser, packrat) via `ft_log`
* ✔ **Strict C++98**, no external dependencies
* ✔ **Clean public API**, all internals encapsulated under `include/ftpp/internal/`

---

## Installation

### Option 1 — One-line install (recommended)

Inside your project directory:

```bash
bash <(curl -s https://raw.githubusercontent.com/samlzz/ft_peg_parse/master/sync_ftpp.sh)
```

Installs a clean `ftpp/` directory (or updates an existing one).

---

### Option 2 — Manual clone

```bash
git clone https://github.com/samlzz/ftpp.git
make -C ftpp
```

Include headers:

```cpp
#include "ftpp/Grammar.hpp"
#include "ftpp/AstNode.hpp"
```

To provide logging sources, link final binary against `ft_log`:

```bash
-Lpath/to/ft_log -lftlog
```

---

# Build

The project includes a **fully automated Makefile**, which takes care of source discovery, dependency generation, debug configuration, and optional test programs.
In most cases, building the library is as simple as:

```bash
make
```
This produces `./libftpp.a`

### Mandatory configuration (inside the Makefile)

```make
NAME    ?= libftpp.a
SRC_DIR = src/ftpp/
OBJ_DIR = build/
```

These generally don’t need modification.

---

### Optional configuration

```make
FTLOG_DIR ?= ../ft_log
DEBUG     ?=
```

* `FTLOG_DIR` must point to the ft_log repository.
  If incorrect, the build stops with a helpful message.

* `DEBUG` enables compile-time debug modes.
  Example:

```bash
make DEBUG="AST"
make DEBUG="GRAMMAR PACKRAT"
```
*Note: make sure to clean the existing object files if a previous build did not use these flags.*

**Debug mapping:**

| Value     | Macro generated          |
| --------- | ------------------------ |
| `AST`     | `-DFTPP_DEBUG_AST=1`     |
| `GRAMMAR` | `-DFTPP_DEBUG_GRAMMAR=1` |
| `PACKRAT` | `-DFTPP_DEBUG_PACKRAT=1` |

Enable all debug features:

```bash
make debug
```

Equivalent to:

```bash
make re DEBUG="AST PACKRAT GRAMMAR"
```

---

### Main build targets

| Command       | Description                          |
| ------------- | ------------------------------------ |
| `make`        | Build `libftpp.a`                    |
| `make clean`  | Remove object files                  |
| `make fclean` | Remove objects + library             |
| `make re`     | Rebuild from scratch                 |
| `make debug`  | Rebuild with all debug flags enabled |

---

### Compiling and running example programs

The folder:

```
exemples/
```

contains commented example programs demonstrating AST display, grammar inspection, logging integration, and packrat statistics.

Pas besoin de revoir ton Makefile — j’ai déjà tout ce qu’il faut pour documenter précisément les règles de test.
Voici un **tableau “Test rules”** clair, concis, professionnel, prêt à remplacer le bloc de texte actuel dans ton README.

---

## Test Rules

| Command            | Description                                                                |
| ------------------ | -------------------------------------------------------------------------- |
| `make test`        | Compile **all example programs** from `exemples/` into `test_bin/`.        |
| `make test_run`    | Compile (if needed) **then run all test binaries**.                        |
| `make test_list`   | Compile (if needed ) then list all available test binaries.                |
| `make test_re`     | Rebuild test binaries from scratch (`test_clean` + `test`).                |
| `make test_clean`  | Remove all compiled test binaries from `test_bin/`.                        |
| `make test_<name>` | Build **and run a single test program**. Example: `make test_ast_display`. |

**Notes:**

* Test binaries link automatically against `ft_log` using `TEST_FLAGS`.
* Default test sources are the example programs found in `exemples/` (`ast_display.cpp`, `grammar_stats.cpp`, etc.).
* `TEST_ARGS` controls the grammar + config passed to each test; by default:

  ```
  assets/advanced.peg assets/advanced.conf
  ```
* When `USE_VLA=true`, all test commands run through Valgrind for leak checking.

---

## Minimal Usage Example

```cpp
#include "ftpp/Grammar.hpp"
#include "ftpp/AstNode.hpp"
#include <iostream>

int main() {
    try {
        // Load grammar from .peg file
        Grammar grammar("config.peg", true);

        // Parse input configuration
        AstNode *ast = grammar.parseFile("example.conf", "config");

        // use parsed ast
        delete ast;
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
```

---

## API Overview

### Grammar

Public header: `ftpp/Grammar.hpp`

```cpp
Grammar(const std::string &grammarPath, bool checkLeftRecursion = true);

AstNode *parseFile(const std::string &path);
AstNode *parseFile(const std::string &path, const std::string &rule);

AstNode *parseString(const std::string &input);
AstNode *parseString(const std::string &input, const std::string &rule);
```

Internally, the grammar is compiled once into `Expr` trees, then evaluated through a **packrat parser skeleton**.

---

### AstNode

Public header: `ftpp/AstNode.hpp`

* Stores node type (`type()`), attributes, children, and byte span of matched input
* Supports attribute-based captures (`key:value` in PEG)
* Provides tree printing and analysis when `FTPP_DEBUG_AST=1`

---

### Exceptions

Public header: `ftpp/FtppException.hpp`

* `FtppException` — base class
* `GrammarError`, `GrammarResolutionError`, `GrammarLeftRecursionError`
* `ConfigError` — input does not conform to grammar
* `EngineError` — unexpected internal failure

---

### Debug & Logging

Header: `ftpp/config.h`

Compile-time debug toggles:

* `FTPP_DEBUG_AST`
* `FTPP_DEBUG_GRAMMAR`
* `FTPP_DEBUG_PACKRAT`

Logging categories for `ft_log`:

```cpp
FTPP_LOG_LEXER
FTPP_LOG_PARSER
FTPP_LOG_PACKRAT
```

Example:

```cpp
ft_log::setLevel(ft_log::LOG_DEBUG);
ft_log::enableCategory(FTPP_LOG_LEXER);
```

---

## Architecture Overview

```
ftpp/
├── include/ftpp/
│   ├── Grammar.hpp            # Public API (PIMPL)
│   ├── AstNode.hpp            # AST node structure
│   ├── FtppException.hpp      # Exception hierarchy
│   ├── config.h               # Debug flags + ft_log categories
│   └── internal/              # Engine internals (not public API)
│       ├── GrammarImpl.hpp
│       ├── packrat/           # Packrat parser core
│       ├── peg/               # PEG lexer, parser, expression trees
│       │   ├── core/          # Base classes for expressions
│       │   ├── syntax/        # PEG operators (Sequence, Choice, Capture…)
│       │   └── visitors/      # Grammar analysis & stats
│       └── utils/             # Input, diagnostics, string helpers
└── src/ftpp/
    ├── internal/...           # Implementation
    ├── debug/...              # Debug-only utilities
    ├── AstNode.cpp
    └── Grammar.cpp

assets/                        # Example grammars (.peg) and configs
exemples/                      # Commented example programs
Makefile                       # Automated build & test system
sync_ftpp.sh                   # One-line sync script
```

---

## Roadmap

Planned improvements:

- [ ] **Packrat memoization table** for full linear-time parsing
- [ ] **Optional serialized grammar format** (runtime precompiled grammars)
- [ ] ...

---

## License

This project is licensed under the **GNU LGPL v3**.
You may use it in both open-source and proprietary software, provided modifications to the library itself remain LGPL-compliant.
