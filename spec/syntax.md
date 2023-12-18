# Clasp basic syntax

## Program
```
compile: blockStmt EOF
```

## Statements
```
statement: exprStmt | declStmt | '{' blockStmt '}'
blockStmt: stmt*
```
### Expression stmt
* Function calls, assignments, etc.
```
exprStmt:   expression ';'
expression: assignment
assignment: term     assignmentOperator assignment
term:       factor   ('+' | '-')        factor
factor:     exponent ('*' | '/' | '%')  exponent
exponent:   primary  '^'                exponent
primary: number | fnCall | ('(' expression ')') | varRef
fnCall: identifier '(' (expression ',')* ')'
varRef: identifier
```
* Examples:
```
x = x + 1;
foo();
y = bar();
x = y - foo(y, x, 25);
```
### Declaration stmt
* Variable/Function/Struct (future) declaration
```
declStmt: (varDecl | letDecl | constDecl | fnDecl) ';'
varDecl:   'var'   identifier  ':' typeName ( '=' expression)?
letDecl:   'let'   identifier  ':' typeName   '=' expression
constDecl: 'const' identifier (':' typeName)? '=' expression

fnDecl: 'fn' identifier '(' (identifier ':' typeName ('=' expression)? ',')* ')' '->' typeName '{' statement* '}'
```
* Examples:
```
var x: int = 0;
    // Mutable variables may not be assigned on declaration
var y: float;

    // Immutable variables must be assigned on declaration
let foo: float = 8.6 / 5;

const pi = 3.14159265; // Type inferenced as float
const sqrt4: float = 2; // Inference of int overridden by definition as float

fn foo(a: float, b: int) -> float {
    return a * b;
}
```

## Type names
TODO: tuples or similar
```
typeName: (arrayType | fnType | singleType | templateType | ptrType)
arrayType: '[' singleType ']'
fnType: '(' (typeName ',')* ')' '->' '{' typeName '}'
singleType: identifier
templateType: identifier '[' typeName ']'
ptrType: '>' typeName
```
* Examples:
```
int
float

[int] // list of int

(int, float) -> { [float] } // function that takes an int and a float and produces a list of float
vector[float] // template class
>vector[int] // pointer to vector of int
```