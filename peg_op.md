| PEG        | Nom                       | Sous-classe `Expr` | Sémantique                    | Exemple PEG                     |
| ---------- | ------------------------- | ------------------ | ----------------------------- | ------------------------------- |
| `"abc"`    | **Littéral**              | `Literal`          | Match exact d’une chaîne      | `"root"`                        |
| `[a-zA-Z]` | **Classe de caractères**  | `CharClass`        | Match 1 caractère du set      | `[0-9]`                         |
| `A`        | **Référence à une règle** | `RuleRef`          | Appelle la règle `A`          | `path`                          |
| `A B`      | **Séquence**              | `Sequence`         | Match `A` puis `B`            | `"root" _ PATH ";"`             |
| `A / B`    | **Choix (alternance)**    | `Choice`           | Tente `A`, sinon `B`          | `BOOL <- "on" / "off"`          |
| `A*`       | **Répétition 0+**         | `ZeroOrMore`       | Répète tant que succès        | `_ <- (" " / "\n")*`            |
| `A+`       | **Répétition 1+**         | `OneOrMore`        | Comme `*` mais ≥1             | `methods <- METHOD+`            |
| `A?`       | **Optionnel**             | `Optional`         | Tente `A` une fois            | `index <- "index" _ PATH? ";"`  |
| `&A`       | **Lookahead positif**     | `AndPredicate`     | Vérifie `A` sans consommer    | `"server" &"{"`                 |
| `!A`       | **Lookahead négatif**     | `NotPredicate`     | Vérifie que `A` ne matche pas | `!( "}" / ";" ) .`              |
| `(...)`    | **Groupement**            | `Group`            | Sous-expression isolée        | `( "a" / "b" )+`                |
| `@rule`    | **Capture (AST)**         | `Capture`          | Crée un nœud AST              | `@server_block <- "server" ...` |

