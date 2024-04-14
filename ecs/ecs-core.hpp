//
//  ecs.hpp
//  new-engine
//
//  Created by Kenneth Esdaile on 1/18/24.
//

#ifndef ecs_hpp
#define ecs_hpp

#include "entity-components.hpp"

namespace kege{namespace ecs{

    typedef kege::PoolIndex EntityId;

    class Core : public kege::RefCounter
    {
    public:

        const ecs::ComponentSignature& signature( const EntityId& entity )const;

        /**
         * @fn add
         * @brief This method is use to add a component to a valid entity.
         * @param entity The entity to add the component too.
         * @param component The component data to set the component to.
         * @tparam T The type of component to return
         * @return A component of type T, or null otherwise
         */
        template<typename T> T* add( const EntityId& entity, const T& component );

        /**
         * @fn add
         * @brief This method is use to add a component to a valid entity.
         * @param entity The entity to add the component too.
         * @tparam T The type of component to add
         * @return A pointer to the component
         */
        template<typename T> T* add( const EntityId& entity );

        /**
         * @fn has
         * @brief This method is check if an entity has a certain component.
         * @param entity The entity to check.
         * @tparam T The type of component to look for
         * @return A true if the component exist, false otherwise.
         */
        template<typename T> bool has( const EntityId& entity )const;

        /**
         * @fn erase
         * @brief This method is remove a specified component from an entity.
         * @param entity The entity to remove the component from.
         * @tparam T The type of component to erase
         */
        template<typename T> void erase( const EntityId& entity );

        /**
         * @fn get
         * @brief This method is used to get a specified component from an entity.
         * @param entity The entity to retrieve the component from.
         * @tparam T The type of component to get
         * @return A pointer to the component
         */
        template<typename T> T* get( const EntityId& entity );

        /**
         * @fn Get the type index of a template typename
         * @return The index value for a specific data type. The index value is unique for each distinct types
         */
        template<typename T> uint32_t getTypeIndex()const;

        /**
         * @fn destroy the given entity
         * @brief This method erase an entity from this EntityComponentSystem.
         * @param entity The entity erase.
         */
        void destroy(const EntityId& entity);

        /**
         * @fn empty
         * @brief This method is used to check is an entity has no components.
         * @param entity The entity to check.
         */
        bool empty(const EntityId& entity)const;

        /**
         * @fn Remove all the components from an entity
         * @brief This method erase all components from the entity.
         * @param entity The entity erase.
         */
        void clear(const EntityId& entity);

        /**
         * @fn Create an entity
         * @brief This method is used to create an empty entity.
         * @return The EntityId for the create Entity.
         */
        EntityId create();

        Core( uint32_t max_distinct_components );
        ~Core();

    private:

        kege::PoolMngrT< ecs::EntityComponents > _entities;
        kege::PoolSysm _components;
    };

}}






namespace kege{namespace ecs{

    template<typename T> T* Core::add( const EntityId& entity, const T& component )
    {
        kege::PoolIndex pool_index;
        const uint32_t type_index = getTypeIndex< T >();
        T* ptr = _components.allocate< T >( type_index, &pool_index );
        _entities.get( entity )->insert( type_index, pool_index );
        *ptr = component;
        return ptr;
    }

    template<typename T> T* Core::add( const EntityId& entity )
    {
        kege::PoolIndex pool_index;
        const uint32_t type_index = getTypeIndex< T >();
        T* component = _components.allocate< T >( type_index, &pool_index );
        _entities.get( entity )->insert( type_index, pool_index );
        return component;
    }

    template<typename T> bool Core::has( const EntityId& entity )const
    {
        return _entities.get( entity )->contained( getTypeIndex< T >() );
    }

    template<typename T> void Core::erase( const EntityId& entity )
    {
        const uint32_t type_index = getTypeIndex< T >();
        ecs::EntityComponents* container = _entities.get( entity );
        kege::PoolIndex component = container->get( type_index );
        if( component )
        {
            _components.free( type_index, component );
            container->erase( type_index );
        }
    }

    template<typename T> T* Core::get( const EntityId& entity )
    {
        if( !entity ) return nullptr;
        const uint32_t type_index = getTypeIndex< T >();
        kege::PoolIndex pool_index = _entities.get( entity )->get( type_index );
        return _components.get< T >( type_index, pool_index );
    }


    template<typename T> uint32_t Core::getTypeIndex()const
    {
        return kege::TypeId< Core, T >::get();
    }
    
}}
#endif /* ecs_hpp */
