//
//  pool-index.hpp
//  new-engine
//
//  Created by Kenneth Esdaile on 1/18/24.
//

#ifndef pool_index_hpp
#define pool_index_hpp

#include <stdio.h>
#include <stdlib.h>
#include <ostream>

namespace kege
{

    typedef class PoolIndex
    {
    public:

        typedef  int64_t Index;
        typedef uint32_t CelID;

        PoolIndex& operator =(const PoolIndex& other);
        PoolIndex& operator =(Index value);
        operator Index() const;
        operator bool()const;
        PoolIndex(Index value);
        PoolIndex();

        union
        {
            struct
            {
                CelID _cell_id;
                CelID _pool_id;
            };
            Index _index;
        };

    } PoolIndex;

}




namespace kege
{

    inline PoolIndex& PoolIndex::operator =(const PoolIndex& other)
    {
        _index = other._index;
        return *this;
    }

    inline PoolIndex& PoolIndex::operator =(Index value)
    {
        _index = value;
        return *this;
    }

    inline PoolIndex::operator Index() const
    {
        return _index;
    }

    inline PoolIndex::operator bool()const
    {
        return _index >= 0;
    }

    inline PoolIndex::PoolIndex(Index value)
    :   _index( value )
    {
        operator =( value );
    }

    inline PoolIndex::PoolIndex()
    :   _index( -1 )
    {}


    inline std::ostream& operator <<(std::ostream& s, const PoolIndex& pi )
    {
        return ( s <<"pool: " << pi._pool_id <<", cell: " << pi._cell_id );
    }

    inline bool operator ==(const PoolIndex& a, const PoolIndex& b)
    {
        return a._index == b._index;
    }

    inline bool operator !=(const PoolIndex& a, const PoolIndex& b)
    {
        return a._index != b._index;
    }

    inline bool operator <(const PoolIndex& a, const PoolIndex& b)
    {
        return a._index < b._index;
    }

    inline bool operator >(const PoolIndex& a, const PoolIndex& b)
    {
        return a._index > b._index;
    }

    inline bool operator <=(const PoolIndex& a, const PoolIndex& b)
    {
        return a._index <= b._index;
    }

    inline bool operator >=(const PoolIndex& a, const PoolIndex& b)
    {
        return a._index >= b._index;
    }

}
#endif /* pool_index_hpp */
