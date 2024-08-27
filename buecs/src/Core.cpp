#include "pch.h"
#include "Math.hpp"
#include "Core.hpp"
#include "Utils.hpp"

//
// Time
//

Timer::Timer() : startTime(GetTime()), lastFrameTime(0), deltaTime(0), pausedTime(0), paused(false)
{
}

void Timer::update()
{
    if (!paused)
    {
        double currentTime = GetTime();
        deltaTime = GetFrameTime(); // currentTime - lastFrameTime;
        lastFrameTime = currentTime;
    }
    else
    {
        deltaTime = 0;
    }
}

void Timer::start()
{
    startTime = GetTime();
    lastFrameTime = 0;
    deltaTime = 0;
    pausedTime = 0;
    paused = false;
}

void Timer::reset()
{
    startTime = GetTime();
    pausedTime = 0;
    paused = false;
}
void Timer::pause()
{
    if (!paused)
    {
        pausedTime = GetTime() - lastFrameTime;
        paused = true;
    }
}

void Timer::resume()
{
    if (paused)
    {
        lastFrameTime = GetTime() - pausedTime;
        paused = false;
    }
}

double Timer::get_elapsed() const
{
    if (paused)
    {
        return pausedTime - startTime;
    }
    else
    {
        return GetTime() - startTime;
    }
}


// static void UpdateRelativeTransformation(GameObject *entity, Transform2D *transform)
// {

//     if (transform->skew.x == 0.0f && transform->skew.y == 0.0f)
//     {

//         if (transform->angle == 0.0)
//         {

//             transform->matrix.set(transform->scale.x, 0.0, 0.0, transform->scale.y,
//                                   transform->position.x - transform->pivot.x * transform->scale.x,
//                                   transform->position.y - transform->pivot.y * transform->scale.y);
//         }
//         else
//         {
//             float acos = cos(transform->angle * RAD);
//             float asin = sin(transform->angle * RAD);
//             float a = transform->scale.x * acos;
//             float b = transform->scale.x * asin;
//             float c = transform->scale.y * -asin;
//             float d = transform->scale.y * acos;
//             float tx = transform->position.x - transform->pivot.x * a - transform->pivot.y * c;
//             float ty = transform->position.y - transform->pivot.x * b - transform->pivot.y * d;

//             transform->matrix.set(a, b, c, d, tx, ty);
//         }
//     }
//     else
//     {

//         transform->matrix.identity();
//         transform->matrix.scale(transform->scale.x, transform->scale.y);
//         transform->matrix.skew(transform->skew.x, transform->skew.y);
//         transform->matrix.rotate(transform->angle);
//         transform->matrix.translate(transform->position.x, transform->position.y);

//         if (transform->pivot.x != 0.0f || transform->pivot.y != 0.0f)
//         {

//             transform->matrix.tx = transform->position.x - transform->matrix.a * transform->pivot.x - transform->matrix.c * transform->pivot.y;
//             transform->matrix.ty = transform->position.y - transform->matrix.b * transform->pivot.x - transform->matrix.d * transform->pivot.y;
//         }
//     }
// }

//
//render
//


const bool FIX_ARTIFACTS_BY_STRECHING_TEXEL = true;

static void RenderQuad(const rQuad *quad)
{

    rlCheckRenderBatchLimit(4); // Make sure there is enough free space on the batch buffer
    rlSetTexture(quad->tex.id);

    rlBegin(RL_QUADS);

    Color a = quad->v[1].col;
    Color b = quad->v[0].col;
    Color c = quad->v[3].col;
    Color d = quad->v[2].col;

    rlNormal3f(0.0f, 0.0f, 1.0f);

    rlColor4ub(a.r, a.g, a.b, a.a);
    rlTexCoord2f(quad->v[1].tx, quad->v[1].ty);
    rlVertex3f(quad->v[1].x, quad->v[1].y, quad->v[1].z);

    rlColor4ub(b.r, b.g, b.b, b.a);
    rlTexCoord2f(quad->v[0].tx, quad->v[0].ty);
    rlVertex3f(quad->v[0].x, quad->v[0].y, quad->v[0].z);

    rlColor4ub(c.r, c.g, c.b, c.a);
    rlTexCoord2f(quad->v[3].tx, quad->v[3].ty);
    rlVertex3f(quad->v[3].x, quad->v[3].y, quad->v[3].z);

    rlColor4ub(d.r, d.g, d.b, d.a);
    rlTexCoord2f(quad->v[2].tx, quad->v[2].ty);
    rlVertex3f(quad->v[2].x, quad->v[2].y, quad->v[2].z);

    rlEnd();
}


