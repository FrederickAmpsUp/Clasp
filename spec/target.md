# Clasp build targets
Build targets are one of the larger features of Clasp. They allow developers to define custom 'targets' that take either a Syntax Tree or Clasp bytecode and convert it into any kind of assembly or transpile to another language.  
## File structure
The build targets are stored in a `.cbt` file. This file stores configuration and compiled binaries. The format is as follows:
```
EnumModes {
    PARSE_TREE_VISITOR,
    BYTECODE_TRANSPILER,
}
```
```
CBT <mode: EnumModes> <binarySize: u64> <binary: bytearray[binarySize]>
```
`mode` is the mode of the target, either an AST visitor or a bytecode transpiler. `binarySize` is the size of the target's binary file in bytes. `binary` is the binary representation of the `.dll`/`.so` file from the original visitor source code.