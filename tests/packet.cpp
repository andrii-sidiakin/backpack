#include <bplib/packet.h>

int main() {

    auto p = bp::packet{'a', 2, 3.14f, 5L, 6.};

    {
        auto [c, i, l] = p.select_if<std::is_integral>();
        static_assert(std::is_same_v<decltype(c), char &>);
        static_assert(std::is_same_v<decltype(i), int &>);
        static_assert(std::is_same_v<decltype(l), long &>);
    }

    {
        auto [d, f] = p.template select<bp::record<double, float>>();
        static_assert(std::is_same_v<decltype(d), double &>);
        static_assert(std::is_same_v<decltype(f), float &>);
    }

    {
        auto [l, i, f] = p.select_index(std::index_sequence<3, 1, 2>{});
        static_assert(std::is_same_v<decltype(l), long &>);
        static_assert(std::is_same_v<decltype(i), int &>);
        static_assert(std::is_same_v<decltype(f), float &>);
    }

    return 0;
}
