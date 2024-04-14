//
//  pool-cell.hpp
//  new-engine
//
//  Created by Kenneth Esdaile on 1/18/24.
//

#ifndef pool_cell_hpp
#define pool_cell_hpp

#include <stdio.h>
#include "pool-index.hpp"

namespace kege
{
    /**
    `PoolCell` which is a container for a single piece of data of type Data. It is used as a building block for a linked list or other data structure.
    */
    template< typename Data > class PoolCell
    {
    public:

        PoolCell& operator=(const Data& data);
        const Data& operator*()const;
        Data& operator*();
        PoolCell();

        Data _data;
        PoolCell* _next;
    };

    template< typename Data > PoolCell< Data >& PoolCell< Data >::operator=(const Data& data)
    {
        _data = data;
        return *this;
    }

    template< typename Data > const Data& PoolCell< Data >::operator*()const
    {
        return _data;
    }

    template< typename Data > Data& PoolCell< Data >::operator*()
    {
        return _data;
    }

    template< typename Data > PoolCell< Data >::PoolCell()
    : _next( nullptr ) {}

}
#endif /* pool_cell_hpp */
