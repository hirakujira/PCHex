#ifndef PKSEED_H
#define PKSEED_H

#include <3ds.h>

struct 		s_3fs
{
  Handle  	handle;
  FS_archive 	arch;
};

struct s_pchex
{
  PrintConsole 	top;
  PrintConsole 	bot;
  struct s_3fs  sav;
  struct s_3fs  sd;
  s8 		game;
  u8 		*save;
};

s32 	loadFile(char *path, void *dst, FS_archive *fsarch, Handle *fshdl, u64 maxSize, u32 *bytesRead);
s32 	filesysInit(Handle *, Handle *, FS_archive *, FS_archive *);
s32 	filesysExit(Handle *, Handle *, FS_archive *, FS_archive *);

s32 	loadSaveInfo(u8 *save);
s32 	loadSave(u8 *save, Handle *fshdl, FS_archive *fsarch);

u16   getSID(u8* mainbuf);
u16   getTID(u8* mainbuf);
u16   getTSV(u8* mainbuf);
u32   getSeed(u8* mainbuf, int index);

#endif /* end of include guard: PCHEX_H */
