import struct
from sys import argv

if __name__ == '__main__':
    if len(argv) < 4:
        print(f"Usage: {argv[0]} <output> <mode> <binary>")
    fname = argv[1]
    with open(fname or "a.out", 'wb') as f:
        f.write(b'CBT')
        typ = argv[2]
        if typ == "visitor" or typ == "":
            f.write(b'\x00') # see spec/target.md, EnumModes.PARSE_TREE_VISITOR
        elif typ == "transpiler":
            f.write(b'\x01') # see spec/target.md, EnumModes.BYTECODE_TRANSPILER
        else:
            print(f"Error: unknown target mode {typ}")
        
        binary = argv[3]

        size = -1
        data = b''
        with open(binary, 'rb') as binf:
            data = binf.read()
            size = len(data)
        
        f.write(struct.pack("Q", size))
        f.write(data)