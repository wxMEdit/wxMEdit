///////////////////////////////////////////////////////////////////////////////
/// \file apply.hpp
/// Proto transforms for applying MPL placeholder expressions.
//
//  Copyright 2007 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_APPLY_HPP_EAN_06_23_2007
#define BOOST_PROTO_TRANSFORM_APPLY_HPP_EAN_06_23_2007

#include <boost/xpressive/proto/detail/prefix.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/xpressive/proto/detail/suffix.hpp>

namespace boost { namespace proto { namespace transform
{
    namespace detail
    {
        struct any
        {
            template<typename T>
            any(T const &)
            {}
        };

        struct default_factory
        {
            default_factory()
            {}

            default_factory const &operator()() const
            {
                return *this;
            }

            default_factory const &operator()(any) const
            {
                return *this;
            }

            default_factory const &operator()(any, any) const
            {
                return *this;
            }

            default_factory const &operator()(any, any, any) const
            {
                return *this;
            }

            template<typename T>
            operator T() const
            {
                return T();
            }
        };
    }
    
    // Always return the specified type/object
    template<typename Grammar, typename Always, typename Factory>
    struct always
      : Grammar
    {
        always() {}

        template<typename, typename, typename>
        struct apply
        {
            typedef Always type;
        };

        template<typename Expr, typename State, typename Visitor>
        static Always
        call(Expr const &, State const &, Visitor &)
        {
            return Factory()();
        }
    };

    // Apply an MPL lambda, passing just Expr
    template<typename Grammar, typename Lambda, typename Factory>
    struct apply1
      : Grammar
    {
        apply1() {}

        template<typename Expr, typename State, typename Visitor>
        struct apply
          : mpl::apply1<Lambda, typename Grammar::template apply<Expr, State, Visitor>::type>
        {};

        template<typename Expr, typename State, typename Visitor>
        static typename apply<Expr, State, Visitor>::type
        call(Expr const &expr, State const &state, Visitor &visitor)
        {
            return Factory()(Grammar::call(expr, state, visitor));
        }
    };

    // Apply an MPL lambda, passing Expr and State
    template<typename Grammar, typename Lambda, typename Factory>
    struct apply2
      : Grammar
    {
        apply2() {}

        template<typename Expr, typename State, typename Visitor>
        struct apply
          : mpl::apply2<Lambda, typename Grammar::template apply<Expr, State, Visitor>::type, State>
        {};

        template<typename Expr, typename State, typename Visitor>
        static typename apply<Expr, State, Visitor>::type
        call(Expr const &expr, State const &state, Visitor &visitor)
        {
            return Factory()(Grammar::call(expr, state, visitor), state);
        }
    };

    // Apply an MPL lambda, passing Expr, State and Visitor
    template<typename Grammar, typename Lambda, typename Factory>
    struct apply3
      : Grammar
    {
        apply3() {}

        template<typename Expr, typename State, typename Visitor>
        struct apply
          : mpl::apply3<Lambda, typename Grammar::template apply<Expr, State, Visitor>::type, State, Visitor>
        {};

        template<typename Expr, typename State, typename Visitor>
        static typename apply<Expr, State, Visitor>::type
        call(Expr const &expr, State const &state, Visitor &visitor)
        {
            return Factory()(Grammar::call(expr, state, visitor), state, visitor);
        }
    };

}}}

namespace boost { namespace proto
{
    template<typename Grammar, typename Always, typename Factory>
    struct is_transform<transform::always<Grammar, Always, Factory> >
      : mpl::true_
    {};

    template<typename Grammar, typename Lambda, typename Factory>
    struct is_transform<transform::apply1<Grammar, Lambda, Factory> >
      : mpl::true_
    {};

    template<typename Grammar, typename Lambda, typename Factory>
    struct is_transform<transform::apply2<Grammar, Lambda, Factory> >
      : mpl::true_
    {};

    template<typename Grammar, typename Lambda, typename Factory>
    struct is_transform<transform::apply3<Grammar, Lambda, Factory> >
      : mpl::true_
    {};
}}

#endif
