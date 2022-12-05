#include <SDL2/SDL.h>

const int window_width = 1280;
const int window_height = 720;

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Pong", 0, 0, window_width, window_height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    {
        bool exit = false;

        //run until exit
        while(!exit)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT)
                    exit = true;
                else if(event.type == SDL_KEYDOWN)
                {
                    if(event.key.keysym.sym == SDLK_ESCAPE)
                        exit = true;
                }
            }

            //set blank window
            SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
            SDL_RenderClear(renderer);

            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

