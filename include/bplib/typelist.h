#pragma once

#include <utility>

namespace bpx {

///
///
///
template <typename... Types> struct typelist {};

///
/// Concatenates two typelists
///
template <typename AA, typename BB> struct concat;
///
template <typename... As, typename... Bs>
struct concat<typelist<As...>, typelist<Bs...>> {
    using type = typelist<As..., Bs...>;
};

///
///
///
template <typename AA, typename T> struct element_index;
/// stop rule:
template <typename... As, typename T>
struct element_index<typelist<T, As...>, T> {
    static constexpr auto value = 0u;
};
///
template <typename Ai, typename... As, typename T>
struct element_index<typelist<Ai, As...>, T> {
    static constexpr auto value = 1u + element_index<typelist<As...>, T>::value;
};

///
/// Checks whether element is present in a given typelist.
///
/// TODO: consider implementation based on element_index<>
///
template <typename AA, typename T> struct element_exists : std::false_type {};
/// stop rule:
template <typename... As, typename T>
struct element_exists<typelist<T, As...>, T> : std::true_type {};
///
template <typename Ai, typename... As, typename T>
struct element_exists<typelist<Ai, As...>, T>
    : element_exists<typelist<As...>, T> {};

///
/// Returns a type of i-th element
///
template <typename AA, std::size_t Ii> struct element_at;
///
template <typename A0, typename... As>
struct element_at<typelist<A0, As...>, 0> {
    using type = A0;
};
///
template <typename A0, typename... As, std::size_t Ii>
struct element_at<typelist<A0, As...>, Ii>
    : element_at<typelist<As...>, Ii - 1> {};

///
/// Returns typelist of elements at a given indices
///
template <typename AA, typename II> struct select_at;
///
template <typename AA, std::size_t... Is>
struct select_at<AA, std::index_sequence<Is...>> {
    using type = typelist<typename element_at<AA, Is>::type...>;
};

///
/// Returns an index sequence of elements in a given typelist [AA] for which
/// condition (Pred(Ai) == Exp) is True
///
template <typename AA, template <typename> class Pred, bool Exp>
struct index_if {
    /// iteraties over [XX] incrementing Ii by 1, and pushes Ii in [II] if
    /// Pred(Xi) equals Exp
    template <typename II, std::size_t Ii, typename XX> struct impl;
    /// stop rule: [XX] is empty
    template <std::size_t... Is, std::size_t Ii>
    struct impl<std::index_sequence<Is...>, Ii, typelist<>> {
        using type = std::index_sequence<Is...>;
    };
    /// recursion step: chooses whether to go with [Is..., Ii] or [Is...],
    template <std::size_t... Is, std::size_t Ii, typename Xi, typename... Xs>
    struct impl<std::index_sequence<Is...>, Ii, typelist<Xi, Xs...>> {
        using type =
            typename impl<std::conditional_t<Pred<Xi>::value == Exp,
                                             std::index_sequence<Is..., Ii>,
                                             std::index_sequence<Is...>>,
                          Ii + 1, typelist<Xs...>>::type;
    };

    using type = typename impl<std::index_sequence<>, 0, AA>::type;
};

///
///
///
template <typename AA, typename BB> struct make_index {
    ///
    template <typename XX> struct impl;
    ///
    template <typename... Xs> struct impl<typelist<Xs...>> {
        using type = std::index_sequence<element_index<BB, Xs>::value...>;
    };

    using type = typename impl<AA>::type;
};

///
/// Returns a typelist of elements from [AA] for which
/// condition (Pred(Ai) == Exp) is True
///
template <typename AA, template <typename> class Pred, bool Exp>
struct select_if {
    using index_type = typename index_if<AA, Pred, Exp>::type;
    using type = typename select_at<AA, index_type>::type;
};

} // namespace bpx