float Render::depth = 0.0f;

void Render::Draw(const Texture2D &texture, float x, float y)
{

    rQuad quad;
    quad.tex = texture;

    float u = 0.0f;
    float v = 0.0f;
    float u2 = 1.0f;
    float v2 = 1.0f;

    float fx2 = x + texture.width;
    float fy2 = y + texture.height;

    quad.v[1].x = x;
    quad.v[1].y = y;
    quad.v[1].tx = u;
    quad.v[1].ty = v;

    quad.v[0].x = x;
    quad.v[0].y = fy2;
    quad.v[0].tx = u;
    quad.v[0].ty = v2;

    quad.v[3].x = fx2;
    quad.v[3].y = fy2;
    quad.v[3].tx = u2;
    quad.v[3].ty = v2;

    quad.v[2].x = fx2;
    quad.v[2].y = y;
    quad.v[2].tx = u2;
    quad.v[2].ty = v;

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = depth;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = WHITE;

    RenderQuad(&quad);
}
void Render::Draw(const Texture2D &texture, float x, float y, float width, float height, const Rectangle &clip, bool flipx, bool flipy, const Color &tint)
{

    float fx2 = x + width;
    float fy2 = y + height;
    rQuad quad;
    quad.tex = texture;

    int widthTex = texture.width;
    int heightTex = texture.height;

    float left, right, top, bottom;

    if (FIX_ARTIFACTS_BY_STRECHING_TEXEL)
    {

        left = (2 * clip.x + 1) / (2 * widthTex);
        right = left + (clip.width * 2 - 2) / (2 * widthTex);
        top = (2 * clip.y + 1) / (2 * heightTex);
        bottom = top + (clip.height * 2 - 2) / (2 * heightTex);
    }
    else
    {
        left = clip.x / widthTex;
        right = (clip.x + clip.width) / widthTex;
        top = clip.y / heightTex;
        bottom = (clip.y + clip.height) / heightTex;
    }

    if (flipx)
    {
        float tmp = left;
        left = right;
        right = tmp;
    }

    if (flipy)
    {
        float tmp = top;
        top = bottom;
        bottom = tmp;
    }

    quad.v[1].tx = left;
    quad.v[1].ty = top;
    quad.v[1].x = x;
    quad.v[1].y = y;

    quad.v[0].x = x;
    quad.v[0].y = fy2;
    quad.v[0].tx = left;
    quad.v[0].ty = bottom;

    quad.v[3].x = fx2;
    quad.v[3].y = fy2;
    quad.v[3].tx = right;
    quad.v[3].ty = bottom;

    quad.v[2].x = fx2;
    quad.v[2].y = y;
    quad.v[2].tx = right;
    quad.v[2].ty = top;

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = depth;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = tint;

    RenderQuad(&quad);
}
void Render::Draw(const Texture2D &texture, const Vec2 &position, const Vec2 &pivot, const Vec2 &scale, double rotation, const Rectangle &clip, bool flipx, bool flipy, const Color &tint)
{

    rQuad quad;
    quad.tex = texture;

    int widthTex = texture.width;
    int heightTex = texture.height;

    float left, right, top, bottom;

    if (FIX_ARTIFACTS_BY_STRECHING_TEXEL)
    {

        left = (2 * clip.x + 1) / (2 * widthTex);
        right = left + (clip.width * 2 - 2) / (2 * widthTex);
        top = (2 * clip.y + 1) / (2 * heightTex);
        bottom = top + (clip.height * 2 - 2) / (2 * heightTex);
    }
    else
    {
        left = clip.x / widthTex;
        right = (clip.x + clip.width) / widthTex;
        top = clip.y / heightTex;
        bottom = (clip.y + clip.height) / heightTex;
    }

    if (flipx)
    {
        float tmp = left;
        left = right;
        right = tmp;
    }

    if (flipy)
    {
        float tmp = top;
        top = bottom;
        bottom = tmp;
    }
    float width = clip.width;
    float height = clip.height;
    float tx1 = width * -0.5f;
    float ty1 = height * -0.5f;
    float tx2 = width * 0.5f;
    float ty2 = height * 0.5f;


    quad.v[1].x = tx1;    quad.v[1].y = ty1;
    quad.v[0].x = tx1;    quad.v[0].y = ty2;
    quad.v[3].x = tx2;    quad.v[3].y = ty2;
    quad.v[2].x = tx2;    quad.v[2].y = ty1;



    Vec2 point;

    for (int i = 0; i < 4; i++)
    {
        point.Set(quad.v[i].x, quad.v[i].y);

        if (rotation != 0)
            point = Math::RotateVector(point, rotation * DEG2RAD, pivot);
        point.x *= scale.x;
        point.y *= scale.y;
        point.x += position.x;
        point.y += position.y;

        quad.v[i].x = point.x;
        quad.v[i].y = point.y;
    }

   

    quad.v[1].tx   = left;     quad.v[1].ty = top;
    quad.v[0].tx   = left;     quad.v[0].ty = bottom;   
    quad.v[3].tx   = right;    quad.v[3].ty = bottom;
    quad.v[2].tx   = right;     quad.v[2].ty = top;

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = depth;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = tint;

    RenderQuad(&quad);
}
void Render::Draw(const Texture2D &texture, const Vec2 &position, const Vec2 &pivot, const Vec2 &scale, double rotation, float width, float height, const Rectangle &clip, bool flipx, bool flipy, const Color &tint)
{

    rQuad quad;
    quad.tex = texture;

    int widthTex = texture.width;
    int heightTex = texture.height;

    float left, right, top, bottom;

    if (FIX_ARTIFACTS_BY_STRECHING_TEXEL)
    {

        left = (2 * clip.x + 1) / (2 * widthTex);
        right = left + (clip.width * 2 - 2) / (2 * widthTex);
        top = (2 * clip.y + 1) / (2 * heightTex);
        bottom = top + (clip.height * 2 - 2) / (2 * heightTex);
    }
    else
    {
        left = clip.x / widthTex;
        right = (clip.x + clip.width) / widthTex;
        top = clip.y / heightTex;
        bottom = (clip.y + clip.height) / heightTex;
    }

    if (flipx)
    {
        float tmp = left;
        left = right;
        right = tmp;
    }

    if (flipy)
    {
        float tmp = top;
        top = bottom;
        bottom = tmp;
    }

    float tx1 = width * -0.5f;
    float ty1 = height * -0.5f;
    float tx2 = width * 0.5f;
    float ty2 = height * 0.5f;


    quad.v[1].x = tx1;    quad.v[1].y = ty1;
    quad.v[0].x = tx1;    quad.v[0].y = ty2;
    quad.v[3].x = tx2;    quad.v[3].y = ty2;
    quad.v[2].x = tx2;    quad.v[2].y = ty1;



    Vec2 point;

    for (int i = 0; i < 4; i++)
    {
        point.Set(quad.v[i].x, quad.v[i].y);

        point = Math::RotateVector(point, rotation * DEG2RAD, pivot);
        point.x *= scale.x;
        point.y *= scale.y;
        point.x += position.x;
        point.y += position.y;

        quad.v[i].x = point.x;
        quad.v[i].y = point.y;
    }

   

    quad.v[1].tx   = left;     quad.v[1].ty = top;
    quad.v[0].tx   = left;     quad.v[0].ty = bottom;   
    quad.v[3].tx   = right;    quad.v[3].ty = bottom;
    quad.v[2].tx   = right;     quad.v[2].ty = top;

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = depth;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = tint;

    RenderQuad(&quad);
}


