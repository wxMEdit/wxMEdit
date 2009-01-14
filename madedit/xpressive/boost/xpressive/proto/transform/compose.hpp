///////////////////////////////////////////////////////////////////////////////
/// \file compose.hpp
/// A special-purpose proto transform for composing two transfomations. Given
/// two Grammars, expressions that match the first grammar are transformed
/// according to that grammar, and the result is forwarded to the second
/// for further transformation.
//
//  Copyright 2007 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_COMPOSE_HPP_EAN_04_01_2007
#define BOOST_PROTO_TRANSFORM_COMPOSE_HPP_EAN_04_01_2007

#include <boost/xpressive/proto/detail/prefix.hpp>
#include <boost/xpressive/proto/proto_fwd.hpp>
#include <boost/xpressive/proto/detail/suffix.hpp>

namespace boost { namespace proto { namespace transform
{

    // Composes two transforms
    template<typename Grammar1, typename Grammar2>
    struct compose
      : Grammar1
    {
        compose() {}

        template<typename Expr, typename State, typename Visitor>
        struct apply
        {
            typedef typename Grammar2::template apply<
                typename Grammar1::template apply<Expr, State, Visitor>::type
              , State
              , Visitor
            >::type type;
        };

        template<typename Expr, typename State, typename Visitor>
        static typename apply<Expr, State, Visitor>::type
        call(Expr const &expr, State const &state, Visitor &visitor)
        {
            return Grammar2::call(Grammar1::call(expr, state, visitor), state, visitor);
        }
    };

}}}

namespace boost { namespace proto
{
    template<typename Grammar1, typename Grammar2>
    struct is_transform<transform::compose<Grammar1, Grammar2> >
      : mpl::true_
    {};
}}

#endif
