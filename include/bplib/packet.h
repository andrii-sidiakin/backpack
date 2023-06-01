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

  private:
    template <typename... Us, std::size_t... Is>
    static constexpr auto select_tuple(std::tuple<Us...> &tuple,
                                       std::index_sequence<Is...>) noexcept {
        static_assert(((Is < sizeof...(Us)) && ...), "index out of range");
        return std::tie(std::get<Is>(tuple)...);
    }

  private:
    std::tuple<Ts...> m_model;
};

///
template <typename... Ts> packet(Ts &&...) -> packet<Ts...>;

} // namespace bp
