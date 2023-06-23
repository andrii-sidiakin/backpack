#pragma once

#include "record.h"

#include <tuple>

namespace bp {

///
///
///
template <typename... Ts> class packet {
  public:
    using record_type = bp::record<Ts...>;

  public:
    explicit packet(Ts &&...args) noexcept
        : m_model(std::forward<Ts>(args)...) {
    }

    template <Record R> auto select() {
        using index = bp::make_index_t<R, record_type>;
        return select_tuple(m_model, index{});
    }

    template <template <typename> class Pred> auto select_if() {
        using match = bp::select_if_t<record_type, Pred>;
        using index = bp::make_index_t<match, record_type>;
        return select_tuple(m_model, index{});
    }

    template <std::size_t... Is>
    auto select_index(
        std::index_sequence<Is...> index = std::index_sequence<Is...>{}) {
        return select_tuple(m_model, index);
    }

    template <std::size_t... Is>
    auto select_index_outer(
        std::index_sequence<Is...> index = std::index_sequence<Is...>{}) {
        constexpr auto total = std::make_index_sequence<record_type::size>{};
        constexpr auto invert = invert_index(index, total);
        return select_tuple(m_model, invert);
    }

  private:
    template <typename... Us, std::size_t... Is>
    static constexpr auto select_tuple(std::tuple<Us...> &tuple,
                                       std::index_sequence<Is...>) noexcept {
        static_assert(((Is < sizeof...(Us)) && ...), "index out of range");
        return std::tie(std::get<Is>(tuple)...);
    }

    template <std::size_t... Xs, std::size_t... Ys>
    static consteval auto invert_index(std::index_sequence<Xs...>,
                                       std::index_sequence<Ys...>) {
        using x_type = bp::record<std::integral_constant<std::size_t, Xs>...>;
        using y_type = bp::record<std::integral_constant<std::size_t, Ys>...>;
        using z_type = bp::subtract_t<y_type, x_type>;
        return record_to_index_sequence(z_type{});
    }

    template <std::size_t... Is>
    static consteval std::index_sequence<Is...> record_to_index_sequence(
        bp::record<std::integral_constant<std::size_t, Is>...>) {
        return std::index_sequence<Is...>{};
    }

  private:
    std::tuple<Ts...> m_model;
};

///
template <typename... Ts> packet(Ts &&...) -> packet<Ts...>;
// template <typename... Ts> packet(std::tuple<Ts...>) -> packet<Ts...>;

} // namespace bp
