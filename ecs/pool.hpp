//
//  pool.hpp
//  new-engine
//
//  Created by Kenneth Esdaile on 1/18/24.
//

#ifndef pool_hpp
#define pool_hpp

#include "pool-cell.hpp"

namespace kege
{

    /**
    class `Pool` manages a pool of PoolCell objects, where each PoolCell contains a piece of data of type Data.
    The Pool will hold an array of PooCells of a given size. This will be it's size throughout the program execution.
    */
    template< typename Data > class Pool
    {
    public:

        /**
        `Pool(PoolCell< Data >* blocks, uint32_t size, bool block_is_allocated)`: a constructor that takes a pointer to
        an array of PoolCell objects blocks, the size of the array size, and a boolean value block_is_allocated that indicates whether the memory for the
        array is already allocated and should be deallocated by the deconstructor. The constructor initializes the member variables of the Pool
        object .
        */
        Pool(PoolCell< Data >* blocks, uint32_t size, bool block_is_allocated);

        /**
        `Data* allocate(PoolIndex* comp)`: a method that takes a pointer to a uint32_t variable comp and returns a pointer to a piece of
        data of type Data from the pool. The comp parameter is used to update the index of the allocated PoolCell.
        */
        Data* allocate( PoolIndex* comp );

        /**
        `Data* get(const PoolIndex& comp)`: a method that takes a const uint32_t& reference comp and returns a pointer to the piece
        of data of type Data at the specified index in the pool.
        */
        Data* get( const PoolIndex& comp );
        const Data* get( const PoolIndex& comp )const;

        /**
        `void free(const PoolIndex& comp)`: a method that takes a const uint32_t& reference comp and frees the PoolCell at the specified
        index in the pool.
        */
        void free( const PoolIndex& comp );

        /**
        `void clear()`:  a method that release the pool memory
        */
        void purge();

        /**
        `bool full()`:  a method that returns a boolean value indicating whether the pool is full.
        */
        bool full();

        /**
        `~Pool()`: a destructor that deallocates the memory for the PoolCell objects in the pool.
        */
        ~Pool();

        PoolCell< Data >* _cells;
        PoolCell< Data >* _free_head;

        Pool< Data >* _next_pool;
        Pool< Data >* _prev_pool;

        uint32_t _count;
        uint32_t _size;
        uint32_t _id;
        bool _allocated;
    };


    template< typename Data > Pool< Data >::Pool(PoolCell< Data >* blocks, uint32_t size, bool block_is_allocated)
    :    _cells( blocks )
    ,    _free_head( nullptr )
    ,    _prev_pool( nullptr )
    ,   _next_pool( nullptr )
    ,   _id( 0 )
    ,   _count( 0 )
    ,   _size( size )
    ,    _allocated( block_is_allocated )
    {}

    template< typename Data > Data* Pool< Data >::allocate( PoolIndex* index )
    {
        if( full() )
        {
            throw std::runtime_error("Pool is full. not space to allocate.");
        }

        if ( _free_head != nullptr )
        {
            int32_t offset = int32_t(_free_head - (&_cells[ 0 ]));
            index->_cell_id = offset;
            Data& data = _free_head->_data;
            _free_head = _free_head->_next;
            return &data;
        }
        index->_cell_id = _count++;
        return &_cells[ index->_cell_id ]._data;
    }

    template< typename Data > Data* Pool< Data >::get( const PoolIndex& index )
    {
        if( index._cell_id >= _count )
        {
            throw std::runtime_error("index out of bound -> Pool::get()");
        }

        return &_cells[ index._cell_id ]._data;
    }

    template< typename Data > void Pool< Data >::free( const PoolIndex& index )
    {
        if( index._pool_id >= _count )
        {
            return;
        }

        PoolCell< Data >* block = &_cells[ index._cell_id ];
        block->_next = _free_head;
        _free_head = block;
    }

    template< typename Data > void Pool< Data >::purge()
    {
        if( _allocated )
        {
            delete [] _cells;
            _allocated = false;
        }
        _cells = nullptr;
    }

    template< typename Data > bool Pool< Data >::full()
    {
        return _count >= _size && _free_head == nullptr;
    }
    
    template< typename Data > Pool< Data >::~Pool()
    {
        purge();
    }
}
#endif /* pool_hpp */
