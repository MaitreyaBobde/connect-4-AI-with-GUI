#include<SDL2/SDL.h>
#include"render.h"
#include"connect4.h"

int main(int argc, char* argv[]) {

    connect4* game = new connect4();
    int win_h = (game->board_row+1)*100,win_w = (game->board_col)*100;

    if(SDL_Init(SDL_INIT_EVERYTHING)) printf("error initializing: %s\n",SDL_GetError());
    SDL_Window *window = SDL_CreateWindow("connect 4",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,win_w,win_h,SDL_WINDOW_SHOWN);
    if(window == NULL) printf("error creating window: %s\n",SDL_GetError());
    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) printf("error creating renderer: %s\n",SDL_GetError());
    SDL_Texture *board = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,win_w,win_h);
    if(board == NULL) printf("error creating Texture: %s\n",SDL_GetError());

    bool quit = false;
    bool wrong_inp = false;
    bool play_first = true;
    while(!quit) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_y || event.key.keysym.sym == SDLK_n) {
                    quit = true;
                    if(event.key.keysym.sym == SDLK_n) play_first = false;
                }
                else wrong_inp = true;
            default:
                break;
            }
        }
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
        initial(renderer,win_w,win_h,wrong_inp);
        SDL_RenderPresent(renderer);

    }
    SDL_SetRenderTarget(renderer,board);
    render_empty_board(renderer,win_w,win_h,game);
    SDL_SetRenderTarget(renderer,NULL);

    if(play_first == false) {
        int x = game->board_col/2,y = game->board_row-1;
        drop_anim(renderer,board,win_w,win_h,x,y,comp,game);
        game->board[game->board_row-1][game->board_col/2] = game->comp;
    }

    quit = false;
    while(!quit){
        SDL_Event event;
        int mouse_x;
        while(SDL_PollEvent(&event)){
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEMOTION:
                mouse_x = event.motion.x;
                break;
            case SDL_MOUSEBUTTONDOWN:
                button_pressed(renderer,board,win_w,win_h,(event.motion.x/100),game);
                break;
            default:
                break;
            }
        }
        if(game->terminate) quit = true;
        SDL_SetRenderDrawColor(renderer,0,0,0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,board,NULL,NULL);
        render_top_coin(renderer,win_w,mouse_x);
        SDL_RenderPresent(renderer);
    }
    int temp = game->evaluate();
    SDL_Rect rect;
    rect.h = 100;
    rect.w = win_w*7/10;
    rect.y = 0;
    rect.x = (win_w-rect.w)/2;
    quit = false;
    while(!quit) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            default:
                break;
            }
        }
        SDL_SetRenderDrawColor(renderer,0,0,0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,board,NULL,NULL);
        if(temp == 1000) print_message(renderer,&rect,comp,(char*)"Computer WINS!!!");
        else if(temp == -1000) print_message(renderer,&rect,opp,(char*)"YOU WIN!!!!");
        else print_message(renderer,&rect,{255,255,255,255},(char*)"It's a Draw!!!");
        SDL_RenderPresent(renderer);

    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}