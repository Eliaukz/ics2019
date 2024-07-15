#include "cpu/exec.h"


// compute.c
make_EHelper(lui);
make_EHelper(auipc);
make_EHelper(imm);
make_EHelper(reg);

// control.c
make_EHelper(jal);
make_EHelper(jalr);
make_EHelper(branch);

// special.c
make_EHelper(inv);
make_EHelper(nemu_trap);


//load store  ldst.c
make_EHelper(ld);
make_EHelper(st);
make_EHelper(lh);
make_EHelper(lb);



make_EHelper(sys);
