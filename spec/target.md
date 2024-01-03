# Clasp build targets
Build targets are one of the larger features of Clasp. They allow developers to define custom 'targets' that take either a Syntax Tree or Clasp bytecode and convert it into any kind of assembly or transpile to another language.  
## File structure
The build targets are stored in a `.cbt` file. This is a zipped folder (ZIP format) that contains the target's libraries, stdlib (if applicable), configuration, and any binaries.

* Directory structure:

```
- root.cbt/

        This is the overall configuration that tells Clasp how to use the build target.
    - config.json

        This directory contains the code that translates the AST or bytecode into the output format.
    - lib/
            AST visitor
        - visitor.(dll/so)
            OR
            Bytecode transpiler (to assembly)
        - transpiler.(dll/so)

        This directory contains any compiler/assembly binaries necessary.
    - bin/
        ...

        This directory contains any custom standard library files for the target.
    - include/
        <standard library filename>.csp
```

* `*.cbt` is unzipped into a temp directory (`/tmp/` on Unix and `%TEMP%` on win32)
* Configuration structure:

```
- "tranlaterType"
    - The type of translater to use, must be either "visitor" or "transpiler"
- "preprocessorCmd"
    - The preprocesser command to use. The command is run in the bin/ directory, and may be a local command. Insert '%s' where the input filename should be placed. This may be "" for no preprocessing.
- "preprocessorOutput"
    - The output file from the preprocesser, or "" for no preprocessing.
- "postprocessorCmd"
    - The postprocessor command to use. The command is run in the bin/ directory, and may be a local command. Insert '%s' where the input filename should be placed. This may be "" for no postprocessing.
- "postprocesserOutput"
    - The output file from the postprocesser, or "" for no postprocessing.
```