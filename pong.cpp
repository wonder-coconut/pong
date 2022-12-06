#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

const int window_width = 1280;
const int window_height = 720;

const int ball_width = 15;
const int ball_height = 15;

const int paddle_width = 10;
const int paddle_height = 125;

const float paddle_speed = 0.5f;
const float ball_speed = 0.5f;

enum Buttons
{
    paddle1Up = 0,
    paddle1Down,
    paddle2Up,
    paddle2Down
};

enum class CollisionType
{
    None,
    Top,
    Middle,
    Bottom
};

struct Contact
{
    CollisionType type;
    float penetration;
};

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
        Vector2 operator * (float n)
        {
            return Vector2(x * n, y * n);
        }
};

class Ball
{
    public:
        Vector2 position;
        Vector2 velocity;
        SDL_Rect rect{};

        Ball(Vector2 position, Vector2 velocity)
        {
            this->position = position;
            this->velocity = velocity;

            rect.x = static_cast<int>(position.x);
            rect.y = static_cast<int>(position.y);
            rect.w = ball_width;
            rect.h = ball_height;
        }

        void update(float frameTime)
        {
            position += velocity * frameTime;
        }

        void CollideWithPaddle(Contact const& contact)
        {
            position.x += contact.penetration;
            velocity.x = -velocity.x;

            if(contact.type == CollisionType::Top)
                velocity.y = -0.75f * ball_speed;
            else if(contact.type == CollisionType::Bottom)
                velocity.y = 0.75 * ball_speed;
        }

        void draw(SDL_Renderer* renderer)
        {
            rect.x = static_cast<int>(position.x);
            rect.y = static_cast<int>(position.y);

            SDL_RenderFillRect(renderer, &rect);
        }
};

class Paddle
{
    public:
        Vector2 position;
        Vector2 velocity;
        SDL_Rect rect{};

        Paddle(Vector2 position, Vector2 velocity)
        {
            this->position = position;
            this->velocity = velocity;
            
            rect.x = static_cast<int>(position.x);
            rect.y = static_cast<int>(position.y);
            rect.w = paddle_width;
            rect.h = paddle_height;
        }

        void update(float frameTime)
        {
            position += velocity * frameTime;

            if(position.y < 0)
                position.y = 0;
            else if(position.y > (window_height - paddle_height))
                position.y = window_height - paddle_height;
        }

        void draw(SDL_Renderer* renderer)
        {
            rect.y = static_cast<int>(position.y);

            SDL_RenderFillRect(renderer, &rect);
        }
};

class PlayerScore
{
    public:
        SDL_Renderer* renderer;
        SDL_Surface* surface {};
        SDL_Texture* texture {};
        SDL_Rect rect {};
        TTF_Font* font;

        PlayerScore(Vector2 position, SDL_Renderer* renderer, TTF_Font* font)
        {
            this->renderer = renderer;
            this->font = font;

            surface = TTF_RenderText_Solid(font, "0", {0xFF ,0xFF ,0xFF ,0xFF});
            texture = SDL_CreateTextureFromSurface(renderer, surface);

            int width,height;
            SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

            rect.x = static_cast<int>(position.x);
            rect.y = static_cast<int>(position.y);
            rect.w = width;
            rect.h = height;
        }

