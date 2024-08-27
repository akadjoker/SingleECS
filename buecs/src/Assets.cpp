#include "pch.h"
#include "Core.hpp"

//
// Assets
//

bool Assets::add_sub_graph(u32 id, float x, float y, float w, float h,u32 toId)
{
    auto it = m_Graphs.find(id);
    if (it == m_Graphs.end()) 
    {
        return false;  
    }
     Graph *graph = it->second;
    if (graph)
    {
        Graph *to = new Graph();
        to->id = toId;
        to->clip.x = x;
        to->clip.y = y;
        to->clip.width = w;
        to->clip.height = h;
        to->texture = graph->texture;
        m_Graphs[toId] = to;
        return true;
    }
    return false;
}

Assets::Assets()
{
}

Assets::~Assets()
{
    clear();
}

Assets &Assets::as()
{
    static Assets instance;
    return instance;
}

u32 Assets::load_graph(const char *name)
{
    Graph *graph = new Graph();
    graph->id = GetFreeID();
    graph->sub = false;
    graph->clip.x = 0;
    graph->clip.y = 0;
    graph->clip.width = graph->texture.width;
    graph->clip.height = graph->texture.height;
    graph->texture = LoadTexture(name);
    m_Graphs[graph->id] = graph;
    return graph->id;
}

bool Assets::load_graph(const char *name, u32 id)
{
    Graph *graph = new Graph();
    graph->id = id;
    graph->sub = false;
    graph->texture = LoadTexture(name);
    graph->clip.x = 0;
    graph->clip.y = 0;
    graph->clip.width = graph->texture.width;
    graph->clip.height = graph->texture.height;
    m_Graphs[id] = graph;
    return true;
}

u32 Assets::load_atlas(const char *name, u32 id, int framesCount, int rows, int columns)
{
    Graph *graph = new Graph();
    graph->id = id;
    graph->texture = LoadTexture(name);
    graph->clip.x = 0;
    graph->clip.y = 0;
    graph->sub = false;
    graph->clip.width  = graph->texture.width;
    graph->clip.height = graph->texture.height;
    m_Atlas[id] = graph;

    int imageWidth = (int)graph->clip.width;
    int imageHeight = (int)graph->clip.height;

    int frame = 0;
    u32 startId = id;
    for (int i = 0; i < framesCount; i++)
    {

        float width = imageWidth / columns;
        float height = imageHeight / rows;
        float x = (frame % columns) * width;
        float y = (frame / columns) * height;
        Graph *to = new Graph();
        to->id = startId;
        to->texture = graph->texture;
        to->clip.x = x;
        to->clip.y = y;
        to->clip.width = width;
        to->clip.height = height;
        to->sub = true;
        m_Graphs [startId] = to;
        startId++;
        frame++;
    }
    return startId;
}

void Assets::clear_graphics()
{

        for (auto &pair : m_Graphs) 
        {
            printf("Unloading graph %d\n", pair.second->id);
            UnloadTexture(pair.second->texture);
            delete pair.second;
        }
        m_Graphs.clear();

for (auto &pair : m_Atlas) 
{
    printf("Unloading graph %d\n", pair.second->id);
    UnloadTexture(pair.second->texture);
    delete pair.second;
}
m_Atlas.clear();


}

void Assets::clear()
{
    clear_graphics();
}

Graph *Assets::get_graph(u32 id)
{
    auto it = m_Graphs.find(id);
    if (it != m_Graphs.end()) 
    {
        return it->second;
    }

    return nullptr;
}

Vec2 Assets::get_graph_size(u32 id)
{
    Graph *graph=nullptr;
   auto it = m_Graphs.find(id);
    if (it != m_Graphs.end()) 
    {
        graph = it->second;
        return Vec2(graph->texture.width, graph->texture.height);
    }
    return Vec2(0, 0);
}

u32 Assets::GetFreeID()
{
    u32 startID = 0;
    for (startID = 0; startID < 100000; startID++)
    {
       if (m_Graphs.find(startID) == m_Graphs.end())
       {
           return startID;
       }
    }
    return 0;
}
