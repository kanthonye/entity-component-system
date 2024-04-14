//
//  entity-iterator.cpp
//  new-engine
//
//  Created by Kenneth Esdaile on 1/18/24.
//

#include "entity-iterator.hpp"
namespace kege{

    EntityIterator::EntityIterator(component::Child* child)
    :   _child( child )
    {}

    void EntityIterator::operator ++(int)
    {
        _child = _child->next.get< component::Child >();
    }

    void EntityIterator::operator --(int)
    {
        _child = _child->prev.get< component::Child >();
    }

    bool EntityIterator::operator !=( const void* p )const
    {
        return _child != p;
    }

    bool EntityIterator::operator ==( const void* p )const
    {
        return _child == p;
    }

    const Entity& EntityIterator::operator ->()const
    {
        return _child->entity;
    }

    Entity* EntityIterator::operator ->()
    {
        return &_child->entity;
    }

    const Entity& EntityIterator::operator *()const
    {
        return _child->entity;
    }

    Entity& EntityIterator::operator *()
    {
        return _child->entity;
    }
}


namespace kege{

    ConstEntityIterator::ConstEntityIterator(const component::Child* child)
    :   _child( child )
    {}

    void ConstEntityIterator::operator ++(int)
    {
        _child = _child->next.get< component::Child >();
    }

    void ConstEntityIterator::operator --(int)
    {
        _child = _child->prev.get< component::Child >();
    }

    bool ConstEntityIterator::operator !=( const void* p )const
    {
        return _child != p;
    }

    bool ConstEntityIterator::operator ==( const void* p )const
    {
        return _child == p;
    }

    const Entity& ConstEntityIterator::operator ->()const
    {
        return _child->entity;
    }

    const Entity& ConstEntityIterator::operator *()const
    {
        return _child->entity;
    }
}
