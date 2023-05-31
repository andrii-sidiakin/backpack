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
    using header_type = bp::record<double, char>;
    auto header = proceed<header_type>(1, 'b', 3.14f, "D", 0xf, 9.8);
    static_assert(std::is_same_v<decltype(header), header_type>);

    return 0;
}
