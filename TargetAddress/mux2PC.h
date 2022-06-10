#ifndef MUX2PC_H
#define MUX2PC_H
#include <systemc.h>

template <unsigned int N=32>
class mux2PC : public ::sc_core::sc_module {
public:
    static_assert(N >= 1, "scMuxT DESIGN ERROR: N must be >=2");

    sc_in<sc_lv<N>>  B_imm;
    sc_in<sc_lv<N>>  J_imm;
    sc_in<bool>  sel2PC{"sel1"}; // Selection input (0 or 1)
    sc_out<sc_lv<N>>  res2;

    SC_CTOR(mux2PC) {
        SC_METHOD(combinational);
        sensitive << B_imm;
        sensitive << J_imm;
        sensitive << sel2PC;
    }

    void combinational () {
        if (sel1PC.read()) {
            res2.write(B_imm.read());
        } else {
            res2.write(J_imm.read());
        }
    }
};

#endif // MUX2PC_H
