#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
int run() {

static int code[]={8,0,3,0,15,36,9,1,0,0,10,10};
static char stab[]={104,101,108,108,111,44,32,119,111,114,108,100,10,0,0,0};
static char data[]={};
static int stack[10000];
static const int _pc[]={&&pc0-&&pc0,&&pc1-&&pc0,&&pc2-&&pc0,&&pc3-&&pc0,&&pc4-&&pc0,&&pc5-&&pc0,&&pc6-&&pc0,&&pc7-&&pc0,&&pc8-&&pc0,&&pc9-&&pc0,&&pc10-&&pc0,&&pc11-&&pc0};
int *pc=code+0, *sp = stack+10000, *bp=sp, a, *t;
pc0:pc++; /*ENT 0*/ pc1:*--sp = (int)bp; bp = sp; sp = sp - *pc++;
pc2:pc++; /*STR 0*/ pc3:a = (int)(stab+*pc++);
pc4:pc++; /*PSH*/ *--sp = a;
pc5:pc++; /*PRTF*/ t = sp + pc[1]; a = printf((char *)t[-1], t[-2], t[-3], t[-4], t[-5], t[-6]);
pc6:pc++; /*ADJ 1*/ pc7:sp = sp + *pc++;
pc8:pc++; /*IMM 0*/ pc9:a = *pc++;
pc10:pc++; /*LEV*/ sp = bp; bp = (int *)*sp++; pc = (int *)*sp++;if (pc==NULL) return 0; else goto *_pc[pc-code];
pc11:pc++; /*LEV*/ sp = bp; bp = (int *)*sp++; pc = (int *)*sp++;if (pc==NULL) return 0; else goto *_pc[pc-code];
}
int main() {
  run();
}
