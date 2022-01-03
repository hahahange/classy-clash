#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>

int main()
{
    // window dimensions
    const int windowWidth{384};
    const int windowHeight{384};
    // initialize the window
    InitWindow(windowWidth, windowHeight, "Classy Clash!");
    SetTargetFPS(60);

    Texture2D worldmap = LoadTexture("nature_tileset/worldmap.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0};

    Character knight{windowWidth, windowHeight};


    Prop props[2]{
        Prop{Vector2{600.f, 300.f}, LoadTexture("nature_tileset/Rock.png")},
        Prop{Vector2{400.f, 500.f}, LoadTexture("nature_tileset/Log.png")}
    };

    Enemy goblin{
        Vector2{800.f, 300.f},
        LoadTexture("characters/goblin_idle_spritesheet.png"),
        LoadTexture("characters/goblin_run_spritesheet.png")
    };

    Enemy slime
    {
        Vector2{500.f, 700.f},
        LoadTexture("characters/slime_idle_spritesheet.png"),
        LoadTexture("characters/slime_run_spritesheet.png")
    };

    Enemy* enemies[]{&goblin, &slime};

    for(auto enemy : enemies)
    {
        enemy->setTarget(&knight);
    }



    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(WHITE);

        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);

        // draw the map

        DrawTextureEx(worldmap, mapPos, 0.0, mapScale, WHITE);
        for (auto prop : props){
            prop.Render(knight.getWorldPos());
        }

        if (!knight.getAlive()) // not alive
        {
            DrawText("Game Over!", 55.f, 45.f, 40, RED);
            EndDrawing;
            continue;
        }
        else // alive
        {
            std::string knightsHealth = "Health: ";
            knightsHealth.append(std::to_string(knight.getHealth()), 0, 5);
            DrawText(knightsHealth.c_str(), 55.f, 45.f, 40, RED);
        }
        
        knight.tick(GetFrameTime());
        if(knight.getWorldPos().x < 0.f || 
           knight.getWorldPos().y < 0.f || 
           knight.getWorldPos().x + windowWidth > worldmap.width * mapScale ||
           knight.getWorldPos().y + windowHeight > worldmap.height * mapScale) {
               knight.undoMovement();
        }

        for (auto prop : props) 
        {
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
            {
                knight.undoMovement();
            }
        }

        for(auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
        {
            for(auto enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollisionRec()))
                {
                    enemy->setAlive(false);
                }
            }
        
        }
    
        EndDrawing();
    }
    UnloadTexture(worldmap);
    CloseWindow();
}