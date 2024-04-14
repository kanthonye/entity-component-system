//
//  ecc.cpp
//  new-engine
//
//  Created by Kenneth Esdaile on 1/18/24.
//

#include "entity-components.hpp"

namespace kege{namespace ecs{

    void EntityComponents::insert( uint32_t type, kege::PoolIndex pool_index )
    {
        _components[ type ] = pool_index;
        _signature.set( type );
    }

    kege::PoolIndex EntityComponents::get( uint32_t type )const
    {
        EntityComponents::Map::const_iterator i  = _components.find( type );
        if ( i != _components.end() ) return i->second;
        return {};
    }

    bool EntityComponents::contained( uint32_t type )const
    {
        return _signature[ type ];
    }

    void EntityComponents::erase( uint32_t type )
    {
        if ( contained( type ) )
        {
            _components.erase( _components.find( type ) );
            _signature.reset( type );
        }
    }

    EntityComponents::EntityComponents()
    {}

}}
