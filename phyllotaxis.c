#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <SDL2/SDL.h>

#ifdef SKIP
  clang -g -Wall -Wextra -Wpedantic --std=gnu99 -O3 \
    `pkg-config --cflags sdl2` \
    -o phyllotaxis phyllotaxis.c \
    `pkg-config --libs sdl2` -lm
  exit 0
#endif

void star(SDL_Renderer *r, uint16_t x, uint16_t y ){
        SDL_RenderDrawPoint(r,   x,   y);
        SDL_RenderDrawPoint(r,   x, y+1);
        SDL_RenderDrawPoint(r,   x, y-1);
        SDL_RenderDrawPoint(r, x-1,   y);
        SDL_RenderDrawPoint(r, x+1,   y);
}

int main(){
    /* defs */
    SDL_Init(SDL_INIT_EVERYTHING);
    char * appname = "phyllotaxis";
    SDL_Window    *w;
    SDL_Renderer  *r;
    SDL_Texture   *t;
    SDL_Event      e;
    uint16_t c, n, x, y;
    float     rad, a;
    uint16_t ww = 1366;
    uint16_t wh = 768;
    n = 0;
    c = 4;

    /* inits */
    w = SDL_CreateWindow(appname, SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, ww, wh, 0x0);
    r = SDL_CreateRenderer(w, -1,
            SDL_RENDERER_ACCELERATED |
            SDL_RENDERER_TARGETTEXTURE |
            SDL_RENDERER_PRESENTVSYNC);
    t = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA4444,
        SDL_TEXTUREACCESS_TARGET, ww, wh);

    /* Set canvas to be the texture t */
    SDL_SetRenderTarget(r, t);
    /* clear it */
    SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 0xF0);
    SDL_RenderClear(r);

    do{
        SDL_PollEvent(&e);
        /* CTRL+Q quits */
        if((e.key.keysym.sym == SDLK_q) &
           (SDL_GetModState() == KMOD_LCTRL)) break;
        n++;

        /* Set actual canvas to be the renderer r */
        SDL_SetRenderTarget(r, 0x00);
        /* clear it */
        SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 0xF0);
        SDL_RenderClear(r);
        /* Set the actual canvas to be the texture t */
        SDL_SetRenderTarget(r, t);
          
        /* draw stuff */
        a = (180/M_PI) * n * 137.3;
        rad = c * sqrt(n);

        x = (uint16_t) rad * cosf(a) + ww/2;
        y = (uint16_t) rad * sinf(a) + wh/2;
        SDL_SetRenderDrawColor(r, (int)n%256, (int)a%256, (int)rad%256, 0xF0);
        if( (0 < x < ww) & (0 < y < wh) )
            star(r, (int) x, (int) y);

        /* actual canvas is r */
        SDL_SetRenderTarget(r, 0x00);
        /* copy t to actual canvas (r) */
        SDL_RenderCopy(r, t, 0x00, 0x00);
        /* present canvas r */
        SDL_RenderPresent(r);
        /*SDL_Delay(5);*/
    } while(1);

    /* exit */
    SDL_DestroyRenderer(r);
    SDL_DestroyTexture(t);
    SDL_DestroyWindow(w);
    SDL_Quit();
    return(0);
}
