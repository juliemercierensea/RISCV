/*!
 * \file MuxEOF.h
 * \brief Multiplexer 2 to 1 with a variable lenght N of input and output
 * \author Julie Mercier
 * \version 0.1
 * \date  26 May 2022
 *
 * It is the multiplexer following the address counter\n
 * \n
 *  - i0 is uCYCLE\n
 *  - i1 is INUM\n
 *  - sel is EOF,the selection input equal to 0 or 1\n
 *  - res is sent to an other multiplexer with EOI\n
 *  - res= i1=INUM if EOF==1 \n
 *  \param[in] N : the lenght of the input (therefore output) signals, its default value is set to 8\n
 */

#ifndef MUXEOF_H
#define MUXEOF_H

#include <systemc.h>

template <unsigned int N=8>
class mux2to1EOF : public ::sc_core::sc_module {
public:
    static_assert(N >= 1, "scMuxT DESIGN ERROR: N must be >=2");

    sc_in<sc_lv<N>>  i0;
    sc_in<sc_lv<N>>  i1;
    sc_in<bool>  sel{"sel"}; // Selection input (0 or 1)
    sc_out<sc_lv<N>>  res;

    SC_CTOR(mux2to1EOF) {
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

#endif // MUXEOF_H
