#include <bplib/bplib.h>

template <bp::Record R, typename... Args>
auto proceed([[maybe_unused]] Args &&...args) {
    using record_type = bp::record<Args...>;

    using actual_type = bp::intersect_t<R, record_type>;
    using actual_index = bp::make_index_t<actual_type, record_type>;

    return bp::select_index_t<record_type, actual_index>{};

    /*using others_type = bp::subtract_t<record_type, actual_type>;
    using others_index = bp::make_index_t<others_type, record_type>;
    using first_part = bp::select_index_t<record_type, actual_index>;
    using second_part = bp::select_index_t<record_type, actual_index>;
    return bp::record<first_part, second_part>{};*/
}

int main() {
    // types I want in order I want
    using header_type = bp::record<double, long, char>;
    {
        // all are present
        using expect_type = header_type;
        auto header = proceed<header_type>(1, 'b', 3.14f, "D", 5L, 9.8);
        static_assert(std::is_same_v<decltype(header), expect_type>);
    }

    {
        // not all are present
        using expect_type = bp::record<double, char>;
        auto header = proceed<header_type>(1, 'b', 3.14f, "D", 5, 9.8);
        static_assert(std::is_same_v<decltype(header), expect_type>);
    }

    {
        using record_type = bp::record<float, long, void, char, int>;
        using expect_type = bp::record<int, bool, long>;
        using index = bp::make_index_t<expect_type, record_type>;
        using diff_index = bp::make_diff_index_t<expect_type, record_type>;
        static_assert(std::is_same_v<index, std::index_sequence<4, 1>>);
        static_assert(std::is_same_v<diff_index, std::index_sequence<0, 2, 3>>);
    }

    return 0;
}
