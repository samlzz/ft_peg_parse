| PEG        | Name                           | `Expr` Subclass   | Semantics                                 | PEG Example                           |
| ---------- | ------------------------------ | ----------------- | ----------------------------------------- | ------------------------------------- |
| `"abc"`    | **Literal**                    | `Literal`         | Matches an exact string                   | `"root"`                              |
| `[a-zA-Z]` | **Character class**            | `CharClass`       | Matches one character from the set        | `[0-9]`                               |
| `.`        | **Any character**              | `Any`             | Matches any single character              | `.`                                   |
| `A`        | **Rule reference**             | `RuleRef`         | Calls rule `A`                            | `path`                                |
| `A B`      | **Sequence**                   | `Sequence`        | Matches `A` followed by `B`               | `"root" _ PATH ";"`                   |
| `(...)`    | **Grouping**                   | `Sequence`        | Parenthesized subexpression               | `( "a" / "b" )+`                      |
| `A / B`    | **Choice (alternation)**       | `Choice`          | Tries `A`, otherwise `B`                  | `BOOL <- "on" / "off"`                |
| `A*`       | **Zero-or-more repetition**    | `ZeroOrMore`      | Repeats `A` while it succeeds             | `_ <- (" " / "\n")*`                  |
| `A+`       | **One-or-more repetition**     | `OneOrMore`       | Same as `*` but requires ≥ 1 match        | `methods <- METHOD+`                  |
| `A?`       | **Optional**                   | `Optional`        | Attempts `A` once, succeeds on failure    | `index <- "index" _ PATH? ";"`        |
| `&A`       | **Positive lookahead**         | `AndPredicate`    | Checks `A` without consuming input        | `"server" &"{"`                       |
| `!A`       | **Negative lookahead**         | `NotPredicate`    | Succeeds only if `A` does not match       | `!( "}" / ";" ) .`                    |
| `~A`       | **Fatal error (hard fail)**    | `Fatal`           | On failure, stops backtracking and raises an error | `~directives`                |
| `@rule`    | **Capture (AST node)**         | `Capture`         | Creates an AST node for the rule          | `@server_block <- "server" ...`       |
| `id:value` | **Capture (property)**         | `Capture`         | Adds an attribute to the parent AST node  | `listen <- "listen" listen:...`       |

