#pragma once

#include <tuple>
#include <type_traits>

#include "record.h"

namespace bp {

struct raw_data_packet_spec {
    template <typename T> struct is_header : std::false_type {};
};

///
///
///
template <typename TT, typename Spec = raw_data_packet_spec> class packet;
///
template <typename... Ts>
packet(Ts &&...) -> packet<bp::record<Ts...>, raw_data_packet_spec>;
///
template <typename... Ts, typename Spec> class packet<bp::record<Ts...>, Spec> {
  public:
    using record_type = bp::record<Ts...>;
    using header_type = bp::select_if_t<record_type, Spec::template is_header>;
    using header_index_type = bp::make_index_t<header_type, record_type>;
    using payload_type = bp::subtract_t<record_type, header_type>;
    using payload_index_type = bp::make_index_t<payload_type, record_type>;

  public:
    explicit packet(Ts &&...args) noexcept
        : m_model(std::forward<Ts>(args)...) {
    }

    constexpr auto header() noexcept {
        return select_tuple(m_model, header_index_type{});
    }

    constexpr auto payload() noexcept {
        return select_tuple(m_model, payload_index_type{});
    }

  private:
    template <std::size_t... Is>
    static constexpr auto select_tuple(auto &tuple,
                                       std::index_sequence<Is...>) noexcept {
        return std::tie(std::get<Is>(tuple)...);
    }

  private:
    std::tuple<Ts...> m_model;
};

} // namespace bp
