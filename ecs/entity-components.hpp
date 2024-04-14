//
//  ecc.hpp
//  new-engine
//
//  Created by Kenneth Esdaile on 1/18/24.
//

#ifndef ecc_hpp
#define ecc_hpp

#include <set>
#include <bitset>
#include <unordered_map>
#include "ref.hpp"
#include "kege-string.hpp"
#include "pool-sysm.hpp"
#include "type-id.hpp"

namespace kege{namespace ecs{

    class Core;
    typedef std::bitset<256> ComponentSignature;

    class EntityComponents
    {
    public:
        
        typedef std::unordered_map<uint32_t, kege::PoolIndex> Map;

        void insert( uint32_t uint32_t, kege::PoolIndex pool_index );
        kege::PoolIndex get( uint32_t uint32_t )const;
        bool contained( uint32_t uint32_t )const;
        void erase( uint32_t uint32_t );
        EntityComponents();

    private:

        ecs::EntityComponents::Map _components;
        ecs::ComponentSignature _signature;
        friend ecs::Core;
    };
    




     template<typename... C> class ComponentBitmask
     {
         public: ComponentBitmask(kege::ecs::ComponentSignature& mask) 
         {
             ( mask.set( kege::TypeId<kege::ecs::Core,C>::get() ), ... );
         }
     };

}}
#endif /* ecc_hpp */
