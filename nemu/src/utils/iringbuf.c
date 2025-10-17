#include <common.h>
#define iringbuf_size 16

typedef struct {
  word_t pc;
  uint32_t inst;
} iringbuf_t;

iringbuf_t iringbuf[iringbuf_size];

int iringbuf_index = 0;

void iringbuf_push(word_t pc, uint32_t inst) {
  iringbuf[iringbuf_index].pc = pc;
  iringbuf[iringbuf_index].inst = inst;
  iringbuf_index = (iringbuf_index + 1) % iringbuf_size;
}

void iringbuf_print() {
  printf("==== Instruction Ring Buffer ====\n");
  for (int i = 0; i < iringbuf_size; i++) {
    int idx = (iringbuf_index + i) % iringbuf_size;
    printf(FMT_WORD ": %08x\n", iringbuf[idx].pc, iringbuf[idx].inst);
  }
  printf("=================================\n");
}

void iringbuf_reset() {
  iringbuf_index = 0;
  for (int i = 0; i < iringbuf_size; i++) {
    iringbuf[i].pc = 0;
    iringbuf[i].inst = 0;
  }
}

void iringbuf_init() {
  iringbuf_reset();
}

void iringbuf_destroy() {
  // do nothing
}

void iringbuf_test() {
  for (int i = 0; i < 20; i++) {
    iringbuf_push(0x1000 + i * 4, 0xdeadbeef + i);
  }
  iringbuf_print();
}