void Render::Draw(const Texture2D &texture, float x, float y, float width, float height, bool flipX, bool flipY,const  Color &color)
{

    rQuad quad;
    quad.tex = texture;
   

    float left = 0.0f;
    float right = 1.0f;
    float top = 0.0f;
    float bottom = 1.0f;


    if (flipX)
    {
        float tmp = left;
        left = right;
        right = tmp;
    }

    if (flipY)
    {
        float tmp = top;
        top = bottom;
        bottom = tmp;
    }

    float TempX1 = x;
    float TempY1 = y;
    float TempX2 = x + width;
    float TempY2 = y + height;

    quad.v[1].x = TempX1;
    quad.v[1].y = TempY1;
    quad.v[1].tx = left;
    quad.v[1].ty = top;

    quad.v[0].x = TempX1;
    quad.v[0].y = TempY2;
    quad.v[0].tx = left;
    quad.v[0].ty = bottom;

    quad.v[3].x = TempX2;
    quad.v[3].y = TempY2;
    quad.v[3].tx = right;
    quad.v[3].ty = bottom;

    quad.v[2].x = TempX2;
    quad.v[2].y = TempY1;
    quad.v[2].tx = right;
    quad.v[2].ty = top;

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = depth;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = color;

    RenderQuad(&quad);
}






