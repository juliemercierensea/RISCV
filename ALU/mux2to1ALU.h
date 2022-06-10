/*!
 * \file mux2to1ALU.h
 * \brief Multiplexer 2 to 1 with a variable lenght N of input and output
 * \author Julie Mercier
 * \version 0.1
 * \date 15 May 2022
 *
 * Written as mux1ALU on schematics\n
 * \n
 *  - i0 is rs1\n
 *  - i1 is PC_value\n
 *  - sel is sel1ALU,the selection input equal to 0 or 1\n
 *  - res is op1\n
 *  - res= i1=PC_value if i1==1 \n
 *  \param[in] N : the lenght of the input (therefore output) signals, its default value is set to 32\n
 */

#ifndef MUX2TO1ALU_H
#define MUX2TO1ALU_H

#include <systemc.h>

template <unsigned int N=32>
class mux2to1ALU : public ::sc_core::sc_module {
public:
    static_assert(N >= 1, "scMuxT DESIGN ERROR: N must be >=2");

    sc_in<sc_lv<N>>  i0;
    sc_in<sc_lv<N>>  i1;
    sc_in<bool>  sel{"sel"};
    sc_out<sc_lv<N>>  res;

    SC_CTOR(mux2to1ALU) {
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

#endif // MUX2TO1ALU_H
