import struct

if __name__ == '__main__':
    fname = input("Output filename? (default: a.out) ")
    with open(fname or "a.out", 'wb') as f:
        f.write(b'CBT')
        typ = input("AST visitor or bytecode transpiler? ('visitor'/'transpiler', default: visitor) ")
        if typ == "visitor" or typ == "":
            f.write(b'\x00') # see spec/target.md, EnumModes.PARSE_TREE_VISITOR
        elif typ == "transpiler":
            f.write(b'\x01') # see spec/target.md, EnumModes.BYTECODE_TRANSPILER
        else:
            print(f"Error: unknown target mode {typ}")
        
        binary = input("Compiled binary file? ")

        size = -1
        data = b''
        with open(binary, 'rb') as binf:
            data = binf.read()
            size = len(data)
        
        f.write(struct.pack("Q", size))
        f.write(data)