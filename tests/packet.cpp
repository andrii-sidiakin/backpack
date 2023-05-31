#include <bplib/packet.h>

#include <cstdlib>
#include <cstring>

#define bp_expect(cond)                                                        \
    if (!(cond)) {                                                             \
        std::abort();                                                          \
    }

template <typename Spec, typename... Args> auto make_packet(Args &&...args) {
    return bp::packet<bp::record<Args...>, Spec>(std::forward<Args>(args)...);
}

struct integral_header_spec {
    template <typename T> using is_header = std::is_integral<T>;
};

int main() {

    {
        bp::packet pkt{10, 3.14, "text"};
        using header_type = decltype(pkt.header());
        static_assert(std::tuple_size_v<header_type> == 0);
    }

    {
        auto pkt = make_packet<integral_header_spec>(10, 3.14, "text");

        auto h = pkt.header();
        using header_type = decltype(h);
        static_assert(std::tuple_size_v<header_type> == 1);
        bp_expect(std::get<0>(h) == 10);

        auto p = pkt.payload();
        using payload_type = decltype(p);
        static_assert(std::tuple_size_v<payload_type> == 2);
        bp_expect(std::get<0>(p) == 3.14);
        bp_expect(strcmp(std::get<1>(p), "text") == 0);
    }

    return 0;
}
