#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <3ds.h>

#include "pchex.h"



s32 	rewritechk(u8 *dec)
{
  u16 	chk = 0;

  for (int i = 8; i < 232; i += 2)
    chk += *(u16 *)(dec + i);
  memcpy(dec + 6, &chk, 2);
  return 0;
}


//switch to a given state
s32	switchState(t_stinf *state, struct s_UIState newst)
{
  state->curState = newst;
  state->curState.initf(state);
  return (0);
}

//Main loop : Executes the current state's functions
//initf : 'init' function
//dispf : 'display' function
//inputf : 'input' function
s32 	startLoop(struct s_pchex *pch)
{
  t_stinf state;

  state.pch = pch;
  //Default state settings
  state.pkmSlot = 0;
  state.cont = 1;
  state.inState = 1;

  //First state is pokemon slot selection
  state.curState = pkmSelectState;
  state.curState.initf(&state);
  // memset(&state.cpy, 0, sizeof(state.cpy));

  //main loop
  while (state.cont > 0 && aptMainLoop())
  {
    hidScanInput();
    state.kPressed = hidKeysDown();
    state.curState.dispf(&state);
    state.curState.inputf(&state);

    gfxFlushBuffers();
    gfxSwapBuffers();
    gspWaitForVBlank();
  }
  if (state.cont == -1)
    return 1;
  return 0;
}
