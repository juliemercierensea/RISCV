/*!
 * \file mux2to1.h
 * \brief multiplexer 2 to 1 with a variable lenght N (as parameter) but not adapted to boolean (must define them as sc_lv<1>)
 * \author Julie Mercier
 * \version 0.2
 * \date  28 June 2022\n
 */

#ifndef MUX2TO1_H
#define MUX2TO1_H

#include <systemc.h>

template <unsigned int N >
class mux2to1 : public ::sc_core::sc_module {
public:
    static_assert(N >= 1, "scMuxT DESIGN ERROR: N must be >=2");

    sc_in<sc_lv<N>>  i0;
    sc_in<sc_lv<N>>  i1;
    sc_in<sc_lv<1>>  sel{"sel"}; // Selection input (0 or 1)
    sc_out<sc_lv<N>>  res;

    SC_CTOR(mux2to1) {
        SC_METHOD(combinational);
        sensitive << i0;
        sensitive << i1;
        sensitive << sel;
    }

    void combinational () {
        if (sel.read()==1) {
            res.write(i1.read());
        } else {
            res.write(i0.read());
        }
    }
};

#endif // MUX2TO1_H
