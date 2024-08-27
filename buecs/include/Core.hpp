#pragma once

#include "Config.hpp"
#include "Math.hpp"
#include <raylib.h>
#include <rlgl.h>

struct rVertex
{
    float x, y, z;
    Color col;
    float tx, ty;
};

struct rQuad
{
    rVertex v[4];
    Texture2D tex;
};

struct Graph
{
    u32 id;
    Texture2D texture;
    Rectangle clip;
    bool sub;
};

class Timer
{
public:
    Timer();
    void update();
    void start();
    void reset();
    void pause();
    void resume();
    double get_delta() const { return deltaTime; }
    double get_elapsed() const;
    bool is_paused() const { return paused; }

private:
    double startTime;
    double lastFrameTime;
    double deltaTime;
    double pausedTime;
    bool paused;
};

class Assets
{

private:
    std::unordered_map<u32, Graph *> m_Graphs;
    std::unordered_map<u32, Graph *> m_Atlas;

    Assets();
    ~Assets();

public:
    static Assets &as();

    u32 load_graph(const char *name);
    bool load_graph(const char *name, u32 id);
    u32 load_atlas(const char *name, u32 startID, int framesCount, int rows, int columns);
    bool add_sub_graph(u32 id, float x, float y, float w, float h, u32 toId);

    void clear_graphics();
    void clear();

    Graph *get_graph(u32 id);
    Vec2 get_graph_size(u32 id);

    u32 GetFreeID();
};



struct Render
{
    static float depth;
    static void Draw(const Texture2D &texture, const Vec2 &position, const Vec2 &pivot, const Vec2 &scale, double rotation, float width, float height,const  Rectangle &clip, bool flipx, bool flipy, const Color &tint);
    static void Draw(const Texture2D &texture, float x, float y, float width, float height,const  Rectangle &clip, bool flipx, bool flipy,const  Color &tint);
    static void Draw(const Texture2D &texture, const Vec2 &position, const Vec2 &pivot, const Vec2 &scale, double rotation, const Rectangle &clip, bool flipx, bool flipy, const Color &tint);
    static void Draw(const Texture2D &texture, float x, float y);
    static void Draw(const Texture2D &texture, float x, float y, float width, float height,  bool flipX, bool flipY, const Color &color);
    static void DrawPolygon(const std::vector<Vec2> &points,const  Color &c);
    static void DrawFillPolygon(const std::vector<Vec2> &polygonVertices, const  Color &c);
};

struct Collider
{
    static bool IntersectPolygons(const std::vector<Vec2> &verticesA, const std::vector<Vec2> &verticesB);
    static bool IntersectCirclePolygon(const Vec2 &center, float circleRadius, const std::vector<Vec2> &vertices);
    static void TransformPoints(std::vector<Vec2> &points, const Vec2 &position, double angle, const Vec2 &pivot, const Vec2 &scale, AABB *aabb);
    static bool IntersectAABBs(AABB a, AABB b);
};

#define FLAG_ALIVE 1

enum ShapeType
{
    NONE_SHAPE,
    CIRCLE,
    RECT
};


struct Circle 
{
 
    float radius;
    
};

struct Rect 
{

    float w, h;
};

struct Entity
{
   
    uint32_t id;

};

struct Component;

typedef void (*LoadFunc)(Component *c);
typedef void (*UpdateFunc)(Component *c,float);
typedef void (*RenderFunc)(Component *c);
typedef void (*CloseFunc)(Component *c);



struct Component
{
    union 
    {
        Circle circle;
        Rect rect;
    }shape;
    ShapeType type;
 
    Vec2 position;
    Vec2 velocity;
    Vec2 pivot;
    Vec2 scale;
    u32  graph;
    bool flipX;
    bool flipY;
    float angle;
    float life;
    u32 depth;
    Color color;
    LoadFunc load;
    UpdateFunc update;
    RenderFunc render;
    CloseFunc  close;
    Component();
    void set_box(float w, float h);
    void set_circle(float r);
};

const u32 INITIAL_CAPACITY = 32;
const u32 MAX_LAYERS = 8;

class ECS
{
private:
    
    std::vector<uint32_t> flag_array;
    std::vector<Component> components; 
    std::vector<uint32_t> entity_pool;
    std::vector<Component*> layers[MAX_LAYERS];
    bool use_layers = false;

public:
    ECS();
    ~ECS();
    void init();

    Entity create();

    Component *get(uint32_t entity_id);

    void set_layers(bool use) { use_layers = use; }


    void kill(uint32_t entity_id);

    void add(uint32_t entity_id, void *data);

    bool collide(uint32_t a);

    u32 update(double delta) ;
    void render();
};