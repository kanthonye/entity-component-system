//
//  esm.cpp
//  new-engine
//
//  Created by Kenneth Esdaile on 1/18/24.
//

#include "entity.hpp"
#include "entity-manager.hpp"

namespace kege{

    void EntityManager::integrate( const kege::Entity& entity, const ecs::ComponentSignature& signature )
    {
        // insert the entity into all the sets that share the same components.
        for ( auto& i : _sets )
        {
            if ( (i.first & signature) != i.first )
            {
                continue;
            }
            i.second->insert( entity );
        }
    }

    void EntityManager::segregate( const kege::Entity& entity, const ecs::ComponentSignature& signature )
    {
        // remove the entity from all the sets that share the same components.
        for ( auto& i : _sets )
        {
            if ( (i.first & signature) != signature ) continue;
            i.second->erase( entity );
        }
    }

    EntitySet* EntityManager::get( const ecs::ComponentSignature& signature )
    {
        if( !signature.none() )
        {
            std::set< kege::Entity >* entity_set = nullptr;
            auto i = _sets.find( signature );
            if( i == _sets.end() )
            {
                entity_set = new std::set< kege::Entity >;
                _sets[ signature ] = entity_set;
            }
            else entity_set = i->second;
            return entity_set;
        }
        return nullptr;
    }

    void EntityManager::remove( const ecs::ComponentSignature& signature )
    {
        if( _sets.find( signature ) != _sets.end() )
        {
            _sets.erase( _sets.find( signature ) );
        }
    }

    void EntityManager::clear()
    {
        for ( auto& i : _sets )
        {
            delete i.second;
        }
        _sets.clear();
    }


}


