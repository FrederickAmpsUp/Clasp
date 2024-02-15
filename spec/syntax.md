# Clasp basic syntax

## Program
```
compile: blockStmt EOF
```

## Statements
```
statement: exprStmt | declStmt | ('{' blockStmt '}') | condStmt | returnStmt
blockStmt: stmt*
```
### Return stmt
* Return values from functions
```
returnStmt: 'return' expression? ';'
```

### Expression stmt and expression
* Function calls, assignments, etc.
```
exprStmt:   expression ';'
expression: assignment
assignment: equality assignmentOperator assignment
equality: comparison ('==' | '!=') comparison
comparison:     term ('<' | '>' | '<=' | '>=') term
term:       factor   ('+' | '-')        factor
factor:     exponent ('*' | '/' | '%')  exponent
exponent:   unary    '^'                exponent
unary: (('+' | '-' | '!' | '~') unary) | postfix
postfix: primary ('++' | '--' | ('(' (expression ',')* expression? ')'))*
primary: (number | ('(' expression ')') | varRef)
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

forLoop: 'for' '(' start: statement ' ' cond: expression ';' inc: nonPunctuatedStmt ')' body: statement ->
    blockStmt {
        start
        while (cond) {
            body
            inc
        }
    }
```
* Examples:
```
if (x < 5) print(x);
while (x++ != 27) {
    print(2*x + 1);
}

for (var i = 0; i < 10; i++) {
    print(i);
}
// parses to:
var i = 0;
while (i < 10) {
    print(i);
    i++;
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