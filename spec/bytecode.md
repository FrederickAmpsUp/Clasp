# Clasp bytecode

## Overview
Clasp bytecode files end with `.clb` and store code and name info. They can be used as libraries or intermediate files in the compilation process.

Clasp Bytecode uses a basic **stack-based** VM. It's designed to closely follow hardware assembly so that it's easier to convert to actual machine code.

Bytecode uses an "address table" to store function addresses **relative to the start of the code section of the bytecode file**. The address table holds `u64`s. It's used when jumping to make branch patching easier. It's also used to jump to system assembly. When inserting system assembly, it's platform specific. However, `.clb` files are platform independent.  
When jumping to assembly, there's an intermediate function inserted that loads the correct assembly for the current platform from the `.clb` file to memory, then jumps to that address.

Aditionally, the VM holds a "symbol table" that stores function locations **in memory** and **by name**. Each `.clb` file contains its own symbol table specifying function names and locations **in that file's code section**.

## File structure
```
CLB
<startAddr: i64> // Start address (address table index). If this is -1, the file is marked as 'not runnable' and is used as a library.
<aLen: u64> <aTable: [u64]>  // Address table
<sTable: hashmap[str, u64]>  // Symbol table
<codeSize: u64> <code: [u8]> // Code section

```

## Opcodes
<!-- this sucked to make -->
<!-- also this comment syntax is terrible -->
| Opcode name | Arguments | Description | Net stack | Output |
| ----: | :------- | :-------- | ----------:       | :----- |
| **Section:** |**Math** | Opcodes for performing math ops | `N/A` | `N/A` |
| `mathbb` | `op: Operation`  | Perform a math op on 2 bytes from the stack | `-1` | `byte` | 
| `mathbw` | `op: Operation`  | Perform a math op on a byte and a word from the stack | `-1` | `word` |
| `mathbd` | `op: Operation` | Perform a math op on a byte and a dword from the stack | `-1` | `dword` |
| `mathbq` | `op: Operation` | Perform a math op on a byte and a qword from the stack | `-1` | `qword` |
| `mathwb` | `op: Operation` | Perform a math op on a word and a byte from the stack | `-1` | `word` |
| `mathww` | `op: Operation` | Perform a math op on 2 words from the stack | `-2` | `word` |
| `mathwd` | `op: Operation` | Perform a math op on a word and a dword from the stack | `-2` | `dword` |
| `mathwq` | `op: Operation` | Perform a math op on a word and a qword from the stack | `-2` | `qword` |
| `mathdb` | `op: Operation` | Perform a math op on a dword and a byte from the stack. | `-1` | `dword` |
| `mathdw` | `op: Operation` | Perform a math op on a dword and a word from the stack. | `-2` | `dword` |
| `mathdd` | `op: Operation` | Perform a math op on 2 dwords from the stack. | `-2` | `dword` |
| `mathdq` | `op: Operation` | Perform a math op on a dword and a qword from the stack. | `-4` | `qword` |
| `mathqb` | `op: Operation` | Perform a math op on a qword and a byte from the stack. | `-1` | `qword` |
| `mathqw` | `op: Operation` | Perform a math op on a qword and a word from the stack. | `-2` | `qword` |
| `mathqd` | `op: Operation` | Perform a math op on a qword and a dword from the stack. | `-4` | `qword` |
| `mathqq` | `op: Operation` | Perform a math op on 2 qwords from the stack. | `-8` | `qword` |
| **Section:** | **Constants** | Opcodes for loading constant values onto the stack. | `N/A` | `N/A` |
| `constb` | `val: u8` | Push a byte constant to the stack. | `+1` | `byte` |
| `constw` | `val: u16` | Push a word constant to the stack. | `+2` | `word` |
`constd` | `val: u32` | Push a dword constant to the stack. | `+4` | `dword` |
| `constq` | `val: u64` | Push a qword constant to the stack. | `+8` | `qword` |
| *TODO* | Constant table | Load constants from a table in the bytecode file. | `+1-+8` | `any` |
| **Section:** | **Branching** | Jump to different addresses in the `code` section. | `N/A` | `N/A` |
| `jmp` | `addr: u64` | Jump to the `code` section at address `atable@addr` | `0` | `N/A` |
| `jsym` | `symb: ptr` | Jump to a symbol in the VM's symbol table. `symb` is a C-string holding the symbol name. | `0` | `N/A`
| **Section:** | **Util** | Utility opcodes | `N/A` | `N/A`
| `asm` | `addr: u64` | Jump to system assembly (`atable@addr`) | `N/A` | `N/A` |