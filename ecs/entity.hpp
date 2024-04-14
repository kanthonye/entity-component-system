//
//  entity.hpp
//  new-engine
//
//  Created by Kenneth Esdaile on 1/18/24.
//

#ifndef entity_hpp
#define entity_hpp

#include "ref.hpp"
#include "channel.hpp"
#include "kege-string.hpp"
#include "ecs-core.hpp"
#include "entity-manager.hpp"

namespace kege
{
    class EntityIterator;
    class ConstEntityIterator;

    class Entity
    {
    public:

        /**
         * @fn Initialize the Entity Component System (Core).
         *
         * @note This function must be called once, and before using the Entity class. For
         * example calling this function at the begining of your application initialization process,
         *
         * @param ecs The entity-component-system core
         * @param em The entity-manager which group entities by component
         *
         * @return True if the initialization was successful, Talse otherwise.
         */
        static bool init( kege::ecs::Core* ecs, kege::EntityManager* em );

        /**
         * @fn Create an Entity
         */
        static kege::Entity create();

        /**
         * @fn Get Core (Entity Component System)
         * @return Pointer to Core
         */
        static ecs::Core* ecs();

        /**
         * @fn Un-initialize the Entity Component System (Core). At the
         * end of your application you shouls call this function
         */
        static void uninit();

        /**
         * @fn Attach a  child entity at the front of the list
         * @param entity the child to attach to the front of the list.
         * @return return the child entity.
         */
        kege::Entity pretach( kege::Entity entity );

        /**
         * @fn Attach a  child entity at the back of the list
         * @param entity the child to attach to the back of the list.
         * @return return the child entity.
         */
        kege::Entity attach( kege::Entity entity );

        /**
         * @fn Set this entity
         */
        kege::Entity& operator =( const kege::Entity& other );

        /**
         * @fn Set this entity id
         */
        kege::Entity& operator =( const ecs::EntityId& id );

        /**
         * @fn Comparison operator (operator<). std::set uses this operator to determine the order of elements in the set.
         */
        bool operator<( const kege::Entity& other )const;

        /**
         * @fn Check the validation of an entity
         */
        operator bool()const;

        /**
         * @fn Get child entity
         * @param name is the name of the entity to remove.
         * @return return a child entity if exit.
         */
        kege::Entity getChild( const kege::string& name );

        kege::Entity getFirstChild();
        kege::Entity getLastChild();

        /**
        The `add()` function is used to add a new component to an existing entity
        */
        template<typename T> T* add( const T& component );
        template<typename T> T* add();

        /**
        The `get()` function is used to retrieve a component data from an entity
        */
        template<typename T> const T* get()const;
        template<typename T> T* get();

        /**
        The `has()` function can be used to check if an entity has a specific component associated with it
        */
        template<typename T> bool has()const;

        /**
        the `erase()` function removes a component from this entity
        */
        template<typename T> void erase();

        static kege::EntityManager* getEntityMngr();

        /**
         * Get entity id
         */
        const ecs::EntityId& id()const;

        /**
         * Integrate the entity into the system. This makes it visible to all systems that fits its signature
         */
        void integrate();

        /**
         * Remove this entity from the system. This makes it invisible to all systems.
         */
        void segregate();

        ConstEntityIterator cbegin()const;
        ConstEntityIterator cend()const;

        EntityIterator begin();
        EntityIterator end();

        /**
         * @fn Get the parent entitiy of this entity
         */
        kege::Entity getParent()const;

        /**
         * @fn Get the root entitiy of this entity
         */
        kege::Entity getRoot()const;

        /**
         * @fn Dettach the front child entitiy from this entity
         */
        void detachFrontChild();

        /**
         * @fn Dettach the back child entitiy from this entity
         */
        void detachBackChild();

        /**
         * @fn Dettach all child entities from this entity
         */
        void detachChildren();

        /**
         * @fn Check if this entity is a parent
         */
        bool isParent()const;

        /**
         * @fn Check if this entity is a child entity
         */
        bool isChild()const;

        /**
         * @fn Dettach this entity from it's parent entity
         */
        void detach();

        /**
         * @fn Destroy entity.
         */
        void destroy();
        void destroyChildren();

        Entity( const ecs::EntityId& id );
        Entity( const kege::Entity& other );
        Entity();

    private:

        ecs::EntityId _id;
        static kege::EntityManager* _entity_mngr;
        static ecs::Core* _core;
        friend ecs::Core;
    };

    std::ostream& operator << (std::ostream& s, const kege::Entity& entity );
}


namespace kege
{
    template<typename T> T* Entity::add( const T& component )
    {
        return _core->add< T >( _id, component );
    }

    template<typename T> T* Entity::add()
    {
        return _core->add< T >( _id );
    }

    template<typename T> bool Entity::has()const
    {
        return _core->has< T >( _id );
    }

    template<typename T> void Entity::erase()
    {
        _core->erase< T >( _id );
    }

    template<typename T> const T* Entity::get()const
    {
        return _core->get< T >( _id );
    }

    template<typename T> T* Entity::get()
    {
        return _core->get< T >( _id );
    }

    inline bool operator ==( const kege::Entity& e1, const kege::Entity& e2 )
    {
        return e1.id() == e2.id();
    }

    inline bool operator !=( const kege::Entity& e1, const kege::Entity& e2 )
    {
        return e1.id() != e2.id();
    }





    struct EntityEvent
    {
        kege::Entity entity;
        uint32_t event;
    };
}

#endif /* entity_hpp */
