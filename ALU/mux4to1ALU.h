/*!
 * \file mux4to1ALU.h
 * \brief Multiplexer 4 to 1 with a variable lenght N of input and output
 * \author Julie Mercier
 * \version 0.1
 * \date 15 May 2022
 *
 * Written as mux2ALU on schematics\n
 * \n
 *  - i0 is rs2\n
 *  - i1 is I_imm\n
 *  - i2 is U_imm\n
 *  - i3 is S_imm\n
 *  - sel is sel2ALU,the selection input equal to 0, 1, 2 or 3\n
 *  - res is op2\n
 *  \param[in] N : the lenght of the input (therefore output) signals, its default value is set to 32\n
 */

#ifndef MUX4TO1ALU_H
#define MUX4TO1ALU_H

#include <systemc.h>

template <unsigned int N=32>
class mux4to1ALU : public ::sc_core::sc_module {
public:
    static_assert(N >= 1, "scMuxT DESIGN ERROR: N must be >=2");

    sc_in<sc_lv<N>>  i0;
    sc_in<sc_lv<N>>  i1;
    sc_in<sc_lv<N>>  i2;
    sc_in<sc_lv<N>>  i3;
    sc_in<sc_lv<2>>  sel{"selection"}; // Selection input (0, 1, 2 or 3)
    sc_out<sc_lv<N>> res;

    SC_CTOR(mux4to1ALU) {
        SC_METHOD(combinational);
        sensitive << i0;
        sensitive << i1;
        sensitive << i2;
        sensitive << i3;
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
                if(sel.read()==2){
                    res.write(i2.read());
                }
                else{
                    res.write(i3.read());
                }
            }
        }
    }
};

#endif // MUX4TO1ALU_H
