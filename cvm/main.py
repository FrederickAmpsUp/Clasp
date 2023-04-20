import time
import subprocess

def bytes_to_num(inp: bytes) -> int:
    sum = 0
    pow = 1
    for b in inp:
        sum += b * pow
        pow *= 256
    return sum

memory = [0] * 1_000_000 # 1 MB memory
registers = [0] * 5      # 5 general purpose registers

# Raised when the program tries to write to read-only memory
class WriteError(Exception):
    def __init__(self, addr):
        self.addr = addr
    def __str__(self):
        return "Write to read-only address " + str(self.addr)

def read(address: int) -> int:
    if address == 0:
        return 0
    if address <= 5:
        return registers[address - 1]
    if address <= 100:
        # devices
        if address == 6:
            return int(input("Program is requesting input >>> "))
    else:
        return memory[(address - 101) % len(memory)]
    
def write(address: int, val: int) -> None:
    if address == 0:
        raise WriteError(0)
    if address <= 5:
        registers[address - 1] = val
    if address <= 100:
        # devices
        if address == 6:
            print(val)
    else:
        memory[(address - 101) % len(memory)] = val

conditions = [
    lambda x, y: x == y,  # Equal to
    lambda x, y: x != y,  # Not equal to
    lambda x, y: x > y,   # Greater than
    lambda x, y: x < y,   # Less than
    lambda x, y: x >= y,  # Greater than or equal to
    lambda x, y: x <= y,  # Less than or equal to
    lambda x, y: True,    # Always true
    lambda x, y: False,   # Always false
]

opcodes = [
    lambda x, y: (0, False),         # Opcode 0 is a NOP
    lambda x, y: (x + y, True),      # Add values and use result
    lambda x, y: (x - y, True),      # Subtract y from x and use result
    lambda x, y: (x * y, True),      # Multiply values and use result
    lambda x, y: (x // y, True),     # Floor divide x by y and use result
    lambda x, y: (x % y, True),      # Get remainder of x divided by y and use result
    lambda x, y: (x ** y, True),     # Raise x to the power of y and use result
    lambda x, y: (abs(x), True),     # Get the absolute value of x and use result
    lambda x, y: (int(x == y), True) # Compare x and y and return 1 if they are equal, 0 otherwise
]



if __name__ == '__main__':
    with open(input("Filename: "), 'rb') as file:
        content = file.read()
        start = 0
        while True:
            opcode = bytes_to_num(content[start:start + 8])
            start += 8
            inp1   = bytes_to_num(content[start:start + 8])
            start += 8
            inp2   = bytes_to_num(content[start:start + 8])
            start += 8
            out    = bytes_to_num(content[start:start + 8])
            start += 8

            cin1   = bytes_to_num(content[start:start + 8])
            start += 8
            cin2   = bytes_to_num(content[start:start + 8])
            start += 8
            cond   = bytes_to_num(content[start:start + 8])
            start += 8
            next   = bytes_to_num(content[start:start + 8])
            start += 8
            fail   = bytes_to_num(content[start:start + 8])

            if (opcode == 9):
                print(inp1)
                write(out, inp1)
            else:
                opc = opcodes[opcode](read(inp1), read(inp2))
                if(opc[1]):
                    write(out, opc[0])

            if (conditions[cond](read(cin1), read(cin2))):
                start = next * 72
            else:
                start = fail * 72
            if start >= len(content): break
            time.sleep(.1)

            