#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <3ds.h>
#include "pchex.h"

s8	getGame(u32 bytesRead)
{
  u8 game = -1;
  if (bytesRead == 0x6BE00)
  {
    printf("Found S/M save\n");
    game = 0;
  }
  else 
    printf("Found no suitable save\n");
  return game;
}

u16 	ccitt16(u8 *data, u32 len)
{
  u16 	crc = 0xFFFF;
  for (u32 i = 0; i < len; i++)
  {
    crc ^= (u16)data[i] << 8;
    for (int j = 0; j < 8; j++)
      if (crc & 0x8000)
	crc = crc << 1 ^ 0x1021;
      else
	crc = crc << 1;
  }
  return crc;
}

s32 	loadSaveInfo(u8 *save)
{
  printf("Loading information now...\n\n");
  printf("TID: %lu\n",(((u32)(getSaveSID(save) * 65536) + getSaveTID(save)) % 1000000));
  printf("SID: %u\n", getSaveSID(save));
  printf("TSV: %u\n", getSaveTSV(save));
  printf("Current Seed:\n%lx %lx %lx %lx\n", getSeed(save, 3), getSeed(save, 2), getSeed(save, 1), getSeed(save, 0));
  return 0;
}

s32 	loadSave(u8 *save, Handle *fshdl, FS_archive *fsarch)
{
  char 	path[] = "/main";
  u32 	bytesRead;
  s32 	ret;

  printf("Loading savefile...");
  ret = loadFile(path, save, fsarch, fshdl, 0xEB000, &bytesRead); //actually loading the file
  if (ret) return -1;
  printf(" OK, read %ld bytes\n", bytesRead);
  return (getGame(bytesRead)); //we return the which game was found
}

u16 getSaveTID(u8* mainbuf) {
    u16 buffer;
    memcpy(&buffer, &mainbuf[0x01200], 2);
    return buffer;
}

u16 getSaveSID(u8* mainbuf) {
    u16 buffer;
    memcpy(&buffer, &mainbuf[(0x01200) + 2], 2);
    return buffer;
}

u16 getSaveTSV(u8* mainbuf) {
  u16 TID = getSaveTID(mainbuf);
  u16 SID = getSaveSID(mainbuf);
  return (TID ^ SID) >> 4;
}

u32 getSeed(u8* mainbuf, int index) {
    u32 buffer;
    memcpy(&buffer, &mainbuf[(0x6B400) + 0x1DC + index * 0x4], 4);
    return buffer;
}