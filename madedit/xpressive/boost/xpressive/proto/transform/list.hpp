///////////////////////////////////////////////////////////////////////////////
/// \file list.hpp
/// A special-purpose proto transform for putting things into a
/// fusion::cons<> list.
//
//  Copyright 2007 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_LIST_HPP_EAN_12_16_2006
#define BOOST_PROTO_TRANSFORM_LIST_HPP_EAN_12_16_2006

#include <boost/xpressive/proto/detail/prefix.hpp>
#include <boost/version.hpp>
#if BOOST_VERSION < 103500
# include <boost/spirit/fusion/sequence/cons.hpp>
#else
# include <boost/fusion/include/cons.hpp>
#endif
#include <boost/xpressive/proto/proto_fwd.hpp>
#include <boost/xpressive/proto/detail/suffix.hpp>

namespace boost { namespace proto { namespace transform
{

    // A list transform, that puts elements into a fusion cons-list
    template<typename Grammar>
    struct list
      : Grammar
    {
        list() {}

        template<typename Expr, typename State, typename Visitor>
        struct apply
        {
            typedef fusion::cons<
                typename Grammar::template apply<Expr, State, Visitor>::type
              , State
            > type;
        };

        template<typename Expr, typename State, typename Visitor>
        static typename apply<Expr, State, Visitor>::type
        call(Expr const &expr, State const &state, Visitor &visitor)
        {
            return typename apply<Expr, State, Visitor>::type
                (Grammar::call(expr, state, visitor), state);
        }
    };

    // A tail transform, that returns the tail of a fusion cons-list
    template<typename Grammar>
    struct tail
      : Grammar
    {
        tail() {}

        template<typename Expr, typename State, typename Visitor>
        struct apply
        {
            typedef typename Grammar::template apply<Expr, State, Visitor>::type::cdr_type type;
        };

        template<typename Expr, typename State, typename Visitor>
        static typename apply<Expr, State, Visitor>::type
        call(Expr const &expr, State const &state, Visitor &visitor)
        {
            return Grammar::call(expr, state, visitor).cdr;
        }
    };

}}}

namespace boost { namespace proto
{
    template<typename Grammar>
    struct is_transform<transform::list<Grammar> >
      : mpl::true_
    {};

    template<typename Grammar>
    struct is_transform<transform::tail<Grammar> >
      : mpl::true_
    {};
}}

#endif
