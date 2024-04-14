//
//  entity.cpp
//  new-engine
//
//  Created by Kenneth Esdaile on 1/18/24.
//

#include "entity-iterator.hpp"

namespace kege{

    bool Entity::init( kege::ecs::Core* ecs, kege::EntityManager* em )
    {
        if ( !_core && !_entity_mngr )
        {
            _core = ecs;
            _entity_mngr = em;
            return true;
        }
        return false;
    }

    kege::Entity Entity::create()
    {
        return Entity( _core->create() );
    }

    kege::ecs::Core* Entity::ecs()
    {
        return _core;
    }

    void Entity::uninit()
    {
        _core = nullptr;
        _entity_mngr = nullptr;
    }


    kege::Entity Entity::pretach( kege::Entity entity )
    {
        if ( !entity )
        {
            //KEGE_ERROR( "attemping to prepending ( attach ) non-entity as child" );
            return entity;
        };

        /* if this entity is not a parent make it a parent
         */
        component::Children* children = _core->get< component::Children >( _id );
        if ( !children )
        {
            children = _core->add< component::Children >( _id );
            children->head._id = kege::PoolIndex( -1L );
            children->tail._id = kege::PoolIndex( -1L );
            children->count = 0;
        }

        /*
         attach child-component to the child entity
         */
        component::Child* child = _core->add< component::Child >( entity._id );
        child->next._id = kege::PoolIndex( -1L );
        child->prev._id = kege::PoolIndex( -1L );
        child->entity._id = entity._id;
        child->parent._id = this->_id;

        /*
         get the component id of the  child-component
         */
        if ( !children->head )
        {
            /* set head and tail component id to child component id
             */
            children->head._id = children->tail._id = entity._id;
        }
        else
        {
            component::Child* head = children->head.get< component::Child >();

            // connect the new child before the head of the list
            head->prev = entity;
            child->next = children->head;

            // update the new child as the head of the list
            children->head = entity;
        }

        // increase the child count
        children->count++;

        // post message of new entity child added
        //kege::Channel::broadcast< const kege::EntityEvent&, const kege::EntityEvent& >({ entity, kege::ENTITY_ATTACHED });
        return entity;
    }

    kege::Entity Entity::attach( kege::Entity entity )
    {
        if ( !entity )
        {
            //KEGE_ERROR( "attemping to appending ( attach ) non-entity as child" );
            return entity;
        };

        /* if this entity is not a parent make it a parent
         */
        component::Children* children = _core->get< component::Children >( _id );
        if ( !children )
        {
            children = _core->add< component::Children >( _id );
            children->head._id = kege::PoolIndex(-1L);
            children->tail._id = kege::PoolIndex(-1L);
            children->count = 0;
        }

        /*
         attach child-component to the child entity
         */
        component::Child* child = _core->add< component::Child >( entity._id );
        child->next._id = kege::PoolIndex( -1L );
        child->prev._id = kege::PoolIndex( -1L );
        child->entity._id = entity._id;
        child->parent._id = this->_id;

        /* get the component id of the  child-component
         */
        if ( !children->head )
        {
            /* set head and tail component id to child component id
             */
            children->tail = children->head = entity;
        }
        else
        {
            component::Child* tail = children->tail.get< component::Child >();

            // add child to tail of the list
            tail->next = entity;
            child->prev = tail->entity;

            // update the child as tail of the list
            children->tail = entity;
        }

        // increase child count
        children->count++;

        // post message of new entity child added
        //kege::Channel::broadcast< const kege::EntityEvent&, const kege::EntityEvent& >({ entity, kege::ENTITY_ATTACHED });
        return entity;
    }

    kege::Entity& Entity::operator =( const kege::ecs::EntityId& id )
    {
        _id = id;
        return *this;
    }

    kege::Entity& Entity::operator =( const kege::Entity& other )
    {
        _id = other._id;
        return *this;
    }

    bool Entity::operator<( const kege::Entity& other )const 
    {
        return _id._index < other._id._index;
    }

    Entity::operator bool()const
    {
        return _id.operator bool();
    }

    kege::Entity Entity::getChild( const kege::string& name )
    {
        /*
         if this entity is not a parent it has no child.
         */
        if ( !has< component::Children >() )
        {
            return {};
        }

        /*
         if this entity is a parent find child. only entities with a comp::Tag can be retrieved.
         */
        component::Tag*  cname;
        for (EntityIterator i = begin(); i != nullptr ; i++ )
        {
            cname = i->get< component::Tag >();
            if ( !cname )
                continue;

            if ( *cname == name )
                return *i;
        }

        /*
         if the entity is not in parent entity child, search child entities.
         */
        kege::Entity entity;
        for (EntityIterator i = begin(); i != nullptr ; i++ )
        {
            entity = this->getChild( name );
            if ( entity )
            {
                return entity;
            }
        }

        /* entity not found.
         */
        return {};
    }

    kege::Entity Entity::getFirstChild()
    {
        const component::Children* children = _core->get< component::Children >( _id );
        if ( !children ) return {};
        return children->head;
    }
    
