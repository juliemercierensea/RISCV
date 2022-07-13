#include <systemc.h>
#ifndef MI_SHIFT_LEFT_H
#define MI_SHIFT_LEFT_H

template <unsigned int N=32,unsigned int SA=24>
class shiftleft : public ::sc_core::sc_module {
public:
    static_assert(N >= 1, "scshiftleft DESIGN ERROR: N must be >=2");

    sc_in<sc_lv<N>>  in{"in"};
    sc_out<sc_lv<N>> out{"out"};

    SC_CTOR(shiftleft) {
        SC_METHOD(combinational);
        sensitive << in;
    }

    void combinational () {
        out.write(in.read()<<SA);
    }
};

#endif // MI_SHIFT_LEFT_H
