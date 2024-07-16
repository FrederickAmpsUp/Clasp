# Clasp Lexical Token and Parsing Specification

## Operator tokens
- Operators are declared as follows:  
`opChar | opType | opFollowedByEqualType | doubleOpType`
### Operator table
|  Operator  |  Type  |  EqualsType  |  DoubleType  |
| ---------: | ------ | :------------ | ------------ |
| `+`        | `PLUS` | `PLUS_EQUAL` | `PLUS_PLUS`  |
| `-`        | `MINUS` | `MINUS_EQUAL` | `MINUS_MINUS`  |
| `*`        | `ASTERISK` | `ASTERISK_EQUAL` | None  |
| `/`        | `SLASH` | `SLASH_EQUAL` | None  |
| `:`        | `COLON` | None | None |
| `;`        | `SEMICOLON` | None | None |

TODO: add more operators

- Operators are parsed from source code like so:
```
op, type, equalsType, doubleType := next

if current == op {
    if consume('=') token = (equalsType)
    if consume(op ) token = (doubleType)
    token = (type)
}
```