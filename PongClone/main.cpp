#include "main.h"

#include <chrono>

#include "Ball.h"
#include "Buttons.h"
#include "Paddle.h"
#include "Score.h"
#include "wtypes.h"
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#undef main

void GetDesktopResolution(int& horizontal, int& vertical);
Contact checkPaddleCollision(Ball const& ball, Paddle const& paddle);
Contact CheckWallCollision(Ball const& ball);

const Vec2 DEFAULT_POSITION = Vec2((WINDOW_WIDTH / 2.0f) - (BALL_WIDTH / 2.0f), (WINDOW_HEIGHT / 2.0f) - (BALL_HEIGHT / 2.0f));
const Vec2 DEFAULT_P1_POSITION = Vec2(50.0f, (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f));
const Vec2 DEFAULT_P2_POSITION = Vec2(WINDOW_WIDTH - 50.0f, (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f));

int main()
{
    int horizontal, vertical, winx, winy;

    GetDesktopResolution(horizontal, vertical);

    winx = horizontal / 2 - WINDOW_WIDTH / 2;
    winy = vertical / 2 - WINDOW_HEIGHT / 2;

    if (winx < 0) winx = 0;
    if (winy < 0) winy = 0;

    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Pong", winx, winy, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    TTF_Font* scoreFont = TTF_OpenFont(FONT_PATH.c_str(), 40);

    Score playerOneScore(Vec2(((WINDOW_WIDTH / 2.0f) / 2.0f), 30.0f), renderer, scoreFont);
    Score playerTwoScore(Vec2((WINDOW_WIDTH - (WINDOW_WIDTH / 2.0f) / 2.0f), 30.0f), renderer, scoreFont);

    Ball ball(DEFAULT_POSITION, Vec2(BALL_SPEED, 0.0f));
    Paddle paddleOne(DEFAULT_P1_POSITION, Vec2(), 1);
    Paddle paddleTwo(DEFAULT_P2_POSITION, Vec2(), 2);

    // TODO: Fix paddles not being true zero on init
    paddleOne.velocity.x = 0;
    paddleTwo.velocity.x = 0;

    {
        int pOneScore = 0;
        int pTwoScore = 0;
        
        bool running = true;
        bool buttons[4] = {};

        float dt = 0.0f;

        while (running)
        {
            auto startTime = std::chrono::high_resolution_clock::now();

            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
                else if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        running = false;
                    }
                    else if (event.key.keysym.sym == SDLK_w)
                    {
                        buttons[Buttons::PaddleOneUp] = true;
                    }
                    else if (event.key.keysym.sym == SDLK_s)
                    {
                        buttons[Buttons::PaddleOneDown] = true;
                    }
                    else if (event.key.keysym.sym == SDLK_UP)
                    {
                        buttons[Buttons::PaddleTwoUp] = true;
                    }
                    else if (event.key.keysym.sym == SDLK_DOWN)
                    {
                        buttons[Buttons::PaddleTwoDown] = true;
                    }
                }
                else if (event.type == SDL_KEYUP)
                {
                    if (event.key.keysym.sym == SDLK_w)
                    {
                        buttons[Buttons::PaddleOneUp] = false;
                    }
                    else if (event.key.keysym.sym == SDLK_s)
                    {
                        buttons[Buttons::PaddleOneDown] = false;
                    }
                    else if (event.key.keysym.sym == SDLK_UP)
                    {
                        buttons[Buttons::PaddleTwoUp] = false;
                    }
                    else if (event.key.keysym.sym == SDLK_DOWN)
                    {
                        buttons[Buttons::PaddleTwoDown] = false;
                    }
                }
            }

            // Paddle move logic
            if (buttons[Buttons::PaddleOneUp])
            {
                paddleOne.velocity.y = -PADDLE_SPEED;
            }
            else if (buttons[Buttons::PaddleOneDown])
            {
                paddleOne.velocity.y = PADDLE_SPEED;
            }
            else
            {
                paddleOne.velocity.y = 0.0f;
            }

            if (buttons[Buttons::PaddleTwoUp])
            {
                paddleTwo.velocity.y = -PADDLE_SPEED;
            }
            else if (buttons[Buttons::PaddleTwoDown])
            {
                paddleTwo.velocity.y = PADDLE_SPEED;
            }
            else
            {
                paddleTwo.velocity.y = 0.0f;
            }

            Contact contactOne = checkPaddleCollision(ball, paddleOne);
            Contact contactTwo = checkPaddleCollision(ball, paddleTwo);

            // Check for Ball Collisions
            if (contactOne.type != CollisionType::None)
            {
                ball.CollideWithPaddle(contactOne);
            }
            else if (contactTwo.type != CollisionType::None)
            {
                ball.CollideWithPaddle(contactTwo);
            }

            Contact contactBound = CheckWallCollision(ball);

            if (contactBound.type != CollisionType::None)
            {
                if(contactBound.type == CollisionType::Botttom || contactBound.type == CollisionType::Top)
                {
                    ball.CollideWithWall(contactBound);
                }
                else if (contactBound.type == CollisionType::Right)
                {
                    // Player one scores
                    pOneScore += 1;
                    playerOneScore.SetScore(pOneScore);
                    
                    ball.SetPosition(DEFAULT_POSITION);
                    ball.SetVelocity(Vec2(BALL_SPEED, 0.0f));

                    paddleOne.SetPosition(DEFAULT_P1_POSITION);
                    paddleTwo.SetPosition(DEFAULT_P2_POSITION);
                }
                else if (contactBound.type == CollisionType::Left)
                {
                    // Player two scores
                    pTwoScore += 1;
                    playerTwoScore.SetScore(pTwoScore);

                    ball.SetPosition(DEFAULT_POSITION);
                    ball.SetVelocity(Vec2(-BALL_SPEED, 0.0f));

                    paddleOne.SetPosition(DEFAULT_P1_POSITION);
                    paddleTwo.SetPosition(DEFAULT_P2_POSITION);
                }
            }

            paddleOne.Update(dt);
            paddleTwo.Update(dt);

            ball.Update(dt);

            // Clear the window to black
            SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
            SDL_RenderClear(renderer);

            // Draw the net
            SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

            for (int y = 0; y < WINDOW_HEIGHT; ++y)
            {
                if (y % 3)
                {
                    SDL_RenderDrawPoint(renderer, WINDOW_WIDTH / 2, y);
                }
            }

            // Draw the ball
            ball.Draw(renderer);

            // Draw paddles
            paddleOne.Draw(renderer);
            paddleTwo.Draw(renderer);

            playerOneScore.Draw();
            playerTwoScore.Draw();

            // Present the backbuffer
            SDL_RenderPresent(renderer);

            auto stopTime = std::chrono::high_resolution_clock::now();
            dt = std::chrono::duration<float, std::chrono::milliseconds::period>(stopTime - startTime).count();
        }
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(scoreFont);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

