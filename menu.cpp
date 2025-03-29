#include "menu.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

// Hàm hỗ trợ tạo Texture từ text
static SDL_Texture* LoadText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

//--------------------------------------------------
// MAIN MENU
//--------------------------------------------------
MainMenuOption ShowMainMenu(SDL_Renderer* renderer, TTF_Font* font)
{
    bool running = true;
    MainMenuOption choice = MAINMENU_NONE;

    SDL_Color white = {255, 255, 255, 255};
    SDL_Texture* titleText = LoadText(renderer, font, "MAIN MENU", white);
    SDL_Texture* playText  = LoadText(renderer, font, "Play Game", white);
    SDL_Texture* quitText  = LoadText(renderer, font, "Quit", white);

    // Toạ độ vẽ
    SDL_Rect titleRect = {100, 50, 0, 0};
    SDL_Rect playRect  = {100, 150, 0, 0};
    SDL_Rect quitRect  = {100, 200, 0, 0};

    SDL_QueryTexture(titleText, NULL, NULL, &titleRect.w, &titleRect.h);
    SDL_QueryTexture(playText,  NULL, NULL, &playRect.w,  &playRect.h);
    SDL_QueryTexture(quitText,  NULL, NULL, &quitRect.w,  &quitRect.h);

    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
                choice = MAINMENU_QUIT;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = e.button.x;
                int y = e.button.y;
                // Click vào "Play Game"
                if (x >= playRect.x && x <= playRect.x + playRect.w &&
                    y >= playRect.y && y <= playRect.y + playRect.h)
                {
                    choice = MAINMENU_PLAY;
                    running = false;
                }
                // Click vào "Quit"
                else if (x >= quitRect.x && x <= quitRect.x + quitRect.w &&
                         y >= quitRect.y && y <= quitRect.y + quitRect.h)
                {
                    choice = MAINMENU_QUIT;
                    running = false;
                }
            }
            else if (e.type == SDL_KEYDOWN)
            {
                // Ví dụ nhấn ESC => Quit
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    choice = MAINMENU_QUIT;
                    running = false;
                }
            }
        }

        // Vẽ background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Vẽ text
        SDL_RenderCopy(renderer, titleText, NULL, &titleRect);
        SDL_RenderCopy(renderer, playText,  NULL, &playRect);
        SDL_RenderCopy(renderer, quitText,  NULL, &quitRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(titleText);
    SDL_DestroyTexture(playText);
    SDL_DestroyTexture(quitText);

    return choice;
}

//--------------------------------------------------
// PAUSE MENU
//--------------------------------------------------
PauseMenuOption ShowPauseMenu(SDL_Renderer* renderer, TTF_Font* font)
{
    bool running = true;
    PauseMenuOption choice = PAUSE_NONE;

    SDL_Color white = {255, 255, 255, 255};
    SDL_Texture* pauseText    = LoadText(renderer, font, "PAUSE MENU", white);
    SDL_Texture* continueText = LoadText(renderer, font, "Continue",   white);
    SDL_Texture* restartText  = LoadText(renderer, font, "Restart",    white);
    SDL_Texture* mainText     = LoadText(renderer, font, "Main Menu",  white);

    SDL_Rect pauseRect    = {100, 50, 0, 0};
    SDL_Rect continueRect = {100, 150, 0, 0};
    SDL_Rect restartRect  = {100, 200, 0, 0};
    SDL_Rect mainRect     = {100, 250, 0, 0};

    SDL_QueryTexture(pauseText,    NULL, NULL, &pauseRect.w,    &pauseRect.h);
    SDL_QueryTexture(continueText, NULL, NULL, &continueRect.w, &continueRect.h);
    SDL_QueryTexture(restartText,  NULL, NULL, &restartRect.w,  &restartRect.h);
    SDL_QueryTexture(mainText,     NULL, NULL, &mainRect.w,     &mainRect.h);

    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
                choice = PAUSE_BACK_TO_MAIN;  // hoặc tuỳ ý
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = e.button.x;
                int y = e.button.y;
                // Continue
                if (x >= continueRect.x && x <= continueRect.x + continueRect.w &&
                    y >= continueRect.y && y <= continueRect.y + continueRect.h)
                {
                    choice = PAUSE_CONTINUE;
                    running = false;
                }
                // Restart
                else if (x >= restartRect.x && x <= restartRect.x + restartRect.w &&
                         y >= restartRect.y && y <= restartRect.y + restartRect.h)
                {
                    choice = PAUSE_RESTART;
                    running = false;
                }
                // Main Menu
                else if (x >= mainRect.x && x <= mainRect.x + mainRect.w &&
                         y >= mainRect.y && y <= mainRect.y + mainRect.h)
                {
                    choice = PAUSE_BACK_TO_MAIN;
                    running = false;
                }
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    // Ấn ESC => Continue
                    choice = PAUSE_CONTINUE;
                    running = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, pauseText,    NULL, &pauseRect);
        SDL_RenderCopy(renderer, continueText, NULL, &continueRect);
        SDL_RenderCopy(renderer, restartText,  NULL, &restartRect);
        SDL_RenderCopy(renderer, mainText,     NULL, &mainRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(pauseText);
    SDL_DestroyTexture(continueText);
    SDL_DestroyTexture(restartText);
    SDL_DestroyTexture(mainText);

    return choice;
}

//--------------------------------------------------
// GAME OVER MENU
//--------------------------------------------------
GameOverOption ShowGameOverMenu(SDL_Renderer* renderer, TTF_Font* font)
{
    bool running = true;
    GameOverOption choice = GAMEOVER_NONE;

    SDL_Color red   = {255, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};

    SDL_Texture* gameOverText = LoadText(renderer, font, "GAME OVER", red);
    SDL_Texture* restartText  = LoadText(renderer, font, "Restart",   white);
    SDL_Texture* mainText     = LoadText(renderer, font, "Main Menu", white);

    SDL_Rect overRect    = {100, 50, 0, 0};
    SDL_Rect restartRect = {100, 150, 0, 0};
    SDL_Rect mainRect    = {100, 200, 0, 0};

    SDL_QueryTexture(gameOverText, NULL, NULL, &overRect.w,   &overRect.h);
    SDL_QueryTexture(restartText,  NULL, NULL, &restartRect.w,&restartRect.h);
    SDL_QueryTexture(mainText,     NULL, NULL, &mainRect.w,   &mainRect.h);

    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
                choice = GAMEOVER_BACK_TO_MAIN;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = e.button.x;
                int y = e.button.y;
                // Restart
                if (x >= restartRect.x && x <= restartRect.x + restartRect.w &&
                    y >= restartRect.y && y <= restartRect.y + restartRect.h)
                {
                    choice = GAMEOVER_RESTART;
                    running = false;
                }
                // Main Menu
                else if (x >= mainRect.x && x <= mainRect.x + mainRect.w &&
                         y >= mainRect.y && y <= mainRect.y + mainRect.h)
                {
                    choice = GAMEOVER_BACK_TO_MAIN;
                    running = false;
                }
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_r)
                {
                    choice = GAMEOVER_RESTART;
                    running = false;
                }
                else if (e.key.keysym.sym == SDLK_m || e.key.keysym.sym == SDLK_ESCAPE)
                {
                    choice = GAMEOVER_BACK_TO_MAIN;
                    running = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, gameOverText, NULL, &overRect);
        SDL_RenderCopy(renderer, restartText,  NULL, &restartRect);
        SDL_RenderCopy(renderer, mainText,     NULL, &mainRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(gameOverText);
    SDL_DestroyTexture(restartText);
    SDL_DestroyTexture(mainText);

    return choice;
}
