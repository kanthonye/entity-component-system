//
//  entity-iterator.hpp
//  new-engine
//
//  Created by Kenneth Esdaile on 1/18/24.
//

#ifndef entity_iterator_hpp
#define entity_iterator_hpp

#include "entity.hpp"

namespace kege{namespace component{

    struct Child
    {
        kege::Entity parent;
        kege::Entity entity;
        kege::Entity next;
        kege::Entity prev;
    };

    struct Children
    {
        kege::Entity head;
        kege::Entity tail;
        uint32_t count = 0;
    };


    //typedef kege::transform Transform;
    typedef kege::string Tag;

}}



namespace kege{
    class Entity;
    
    class EntityIterator
    {
        public: EntityIterator( component::Child* child );

        bool operator !=( const void* p )const;
        bool operator ==( const void* p )const;

        void operator ++(int);
        void operator --(int);

        const Entity& operator ->()const;
        Entity* operator ->();

        const Entity& operator *()const;
        Entity& operator *();

        private: component::Child* _child;
    };
}


namespace kege{

    class ConstEntityIterator
    {
        public: ConstEntityIterator( const component::Child* child);
        
        bool operator !=( const void* p )const;
        bool operator ==( const void* p )const;

        void operator ++(int);
        void operator --(int);

        const Entity& operator ->()const;
        const Entity& operator *()const;

        private: const component::Child* _child;
    };

}

#endif /* entity_iterator_hpp */
