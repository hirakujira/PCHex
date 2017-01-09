#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <3ds.h>

#include "pkseed.h"

void 	waitKey(u32 keyWait) {
  while (aptMainLoop()) {
    hidScanInput();

    u32 kPressed = hidKeysDown();
    if (kPressed & keyWait) break;

    gfxFlushBuffers();
    gfxSwapBuffers();
    gspWaitForVBlank();
  } 
}

int 	pchexinit(struct s_pchex *pch) {
  int 	fs;

  //General Init
  srand(time(NULL));
  gfxInitDefault();
  consoleInit(GFX_BOTTOM, &pch->bot);
  consoleInit(GFX_TOP, &pch->top);

  //Show build time
  printf("PKSeed build: %s %s\n",__DATE__,__TIME__);
  //Filesystem Init
  printf("Init Filesystem...\n");
  fs = filesysInit(&pch->sd.handle, &pch->sav.handle, &pch->sd.arch, &pch->sav.arch);
  if (fs)
    printf("Init FS Failed\n");
  else
    printf("Init FS OK\n");

  //save loading, save is loaded into the array 'save'
  pch->save = malloc(0xEB000);
  pch->game = loadSave(pch->save, &pch->sav.handle, &pch->sav.arch);
  if (pch->game < 0)
    pch->game = loadSave(pch->save, &pch->sd.handle, &pch->sd.arch);
  if (pch->game < 0)
    return -1;

  if (loadSaveInfo(pch->save)) {
    printf("Load Failed, Exiting\n");
    return -1;
  }
  return 0;
}

int 	pchexexit(struct s_pchex *pch) {
  consoleSelect(&pch->bot);
  consoleClear();
  printf("\x1B[15;2H");
  printf("Program ended, press A to finish\n");
  waitKey(KEY_A);
  free(pch->save);
  gfxExit();
  filesysExit(&pch->sd.handle, &pch->sav.handle, &pch->sd.arch, &pch->sav.arch);
  return (0);
}

int 	main() {
  struct s_pchex pch;
  pchexinit(&pch);
  return pchexexit(&pch);
}
