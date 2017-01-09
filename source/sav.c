#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <3ds.h>
#include "pkseed.h"

s8	getGame(u32 bytesRead) {
  u8 game = -1;
  if (bytesRead == 0x6BE00) {
    printf("Found S/M save\n");
    game = 0;
  }
  else {
    printf("Found no suitable save\n");
  }
  return game;
}

s32 	loadSaveInfo(u8 *save) {
  printf("Loading information now...\n\n");
  printf("TID: %lu\n",(((u32)(getSID(save) * 65536) + getTID(save)) % 1000000));
  printf("SID: %u\n", getSID(save));
  printf("TSV: %u\n\n", getTSV(save));
  printf("Current Seed:\n%lx %lx %lx %lx\n", getSeed(save, 3), getSeed(save, 2), getSeed(save, 1), getSeed(save, 0));
  return 0;
}

s32 	loadSave(u8 *save, Handle *fshdl, FS_archive *fsarch) {
  char 	path[] = "/main";
  u32 	bytesRead;
  s32 	ret;

  printf("Loading savefile...");
  ret = loadFile(path, save, fsarch, fshdl, 0xEB000, &bytesRead); //actually loading the file
  if (ret) return -1;
  printf(" OK, read %ld bytes\n", bytesRead);
  return (getGame(bytesRead)); //we return the which game was found
}

u16 getTID(u8* mainbuf) {
    u16 buffer;
    memcpy(&buffer, &mainbuf[0x01200], 2);
    return buffer;
}

u16 getSID(u8* mainbuf) {
    u16 buffer;
    memcpy(&buffer, &mainbuf[(0x01200) + 2], 2);
    return buffer;
}

u16 getTSV(u8* mainbuf) {
  u16 TID = getTID(mainbuf);
  u16 SID = getSID(mainbuf);
  return (TID ^ SID) >> 4;
}

u32 getSeed(u8* mainbuf, int index) {
    u32 buffer;
    memcpy(&buffer, &mainbuf[(0x6B400) + 0x1DC + index * 0x4], 4);
    return buffer;
}