/*!
 * \file mux4to1ALU.h
 * \brief Multiplexer 4 to 1 with a variable lenght N of input and output
 * \author Julie Mercier
 * \version 0.1
 * \date 28 June 2022
 * \n
 *  - i0 is \n
 *  - i1 is \n
 *  - i2 is \n
 *  - i3 is \n
 *  - sel is \n
 *  - res is \n
 *  \param[in] N : the lenght of the input (therefore output) signals, its default value is set to 32\n
 */

#include <systemc.h>
#ifndef MUX3TO1_H
#define MUX3TO1_H

template <unsigned int N=32>
class mux3to1 : public ::sc_core::sc_module {
public:
    static_assert(N >= 1, "scMuxT DESIGN ERROR: N must be >=2");

    sc_in<sc_lv<N>>  i0;
    sc_in<sc_lv<N>>  i1;
    sc_in<sc_lv<N>>  i2;

    sc_in<sc_lv<2>>  sel{"selection"}; // Selection input (0, 1, or 2)
    sc_out<sc_lv<N>> res;

    SC_CTOR(mux3to1) {
        SC_METHOD(combinational);
        sensitive << i0;
        sensitive << i1;
        sensitive << i2;
        sensitive << sel;
    }

    void combinational () {
        if(sel.read()==0){
            res.write(i0.read());
        }
        else{
            if(sel.read()== 1){
                res.write(i1.read());
            }
            else{
                res.write(i2.read());
            }
        }
    }
};

#endif // MUX3TO1_H