    kege::Entity Entity::getLastChild()
    {
        const component::Children* children = _core->get< component::Children >( _id );
        if ( !children ) return {};
        return children->tail;
    }

    kege::Entity Entity::getParent()const
    {
        const component::Child* child = _core->get< component::Child >( _id );
        if ( !child ) return {};
        return Entity( child->parent );
    }

    kege::Entity Entity::getRoot()const
    {
        const component::Child* child = this->get< component::Child >();;
        if( child )
        {
            while( child->parent )
            {
                child = child->parent.get< component::Child >();
            }
            return child->entity;
        }
        return *this;
    }

    void Entity::detachFrontChild()
    {
        component::Children* children = _core->get< component::Children >( _id );
        if ( !children )
        {
            component::Child* head = children->head.get< component::Child >();
            children->head = head->next;

            if ( head->next.has< component::Child >() )
            {
                component::Child* next = head->next.get< component::Child >();
                next->prev._id = {};
            }
        }
    }

    void Entity::detachBackChild()
    {
        component::Children* children = _core->get< component::Children >( _id );
        if ( !children )
        {
            component::Child* tail = children->tail.get< component::Child >();
            children->tail = tail->prev;
            if ( tail->prev.has< component::Child >() )
            {
                component::Child* prev = tail->prev.get< component::Child >();
                prev->next._id = {};
            }
        }
    }

    void Entity::detachChildren()
    {
        if ( _core->has< component::Children >( _id ) )
        {
            for (EntityIterator i = begin(); i != nullptr ; i++ )
            {
                i->erase< component::Child >();
            }
            _core->erase< component::Children >( _id );
        }
    }

    bool Entity::isParent()const
    {
        return _core->has< component::Children >( _id );
    }

    bool Entity::isChild()const
    {
        return _core->has< component::Child >( _id );
    }

    void Entity::detach()
    {
        if ( _core->has< component::Child >( _id ) )
        {
            component::Child* child = _core->get< component::Child >( _id );
            component::Children* children = child->parent.get< component::Children >();

            if ( child->entity == children->head )
            {
                children->head = child->next;
                component::Child* prev = child->prev.get< component::Child >();
                if ( prev )
                {
                    prev->prev._id = kege::PoolIndex( -1L );
                }
            }
            else if( child->entity == children->tail )
            {
                children->tail = child->prev;
                if ( children->tail )
                {
                    component::Child* tail = children->tail.get< component::Child >();
                    tail->next = kege::PoolIndex( -1L );
                }
            }
            else
            {
                component::Child* prev = child->prev.get< component::Child >();
                component::Child* next = child->next.get< component::Child >();
                prev->next = next->entity;
                next->prev = prev->entity;
            }

            children->count--;
        }
    }


    kege::EntityManager* Entity::getEntityMngr()
    {
        return _entity_mngr;
    }

    const kege::ecs::EntityId& Entity::id()const
    {
        return _id;
    }

    void Entity::integrate()
    {
        _entity_mngr->integrate( *this, _core->signature( _id ) );
        for (EntityIterator entity = begin(); entity != nullptr ; entity++ )
        {
            entity->integrate();
        }
    }

    void Entity::segregate()
    {
        for (EntityIterator entity = begin(); entity != nullptr ; entity++ )
        {
            entity->integrate();
        }
        _entity_mngr->segregate( *this, _core->signature( _id ) );
    }

    ConstEntityIterator Entity::cbegin()const
    {
        const component::Children* component = _core->get< component::Children >( _id );
        if( component == nullptr ) return ConstEntityIterator( nullptr );
        return ConstEntityIterator( component->head.get< component::Child >() );
    }

    ConstEntityIterator Entity::cend()const
    {
        const component::Children* component = _core->get< component::Children >( _id );
        if( component == nullptr ) return ConstEntityIterator( nullptr );
        return ConstEntityIterator( component->tail.get< component::Child >() );
    }

    EntityIterator Entity::begin()
    {
        component::Children* component = _core->get< component::Children >( _id );
        if( component == nullptr ) return EntityIterator( nullptr );
        return EntityIterator( component->head.get< component::Child >() );
    }

    EntityIterator Entity::end()
    {
        component::Children* component = _core->get< component::Children >( _id );
        if( component == nullptr ) return EntityIterator( nullptr );
        return EntityIterator( component->tail.get< component::Child >() );
    }

    void Entity::destroyChildren()
    {
        for (EntityIterator entity = begin(); entity != nullptr ; entity++ )
        {
            entity->destroy();
        }
    }

    void Entity::destroy()
    {
        destroyChildren();
        _entity_mngr->segregate( *this, _core->signature( _id ) );
        _core->destroy( _id );
        _id = {};
    }

    Entity::Entity( const ecs::EntityId& id )
    :   _id( id )
    {}

    Entity::Entity( const Entity& entity )
    :   _id( entity._id )
    {}

    Entity::Entity()
    :   _id()
    {}


    kege::ecs::Core* Entity::_core = nullptr;
    kege::EntityManager* Entity::_entity_mngr = nullptr;


    std::ostream& operator << (std::ostream& s, const kege::Entity& entity )
    {
        s <<"Entity: "<< entity.id()._index;
        return s;
    }
}
