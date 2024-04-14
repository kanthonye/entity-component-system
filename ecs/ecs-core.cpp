//
//  ecs.cpp
//  new-engine
//
//  Created by Kenneth Esdaile on 1/18/24.
//


#include "ecs-core.hpp"

namespace kege{namespace ecs{

    const ecs::ComponentSignature& Core::signature( const EntityId& entity )const
    {
        return _entities.get( entity )->_signature;
    }

    Core::Core( uint32_t max_distinct_components )
    :   _components( max_distinct_components, 8, 6 )
    ,   _entities( 8, 6 )
    {}

    void Core::destroy( const ecs::EntityId& entity )
    {
        ecs::EntityComponents* container = _entities.get( entity );
        std::unordered_map<uint32_t, kege::PoolIndex>::iterator i;
        for (i = container->_components.begin(); i != container->_components.end(); i++)
        {
            _components.free(i->first, i->second);
        }
    }

    void Core::clear(const ecs::EntityId& entity)
    {
        ecs::EntityComponents* container = _entities.get( entity );
        std::unordered_map<uint32_t, kege::PoolIndex>::iterator i;
        for (i = container->_components.begin(); i != container->_components.end(); i++)
        {
            _components.free(i->first, i->second);
        }
    }

    ecs::EntityId Core::create()
    {
        ecs::EntityId id;
        _entities.allocate( &id );
        return id;
    }

    Core::~Core()
    {
    }

}}
