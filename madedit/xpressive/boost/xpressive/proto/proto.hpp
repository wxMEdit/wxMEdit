///////////////////////////////////////////////////////////////////////////////
/// \file proto.hpp
/// The proto expression template compiler and supporting utilities.
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_HPP_EAN_04_01_2005
#define BOOST_PROTO_HPP_EAN_04_01_2005

#include <boost/xpressive/proto/proto_fwd.hpp>
#include <boost/xpressive/proto/op_tags.hpp>
#include <boost/xpressive/proto/op_base.hpp>
#include <boost/xpressive/proto/operators.hpp>
#include <boost/xpressive/proto/arg_traits.hpp>

namespace boost { namespace proto
{
    ///////////////////////////////////////////////////////////////////////////////
    // compile
    template<typename Op, typename State, typename Visitor, typename DomainTag>
    typename lazy_enable_if<
        is_op<Op>
      , typename compiler<typename tag_type<Op>::type, DomainTag>::BOOST_NESTED_TEMPLATE apply
        <
            Op
          , State
          , Visitor
        >
    >::type const
    compile(Op const &op, State const &state, Visitor &visitor, DomainTag)
    {
        typedef compiler<typename tag_type<Op>::type, DomainTag> compiler;
        return compiler::call(as_op<Op>::make(op), state, visitor);
    }

}} // namespace boost::proto

#endif
