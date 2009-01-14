///////////////////////////////////////////////////////////////////////////////
/// \file proto_typeof.hpp
/// Type registrations so that proto1 expression templates can be used together
/// with the Boost.Typeof library.
//
//  Copyright 2007 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_XPRESSIVE_PROTO_PROTO_TYPEOF_H
#define BOOST_XPRESSIVE_PROTO_PROTO_TYPEOF_H

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/xpressive/proto/v1_/proto_fwd.hpp>

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::unary_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::binary_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::nary_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::noop_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::unary_plus_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::unary_minus_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::unary_star_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::complement_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::address_of_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::logical_not_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::pre_inc_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::pre_dec_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::post_inc_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::post_dec_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::left_shift_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::right_shift_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::multiply_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::divide_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::modulus_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::add_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::subtract_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::less_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::greater_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::less_equal_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::greater_equal_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::equal_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::not_equal_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::logical_or_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::logical_and_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::bitand_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::bitor_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::bitxor_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::comma_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::mem_ptr_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::assign_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::left_shift_assign_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::right_shift_assign_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::multiply_assign_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::divide_assign_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::modulus_assign_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::add_assign_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::subtract_assign_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::bitand_assign_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::bitor_assign_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::bitxor_assign_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::subscript_tag)
BOOST_TYPEOF_REGISTER_TYPE(boost::proto1::function_tag)

BOOST_TYPEOF_REGISTER_TEMPLATE(boost::proto1::unary_op, (typename)(typename))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::proto1::binary_op, (typename)(typename)(typename))

#endif
