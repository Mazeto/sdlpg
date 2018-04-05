#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <limits.h>

#define RND() random()

#ifdef SKIP
: ${CC=gcc}
  $CC `pkg-config --cflags sdl2` \
      -o rndwlkr rndwlkr.c        \
      `pkg-config --libs sdl2`
  exit 0
#endif

struct {
  uint16_t w;
  uint16_t h;
} std = {
  .w = 1024,
  .h =  512
};

int main(){
  srandom(time(0));

  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_Window * w = SDL_CreateWindow(
    "SDL",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, std.w, std.h,
    0
    /*
    | SDL_WINDOW_MAXIMIZED
    | SDL_WINDOW_FULLSCREEN
    | SDL_WINDOW_BORDERLESS
    | SDL_WINDOW_FULLSCREEN_DESKTOP
    */
  );

  SDL_Surface * wicon = SDL_LoadBMP("wicon.bmp");

  SDL_SetWindowIcon(w, wicon);

  /* The surface we should be rendering on
  SDL_Surface * s = SDL_CreateRGBSurface(
    0, 512, 256, 8, 0, 0, 0, 0);
  */
  SDL_Surface * s = SDL_GetWindowSurface(w);

  SDL_Renderer * r = SDL_CreateSoftwareRenderer(s);

  printf(
    "s.w = %u\n"
    "s.h = %u\n"
    "s.flags = %u\n"
    "s.pitch = %u\n"
    "s.pixels = %p\n"
    "s.locked = %u\n"
    "s.format.format = %u\n"
    "s.format.BitsPerPixel = %u\n"
    "s.format.BytesPerPixel = %u\n"
    "SDL_GetNumVideoDrivers = %d\n"
    "SDL_GetCurrentVideoDriver = %s\n"
    "SDL_GetDisplayName = %s\n"
    "SDL_GetCPUCount = %d\n"
    "SDL_GetCPUCacheLineSize = %d\n"
    /*"s.format.palette.ncolors = %d\n"*/,
    (*s).w, (*s).h, (*s).flags, (*s).pitch, (*s).pixels,
    (*s).locked, (*(*s).format).format,
    (*(*s).format).BitsPerPixel,
    (*(*s).format).BytesPerPixel,
    SDL_GetNumVideoDrivers(),
    SDL_GetCurrentVideoDriver(),
    SDL_GetDisplayName(0),
    SDL_GetCPUCount(),
    SDL_GetCPUCacheLineSize()
    /*(*(*(*s).format).palette).ncolors*/
    );

  /*
  SDL_Renderer * r = SDL_CreateRenderer(
    w, -1,
    0
    | SDL_RENDERER_ACCELERATED
    | SDL_RENDERER_PRESENTVSYNC
    | SDL_RENDERER_TARGETTEXTURE
  );
  */

  enum {
    RNDWLK,
    RNDLNS
  };

  SDL_Event e;
  uint16_t i = 0xffff;
  uint8_t _r, _g, _b, m=RNDWLK;
  SDL_Point * points = malloc(sizeof(SDL_Point)*i);
  SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

  uint16_t x=RND()%std.w;
  uint16_t y=RND()%std.h;

  do{
    /* Get keyboard and mouse events */
    SDL_PollEvent(&e);

    /* q quits */
    if((e.key.keysym.sym == SDLK_q)
      /*
      & (SDL_GetModState() == KMOD_LCTRL)
      */
      ) break;

    if(e.key.keysym.sym == SDLK_m)
      SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_MOD);
    if(e.key.keysym.sym == SDLK_n)
      SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    if(e.key.keysym.sym == SDLK_b)
      SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    if(e.key.keysym.sym == SDLK_a)
      SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_ADD);
    if(e.key.keysym.sym == SDLK_l)
      m = RNDLNS;
    if(e.key.keysym.sym == SDLK_w)
      m = RNDWLK;
    if(e.key.keysym.sym == SDLK_c){
      SDL_SetRenderDrawColor(r, 0, 0, 0, 0xff);
      SDL_RenderClear(r);
    }

    /* Random Colors */
    _r += RND()%2 ? _r-1 ? 1 : 1 : -1;
    _g += RND()%2 ? _g-1 ? 1 : 1 : -1;
    _b += RND()%2 ? _b-1 ? 1 : 1 : -1;

    if(m == RNDLNS) goto rndlns;
    if(m == RNDWLK) goto rndwlk;

    /* clear the canvas
    SDL_SetRenderDrawColor(r, 0x0f, 0x0f, 0x0f, 0x01);
    SDL_RenderClear(r);
    */

    /* Set actual canvas to be the renderer r */
    SDL_SetRenderTarget(r, 0x00);

    /* Random Lines
    */
    rndlns:
    i = 0x7ff;
    while(--i)
      points[i] =
        (struct SDL_Point) { RND()%std.w, RND()%std.h};

    SDL_SetRenderDrawColor(r, _r, _g, _b, 0x0f);
    i = 0x7ff;
    SDL_RenderDrawLines(r, points, i);
    goto show;

    /* Random Walker */
    i = 0xffff;
    rndwlk:
    x += RND()%2 ? x+1 < std.w ? 1 : 0 : x-1 < 0 ? 0 : -1;
    y += RND()%2 ? y+1 < std.h ? 1 : 0 : y-1 < 0 ? 0 : -1;

    SDL_SetRenderDrawColor(r, _r, _g, _b, 0x0f);
    SDL_RenderDrawPoint(r, x, y);
    i--;
    if(i) goto rndwlk;
    goto show;

    show:
    /* Show the canvas */
    SDL_RenderPresent(r);

    /* Updates the screen? */
    SDL_UpdateWindowSurface(w);
    /*
    break;
    */
    SDL_Delay(40);
  }while(1);

  SDL_DestroyWindow(w);
  /*SDL_FreeSurface(s);*/

  SDL_Quit();
  return 0;
}
