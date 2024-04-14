//
//  Ref.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 1/15/23.
//

#ifndef KEGE_REF
#define KEGE_REF

#include <stdlib.h>

namespace kege
{

    class RefCounter
    {
    public:

        inline int referenceRefCounter() const { return _ref_RefCounter; }
        inline void decrementRefRefCounter()const { _ref_RefCounter--; }
        inline void incrementRefRefCounter()const { _ref_RefCounter++; }
        
    protected:

        RefCounter(): _ref_RefCounter( 0 ) {}
        
    private:

        mutable int _ref_RefCounter;
    };
    
}


namespace kege
{

    template< typename T > class Ref
    {
    public:

        inline friend bool operator ==( const Ref& a, const Ref& b )
        {
            return a._ref == b._ref;
        }
        inline friend bool operator !=( const Ref& a, const Ref& b )
        {
            return a._ref != b._ref;
        }
        
        inline friend bool operator ==( const T* a, const Ref& b )
        {
            return a == b._ref;
        }
        inline friend bool operator !=( const Ref& a, const T* b )
        {
            return a._ref != b;
        }
        
        inline Ref& operator =( const Ref& a )
        {
            return init( a._ref );
        }
        inline Ref& operator =( T* a )
        {
            return init( a );
        }
        
        inline const T* operator ->()const
        {
            return _ref;
        }
        inline T* operator ->()
        {
            return _ref;
        }
        
        inline const T* ref()const
        {
            return _ref;
        }
        inline T* ref()
        {
            return _ref;
        }
        
        inline const T& operator *()const
        {
            return *_ref;
        }
        inline T& operator *()
        {
            return *_ref;
        }

        inline operator bool()const
        {
            return _ref != nullptr;
        }

        virtual void clear()
        {
            if( _ref != nullptr )
            {
                _ref->RefCounter::decrementRefRefCounter();
                if( _ref->RefCounter::referenceRefCounter() <= 0 )
                {
                    delete _ref;
                }
                _ref = nullptr;
            }
        }

        template<typename I>
        Ref( const Ref<I>& r )
        :   _ref( nullptr )
        {
            Ref<I> p = r;
            init( p.ref() );
        }
        Ref( const Ref<T>& r )
        :   _ref( nullptr )
        {
            init( r._ref );
        }
        Ref( T* r )
        :   _ref( nullptr )
        {
            init( r );
        }
        Ref()
        :   _ref( nullptr )
        {}
        
        virtual ~Ref()
        {
            clear();
        }
        
    protected:

        Ref& init( T* a )
        {
            if ( _ref != nullptr )
                clear();

            _ref = a;

            if ( a != nullptr )
                _ref->RefCounter::incrementRefRefCounter();

            return *this;
        }

        T* _ref;
    };

}

#endif /* KEGE_REF */