void Render::DrawFillPolygon(const std::vector<Vec2> &v,const  Color &c)
{
    u32 pointCount = v.size();


        if (pointCount >= 3)
        {

            rlBegin(RL_QUADS);
            rlColor4ub(c.r, c.g, c.b, c.a);
            Vec2 v0;
            Vec2 v1;
            Vec2 v2;
            Vec2 v3;

            for (u32 i = 0; i < pointCount - 1; i++)
            {
                v0.Set(v[0].x, v[0].y);
                v1.Set(v[i + 1].x, v[i + 1].y);
                v2.Set(v[i + 1].x, v[i + 1].y);
                v3.Set(v[i].x, v[i].y);

                rlVertex2f(v0.x, v0.y);
                rlVertex2f(v1.x, v1.y);
                rlVertex2f(v2.x, v2.y);
                rlVertex2f(v3.x, v3.y);
            }
            rlEnd();
        }
}

void Render::DrawPolygon(const std::vector<Vec2> &points, const Color &c)
{
    int pointCount = (int)points.size();
    for (int i = 0; i < pointCount - 1; i++)
    {
        Vec2 v0(points[i].x, points[i].y);
        Vec2 v1(points[i + 1].x, points[i + 1].y);

        DrawLine(v0.x, v0.y, v1.x, v1.y, c);
    }

    Vec2 v0(points[0].x, points[0].y);
    Vec2 v1(points[pointCount - 1].x, points[pointCount - 1].y);
    DrawLine(v0.x, v0.y, v1.x, v1.y, c);
}



//
// ecs
//


static void default_load(Component *c)
{
    (void)c;
}

static void default_update(Component *c,float delta)
{
    (void)c;
    (void)delta;
}

static void default_render(Component *c)
{
   (void)c; 
}

void default_close(Component *c)
{
    (void)c;

}

ECS::ECS()
{
    init();
}

ECS::~ECS()
{
}

void ECS::init()
{
    flag_array.reserve(INITIAL_CAPACITY);
    components.reserve(INITIAL_CAPACITY);
    for (u32 i = 0; i <MAX_LAYERS; i++)
    {
       layers[i].reserve(INITIAL_CAPACITY);
    }
}

Entity ECS::create()
{
    uint32_t id;
    if (!entity_pool.empty())
    {
        id = entity_pool.back();
        entity_pool.pop_back();
    }
    else
    {
        id = flag_array.size();
        flag_array.push_back(FLAG_ALIVE);
        components.resize(components.size() + 1);
    }

    flag_array[id] = FLAG_ALIVE;
    return {id};
    
}

Component *ECS::get(uint32_t entity_id)
{
   return &components[entity_id];
}

void ECS::add(uint32_t entity_id,  void *data)
{
    void *ptr = get(entity_id);
    std::memcpy(ptr, data, sizeof(Component));
    Component *c = get(entity_id);
    c->load(c);
}


void ECS::kill(uint32_t entity_id)
{
    if (flag_array[entity_id] & FLAG_ALIVE)
    {
        Component *c = get(entity_id);
        c->close(c);
        flag_array[entity_id] &= ~FLAG_ALIVE;
        entity_pool.push_back(entity_id);
    }
}

bool CircleInCircle(const Component *a, const Component *b)
{
    float dx = a->position.x - b->position.x;
    float dy = a->position.y - b->position.y;
    float r = a->shape.circle.radius + b->shape.circle.radius;
    return dx * dx + dy * dy <= r * r;
}

bool RectInRect(const Component *a, const Component *b)
{
    std::vector<Vec2> verticesA;
    std::vector<Vec2> verticesB;

    {
        float w = a->shape.rect.w;
        float h = a->shape.rect.h;
        float left   = w  * -0.5f;
        float right  = w  * 0.5f;
        float bottom = h * -0.5f;
        float top    = h * 0.5f;
        verticesA.push_back(Vec2(left, top));
        verticesA.push_back(Vec2(right, top));
        verticesA.push_back(Vec2(right, bottom));
        verticesA.push_back(Vec2(left, bottom));
    }
    {
        float w = b->shape.rect.w;
        float h = b->shape.rect.h;
        float left   = w  * -0.5f;
        float right  = w  * 0.5f;
        float bottom = h * -0.5f;
        float top    = h * 0.5f;
        verticesB.push_back(Vec2(left, top));
        verticesB.push_back(Vec2(right, top));
        verticesB.push_back(Vec2(right, bottom));
        verticesB.push_back(Vec2(left, bottom));
    }

    AABB aabbA, aabbB;
    Collider::TransformPoints(verticesA, a->position, a->angle, a->pivot, a->scale, &aabbA);
    Collider::TransformPoints(verticesB, b->position, b->angle, b->pivot, b->scale, &aabbB);



    Render::DrawPolygon(verticesA, RED);
    Render::DrawPolygon(verticesB, GREEN);

    if (!Collider::IntersectAABBs(aabbA, aabbB))
    {
        return false;
    }

   
    return Collider::IntersectPolygons(verticesA, verticesB);
}

