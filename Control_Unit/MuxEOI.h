/*!
 * \file MuxEOI.h
 * \brief Multiplexer 2 to 1 with a variable lenght N of input and output
 * \author Julie Mercier
 * \version 0.1
 * \date  26 May 2022
 *
 * It is the multiplexer following the address counter and the multiplexer EOF\n
 * \n
 *  - i0 is the res of the EOF multiplexer (so either INUM or uCYCLE)\n
 *  - i1 is 0x00\n
 *  - sel is EOI,the selection input equal to 0 or 1\n
 *  - res is uADD, one input of the ROM_Memory\n
 *  - res= i1=0x00 if EOI==1 \n
 *  \param[in] N : the lenght of the input (therefore output) signals, its default value is set to 8\n
 */
#ifndef MUXEOI_H
#define MUXEOI_H
#include <systemc.h>

template <unsigned int N=8>
class mux2to1EOI : public ::sc_core::sc_module {
public:
    static_assert(N >= 1, "scMuxT DESIGN ERROR: N must be >=2");

    sc_in<sc_lv<N>>  i0;
    sc_in<sc_lv<N>>  i1;
    sc_in<bool>  sel{"sel"}; // Selection input (0 or 1)
    sc_out<sc_lv<N>>  res;

    SC_CTOR(mux2to1EOI) {
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

#endif // MUXEOI_H
