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

int 	pkseedinit(struct s_pkseed *pks) {
  int 	fs;

  //General Init
  srand(time(NULL));
  gfxInitDefault();
  consoleInit(GFX_BOTTOM, &pks->bot);
  consoleInit(GFX_TOP, &pks->top);

  //Show build time
  printf("PKSeed build: %s %s\n",__DATE__,__TIME__);
  //Filesystem Init
  printf("Init Filesystem...\n");
  fs = filesysInit(&pks->sd.handle, &pks->sav.handle, &pks->sd.arch, &pks->sav.arch);
  if (fs)
    printf("Init FS Failed\n");
  else
    printf("Init FS OK\n");

  //save loading, save is loaded into the array 'save'
  pks->save = malloc(0xEB000);
  pks->game = loadSave(pks->save, &pks->sav.handle, &pks->sav.arch);
  if (pks->game < 0)
    pks->game = loadSave(pks->save, &pks->sd.handle, &pks->sd.arch);
  if (pks->game < 0)
    return -1;

  if (loadSaveInfo(pks->save)) {
    printf("Load Failed, Exiting\n");
    return -1;
  }
  return 0;
}

int 	pkseedexit(struct s_pkseed *pks) {
  consoleSelect(&pks->bot);
  consoleClear();
  printf("\x1B[15;2H");
  printf("Program ended, press A to finish\n");
  waitKey(KEY_A);
  free(pks->save);
  gfxExit();
  filesysExit(&pks->sd.handle, &pks->sav.handle, &pks->sd.arch, &pks->sav.arch);
  return (0);
}

int 	main() {
  struct s_pkseed pks;
  pkseedinit(&pks);
  return pkseedexit(&pks);
}
