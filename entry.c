#include "minicrt.h"
extern int main(int argc, char** argv);
void exit(int);

static void crt_fatal_error(const char* msg) {
  printf("fatal error: %s\n", msg);
  exit(1);
}

void mini_crt_entry(void) {
  int ret;
  int argc;
  char** argv;
  char* ebp_reg = 0;
  asm("mov %%rbp, %0 \n" : "=r"(ebp_reg));
  argc = *(int*)(ebp_reg + 4);
  argv = (char**)(ebp_reg + 8);

  if (!mini_crt_init_heap()) {
    crt_fatal_error("heap initialize failed");
  }
  if (!mini_crt_init_io()) {
    crt_fatal_error("io initialize failed");
  }
  ret = main(argc, argv);
  exit(ret);
}

void exit(int exitCode) {
  asm("movq %0, %%rax \n\t"
      "movq $1, %%rbx \n\t"
      "int $0x80      \n\t"
      "hlt            \n\t" ::"m"(exitCode));

}
