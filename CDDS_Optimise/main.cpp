/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#include <random>
#include <time.h>
#include <unordered_map>
#include <utility>
#include "Critter.h"

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    //SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    srand(time(NULL));

    std::unordered_map<const char*, Texture2D> textureMap;
    textureMap.insert(std::pair<const char*, Texture2D>("DESTROYER", LoadTexture("./res/9.png")));
    textureMap.insert(std::pair<const char*, Texture2D>("CRITTER", LoadTexture("./res/10.png")));

    // create some critters
    const int CRITTER_COUNT = 5;
    const int MAX_VELOCITY = 80;

    CritterPool *critters = new CritterPool(CRITTER_COUNT);

    for (int i = 0; i < CRITTER_COUNT; i++)
    {
        // create a random direction vector for the velocity
        Vector2 velocity = { -100+(rand()%200), -100+(rand()%200) };
        // normalize and scale by a random speed
        velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);

        // create a critter in a random location
        Critter *critter = new Critter();
        critter->Init({ (float)(5+rand() % (screenWidth-10)), (float)(5+(rand() % screenHeight-10)) }, velocity, 12, textureMap["CRITTER"]);
        critters->AllocateObject(critter);
    }

    Critter *destroyer;
    Vector2 velocity = { -100 + (rand() % 200), -100 + (rand() % 200) };
    velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);
    destroyer = new Critter();
    destroyer->Init(Vector2{ (float)(screenWidth >> 1), (float)(screenHeight >> 1) }, velocity, 20, textureMap["DESTROYER"]);

    float timer = 1;
    Vector2 nextSpawnPos = destroyer->GetPosition();
    bool respawn = false;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        float delta = GetFrameTime();

        // update the destroyer
        destroyer->Update(delta);
        // check each critter against screen bounds
        if (destroyer->GetX() < 0) {
            destroyer->SetX(0);
            destroyer->SetVelocity(Vector2{ -destroyer->GetVelocity().x, destroyer->GetVelocity().y });
        }
        if (destroyer->GetX() > screenWidth) {
            destroyer->SetX(screenWidth);
            destroyer->SetVelocity(Vector2{ -destroyer->GetVelocity().x, destroyer->GetVelocity().y });
        }
        if (destroyer->GetY() < 0) {
            destroyer->SetY(0);
            destroyer->SetVelocity(Vector2{ destroyer->GetVelocity().x, -destroyer->GetVelocity().y });
        }
        if (destroyer->GetY() > screenHeight) {
            destroyer->SetY(screenHeight);
            destroyer->SetVelocity(Vector2{ destroyer->GetVelocity().x, -destroyer->GetVelocity().y });
        }

        // update the critters
        // (dirty flags will be cleared during update)
        critters->Update(delta);
        for (int i = 0; i < CRITTER_COUNT; i++)
        {
            // check each critter against screen bounds
            if (critters->GetX(i) < 0) {
                critters->SetX(i, 0);
                critters->SetVelocity(i, Vector2{ -critters->GetVelocity(i).x, critters->GetVelocity(i).y });
            }
            if (critters->GetX(i) > screenWidth) {
                critters->SetX(i, screenWidth);
                critters->SetVelocity(i, Vector2{ -critters->GetVelocity(i).x, critters->GetVelocity(i).y });
            }
            if (critters->GetY(i) < 0) {
                critters->SetY(i, 0);
                critters->SetVelocity(i, Vector2{ critters->GetVelocity(i).x, -critters->GetVelocity(i).y });
            }
            if (critters->GetY(i) > screenHeight) {
                critters->SetY(i, screenHeight);
                critters->SetVelocity(i, Vector2{ critters->GetVelocity(i).x, -critters->GetVelocity(i).y });
            }

            // kill any critter touching the destroyer
            // simple circle-to-circle collision check
            float dist = Vector2Distance(critters->GetPosition(i), destroyer->GetPosition());
            if (dist < critters->GetRadius(i) + destroyer->GetRadius())
            {
                Critter *critter = critters->DeallocateObject(i);
                critter->Destroy();
                // this would be the perfect time to put the critter into an object pool
            }

            // check for critter-on-critter collisions
            for (int j = 0; j < CRITTER_COUNT; j++) {
                if (i == j || critters->IsDirty(i)) // note: the other critter (j) could be dirty - that's OK
                    continue;
                // check every critter against every other critter
                float dist = Vector2Distance(critters->GetPosition(i), critters->GetPosition(j));
                if (dist < critters->GetRadius(i) + critters->GetRadius(j))
                {
                    // collision!
                    // do math to get critters bouncing
                    Vector2 normal = Vector2Normalize(Vector2Subtract(critters->GetPosition(j), critters->GetPosition(i)));

                    // not even close to real physics, but fine for our needs
                    critters->SetVelocity(i, Vector2Scale(normal, -MAX_VELOCITY));
                    // set the critter to *dirty* so we know not to process any more collisions on it
                    critters->SetDirty(i);

                    // we still want to check for collisions in the case where 1 critter is dirty - so we need a check 
                    // to make sure the other critter is clean before we do the collision response
                    if (!critters->IsDirty(j)) {
                        critters->SetVelocity(j, Vector2Scale(normal, MAX_VELOCITY));
                        critters->SetDirty(j);
                    }
                    break;
                }
            }
        }

        for (int i = 0; i < critters->InactiveObjects(); i++)
        {
            if (critters->InactiveObjects() <= 0)
                break;
            if (critters->IsDead(i) && respawn == true)
            {
                respawn = false;

                Vector2 normal = Vector2Normalize(destroyer->GetVelocity());

                // get a position behind the destroyer, and far enough away that the critter won't bump into it again
                Vector2 pos = destroyer->GetPosition();
                pos = Vector2Add(pos, Vector2Scale(normal, -50));
                // its pretty ineficient to keep reloading textures. ...if only there was something else we could do
                Critter *critter = critters->AllocateObject();
                critter->Init(pos, Vector2Scale(normal, -MAX_VELOCITY), 12, textureMap["CRITTER"]);
                break;
            }
        }

        timer -= delta;
        if (timer <= 0)
        {
            timer = 1;
            respawn = true;
            nextSpawnPos = destroyer->GetPosition();
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // draw the critters
        critters->Draw();
        // draw the destroyer
        // (if you're wondering why it looks a little odd when sometimes critters are destroyed when they're not quite touching the 
        // destroyer, it's because the origin is at the top-left. ...you could fix that!)
        destroyer->Draw();

        DrawFPS(10, 10);
        //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    for (const auto &[key, value] : textureMap)
    {
        UnloadTexture(value);
    }

    destroyer->Destroy();
    delete destroyer;

    delete critters;

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}