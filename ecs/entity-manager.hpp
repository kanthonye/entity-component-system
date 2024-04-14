//
//  esm.hpp
//  new-engine
//
//  Created by Kenneth Esdaile on 1/18/24.
//

#ifndef esm_hpp
#define esm_hpp

#include "entity-components.hpp"

namespace kege{
    class Entity;
    typedef std::set< kege::Entity > EntitySet;

    /**
     * @brief EntityManager groups entities based on component signatures.
     *
     * The EntitySetManager (EGM) is responsible for organizing entities into sets based on their component signatures.
     * It provides functions to integrate entities into sets and segregate entities from sets, making it easier to
     * query and manage entities with specific combinations of components.
     *
     * @see kege::PoolIndex component-object
     */
    class EntityManager : public kege::RefCounter
    {
    public:

         template< typename... C > EntitySet* getEntitySet()
         {
             ecs::ComponentSignature signature;
             (ecs::ComponentBitmask< C... >( signature ));
             return get( signature );
         }

        /**
         * @brief Integrate an entity into a set based on its component bitmask.
         *
         * @param entity The ID of the entity to integrate into the set.
         * @param signature The bitmask representing the entity's component signature.
         */
        void integrate( const kege::Entity& entity, const ecs::ComponentSignature& signature );

        /**
         * @brief Segregate an entity from a set based on its component bitmask.
         *
         * @param entity The ID of the entity to segregate from the set.
         * @param signature The bitmask representing the entity's component signature.
         */
        void segregate( const kege::Entity& entity, const ecs::ComponentSignature& signature );

        /**
         * @fn getEntitySet
         * @brief Get a set of entities that shares a specific component signature.
         * @param signature The bitmask representing the desired component signature.
         * @return A pointer to the set of entities matching the specified component signature.
         */
        EntitySet* get( const ecs::ComponentSignature& signature );

        /**
         * @brief Remove a set of entities based on a specific component bitmask.
         *
         * @param signature The bitmask representing the component signature of the set to remove.
         */
        void remove( const ecs::ComponentSignature& signature );

        /**
         * @brief Clear all sets and remove all entities from the EntitySetManager.
         */
        void clear();

    private:

        std::unordered_map< ecs::ComponentSignature, EntitySet* > _sets;
    };

}

#endif /* esm_hpp */
