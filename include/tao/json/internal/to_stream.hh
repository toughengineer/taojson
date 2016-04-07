// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_TO_STREAM_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_TO_STREAM_HH

#include <ostream>

#include "../external/double.hh"
#include "../value.hh"

#include "escape.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         inline void to_stream( std::ostream & o, const std::string & s )
         {
            o << escape( s );  // TODO: Check whether direct-to-stream is faster.
         }

         template< typename Value >
         void to_stream( std::ostream & o, const Value & v );

         template< typename Value >
         void to_stream( std::ostream & o, const Value & v, const unsigned indent, const unsigned current = 0 );

         template< typename Value >
         void to_stream( std::ostream & o, const std::vector< Value > & v )
         {
            o << '[';
            if ( ! v.empty() ) {
               to_stream( o, v[ 0 ] );
               for ( size_t i = 1; i < v.size(); ++i ) {
                  o << ',';
                  to_stream( o, v[ i ] );
               }
            }
            o << ']';
         }

         template< typename Value >
         void to_stream( std::ostream & o, const std::vector< Value > & v, const unsigned indent, unsigned current = 0 )
         {
            o << '[';
            current += indent;
            const std::string padding = std::string( current, ' ' );
            if ( ! v.empty() ) {
               o << '\n';
               o << padding;
               to_stream( o, v[ 0 ], indent, current );
               for ( size_t i = 1; i < v.size(); ++i ) {
                  o << ",\n";
                  o << padding;
                  to_stream( o, v[ i ], indent, current );
               }
               o << '\n';
               o.write( padding.data(), current - indent );
            }
            o << ']';
         }

         template< typename Value >
         void to_stream( std::ostream & o, const std::map< std::string, Value > & v )
         {
            o << '{';
            if ( ! v.empty() ) {
               to_stream( o, v.begin()->first );
               o << ':';
               to_stream( o, v.begin()->second );
               for ( auto i = ++v.begin(); i != v.end(); ++i ) {
                  o << ',';
                  to_stream( o, i->first );
                  o << ':';
                  to_stream( o, i->second );
               }
            }
            o << '}';
         }

         template< typename Value >
         void to_stream( std::ostream & o, const std::map< std::string, Value > & v, const unsigned indent, unsigned current = 0 )
         {
            o << '{';
            current += indent;
            const std::string padding = std::string( current, ' ' );
            if ( ! v.empty() ) {
               o << '\n';
               o << padding;
               to_stream( o, v.begin()->first );
               o << ": ";
               to_stream( o, v.begin()->second, indent, current );
               for ( auto i = ++v.begin(); i != v.end(); ++i ) {
                  o << ",\n";
                  o << padding;
                  to_stream( o, i->first );
                  o << ": ";
                  to_stream( o, i->second, indent, current );
               }
               o << '\n';
               o.write( padding.data(), current - indent );
            }
            o << '}';
         }

         template< typename Value >
         void to_stream( std::ostream & o, const Value & v )
         {
            switch ( v.type() ) {
               case type::NULL_:
                  o << "null";
                  return;
               case type::BOOL_:
                  o << ( v.unsafe_bool() ? "true" : "false" );
                  return;
               case type::INT64:
                  o << v.unsafe_int64();
                  return;
               case type::DOUBLE:
                  // Assumes std::isfinite( v.unsafe_double() ).
                  json_double_conversion::Dtostr( o, v.unsafe_double() );
                  return;
               case type::STRING:
                  to_stream( o, v.unsafe_string() );
                  return;
               case type::ARRAY:
                  to_stream( o, v.unsafe_array() );
                  return;
               case type::OBJECT:
                  to_stream( o, v.unsafe_object() );
                  return;
               case type::POINTER:
                  to_stream( o, *v.unsafe_pointer() );
                  return;
            }
            assert( false );  // LCOV_EXCL_LINE
         }

         template< typename Value >
         void to_stream( std::ostream & o, const Value & v, const unsigned indent, const unsigned current )
         {
            switch ( v.type() ) {
               case type::NULL_:
                  o << "null";
                  return;
               case type::BOOL_:
                  o << ( v.unsafe_bool() ? "true" : "false" );
                  return;
               case type::INT64:
                  o << v.unsafe_int64();
                  return;
               case type::DOUBLE:
                  // Assumes std::isfinite( v.unsafe_double() ).
                  json_double_conversion::Dtostr( o, v.unsafe_double() );
                  return;
               case type::STRING:
                  to_stream( o, v.unsafe_string() );
                  return;
               case type::ARRAY:
                  to_stream( o, v.unsafe_array(), indent, current );
                  return;
               case type::OBJECT:
                  to_stream( o, v.unsafe_object(), indent, current );
                  return;
               case type::POINTER:
                  to_stream( o, *v.unsafe_pointer(), indent, current );
                  return;
            }
            assert( false );  // LCOV_EXCL_LINE
         }

      } // internal

   } // json

} // tao

#endif
