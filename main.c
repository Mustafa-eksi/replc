#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#define CMD_CAP 256
#define ENDING_LEN 11
#define LINELEN 256

const char * filepath = "repl.c";
char cmdBuffs[LINELEN][CMD_CAP];
char libBuffs[LINELEN][CMD_CAP];
int cbp = 0;
int lbp = 0;

void assignArray(char dest[CMD_CAP], char src[CMD_CAP]) {
  for(int i = 0; i < CMD_CAP; i++) {
    dest[i] = src[i];
  }
}

void addLib(char cmdBufs[LINELEN][CMD_CAP], char libLine[CMD_CAP]) {
  assert(cbp < LINELEN);
  bool move = false;
  char buf[CMD_CAP] = {0};
  char buf2[CMD_CAP] = {0};
  for(int i = 0; i < cbp; i++) {
    if(strncmp(cmdBufs[i], "#", 1) == 0) {
      continue;
    }else if(!move) {
      move = true;
      assignArray(buf, cmdBufs[i]);
      assignArray(cmdBufs[i], libLine);
    } else {
      assignArray(buf2, cmdBufs[i]);
      assignArray(cmdBufs[i], buf);
      assignArray(buf, buf2);
    }
  }
}

void run(char cmdBufs[LINELEN][CMD_CAP], char libBufs[LINELEN][CMD_CAP]) {
  FILE *fl = fopen(filepath, "w+");
  fprintf(fl, "#include <stdio.h>\n");
  for(int i = 0; i < lbp; i++) {
    fprintf(fl, libBufs[i]);
  }
  fprintf(fl, "int main(void) {\n");
  for(int i = 0; i < cbp; i++) {
    fwrite(cmdBufs[i], 1, strlen(cmdBufs[i]), fl);
  }
  fprintf(fl, "return 0;}");
  fclose(fl);
  char cb[256] = {0};
  sprintf(cb, "tcc -run %s", filepath);
  system(cb);
  remove(filepath);
}

void reset() {
  for (int i = 0; i < cbp; i++) {
    for(int j = 0; j < CMD_CAP; j++) {
      cmdBuffs[i][j] = (char) 0;
    }
  }
}

int main(void) {
  while(true) {
    printf("-> ");
    char TmpBuff[CMD_CAP] = {0};
    read(0, TmpBuff, CMD_CAP);
    if(strncmp(TmpBuff, "quit", 4) == 0){
      return 0;
    }else if(strncmp(TmpBuff, "reset", 5) == 0) {
      reset();
    }else if (strncmp(TmpBuff, "#", 1) == 0) {
      assignArray(libBuffs[lbp++], TmpBuff);
      run(cmdBuffs, libBuffs);
    }else {
      assignArray(cmdBuffs[cbp++], TmpBuff);
      run(cmdBuffs, libBuffs);
    }
  }
  return 0;
}
