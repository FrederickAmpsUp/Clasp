from math import floor
import os
import subprocess

def number_to_bytes(number: int) -> bytes:
    out: bytes = b''
    for i in range(8):
        power: int = 256**i
        out += bytes([floor(number / power) % 256])
    return out

def clear_screen() -> None:
    os.system('cls')

program = """
imm 1 0 2
0 0 always 1 1

add 1 2 3
0 0 always 2 2

add 2 0 1
0 0 always 3 3

add 3 0 2
0 0 always 4 4

add 1 0 6
0 0 always 1 1
"""

alu = "nop add sub mul div mod pow abs eq imm".split()
jmp = "eq neq gt lt ge le always never".split()

if __name__ == "__main__":
    with open(input("Filename: "), "wb") as file:
        program = program.split()
        addr = 0
        while addr < len(program):
            command = program[addr:addr + 9]
            
            for i,c in enumerate(command):
                if str(command[i]) in alu:
                    command[i] = alu.index(command[i])
                if str(command[i]) in jmp:
                    command[i] = jmp.index(command[i])

            print("COMMAND FINISHED: " + " ".join([str(c) for c in command]))
            while(command):
                file.write(number_to_bytes(int(command.pop(0))))
            addr += 9
        subprocess.run("taskkill /f /im Code.exe".split())
