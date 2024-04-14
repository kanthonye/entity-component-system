//
//  kege-string.cpp
//  CacheSystem
//
//  Created by Kenneth Esdaile on 3/12/23.
//
#include <ctype.h>
#include "kege-string.hpp"
namespace kege{

    std::ostream& operator <<(std::ostream& os, const kege::string& str)
    {
        return (os << (( !str.empty() ) ? str.c_str() : ""));
    }

    bool ischar(char c, const char* s)
    {
        while ( *s != c && *s != 0 ) s++;
        return *s != 0;
    }
    

    const char* strskp( const char* str, const char* skips )
    {
        while ( *str != 0 )
        {
            if ( strchr( skips, *str ) != nullptr )
            {
                str++;
                continue;
            }
            break;
        }
        return str;
    }
    const char* strcpy( char* buf, int size, const char* brks, const char* str )
    {
        uint32_t i = 0;
        while ( strchr( brks, *str ) == nullptr && i < size - 1 && *str != 0 )
        {
            buf[i++] = *str++;
        }
        buf[i] = 0;
        return str;
    }

    bool match( const char* s1, const char* s2 )
    {
        if( *s1 != 0 && *s2 != 0 )
        {
            while ( *s1 == *s2 && *s1 != 0 && *s2 != 0 )
            {
                s1++;
                s2++;
            }
            return *s1 == 0 || *s2 == 0;
        }
        return *s1 == *s2;
    }

//    string operator +(const string& a, const string& b)
//    {
//        int64_t length = a.length() + b.length();
//        char* str = new char[ length + 1 ];
//        memcpy( &str[0], a.c_str(), a.length() );
//        memcpy( &str[a.length()], b.c_str(), b.length() );
//        str[length] = 0;
//        return string( str, length );
//    }

//    string operator +(const string& a, const char* b)
//    {
//        int64_t b_length = strlen(b);
//        int64_t length = a.length() + b_length;
//        char* str = new char[ length + 1 ];
//        memcpy( &str[0], a.c_str(), a.length() );
//        memcpy( &str[a.length()], b, b_length );
//        str[length] = 0;
//        return string( str, length );
//    }

    string operator +(const char* a, const string& b)
    {
        int64_t a_length = strlen( a );
        int64_t length = b.length() + a_length;
        char* str = new char[ length + 1 ];
        memcpy( &str[ 0 ], a, a_length );
        memcpy( &str[ a_length ], b.c_str(), b.length() );
        str[ length ] = 0;
        return string( str, length );
    }

//    string operator +(const string& a, int8_t b)
//    {
//        int64_t length = a._length + 1;
//        char* str = new char[ length + 1 ];
//        memcpy( &str[0], a._str, a._length );
//        str[a._length] = b;
//        str[length] = 0;
//        return string( str, length );
//    }
    string operator +(int8_t a, const string& b)
    {
        int64_t length = b._length + 1;
        char* str = new char[ length + 1 ];
        str[ 0 ] = a;
        memcpy( &str[ 1 ], b._str, b._length );
        str[ length ] = 0;
        return string( str, length );
    }

    bool operator ==(const string& a, const string& b)
    {
        if ( a.empty() && b.empty() ) return true;
        if (( a.empty() || b.empty() )) return false;
        return strcmp( a._str, b._str ) == 0;
    }

    const char& string::operator [](uint64_t i)const
    {
        return _str[ i ];
    }
    char& string::operator [](uint64_t i)
    {
        return _str[ i ];
    }

    string& string::operator +=( const string& str )
    {
        string s = string::operator +( str );

        clear();
        _allocated = true;
        _length = s._length;
        _str = s._str;

        s._str = nullptr;
        return *this;
    }
    string& string::operator +=( const char* str )
    {
        string s = string::operator +( str );

        clear();
        _allocated = true;
        _length = s._length;
        _str = s._str;

        s._str = nullptr;
        return *this;
    }
    string& string::operator +=( const char chr )
    {
        string s = string::operator +( chr );

        clear();
        _allocated = true;
        _length = s._length;
        _str = s._str;

        s._str = nullptr;
        return *this;
    }

    string string::operator +( const string& str )const
    {
        int64_t length = _length + str._length;
        char* s = new char[ length + 1 ];
        if( !empty() ) memcpy( &s[0], _str, _length );
        memcpy( &s[ _length ], str._str, str._length );
        s[ length ] = 0;

        return string( s, length );
    }
    string string::operator +( const char* str )const
    {
        int64_t str_length = strlen( str );
        int64_t length = _length + str_length;

        char* s = new char[ length + 1 ];
        if( !empty() ) memcpy( &s[0], _str, _length );
        memcpy( &s[ _length ], str, str_length );
        s[ length ] = 0;

        return string( s, length );
    }
    string string::operator +( int chr )const
    {
        int64_t length = _length + 1;

        char* s = new char[ length + 1 ];
        if( !empty() ) memcpy( &s[0], _str, _length );
        s[ _length ] = chr;
        s[ length ] = 0;

        return string( s, length );
    }

