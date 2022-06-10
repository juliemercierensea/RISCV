/*!
 * \file ControlUnitSymbol.h
 * \brief   It is the control unit : it produces a 32 signal with the values of sel1PC, sel2PC, iPC... corresponding to the instruction instr and membusy inputs.
 * \author Julie Mercier
 * \version 0.1
 * \date  1 June 2022
 */

/*! \todo
 *  For now, everything : we only have defined the inputs, outputs and the method \n
 */

#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H
#include <systemc.h>

SC_MODULE (scControlUnit) {
    // ---------------------      Ports      ---------------------

    sc_in_clk           clock{"clock_i"};
    sc_in<bool>         RST{"reset_i"};
    sc_in<bool>         membusy{"Membusy_i"};
    sc_in<sc_lv<32>>    instr{"instruction_i"};

    sc_out<bool>         sel1PC;
    sc_out<bool>         sel2PC;
    sc_out<bool>         iPC;
    sc_out<bool>         JB;
    sc_out<bool>         XZ;
    sc_out<bool>         XN;
    sc_out<bool>         XF;
    sc_out<bool>         wRD {"write_RD_o"};
    sc_out<bool>         selRD;
    sc_out<bool>         sel1ALU;
    sc_out<sc_lv<2>>     sel2ALU;
    sc_out<sc_lv<4>>     selopALU;
    sc_out<bool>         wIR{"write_IR_o"};
    sc_out<bool>         RD {"RD_o"};
    sc_out<bool>         WR;
    sc_out<bool>         IDMEM;


    SC_CTOR(scControlUnit) {
        SC_THREAD(operations);
        sensitive << clock;
        sensitive << RST;
        sensitive << membusy;
        sensitive << instr;
    }

    void operations() {
        while (1){

        wait();
        }
     }
};

#endif // CONTROLUNIT_H
