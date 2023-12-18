# Clasp operator precedence


| Precedence | Operators | Descriptions | Associativity |
| ----: | :---- | :---- | :--- |
| 1  | `++ --` | Postfix increment/decrement | Left
| 2  | `- +` <br> `! ~`  | Unary negation/absolution <br>Logical/bitwise NOT | Right |
| 3  | `^` | Exponentiation | Right |
| 4  | `* / %` | Factor and modulo | Left |
| 5  | `+ -`   | Terms | Left |
| 6  | `=` <br> `+= -= *= /= ^= %=` | Assignment <br> Compound assignment | Right