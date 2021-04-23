#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#define EXIT_SUCCESS 0

typedef enum {
  LOAD_WORD = 0x01,
  STORE_WORD = 0x02,
  ADD = 0x03,
  SUB = 0x04,
  HALT = 0xff,
} opcode_t;

typedef enum {
  PROGRAM_COUNTER = 0x00,
  REGISTER_1 = 0x01,
  REGISTER_2 = 0x02,
} cpu_register_t;

void executeProgram(uint8_t mem[]);

int main(void) {
  /*
   * NOTE: These test cases are shamelessly stolen from the solution, but NONE 
   * of the implementation details were taken from the solution.
   */
  // 255 + 3 = 258
  uint8_t mem1[20] = {
      0x01, 0x01, 0x10,  // 0x00: load A 0x10
      0x01, 0x02, 0x12,  // 0x03: load B 0x12
      0x03, 0x01, 0x02,  // 0x06: add A B
      0x02, 0x01, 0x0e,  // 0x09: store A 0x0e
      0xff,              // 0x0c: halt
      0x00,              // 0x0d: <<unused>>
      0x00, 0x00,        // 0x0e: output
      0xff, 0x00,        // 0x10: input X = 255
      0x03, 0x00         // 0x12: input Y = 3
  };

  executeProgram(mem1);
  printf("Testing 255 + 3 = 258\n");
  assert(mem1[0x0e] == 2 && mem1[0x0f] == 1);

  // 256 - 3 = 253
  uint8_t mem2[20] = {
      0x01, 0x01, 0x10,  // 0x00: load A 0x10
      0x01, 0x02, 0x12,  // 0x03: load B 0x12
      0x04, 0x01, 0x02,  // 0x06: sub A B
      0x02, 0x01, 0x0e,  // 0x09: store A 0x0e
      0xff,              // 0x0c: halt
      0x00,              // 0x0d: <<unused>>
      0x00, 0x00,        // 0x0e: output
      0x00, 0x01,        // 0x10: input X = 256
      0x03, 0x00         // 0x12: input Y = 3
  };

  executeProgram(mem2);
  printf("Testing 256 - 3 = 253\n");
  assert(mem2[0x0e] == 253 && mem2[0x0f] == 0);

  printf("OK\n");

  return EXIT_SUCCESS;
}

void executeProgram(uint8_t mem[]) {
  // [program_counter, register_1, register_2]
  uint16_t registers[3] = {0, 0, 0};

  for (;;) {
    // fetch
    opcode_t opcode = mem[registers[PROGRAM_COUNTER]];

    if (opcode == HALT) return;

    // All instructions are 3 bytes, with the latter 2 bytes being arguments
    uint16_t arg1 = mem[registers[PROGRAM_COUNTER] + 1];
    uint16_t arg2 = mem[registers[PROGRAM_COUNTER] + 2];
    registers[PROGRAM_COUNTER] += 3;

    // decode and execute
    switch (opcode) {
      // load_word  reg (addr)  # Load value at given address into register
      case LOAD_WORD: {
        // store little-endian 2 byte int in target register
        registers[arg1] = mem[arg2] | (mem[arg2 + 1] << 8);
        break;
      }
      // store_word reg (addr)  # Store the value in register at the given
      // address
      case STORE_WORD: {
        // // bit mask to remove the first 8 bits
        mem[arg2] = registers[arg1] & 0x00FF;
        // // shift to get last 8 bits
        mem[arg2 + 1] = registers[arg1] >> 8;
        break;
      }
      // add reg1 reg2          # Set reg1 = reg1 + reg2
      case ADD: {
        registers[arg1] += registers[arg2];
        break;
      }
      // sub reg1 reg2          # Set reg1 = reg1 - reg2
      case SUB: {
        registers[arg1] -= registers[arg2];
        break;
      }
      case HALT: {
        return;
      }
    }
  }
}