    string& string::operator =( const string& str )
    {
        clear();
        if ( str._str != nullptr )
        {
            _length = str._length;
            _str = new char[ _length + 1 ];
            strncpy( _str, str._str, _length );
            _str[ _length ] = 0;
            _allocated = true;
        }
        return *this;
    }
    string& string::operator =( const char* str )
    {
        clear();
        if ( str != nullptr ) 
        {
            _length = strlen( str );
            _str = new char[ _length + 1 ];
            strncpy( _str, str, _length );
            _str[ _length ] = 0;
            _allocated = true;
        }
        return *this;
    }

    const char& string::operator *()const
    {
        return *_str;
    }
    char& string::operator *()
    {
        return *_str;
    }

    string::operator bool()const
    {
        return (_str == nullptr) ? false: *_str != 0;
    }
    
    void string::copyFrom( uint64_t size, const char* str )
    {
        clear();
        char* s = new char[ size + 1];
        memcpy(s, str, size);
        s[size] = 0;
        _allocated = true;
        _length = size;
        _str = s;
    }

    bool string::cmp( const char* str )const
    {
        if (_str==nullptr || str==nullptr) return 0;
        if (*_str=='\0' || *str=='\0') return true;
        const char *r = str;
        const char *l = _str;
        while (*r == *l && *r != '\0' && *l != '\0')
        {
            r++;
            l++;
        }
        if( *r != '\0' && *l != '\0' )
        {
            return true;
        }
        if( *r == '\0' && *l != '\0' )
        {
            l--;
            return (*r == *l);
        }
        return false;
    }
    
    string string::read( const char* flags )const
    {
        char* s = _str;
        while (*s != 0)
        {
            if ( ischar(*s, flags) )
                break;
            s++;
        }
        int64_t size = s - _str;
        char* dst = new char[ size + 1 ];
        memcpy(dst, _str, size);
        dst[ size ] = 0;
        return string( dst, size, true );
    }

    int64_t string::position(const char* flags)const
    {
        const char* f;
        char* s = _str;
        while (*s != 0)
        {
            f = flags;
            while (*f != 0)
            {
                if (*s == *f++)
                {
                    return (s - _str);
                }
            }
            s++;
        }
        return (s - _str);
    }
    string string::seekfwd(const char* flags)const
    {
        const char* f;
        char* s = _str;
        while (*s != 0)
        {
            f = flags;
            while (*f != 0)
            {
                if (*s == *f++)
                {
                    return string( s, _length - (s - _str), false );
                }
            }
            s++;
        }
        return string( s, _length - (s - _str), false );
    }
    string string::seekbwd(const char* flags)const
    {
        const char* f;
        char* s = &_str[ _length - 1 ];
        while ((s - _str) != 0)
        {
            f = flags;
            while (*f != 0)
            {
                if (*s == *f++)
                {
                    return string( s, _length - (s - _str), false );
                }
            }
            s--;
        }
        return string( s, _length - (s - _str), false );
    }

    string string::skipfwd(const char* flags)const
    {
        const char * f;
        char* s = _str;
        while (*s != 0)
        {
            f = flags;
            while(*s != *f && *f != 0) f++;
            if(*f != 0) s++;
            else break;
        }
        return string( s, _length - (s - _str), false );
    }
    string string::skipbwd(const char* flags)const
    {
        const char * f;
        char* s = &_str[ _length - 1 ];
        while ( s - _str >= 0 )
        {
            f = flags;
            while(*s != *f && *f != 0) f++;
            if(*f != 0) s--;
            else break;
        }
        s++;
        
        int64_t l = s - _str;
        s = new char[ l + 1 ];
        strncpy( s, _str, l );
        s[l] = 0;

        return string( s, l );
    }

    string string::parse_fname()const
    {
        char* s = &_str[ _length - 1 ];
        while( !ischar(*s, "/\\") ) s--;
        s++;
        return kege::string( s, _length - (s - _str), false );
    }

    string string::parse_fpath()const
    {
        char* s = &_str[ _length - 1 ];
        while( !ischar(*s, "/\\") ) s--;
        uint64_t size = s - _str;
        s = new char[ size + 1 ];
        memcpy( s, _str, size );
        s[size] = 0;
        return kege::string( s, size, true );
    }

    string string::parse_name()const
    {
        char* s = &_str[ _length - 1 ];
        while( !ischar(*s, "./\\") ) s--;
        uint64_t period_position = s - _str;

        while( !ischar(*s, "/\\") ) s--;
        s++;

        uint64_t slash_position = s - _str;
        uint64_t size = period_position - slash_position;
        s = new char[ size + 1 ];
        memcpy( s, &_str[ slash_position ], size );
        s[ size ]  = 0;

        return kege::string( s, size, false );
    }
    
