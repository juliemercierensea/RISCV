/*!
 * \file MI_shiftright.h
 * \brief general implementation of a shiftright, can be used in any design\n
 * \author Julie Mercier
 * \version 0.1
 * \date  July 8th 2022\n
 * We can chose both the shift amount and the size of the signal\n
 *
 */
#ifndef MI_SHIFT_RIGHT_H
#define MI_SHIFT_RIGHT_H
#include <systemc.h>

template <unsigned int N=32,unsigned int SA=24>
class shiftright : public ::sc_core::sc_module {
public:
    static_assert(N >= 1, "scshiftright DESIGN ERROR: N must be >=2");

    sc_in<sc_lv<N>>  in{"in"};
    sc_out<sc_lv<N>> out{"out"};

    SC_CTOR(shiftright) {
        SC_METHOD(combinational);
        sensitive << in;
    }

    void combinational () {
        out.write(in.read()>>SA);
    }
};

#endif // MI_SHIFT_RIGHT_H
