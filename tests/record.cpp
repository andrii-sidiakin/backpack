#include "bplib/record.h"
#include <bplib/bplib.h>

#include <utility>

template <typename... Args> using R = bp::record<Args...>;

using Empty = R<>;
using R_ci = R<char, int>;
using R_cif = R<char, int, float>;
using R_cifd = R<char, int, float, double>;
using R_f = R<float>;
using R_d = R<double>;
using R_fd = R<float, double>;

int main() {

    // basics

    static_assert(Empty::size == 0);
    static_assert(Empty::is_empty == true);

    // element-index
    static_assert(bp::element_index_v<R_cif, char> == 0);
    static_assert(bp::element_index_v<R_cif, float> == 2);
    // these lines fail at compile time (as intended)
    // auto i = bp::element_index_v<Empty, char>;
    // auto i = bp::element_index_v<R_cif, double>;

    // select-if
    static_assert(
        std::is_same_v<bp::select_if_t<Empty, std::is_integral>, Empty>);

    static_assert(
        std::is_same_v<bp::select_if_t<R_cif, std::is_integral, true>, R_ci>);

    static_assert(
        std::is_same_v<bp::select_if_t<R_cifd, std::is_integral, false>, R_fd>);

    static_assert(
        std::is_same_v<bp::select_if_t<R_cif, std::is_pointer, true>, Empty>);

    // remove-if
    static_assert(
        std::is_same_v<bp::remove_if_t<Empty, std::is_integral>, Empty>);

    static_assert(
        std::is_same_v<bp::remove_if_t<R_cifd, std::is_integral, true>, R_fd>);

    static_assert(
        std::is_same_v<bp::remove_if_t<R_cif, std::is_integral, false>, R_ci>);

    static_assert(
        std::is_same_v<bp::remove_if_t<R_cif, std::is_pointer, true>, R_cif>);

    // subtract
    static_assert(std::is_same_v<bp::subtract_t<Empty, Empty>, Empty>);
    static_assert(std::is_same_v<bp::subtract_t<Empty, R_cif>, Empty>);
    static_assert(std::is_same_v<bp::subtract_t<R_cif, Empty>, R_cif>);
    static_assert(std::is_same_v<bp::subtract_t<R_cifd, R_cif>, R_d>);

    // intersect
    static_assert(std::is_same_v<bp::intersect_t<Empty, Empty>, Empty>);
    static_assert(std::is_same_v<bp::intersect_t<Empty, R_cif>, Empty>);
    static_assert(std::is_same_v<bp::intersect_t<R_cif, Empty>, Empty>);
    static_assert(std::is_same_v<bp::intersect_t<R_cif, R_fd>, R_f>);

    {
        // make sure intersection keeps target's order
        using origin = R<int, char, double, short>;
        using target = R<double, int *, char>;
        using expect = R<double, char>;
        static_assert(std::is_same_v<bp::intersect_t<target, origin>, expect>);
    }

    {
        static_assert(std::is_same_v<bp::concat_many_t<R_f>, R_f>);
        static_assert(std::is_same_v<bp::concat_many_t<R_f, R_d>, R_fd>);
        static_assert(
            std::is_same_v<bp::concat_many_t<R_ci, R_f, R_d>, R_cifd>);
    }

    {
        static_assert(
            std::is_same_v<bp::unique_t<R<char, int, int>>, R<char, int>>);
    }

    return 0;
}
