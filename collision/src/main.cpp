

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
        c->angle += 1;
        //   c->life -= 1;
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
        //  if (c->type == CIRCLE)
        //         DrawCircle(c->position.x, c->position.y, c->shape.circle.radius, RED);
        // else 
        //     if (c->type == RECT)
        //         DrawRectangle(c->position.x, c->position.y, c->shape.rect.w, c->shape.rect.h, RED);
        }
}

void player_update(Component *c, float delta)
{
   		c->position.x = GetMouseX();
		c->position.y = GetMouseY();

      
	
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

    u32 playerID;

    bool useLayer = false;
    {
            Entity e = ecs.create();
            playerID = e.id;
            Component c;
            c.position.x = screenWidth / 2;
            c.position.y = screenHeight / 2;
            c.scale = Vec2(1, 1);
            c.pivot = Vec2(0.5, 0.5);
            c.velocity.x = Math::range() * 8;
            c.velocity.y = Math::range() * 5 - 2.5;
            c.depth= GetRandomValue(0, 3);
            c.life = 100;
            c.set_box(60,60);
            c.graph = 0;
            c.load = bunny_load;
            c.update = bunny_update;
            c.render = bunny_render;
            ecs.add(e.id, &c);
    }
    {
            Entity e = ecs.create();
            Component c;
            c.position.x = screenWidth / 2;
            c.position.y = screenHeight / 2;
            c.scale = Vec2(1, 1);
            c.pivot = Vec2(0.5, 0.5);
            c.velocity.x = Math::range() * 8;
            c.velocity.y = Math::range() * 5 - 2.5;
            c.depth= GetRandomValue(0, 3);
            c.life = 100;
            c.set_circle(30);
            c.graph = 0;
            c.load = bunny_load;
            c.update = player_update;
            c.render = bunny_render;
            ecs.add(e.id, &c);
    }

    

    while (!WindowShouldClose())
    {

           

 
            if(IsKeyPressed(KEY_SPACE))
            {
                useLayer = !useLayer;
                ecs.set_layers(useLayer);
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

        if (ecs.collide(playerID))
        {
            DrawText("COLLISION", 10, 90, 20, GREEN);
        }


        EndDrawing();

       
    }
    



    Assets::as().clear();



    CloseWindow();

    return 0;
}
