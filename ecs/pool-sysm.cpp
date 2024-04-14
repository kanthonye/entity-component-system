//
//  pool-sysm.cpp
//  new-engine
//
//  Created by Kenneth Esdaile on 1/18/24.
//

#include "pool-sysm.hpp"
namespace kege
{

    PoolSysm::PoolSysm(uint32_t max_distinct_pool_types, uint32_t growth_base, uint32_t growth_exponent)
    :   _exponent( growth_exponent )
    ,   _base( growth_base )
    {
        _poollist.resize( max_distinct_pool_types );
        memset( _poollist.data(), 0x0, max_distinct_pool_types * 8 );
    }

    void PoolSysm::free( uint32_t type, const PoolIndex& access )
    {
        _poollist[ type ]->free( access );
    }

    uint32_t PoolSysm::exponent()const
    {
        return _exponent;
    }

    uint32_t PoolSysm::base()const
    {
        return _base;
    }

    void PoolSysm::purge()
    {
        for (int i=0; i<_poollist.size(); i++)
        {
            if ( _poollist[ i ] != nullptr )
            {
                delete _poollist[ i ];
                _poollist[ i ] = nullptr;
            }
        }
        _poollist.clear();
    }

    PoolSysm::~PoolSysm()
    {
        purge();
    }

}
