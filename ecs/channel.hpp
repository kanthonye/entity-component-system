//
//  Channel.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 7/21/23.
//

#ifndef Channel_hpp
#define Channel_hpp
#include <vector>
#include <algorithm>
#include <functional>
#include <mutex>
#include <iostream>
namespace kege
{
    class Channel
    {
    private:

        /**
         *  The C signifies the Channel and the M signifies the message. Messages are sent through a channel which multiple listeners maybe listening to.
         *  The reason I why both C and M are necessary is because my message system has an inheritance hierarchy and messages will be sent as the
         *  base-message class. I need to use the base-message class-name to pass messages of all kinds, and use C as a key to select the message
         *  channel to broadcast the message on
         */
        template < typename M, typename C > class InternalChannel
        {
        public:

            static InternalChannel& instance()
            {
                static InternalChannel result;
                return result; // return a reference to an internal static
            }

            template<class T> void add( void(T::*func)( M ), T* instance )
            {
                std::lock_guard< std::mutex > lock( _add_listener_mutex );
                _callbacks.push_back([func, instance](const M& message) {
                    (instance->*func)(message);
                });
                _listeners.push_back( instance );
            }

            void add( void(*func)( M ) )
            {
                std::lock_guard< std::mutex > lock( _add_listener_mutex );
                _callbacks.push_back( [func](M message){ func(message); } );
            }

            void broadcast( M m )
            {
                std::vector< Callback > local_queue( _callbacks.size() );
                {
                    std::lock_guard< std::mutex > lock( _msg_mutex );
                    local_queue = _callbacks;
                }

                for ( auto& callback : local_queue )
                    callback( m );
            }

            void remove( const void* ptr )
            {
                std::lock_guard< std::mutex > lock( _msg_mutex );

                auto it = std::find( _listeners.begin(), _listeners.end(), ptr );
                if ( it == _listeners.end() )
                {
                    return;
                }

                auto idx = it - _listeners.begin();

                _callbacks.erase( _callbacks.begin() + idx );
                _listeners.erase( it );
            }

            void remove( void(*func)( M ) )
            {
                std::lock_guard< std::mutex > lock( _msg_mutex );

                auto it = std::find( _callbacks.begin(), _callbacks.end(), func );
                if ( it != _listeners.end() )
                {
                    _callbacks.erase( it );
                }
            }

            void purge()
            {
                _callbacks.clear();
                _listeners.clear();
            }

        private:

            using Callback = std::function< void( M ) >;
            std::vector< Callback > _callbacks;
            std::vector< const void* > _listeners;
            std::mutex _add_listener_mutex;
            std::mutex _msg_mutex;
        };

    public:

        template < typename M, typename C, typename T > static void add( void(T::*func)( M ), T* instance )
        {
            InternalChannel< M,C >::instance().template add< T >( func, instance );
        }

        template < typename M, typename C > static void add( void(*func)( M ) )
        {
            InternalChannel< M,C >::instance().add( func );
        }

        template < typename M, typename C > static void remove( void(*func)( M ) )
        {
            InternalChannel< M,C >::instance().remove( func );
        }

        template < typename M, typename C, typename T > static void remove( T* t )
        {
            InternalChannel< M,C >::instance().remove( t );
        }

        template < typename M, typename C > static void broadcast( C m )
        {
            InternalChannel< M,C >::instance().broadcast( m );
        }
    };
}
#endif /* Channel_hpp */
