## Wut

This is a virtual machine for a very simple architecture. It simulates the behavior of computer hardware.

The device we’re simulating is much simpler than a modern CPU; it has:

- **20 bytes of memory**, simulated by an array with length 20
- **3 registers**: 2 general purpose register and 1 for the “program counter”
- **5 instructions**: load word, store word, add, subtract and halt