        PlayerScore()
        {
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        void draw()
        {
            SDL_RenderCopy(renderer, texture, nullptr, &rect);
        }

};

Contact CheckPaddleCollision(Ball const& ball, Paddle const& paddle)
{

	float ballLeft = ball.position.x;
	float ballRight = ball.position.x + ball_width;
	float ballTop = ball.position.y;
	float ballBottom = ball.position.y + ball_height;

	float paddleLeft = paddle.position.x;
	float paddleRight = paddle.position.x + paddle_width;
	float paddleTop = paddle.position.y;
	float paddleBottom = paddle.position.y + paddle_height;

    Contact contact{};

    if(ballLeft >= paddleRight) //left of ball > right of paddle
        return contact;
    if(ballRight <= paddleLeft) //right of ball < left of paddle
        return contact;
    if(ballTop >= paddleBottom) //top of ball > bottom of paddle
        return contact;
    if(ballBottom <= paddleTop) //bottom of ball < top of paddle
        return contact;
    
    float paddleRangeUpper = paddleBottom - (2.0f * paddle_height / 3.0f);//to check relative of ball collision wrt paddle
    float paddleRangeMiddle = paddleBottom - (paddle_height / 3.0f);

    if(ball.velocity.x < 0) //left
        contact.penetration = paddleRight - ballLeft;
    else if(ball.velocity.x > 0) //right
        contact.penetration = paddleLeft - ballRight;

    if((ballBottom > paddleTop) && (ballBottom < paddleRangeUpper))//checking section of paddle collision
        contact.type = CollisionType::Top;
    else if((ballBottom > paddleRangeUpper) && (ballBottom < paddleRangeMiddle))
        contact.type = CollisionType::Middle;
    else
        contact.type = CollisionType::Bottom;

    return contact;
}

int main()
{
    //video and rendering component initialization
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Pong", 0, 0, window_width, window_height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    //ball and paddles initialization
    Ball ball(
        Vector2((window_width/2.0f) - (ball_width/2.0f),
        (window_height/2.0f) - (ball_height/2.0f)),
        Vector2(ball_speed, 0.0f)
    );

    Paddle paddle1(
        Vector2(50.0f, (window_height/2.0f) - (paddle_height/2.0f)),
        Vector2(0.0f, 0.0f)
    );
    
    Paddle paddle2(
        Vector2((window_width - 50.0f), (window_height/2.0f) - (paddle_height/2.0f)),
        Vector2(0.0f, 0.0f)
    );

    //font initialization
    TTF_Init();
    TTF_Font* scoreFont = TTF_OpenFont("SevenSegment.ttf", 69);

    //player score initialization
    PlayerScore p1ScoreText(Vector2(window_width/4, 20), renderer, scoreFont);
    PlayerScore p2ScoreText(Vector2(3 * window_width/4, 20), renderer, scoreFont);

    {
        bool exit = false;
        bool buttons[4] = {};

        //frame time initialization
        float frameTime = 0.0f;

        //run until exit
        while(!exit)
        {
            //frametime Start
            auto startTime = std::chrono::high_resolution_clock::now();

            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT)
                    exit = true;
                else if(event.type == SDL_KEYDOWN)
                {
                    if(event.key.keysym.sym == SDLK_ESCAPE)//quit
                        exit = true;
                    else if(event.key.keysym.sym == SDLK_w)//keydown cases for paddle input
                        buttons[Buttons::paddle1Up] = true;
                    else if(event.key.keysym.sym == SDLK_s)
                        buttons[Buttons::paddle1Down] = true;
                    else if(event.key.keysym.sym == SDLK_UP)
                        buttons[Buttons::paddle2Up] = true;
                    else if(event.key.keysym.sym == SDLK_DOWN)
                        buttons[Buttons::paddle2Down] = true;
                }
                else if(event.type == SDL_KEYUP)
                {
                    if(event.key.keysym.sym == SDLK_w)//keyup cases for paddle input
                        buttons[Buttons::paddle1Up] = false;
                    else if(event.key.keysym.sym == SDLK_s)
                        buttons[Buttons::paddle1Down] = false;
                    else if(event.key.keysym.sym == SDLK_UP)
                        buttons[Buttons::paddle2Up] = false;
                    else if(event.key.keysym.sym == SDLK_DOWN)
                        buttons[Buttons::paddle2Down] = false;
                }
            }

            //set blank window
            SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
            SDL_RenderClear(renderer);

            //render:

            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

            //middle line render
            for(int y = 0; y < window_height; y++)
            {
                if(y % 5)
                    SDL_RenderDrawPoint(renderer, window_width/2, y);
            }

            //ball and paddle render
            ball.draw(renderer);
            paddle1.draw(renderer);
            paddle2.draw(renderer);

            //player score render
            p1ScoreText.draw();
            p2ScoreText.draw();

            SDL_RenderPresent(renderer);

            //paddle velocity
            if(buttons[Buttons::paddle1Up])
                paddle1.velocity.y = -paddle_speed;
            else if(buttons[Buttons::paddle1Down])
                paddle1.velocity.y = paddle_speed;
            else
                paddle1.velocity.y = 0.0f;

            if(buttons[Buttons::paddle2Up])
                paddle2.velocity.y = -paddle_speed;
            else if(buttons[Buttons::paddle2Down])
                paddle2.velocity.y = paddle_speed;
            else
                paddle2.velocity.y = 0.0f;

            //std::cout << "paddle1 :\t" << paddle1.velocity.y << std::endl;
            //std::cout << "paddle2 :\t" << paddle2.velocity.y << std::endl;

            //frametime stop, calculate time
            auto stopTime = std::chrono::high_resolution_clock::now();
            frameTime = std::chrono::duration<float, std::chrono::milliseconds::period>(stopTime - startTime).count();

            //paddle velocity update
            paddle1.update(frameTime);
            paddle2.update(frameTime);

            //ball velocity update
            ball.update(frameTime);

            //collision detection
            Contact contact = CheckPaddleCollision(ball, paddle1);
            if(contact.type != CollisionType::None)
                ball.CollideWithPaddle(contact);
            else
            {
                contact = CheckPaddleCollision(ball, paddle2);
                if(contact.type != CollisionType::None)
                    ball.CollideWithPaddle(contact);
            }
        }
    }

    //cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(scoreFont);
    SDL_Quit();

    return 0;
}