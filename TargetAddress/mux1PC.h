#ifndef MUX1PC_H
#define MUX1PC_H
#include <systemc.h>

template <unsigned int N=32>
class mux1PC : public ::sc_core::sc_module {
public:
    static_assert(N >= 1, "scMuxT DESIGN ERROR: N must be >=2");

    sc_in<sc_lv<N>>  rs1;
    sc_in<sc_lv<N>>  PC_value;
    sc_in<bool>  sel1PC{"sel1"}; // Selection input (0 or 1)
    sc_out<sc_lv<N>>  res1;

    SC_CTOR(mux1PC) {
        SC_METHOD(combinational);
        sensitive << rs1;
        sensitive << PC_value;
        sensitive << sel1PC;
    }

    void combinational () {
        if (sel1PC.read()) {
            res1.write(PC_value.read());
        } else {
            res1.write(rs1.read());
        }
    }
};

#endif // MUX1PC_H
