#ifndef render_h_
#define render_h_

#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<stdio.h>
#include"connect4.h"

SDL_Color comp = {.r=255,.g=10,.b=10,.a=255};
SDL_Color opp = {.r=255,.g=255,.b=0,.a=255};
SDL_Color empty = {.r=0,.g=0,.b=0,.a=255};


void print_message(SDL_Renderer* renderer,SDL_Rect* rect,SDL_Color color,char* message) {
    if(TTF_Init()) printf("ttf initialization failed: %s\n",TTF_GetError());

    TTF_Font* font = TTF_OpenFont("../src/fonts/arial.ttf",24);
    if(font == NULL) printf("font failed to load: %s\n",TTF_GetError());
    SDL_Surface *surface = TTF_RenderText_Solid(font,message,color);
    TTF_CloseFont(font);
    if(surface == NULL) printf("surface failed to create: %s\n",TTF_GetError());

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    if(texture == NULL) printf("texture failed to create: %s\n",TTF_GetError());

    SDL_RenderCopy(renderer,texture,NULL,rect);
    SDL_DestroyTexture(texture);

    TTF_Quit();
}

void initial(SDL_Renderer *renderer,int win_w,int win_h,bool wrong_inp){
    SDL_Rect temp;
    temp.h = win_h*1/10;
    temp.w = win_w*9/10;
    temp.x = (win_w-temp.w)/2;
    temp.y = win_h*2/10;
    if(wrong_inp) print_message(renderer,&temp,{255,10,10,255},(char*)"Please input y or n only");
    SDL_Rect rect1;
    rect1.h = win_h*1/10;
    rect1.w = win_w*9/10;
    rect1.x = (win_w-rect1.w)/2;
    rect1.y = win_h*(wrong_inp?4:3.5)/10;
    SDL_Rect rect2;
    rect2.h = win_h*1/10;
    rect2.w = win_w*5/10;
    rect2.x = (win_w-rect2.w)/2;
    rect2.y = rect1.y+win_h*1/10;
    print_message(renderer,&rect1,{255,255,255,255},(char*)"Do You want to Play First?");
    print_message(renderer,&rect2,{255,255,255,255},(char*)"input y or n");
}

void draw_circle(SDL_Renderer *renderer, int x, int y, int radius, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void render_empty_board(SDL_Renderer *renderer,int win_w,int win_h,const connect4* game) {
    SDL_Rect blu;
    blu.h = win_h;
    blu.w = win_w;
    blu.x = 0;
    blu.y = 100;
    
    SDL_SetRenderDrawColor(renderer,10,10,255,255);
    SDL_RenderFillRect(renderer,&blu);
    for(int i=0;i<game->board_row;i++) {
        for(int j=0;j<game->board_col;j++) {
            draw_circle(renderer,50+(j*100),150+(i*100),45,empty);
        }   
    }
}

void render_top_coin(SDL_Renderer *renderer,int win_w,int x) {
    if(x<50) x = 50;
    if(x>win_w-50) x = win_w-50;
    draw_circle(renderer,x,50,45,opp);
}

void drop_anim(SDL_Renderer *renderer,SDL_Texture *texture,int win_w,int win_h,int x,int y,SDL_Color color,const connect4* game) {
    bool quit = false;
    int y_coin = 50;

    while(!quit) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                default:
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer,0,0,0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,texture,NULL,NULL);
        // printf("x: %d y: %d y_coin: %d\n",x,y,y_coin);
        draw_circle(renderer,x*100+50,y_coin,45,color);
        y_coin+=25;
        SDL_Delay(1);
        SDL_RenderPresent(renderer);
        if(y_coin>y*100+150) break;
    }
    SDL_SetRenderTarget(renderer,texture);
    draw_circle(renderer,x*100+50,y*100+150,45,color);
    SDL_SetRenderTarget(renderer,NULL);
}

void button_pressed(SDL_Renderer *renderer,SDL_Texture *texture,int win_w,int win_h,int x, connect4* game) {
    int y=game->get_empty(x);
    drop_anim(renderer,texture,win_w,win_h,x,y,opp,game);
    game->board[y][x] = game->opp;
    game->is_terminate();
    if(game->terminate) return;

    SDL_Rect rect;
    rect.h = 100;
    rect.w = win_w*9/10;
    rect.y = 0;
    rect.x = (win_w-rect.w)/2;
    print_message(renderer,&rect,opp,(char*)"Computer is Playing");
    SDL_RenderPresent(renderer);
    
    int comp_col = game->find_best_move();
    // auto ftr = std::async(&connect4::find_best_move,game);
    // int comp_col = ftr.get();
    x = comp_col;
    y = game->get_empty(comp_col);
    drop_anim(renderer,texture,win_w,win_h,x,y,comp,game);
    game->board[y][x]=game->comp;
    game->is_terminate();
}

#endif