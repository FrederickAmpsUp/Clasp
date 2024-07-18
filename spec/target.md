# Clasp Build Target file specification

## Header
- Header consists of a magic string followed by some metadata
```
MAGIC : "CBT "
    - 4 characters, end with ' ' (0x20)

VERSION : u8, 000 for this specification

METADATA : {
    TYPE : enum (u8) {
        AST_VISITOR = 0
         // TODO - bytecode transpiler support
    }
}
```
## Library
- FULLY TODO