    string string::lowercase()const
    {
        char* s = new char[ _length + 1 ];
        char* k = _str;
        while ( *_str != 0 )
        {
            *s = tolower(*k++);
        }
        s[ _length ] = 0;
        return string( s, _length );
    }
    string string::uppercase()const
    {
        char* s = new char[ _length + 1 ];
        char* k = _str;
        while ( *_str != 0 )
        {
            *s = toupper(*k++);
        }
        s[ _length ] = 0;
        return string( s, _length );
    }

    void string::setLength( int64_t length )
    {
        clear();
        _length = length;
        _allocated = true;
        _str = new char[ length ];
        _str[ 0 ] = 0;
    }

    int64_t string::length()const
    {
        return _length;
    }
    void string::resize( uint64_t size )
    {
        if( size == 0 ) 
        {
            clear();
        }
        else
        {
            char* s = new char[ size + 1 ];;

            if ( _str != nullptr )
            {
                uint64_t n = (_length >= size ) ? size : _length;
                memcpy(s, _str, n);
                s[ size ] = 0;
                clear();
            }
            _allocated = true;
            _length = size;
            _str = s;
        }
    }
    const char* string::c_str()const
    {
        return _str;
    }
    char* string::str()
    {
        return _str;
    }

    void string::move( string& str )
    {
        _length = str._length;
        _str = str._str;
        _allocated = true;
        str._allocated = false;
    }
    
    std::size_t string::hash(const char* str)
    {
//        std::size_t n = std::hash<std::string>{}( str );
//        return n;
        if (str == NULL) return 0;

        enum
        {
            A = 54059, /* a prime */
            B = 76963, /* another prime */
            C = 86969, /* yet another prime */
            FIRSTH  =37 /* also prime */
        };

        const char * s = str;

        std::size_t h = FIRSTH;
        while (*s)
        {
            h = (h * A) ^ (s[0] * B);
            s++;
        }
        return h;
    };

    std::size_t string::hash()const
    {
        return string::hash( _str );
    };

    double string::to_float()const
    {
        if ( empty() ) return 0.0f;
        string n = *this;
        return atof( _str );
    }

    int64_t string::to_int64()const
    {
        if ( empty() ) return 0;
        string n = *this;
        return atol( _str );
    }

    int32_t string::to_int32()const
    {
        if ( empty() ) return 0;
        string n = *this;
        return atoi( _str );
    }

    bool string::empty()const
    {
        return (_str == nullptr)? true: _length == 0;
    }
    void string::clear()
    {
        if ( _str != nullptr )
        {
            delete [] _str;
            _str = nullptr;
            _length = 0;
        }
    }



    string::string( const string& str )
    :   _allocated( false )
    ,   _length( 0 )
    ,   _str( nullptr )
    {
        if ( str._str != nullptr )
        {
            _allocated = true;
            _length = str._length;
            _str = new char[ _length + 1 ];
            memcpy( _str, str._str, _length );
            _str[ _length ] = 0;
        }
    }

    string::string( string& str )
    :   _allocated( str._allocated )
    ,   _length( str._length )
    ,   _str( str._str )
    {}

    string::string( char* str, uint64_t length, bool is_alloc )
    :   _allocated( is_alloc )
    ,   _length( length )
    ,   _str( str )
    {}

    string::string( char* str, uint64_t length )
    :   _allocated( true )
    ,   _length( length )
    ,   _str( str )
    {
        if ( str == nullptr )
        {
            _str = new char[ length + 1 ];
            memset( _str, 0x0, length );
            _str[ _length ] = 0;
        }
    }
    
    string::string( const char* str )
    :   _allocated( false )
    ,   _length( 0 )
    ,   _str( nullptr )
    {
        if ( str != nullptr )
        {
            _allocated = true;
            _length = strlen( str );
            _str = new char[ _length + 1 ];
            memcpy( _str, str, _length );
            _str[ _length ] = 0;
        }
    }

    string::string()
    :   _allocated( false )
    ,   _length( 0 )
    ,   _str( nullptr )
    {}




    kege::string dtostr(double n, int space)
    {
        char str[32];
        long w = snprintf(str, 32, "%.*f", space,n);
        str[w] = 0;
        return string(str, w);
    }

    kege::string ftostr(float n, int space)
    {
        char str[32] = {0,0,0,0,0,0,0,0};
        long w = snprintf(str, 32, "%.*f", space,n);
        str[w] = 0;
        return string(str, w);
    }

    kege::string itostr(int32_t n)
    {
        char str[32] = {0,0,0,0,0,0,0,0};
        long w = snprintf(str, 32, "%i", n);
        str[w] = 0;
        return string(str, w);
    }

    kege::string itostr(int64_t n)
    {
        char str[32] = {0,0,0,0,0,0,0,0};
        long w = snprintf(str, 32, "%lld", n);
        str[w] = 0;
        return string(str, w);
    }
}
