
#include <iostream>
#include <vector>
#include "ecs/entity.hpp"

struct Position{ float x,y,z; };
struct Movement{ float x,y,z; };
struct Camera{ kege::string name; };
struct Script{ kege::string name; };
struct Mesh{ std::vector< Position > vertices; };
typedef kege::string Tag;


class System : public kege::RefCounter
{
public:
    virtual void update(){}
    virtual ~System(){}
    kege::EntitySet* entities;
};


class MeshPositionSystem : public System
{
public:
    void update()
    {
        std::cout <<"\nmesh-position-system update\n";
        for (kege::Entity entity : *entities)
        {
            std::cout << entity.get< Tag >()->c_str() <<" : (";
            std::cout << entity.get< Position >()->x <<", ";
            std::cout << entity.get< Position >()->y <<", ";
            std::cout << entity.get< Position >()->z <<")\n";
        }
    }
    MeshPositionSystem()
    {
        entities = kege::Entity::getEntityMngr()->getEntitySet< Position, Mesh >();
    }
};
class CameraSystem : public System
{
public:
    void update()
    {
        std::cout <<"\ncamera-system update\n";
        for (kege::Entity entity : *entities)
        {
            std::cout << entity.get< Camera >()->name <<" : (";
            std::cout << entity.get< Position >()->x <<", ";
            std::cout << entity.get< Position >()->y <<", ";
            std::cout << entity.get< Position >()->z <<")\n";
        }
    }
    CameraSystem()
    {
        entities = kege::Entity::getEntityMngr()->getEntitySet< Position, Camera >();
    }
};


int main()
{
    // initalize the entity system
    kege::Ref< kege::ecs::Core > core = new kege::ecs::Core( 42 );
    kege::Ref< kege::EntityManager > emgr = new kege::EntityManager;
    if ( !kege::Entity::init( core.ref(), emgr.ref() ) )
    {
        return 0;
    }

    // create systems
    std::vector< kege::Ref< System > > systems;
    systems.push_back( new CameraSystem );
    systems.push_back( new MeshPositionSystem );



    // create entities

    kege::Entity entity[10];

    entity[0] = kege::Entity::create();
    entity[0].add< Position >({ 100, 10, 34 });
    entity[0].add< Camera >({ "camera-1" });
    entity[0].integrate();

    entity[1] = kege::Entity::create();
    entity[1].add< Position >({ 100, 10, 34 });
    entity[1].add< Tag >({ "entity-00" });
    entity[1].integrate();

    char name[32];
    for (int i=2; i<10; i++)
    {
        snprintf(name, 32, "entity-%i", i);
        entity[i] = kege::Entity::create();
        entity[i].add< Position >({ float(i), 10, 34 });
        entity[i].add< Script >({});
        entity[i].add< Mesh >({});
        entity[i].add< Tag >({name});
        entity[i].integrate();
    }

    // update systems
    for (kege::Ref< System >& system : systems)
    {
        system->update();
    }
    return 0;
}
