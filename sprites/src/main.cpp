

#include "pch.h"
#include <raylib.h>
#include <Config.hpp>
#include <Core.hpp>
#include <Utils.hpp>


const int screenWidth = 1024;
const int screenHeight = 720;



float gravity = 0.5;
int maxX = 0;
int maxY = 0;
int minX = 0;
int minY = 0;

void bunny_load(Component *c)
{
}

void bunny_update(Component *c, float delta)
{
   		c->position.x += c->velocity.x;
		c->position.y += c->velocity.y;

        c->velocity.y += gravity;

        if (c->position.x  > maxX)
        {
            c->velocity.x *= -1;
            c->position.x  = maxX;
        }
        else if (c->position.x  < minX)
        {
            c->velocity.x *= -1;
            c->position.x  = minX;
        }

        if (c->position.y > maxY)
        {
            c->velocity.y *= -0.8;
            c->position.y = maxY;

            if (Math::range() > 0.5)
                c->velocity.y -= 3 + Math::range() * 4;
        }
        else if (c->position.y < minY)
        {
            c->velocity.y = 0;
            c->position.y = minY;
        }

        c->life -= 1;
	
	
}

void bunny_render(Component *c)
{
   
        Graph *graph = Assets::as().get_graph(c->graph);
        if (graph == nullptr)
        {
            DrawCircle(c->position.x, c->position.y, 10, RED);
        } else 
        {
           // DrawTexture(graph->texture, c->position.x, c->position.y, c->color);
           // static void Draw(const Texture2D &texture, const Vec2 &position, const Vec2 &pivot, const Vec2 &scale, double rotation, const Rectangle &clip, bool flipx, bool flipy, const Color &tint);
   
            Render::Draw(graph->texture,c->position,c->pivot,c->scale,c->angle,graph->clip, c->flipX,c->flipY,c->color);
          //  DrawCircle(c->position.x, c->position.y, c->shape.circle.radius, RED);
        }
}


ECS ecs;



int main()
{

    InitWindow(screenWidth, screenHeight, "BuEngine");
    SetTargetFPS(60);

    Assets::as().load_graph("assets/wabbit_alpha.png", 0);





    maxX = screenWidth -40;
    maxY = screenHeight - 40;
    minX= 10;
    minY = 10;


    bool useLayer = false;


    

    while (!WindowShouldClose())
    {

            // if (GetFPS() >= 59)
            // {
            //      for (int i = 0; i < 500; i++)
            // {
            //          Entity e = ecs.create();
            //          Component c;
 
            //              c.position.x = GetMouseX();
            //              c.position.y = GetMouseY();
            //              c.scale = Vec2(1, 1);
            //              c.pivot = Vec2(0.5, 0.5);
            //              c.velocity.x = Math::range() * 8;
            //              c.velocity.y = Math::range() * 5 - 2.5;
            //              c.depth= GetRandomValue(0, 3);
            //              c.set_circle(10);
            //              if (c.depth == 0)
            //              {
            //                  c.color= RED;
            //                  c.life = 400;
            //              }
            //              else if (c.depth == 1)
            //              {
            //                  c.color= GREEN;
            //                  c.life = 300;
            //              } else if (c.depth == 2)
            //              {
            //                  c.color= BLUE;
            //                  c.life = 200;
            //              } else if (c.depth == 3)
            //              {
            //                  c.color= YELLOW;
            //                  c.life = 100;
            //              }
            //              c.graph = 0;
            //              c.load = bunny_load;
            //              c.update = bunny_update;
            //              c.render = bunny_render;
            //              ecs.add(e.id, &c);
         
            // }
            // }

 
            if(IsKeyPressed(KEY_SPACE))
            {
                useLayer = !useLayer;
                ecs.set_layers(useLayer);
            }
        

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            for (int i = 0; i < 500; i++)
            {
                     Entity e = ecs.create();
                     Component c;
 
                         c.position.x = GetMouseX();
                         c.position.y = GetMouseY();
                         c.scale = Vec2(1, 1);
                         c.pivot = Vec2(0.5, 0.5);
                         c.velocity.x = Math::range() * 8;
                         c.velocity.y = Math::range() * 5 - 2.5;
                         c.depth= GetRandomValue(0, 3);
                         if (c.depth == 0)
                         {
                             c.color= RED;
                             c.life = 400;
                         }
                         else if (c.depth == 1)
                         {
                             c.color= GREEN;
                             c.life = 300;
                         } else if (c.depth == 2)
                         {
                             c.color= BLUE;
                             c.life = 200;
                         } else if (c.depth == 3)
                         {
                             c.color= YELLOW;
                             c.life = 100;
                         }
                         c.graph = 0;
                         c.load = bunny_load;
                         c.update = bunny_update;
                         c.render = bunny_render;
                         ecs.add(e.id, &c);
         
            }
        }





        BeginDrawing();

        ClearBackground(BLACK);


        u32 count = ecs.update(GetFrameTime());

        ecs.render();
      

      DrawRectangle(0, 0, 160,80, Fade(RED, 0.5f));

        DrawFPS(10, 10);
        DrawText(TextFormat("Count: %d", count), 10, 30, 20, GREEN);
        if (useLayer)
            DrawText("Layers: ON", 10, 60, 20, GREEN);
        else
            DrawText("Layers: OFF", 10, 60, 20, GREEN);

        EndDrawing();

       
    }
    



    Assets::as().clear();



    CloseWindow();

    return 0;
}
