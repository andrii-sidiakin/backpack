#pragma once

#include "typelist.h"

namespace bp {

///
/// A list of types
///
template <typename... Es> struct record {
    /// number of elements in the record
    static constexpr auto size = sizeof...(Es);
    /// defines whether record is empty or not
    static constexpr auto is_empty = (size == 0);

    template <typename E> struct contains {
        static constexpr auto value =
            bpx::element_exists<bpx::typelist<Es...>, E>::value;
    };
};

///
///
///
template <typename> struct is_record : std::false_type {};
///
template <typename... Ts>
struct is_record<bp::record<Ts...>> : std::true_type {};
///
template <typename R>
concept Record = bp::is_record<R>::value;

///
/// Converts given Type to bp::typelist
///
template <typename> struct to_typelist;
template <typename... Ts> struct to_typelist<bpx::typelist<Ts...>> {
    using type = bpx::typelist<Ts...>;
};
template <typename... Ts> struct to_typelist<bp::record<Ts...>> {
    using type = bpx::typelist<Ts...>;
};
template <typename T> using to_typelist_t = typename to_typelist<T>::type;

///
/// Converts given Type to bp::record
///
template <typename> struct to_record;
template <typename... Ts> struct to_record<bpx::typelist<Ts...>> {
    using type = bp::record<Ts...>;
};
template <typename... Ts> struct to_record<bp::record<Ts...>> {
    using type = bp::record<Ts...>;
};
template <typename T> using to_record_t = typename to_record<T>::type;

///
///
///
template <Record R, template <typename> class Pred, bool Exp = true>
struct select_if {
    using type = bp::to_record_t<
        typename bpx::select_if<bp::to_typelist_t<R>, Pred, Exp>::type>;
};
///
template <Record R, template <typename> class Pred, bool Exp = true>
using select_if_t = typename select_if<R, Pred, Exp>::type;

///
///
///
template <Record R, template <typename> class Pred, bool Exp = true>
struct remove_if {
    using type = bp::to_record_t<
        typename bpx::select_if<bp::to_typelist_t<R>, Pred, !Exp>::type>;
};
///
template <Record R, template <typename> class Pred, bool Exp = true>
using remove_if_t = typename remove_if<R, Pred, Exp>::type;

///
///
///
template <Record R, Record Q> struct subtract {
    using type = remove_if_t<R, Q::template contains, true>;
};
///
template <Record R, Record Q> using subtract_t = typename subtract<R, Q>::type;

///
///
///
template <Record R, Record Q> struct intersect {
    using type = select_if_t<R, Q::template contains, true>;
};
///
template <Record R, Record Q>
using intersect_t = typename intersect<R, Q>::type;

} // namespace bp