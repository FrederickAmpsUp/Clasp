# Clasp operator precedence


| Precedence | Operators | Descriptions | Associativity |
| ----: | :---- | :---- | :--- |
| 1  | `- +` <br> `! ~`  | Unary negation/absolution <br>Logical/bitwise NOT | Right
| 2  | `^` | Exponentiation | Right
| 3  | `* / %` | Factor and modulo | Left
| 4  | `+ -`   | Terms | Left