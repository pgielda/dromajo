#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <dromajo.h>

int main() {
  void *machine = malloc(0x10000);
   RISCVCPUState * riscv_cpu_state = riscv_cpu_init(machine, 0);
   riscv_cpu_state->mem_map = phys_mem_map_init();
   riscv_cpu_state->physical_addr_len = 40;
   PhysMemoryRange *pr = riscv_cpu_state->mem_map->register_ram(riscv_cpu_state->mem_map, 0, 0x10000, 0x0);
   void *mem = malloc(0x10000);
   pr->phys_mem = mem; // hack some ram

   uint32_t *opcodes = (uint32_t*)mem;
   opcodes[0x0000/4] = 0x1f80006f; // jmp +0x1f8 
   opcodes[0x01f8/4] = 0x00000000;  

   opcodes[0x1000/4] = 0x1f80006f; // jmp +0x1f8
   opcodes[0x11f8/4] = 0x00000093; // li ra,0
   opcodes[0x11fc/4] = 0x2d70106f; // jmp 2cd2
   opcodes[0x2cd2/4] = 0x00000000; 

   riscv_set_pc(riscv_cpu_state, 0x1000);

   while (1) {
     uint64_t pc= riscv_get_pc(riscv_cpu_state);
     printf("\n");
     printf("_____ Executing one instruction pc = %16lX\n", pc);
     printf("\n");
     int result =  riscv_cpu_interp(riscv_cpu_state, 1);
     printf("_____ result = %d\n", result);
   }
}
