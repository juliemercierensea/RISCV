#ifndef MUX2TO1_H
#define MUX2TO1_H

#include <systemc.h>

template <unsigned int N=8>
class mux2to1 : public ::sc_core::sc_module {
public:
    static_assert(N >= 1, "scMuxT DESIGN ERROR: N must be >=2");

    sc_in<sc_lv<N>>  i0;
    sc_in<sc_lv<N>>  i1;
    sc_in<bool>  sel{"sel"}; // Selection input (0 or 1)
    sc_out<sc_lv<N>>  res;

    SC_CTOR(mux2to1) {
        SC_METHOD(combinational);
        sensitive << i0;
        sensitive << i1;
        sensitive << sel;
    }

    void combinational () {
        if (sel.read()) {
            res.write(i1.read());
        } else {
            res.write(i0.read());
        }
    }
};

#endif // MUX2TO1_H
