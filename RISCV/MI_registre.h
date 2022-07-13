#include <systemc.h>
#ifndef MI_REGISTRE_H
#define MI_REGISTRE_H

template <unsigned int N=32>
class registre : public ::sc_core::sc_module {
public:
    static_assert(N >= 1, "scregistre DESIGN ERROR: N must be >=2");

    sc_in_clk        clock{"clock"};
    sc_in<sc_lv<N>>  d{"d"};
    sc_in<sc_lv<1>>  load{"load"};
    sc_out<sc_lv<N>> q{"q"};

    SC_CTOR(registre) {
        SC_METHOD(combinational);
        sensitive << clock.pos();
        sensitive << d;
        sensitive << load;
    }

    void combinational () {
        if (load.read()==1) {
            q.write(d.read());
        }
    }
};

#endif // MI_REGISTRE_H