void GetDesktopResolution(int& horizontal, int& vertical)
{
    RECT desktop;

    const HWND hDesktop = GetDesktopWindow();

    GetWindowRect(hDesktop, &desktop);

    horizontal = desktop.right;
    vertical = desktop.bottom;
}

Contact checkPaddleCollision(Ball const& ball, Paddle const& paddle)
{
    float ballLeft = ball.position.x;
    float ballRight = ball.position.x + BALL_WIDTH;
    float ballTop = ball.position.y;
    float ballBottom = ball.position.y + BALL_HEIGHT;

    float paddleLeft = paddle.position.x;
    float paddleRight = paddle.position.x + PADDLE_WIDTH;
    float paddleTop = paddle.position.y;
    float paddleBottom = paddle.position.y + PADDLE_HEIGHT;

    Contact contact{};

    if (ballLeft >= paddleRight)
    {
        return contact;
    }
    if (ballRight <= paddleLeft)
    {
        return contact;
    }
    if (ballTop >= paddleBottom)
    {
        return contact;
    }
    if (ballBottom <= paddleTop)
    {
        return contact;
    }

    float paddleRangeUpper = paddleBottom - (2.0f * PADDLE_HEIGHT / 3.0f);
    float paddleRangeMiddle = paddleBottom - (PADDLE_HEIGHT / 3.0f);

    if (ball.velocity.x < 0)
    {
        // Left paddle
        contact.penetration = paddleRight - ballLeft;
    }
    else if (ball.velocity.x > 0)
    {
        //Right paddle
        contact.penetration = paddleLeft - ballRight;
    }

    if ((ballBottom > paddleTop)
        && (ballBottom < paddleRangeUpper))
    {
        contact.type = CollisionType::Top;
    }
    else if ((ballBottom > paddleRangeUpper)
        && (ballBottom < paddleRangeMiddle))
    {
        contact.type = CollisionType::Middle;
    }
    else
    {
        contact.type = CollisionType::Botttom;
    }

    return contact;
}

Contact CheckWallCollision(Ball const& ball)
{
    float ballLeft = ball.position.x;
    float ballRight = ball.position.x + BALL_WIDTH;
    float ballTop = ball.position.y;
    float ballBottom = ball.position.y + BALL_HEIGHT;

    Contact contact{};

    if (ballLeft < 0.0f)
    {
        contact.type = CollisionType::Left;
    }
    else if (ballRight > WINDOW_WIDTH)
    {
        contact.type = CollisionType::Right;
    }
    else if (ballTop < 0.0f)
    {
        contact.type = CollisionType::Top;
    }
    else if (ballBottom > WINDOW_HEIGHT)
    {
        contact.type = CollisionType::Botttom;
    }

    return contact;
}
