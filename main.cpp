#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "BoosterObject.h"
#include "TextObject.h"
#include "menu.h"

BaseObject g_background;

TTF_Font* font = nullptr;

ImpTimer fps_timer;
bool is_game_paused = false;

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if(ret < 0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("24020256 - Nguyen Le Duy Nhan",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH, SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN);

    if(g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if(g_screen == NULL)
        {
            success = false;
        }
        else
        {
            SDL_SetRenderDrawColor(g_screen,
                                   RENDER_DRAW_COLOR,
                                   RENDER_DRAW_COLOR,
                                   RENDER_DRAW_COLOR,
                                   RENDER_DRAW_COLOR);

            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) & imgFlags))
            {
                success = false;
            }
            if (TTF_Init() == -1)
            {
                success = false;
            }
        }
    }
    return success;
}

bool LoadBackground()
{
    bool ret = g_background.LoadImg("img//background.png", g_screen);
    if(ret == false)
    {
        return false;
    }
    return true;
}

void close()
{
    g_background.Free();

    if (font != nullptr)
    {
        TTF_CloseFont(font);
        font = nullptr;
    }

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;
    SDL_DestroyWindow(g_window);
    g_window = NULL;
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    srand((unsigned int)time(NULL));

    if(InitData() == false)
        return -1;

    if(LoadBackground() == false)
        return -1;

    font = TTF_OpenFont("font/dlxfont.ttf", 20);
    if (font == nullptr) return -1;

    TextObject hp_text;
    TextObject coins_text;
    TextObject score_text;
    TextObject Cooldown_text;
    TextObject shieldCooldown_text;
    TextObject bulletCooldown_text;
    TextObject shieldDur_text;

    GameMap game_map;
    game_map.LoadMap("map/map.txt");
    game_map.LoadTiles(g_screen);

    MainObject p_player;
    if(!p_player.LoadShieldImg("img//shield.png", g_screen))
    {
        return -1;
    }
    p_player.LoadImg("img//right.png", g_screen);
    p_player.set_clips();


    std::vector<ThreatsObject*> threatList;
    std::vector<BoosterObject*> coinList;
    std::vector<BoosterObject*> hpList;


    unsigned lastSpawnTime_threat = SDL_GetTicks();
    unsigned lastSpawnTime_coin = SDL_GetTicks();
    unsigned lastSpawnTime_hp = SDL_GetTicks();

    // thoi gian spawn
    unsigned spawn_threat = 3000 + rand() % 2000;
    unsigned spawn_coin = 3000 + rand() % 2000;
    unsigned spawn_hp = 3000 + rand() % 2000;

    int coinCounter = 0;

    bool is_quit = false;

    ThreatsObject::game_start_time_ = SDL_GetTicks();



    while(!is_quit)
    {
        fps_timer.start();

        while(SDL_PollEvent(&g_event) != 0)
        {
            if(g_event.type == SDL_QUIT)
            {
                is_quit = true;
            }
            p_player.HandleInputAction(g_event, g_screen);
        }
        if (!is_game_paused)
        {
            SDL_SetRenderDrawColor(g_screen,
                                   RENDER_DRAW_COLOR,
                                   RENDER_DRAW_COLOR,
                                   RENDER_DRAW_COLOR,
                                   RENDER_DRAW_COLOR);
            SDL_RenderClear(g_screen);

            g_background.Render(g_screen, NULL);

            Map map_data = game_map.getMap();

            p_player.HandleBullet(g_screen);

            p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
            p_player.UpdateShield();
            p_player.DoPlayer(map_data);


            unsigned currentTime = SDL_GetTicks();
            //spawn quai
            if(currentTime - lastSpawnTime_threat >= spawn_threat)
            {
                ThreatsObject* newThreat = new ThreatsObject();
                if(newThreat != NULL)
                {
                    int randomType = rand() % 2;
                    ThreatType tType = (randomType == 0) ? THREAT_GROUND : THREAT_FLYING;

                    if (tType == THREAT_GROUND)
                    {
                    newThreat->LoadImg("img//threat_ground.png", g_screen);
                    }
                    else // THREAT_FLYING
                    {
                    newThreat->LoadImg("img//threat_flying.png", g_screen);
                    }

                    newThreat->set_clips();

                    newThreat->SpawnRandom(map_data, tType);

                    newThreat->SetMapXY(map_data.start_x_, map_data.start_y_);

                    threatList.push_back(newThreat);
                }

                lastSpawnTime_threat = currentTime;
                spawn_threat = SPAWN_MIN_THREAT + rand() % (SPAWN_MAX_THREAT - SPAWN_MIN_THREAT + 1);
            }
            // spawn coin
            if (currentTime - lastSpawnTime_coin >= spawn_coin)
                {
                BoosterObject* newCoin = new BoosterObject();
                if(newCoin != NULL)
                {

                    newCoin->LoadImg("img//coin.png", g_screen);

                    newCoin->set_clips();

                    newCoin->RandomSpawn(map_data);

                    coinList.push_back(newCoin);
                }

                lastSpawnTime_coin = currentTime;
                spawn_coin = SPAWN_MIN_COIN + rand() % (SPAWN_MAX_COIN - SPAWN_MIN_COIN + 1);
                }
            //spawn hp
            if (currentTime - lastSpawnTime_hp >= spawn_hp)
            {
                BoosterObject* newHp = new BoosterObject();
                if(newHp != NULL)
                {

                    newHp->LoadImg("img//hp.png", g_screen);

                    newHp->set_clips();

                    newHp->RandomSpawn(map_data);

                    hpList.push_back(newHp);
                }

                lastSpawnTime_hp = currentTime;
                spawn_hp = SPAWN_MIN_HP + rand() % (SPAWN_MAX_HP - SPAWN_MIN_HP + 1);
            }


            std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
            for(size_t r = 0;r < bullet_arr.size(); r++)
            {
                BulletObject* p_bullet = bullet_arr.at(r);
                if(p_bullet != NULL)
                {
                    for(size_t t = 0;t < threatList.size(); ++t)
                    {
                        ThreatsObject* obj_threat = threatList.at(t);
                        if(obj_threat != NULL)
                        {
                            SDL_Rect tRect;
                            tRect.x = obj_threat->GetRect().x;
                            tRect.y = obj_threat->GetRect().y;
                            tRect.w = obj_threat->get_width_frame();
                            tRect.h = obj_threat->get_height_frame();

                            SDL_Rect bRect = p_bullet->GetRect();

                            bool bCol = CheckCollision(bRect, tRect);

                            if(bCol)
                            {
                                p_player.RemoveBullet(r);
                                obj_threat->Free();
                                threatList.erase(threatList.begin() + t);
                            }
                        }
                    }
                }
            }

            //va cham coin
            for(auto it = coinList.begin(); it != coinList.end();)
            {
                BoosterObject* coin = *it;
                if (coin != NULL)
                {
                    coin->SetMapXY(map_data.start_x_, map_data.start_y_);

                    coin->Show(g_screen);

                    SDL_Rect rect_player = p_player.GetRectFrame();
                    SDL_Rect rect_coin = coin->GetRectFrame();

                    if (CheckCollision(rect_player, rect_coin))
                    {
                        coinCounter++;
                        delete coin;
                        it = coinList.erase(it);
                        continue;
                    }
                }
                    ++it;
            }

            //va cham hp
            for(auto it = hpList.begin(); it != hpList.end();)
            {
                BoosterObject* hp = *it;
                if (hp != NULL)
                {
                    hp->SetMapXY(map_data.start_x_, map_data.start_y_);

                    hp->Show(g_screen);

                    SDL_Rect rect_player = p_player.GetRectFrame();
                    SDL_Rect rect_hp = hp->GetRectFrame();

                    if (CheckCollision(rect_player, rect_hp))
                    {
                        if(p_player.get_health() < 5)
                        {
                            p_player.IncreaseHealth(1);
                        }
                        delete hp;
                        it = hpList.erase(it);
                        continue;
                    }
                }
                ++it;
            }

            //va cham quai
            for(auto it = threatList.begin(); it != threatList.end();)
            {
                ThreatsObject* threat = *it;
                if(threat != NULL)
                {
                    threat->SetMapXY(map_data.start_x_, map_data.start_y_);
                    threat->DoPlayer(map_data);

                    threat->Show(g_screen);


                    SDL_Rect rect_player = p_player.GetRectFrame();
                    SDL_Rect rect_threat = threat->GetRectFrame();

                    if(CheckCollision(rect_player, rect_threat))
                    {
                        if(p_player.IsShieldActive())
                        {
                            it = threatList.erase(it);
                            continue;
                        }
                        else
                        {
                            it = threatList.erase(it);
                            SDL_Delay(60);
                            p_player.DecreaseHealth(1);
                            p_player.shield_on_cooldown_ = false;
                            p_player.ActivateShield();
                            continue;
                        }
                    }

                    if(threat->GetRect().x <= 0)
                    {
                        delete threat;
                        threat = NULL;
                        it = threatList.erase(it);
                        continue;
                    }
                }
                ++it;
            }

            if(p_player.get_health() == 0)
            {
                TextObject gameOverText;
                gameOverText.setText("GAME OVER");

                int text_width = 0, text_height = 0;

                TTF_Font* gameOverFont = TTF_OpenFont("font/dlxfont.ttf", 50);

                TTF_SizeText(gameOverFont, "GAME OVER", &text_width, &text_height);

                gameOverText.RenderTextObject(gameOverFont, g_screen,
                                              (SCREEN_WIDTH - text_width) / 2,
                                              (SCREEN_HEIGHT - text_height) / 2);

                SDL_RenderPresent(g_screen);
                SDL_Delay(3000);

                TTF_CloseFont(gameOverFont);
                gameOverFont = nullptr;

                close();
                SDL_Quit();
                return 0;
            }


            game_map.SetMap(map_data);

            game_map.DrawMap(g_screen);

            p_player.Show(g_screen);


            // -------------- TEXT --------------
            // HP, Coins, Score
            hp_text.setText("HP: " + std::to_string(p_player.get_health()));
            hp_text.RenderTextObject(font, g_screen, 10, 10);

            coins_text.setText("Coins: " + std::to_string(coinCounter));
            coins_text.RenderTextObject(font, g_screen, 10, 40);

            score_text.setText("Score: " + std::to_string(p_player.GetScore()));
            score_text.RenderTextObject(font, g_screen, 10, 70);

            double bullet_cd = p_player.GetBulletCooldown();
            double shield_cd = p_player.GetShieldCooldownTime();
            double shield_dur = p_player.GetShieldTimeRemaining();

            std::ostringstream bullet_oss;
            bullet_oss << std::fixed << std::setprecision(2) << bullet_cd;
            std::string bullet_str = "Bullet: " + bullet_oss.str() + "s";

            std::ostringstream shield_cd_oss;
            shield_cd_oss << std::fixed << std::setprecision(2) << shield_cd;
            std::string shield_cd_str = "Shield: " + shield_cd_oss.str() + "s";

            std::ostringstream shield_dur_oss;
            shield_dur_oss << std::fixed << std::setprecision(2) << shield_dur;
            std::string shield_dur_str = "ShieldDur: " + shield_dur_oss.str() + "s";

            shieldDur_text.setText(shield_dur_str);
            shieldDur_text.RenderTextObject(font, g_screen, 930, 10);

            Cooldown_text.setText("COOLDOWN :");
            Cooldown_text.RenderTextObject(font, g_screen, 930, 40);

            shieldCooldown_text.setText(shield_cd_str);
            shieldCooldown_text.RenderTextObject(font, g_screen, 970, 70);

            bulletCooldown_text.setText(bullet_str);
            bulletCooldown_text.RenderTextObject(font, g_screen, 970, 100);
        }
        else
        {
            SDL_SetRenderDrawColor(g_screen,
                                   RENDER_DRAW_COLOR,
                                   RENDER_DRAW_COLOR,
                                   RENDER_DRAW_COLOR,
                                   RENDER_DRAW_COLOR);
            SDL_RenderClear(g_screen);
            g_background.Render(g_screen, NULL);

            TextObject pausedText;
            pausedText.setText("PAUSED");
            pausedText.RenderTextObject(font, g_screen, SCREEN_WIDTH/2 - 30, SCREEN_HEIGHT/2 - 20);
        }

        SDL_RenderPresent(g_screen);

        // fps tinh theo ms
        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000 / FRAME_PER_SECOND;
        if(real_imp_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            if(delay_time >= 0)
            {
                SDL_Delay(delay_time);
            }
        }
    }

    for(auto threat : threatList)
    {
        delete threat;
    }
    threatList.clear();
    for(auto c : coinList)
    {
        delete c;
    }
    for(auto hp : hpList)
    {
        delete hp;
    }

    close();

    return 0;
}
