# Clasp basic syntax

## Program
```
compile: blockStmt EOF
```

## Statements
```
statement: exprStmt | declStmt | ('{' blockStmt '}') | condStmt
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
primary: (number | fnCall | ('(' expression ')') | varRef) ('++' | '--')?
fnCall: identifier '(' (expression ',')* expression? ')'
varRef: identifier
```
* Examples:
```
x = x + 1;
foo();
y = bar();
x = y - foo(y, x, 25);
y = foo(5,);

x++;
```
### Declaration stmt
* Variable/Function/Struct (future) declaration
```
declStmt: (varDecl | letDecl | constDecl | fnDecl) ';'
varDecl:   'var'   identifier((':' typeName)|('=' expression)) | (':' typeName '=' expression)
letDecl:   'let'   identifier (':' typeName)? '=' expression
constDecl: 'const' identifier (':' typeName)? '=' expression

fnDecl: 'fn' identifier '(' (identifier ':' typeName ('=' expression)? ',')* ')' '->' typeName statement
```
* Examples:
```
var x: int = 0;
    // Mutable variables may not be assigned on declaration
var y: float;
var x = 2.5;

    // Immutable variables must be assigned on declaration, may be type inferenced
let foo: float = 8.6 / 5;
let bar = 5;

const pi = 3.14159265; // Type inferenced as float
const sqrt4: float = 2; // Inference of int overridden by definition as float

fn foo(a: float, b: int) -> float {
    return a * b;
}
```

### Conditional stmt
* If/While statement
* For loops are parsed into a block of statements with a while
```
condStmt: (ifStmt | whileStmt)
ifStmt: 'if' '(' expression ')' statement
whileStmt: 'while' '(' expression ')' statement
```
* Examples:
```
if (x < 5) print(x);
while (x++ != 27) {
    print(2*x + 1);
}
```

## Type names
TODO: tuples or similar
```
typeName: (arrayType | fnType | singleType | templateType | ptrType)
arrayType: '[' singleType ']'
fnType: '(' (typeName ',')* ')' '->' '{' typeName '}'
singleType: identifier
templateType: identifier '[' (typeName ',')* typeName? ']'
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