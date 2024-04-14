//
//  pool-sysm.hpp
//  new-engine
//
//  Created by Kenneth Esdaile on 1/18/24.
//

#ifndef pool_sysm_hpp
#define pool_sysm_hpp

#include "pool-mngr.hpp"

namespace kege
{

    /**
     * The class `PoolSysm` manages a set of PoolMngr objects, where each PoolMngr object manages a pool of objects of a specific type.
     * @see PoolMngr, Pool
     */
    class PoolSysm
    {
    public:

        /**
         * @fn The constructor
         *
         * @param max_distinct_pool_types The total number of pool-mngrs to reserve.
         * @param growth_base The base parameter use in the function pow( base, exponent ).
         * @param growth_exponent The exponent parameter use in the function pow( base, exponent )
         *
         * @note the growth_base and growth_exponent params are use to calculate the max storage size of a pool.
         * For each new pool created in a pool-mngr, the exponent will increase with growth_exponent being the
         * max value the exponent can be. Pool size is calculated like so "pool_size = pow( base, exponent++ )"
         */
        PoolSysm(uint32_t max_distinct_pool_types, uint32_t growth_base, uint32_t growth_exponent);

        /**
         * @fn Get the data element of a pool-cell.
         *
         * @param type The index where the pool-mngr is stored.
         * @param pool_index The pool-mngr index location of the element.
         * @return A pointer to the pool-cell data element
         */
        template<typename T> const T* get(  uint32_t type, const PoolIndex& pool_index )const;
        template<typename T> T* get(  uint32_t type, const PoolIndex& pool_index );

        /**
         * Allocation a new pool cell.
         *
         * @param type The index where the pool-mngr is stored.
         * @param pool_index The pool-index to store the pool cell location to.
         * @return A pointer to the pool-cell data element
         */
        template<typename T> T* allocate( uint32_t type, PoolIndex* pool_index );

        /**
         * Release the cell at the specified pool-index back to the pool.
         *
         * @param type The index where the pool-mngr is stored.
         * @param pool_index The index location of the coll to release.
         */
        void free(  uint32_t type, const PoolIndex& pool_index );

        uint32_t exponent()const;
        uint32_t base()const;

        void purge();
        ~PoolSysm();

    private:

        /**
        `_poollist`: a vector of pointers to PoolMngr objects, each of which manages a pool of objects of a specific type
        */
        std::vector< PoolMngr* > _poollist;

        uint32_t _exponent;
        uint32_t _base;
    };

}





namespace kege
{

    template<typename T> T* PoolSysm::allocate( uint32_t type, PoolIndex* access )
    {
        if ( type >= _poollist.size() )
        {
            throw std::runtime_error( "[ error ][ INVALID_OPERATION ]: PoolSysm::allocate(type, access): out of bound index -> type." );
        }
        if ( _poollist[ type ] == nullptr )
        {
            _poollist[ type ] = new PoolMngrT< T >( _base, _exponent );
        }
        return ((PoolMngrT< T >*) _poollist[ type ])->allocate( access );
    }

    template<typename T> T* PoolSysm::get( uint32_t type, const PoolIndex& access )
    {
        if ( type >= _poollist.size() ) return nullptr;
        if ( !_poollist[ type ] ) return nullptr;
        return ((PoolMngrT< T >*) _poollist[ type ])->get( access );
    }

    template<typename T> const T* PoolSysm::get( uint32_t type, const PoolIndex& access )const
    {
        if ( type >= _poollist.size() ) return nullptr;
        return ((PoolMngrT< T >*) _poollist[ type ])->get( access );
    }



    template< typename Data > Data* PoolMngrT< Data >::allocate( PoolIndex* access )
    {
        if ( _head )
        {
            if ( !_head->full() )
            {
                access->_pool_id = _head->_id;
                Data* data = _head->allocate( access );
                return data;
            }
            _head->_prev_pool = nullptr;
            _head = _head->_next_pool;
        }

        uint32_t size = pow( _base, _exponent );
        size = ( size <= 0 ) ? _base : size;

        _exponent = ( _exponent < _max_exponent ) ? _exponent : _max_exponent;
        _exponent++;

        Pool< Data >* pool = new Pool< Data >
        (
            new PoolCell< Data >[ size ],
            size, true
        );

        if ( _head == nullptr )
        {
            _head = pool;
        }
        else
        {
            _head->_prev_pool = pool;
            pool->_next_pool = _head;
            _head = pool;
        }

        _pools.push_back( pool );

        pool->_id = uint32_t( _pools.size() - 1 );
        access->_pool_id = pool->_id;

        return _pools[ access->_pool_id ]->allocate( access );
    }

}


#endif /* pool_sysm_hpp */