bool CircleInRect(const Component *a, const Component *b)
{

    AABB aabb;
    std::vector<Vec2> points;

    float w = b->shape.rect.w;
    float h = b->shape.rect.h;
    float left   = w  * -0.5f;
    float right  = w  * 0.5f;
    float bottom = h * -0.5f;
    float top    = h * 0.5f;

    points.push_back(Vec2(left, top));
    points.push_back(Vec2(right, top));
    points.push_back(Vec2(right, bottom));
    points.push_back(Vec2(left, bottom));


    Collider::TransformPoints(points, b->position, b->angle, b->pivot, b->scale, &aabb);

    Render::DrawPolygon(points, RED);
    DrawCircle(a->position.x, a->position.y, a->shape.circle.radius, RED);

  
    if (!aabb.Intersects(a->position, a->shape.circle.radius))
        return false;


    return Collider::IntersectCirclePolygon(a->position, a->shape.circle.radius, points);
  
}


bool ProcessCollide(Component *a, Component *b)
{
    if (!a || !b) return false;
    if (a->type==NONE_SHAPE || b->type == NONE_SHAPE) return false;

    if (a->type == CIRCLE)
    {
        if (b->type == CIRCLE)
        {
            return CircleInCircle(a, b);
        }
        else if (b->type == RECT)
        {
            return CircleInRect(a, b);
        }
    } else if (a->type == RECT)
    {
        if (b->type == CIRCLE)
        {
            return CircleInRect(b, a);
        }
        else if (b->type == RECT)
        {
            return RectInRect(a, b);
        }
    }
    return false;
}


bool ECS::collide(uint32_t a)
{
     Component *cA = get(a);
     if (flag_array[a] & FLAG_ALIVE)
     {
             for (u32 i = 0; i < flag_array.size(); i++)
             {  
                    if (i==a) continue;
                    if (flag_array[i] & FLAG_ALIVE)
                    {
                        Component *cB = get(i);
                        if (ProcessCollide(cA,cB)) return true;
                    }
            }
     }
    return false;
}


u32 ECS::update(double delta)
{
   u32 count = 0;
   if (use_layers)
   {
        for (u32 i = 0; i <MAX_LAYERS; i++)
        {
                if (layers[i].empty())  continue;
                layers[i].clear();
        }
   }
   for (u32 i = 0; i < flag_array.size(); i++)
   {
       if (flag_array[i] & FLAG_ALIVE)
       {
            Component *c = get(i);
            if (c->life<=0)
            {
                kill(i);
                continue;
            }
            c->update(c,delta);
            if (c->depth > MAX_LAYERS) c->depth = MAX_LAYERS;
            if (use_layers)
            {
                layers[c->depth].push_back(c);
            } else 
            {
                c->render(c);
            }
            count++;
       }
   }


  
   return count;
}

void ECS::render()
{
    if (!use_layers) return;
    for (u32 i = 0; i < MAX_LAYERS; i++)
    {
       if (layers[i].empty())  continue;
       for (u32 j = 0; j < layers[i].size(); j++)
       {
           layers[i][j]->render(layers[i][j]);
       }
   }

}

Component::Component()
{
    load = default_load;
    update = default_update;
    render = default_render;
    close = default_close;
    type = NONE_SHAPE;
    depth = 0;
    life = 0;
    position = Vec2(0, 0);
    velocity = Vec2(0, 0);
    pivot = Vec2(0, 0);
    scale = Vec2(1, 1);
    graph = 0;
    flipX = false;
    flipY = false;
    angle = 0;
    color = WHITE;
}

void Component::set_box(float w, float h)
{
    shape.rect.w = w;
    shape.rect.h = h;


    type = RECT;
}

void Component::set_circle(float r)
{
    shape.circle.radius = r;
    type= CIRCLE;
}
