#include <SDL2/SDL.h>

const int window_width = 1280;
const int window_height = 720;

const int ball_width = 15;
const int ball_height = 15;

class Vector2
{
    public:
        float x,y;
        Vector2()
        {
            x = 0.0f;
            y = 0.0f;
        }
        Vector2(float x, float y)
        {
            this->x = x;
            this->y = y;
        }
        Vector2 operator + (Vector2 const& obj)
        {
            return Vector2(x + obj.x, y + obj.y);
        }
        Vector2 operator += (Vector2 const& obj)
        {
            x += obj.x;
            y += obj.y;

            return *this;
        }
};

class Ball
{
    public:
        Vector2 position;
        SDL_Rect rect{};

        Ball(Vector2 position)
        {
            this->position = position;

            rect.x = static_cast<int>(position.x);
            rect.y = static_cast<int>(position.y);
            rect.w = ball_width;
            rect.h = ball_height;
        }

        void draw(SDL_Renderer* renderer)
        {
            rect.x = static_cast<int>(position.x);
            rect.y = static_cast<int>(position.y);

            SDL_RenderFillRect(renderer, &rect);
        }
};

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

            //render:

            Ball ball(
                Vector2((window_width/2.0f) - (ball_width/2.0f),
                (window_height/2.0f) - (ball_height/2.0f))
            );

            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

            for(int y = 0; y < window_height; y++)
            {
                if(y % 5)
                    SDL_RenderDrawPoint(renderer, window_width/2, y);
            }

            ball.draw(renderer);

            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}