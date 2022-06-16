/*!
 * \file Instruction.h
 * \brief Extract the values of func10_0, func7_5, func3 ad opcode_6to2 from the instruction signal (32bits)\n
 * \author Julie Mercier
 * \version 0.1
 * \date  9 June 2022\n
 */

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <systemc.h>

SC_MODULE (scInstruction) {

    sc_in<sc_lv<32>>     instruction;

    sc_out<bool>         func12_0;
    sc_out<bool>         func7_5;
    sc_out<sc_lv<3>>     func3;
    sc_out<sc_lv<5>>     opcode_6to2;

    SC_CTOR(scInstruction) {
        SC_THREAD(selection);
        sensitive << instruction;
    }
    void selection (){
        while(1){

        func12_0.write((instruction.read().to_uint()& 0x100000)>>0x14);
        func7_5.write((instruction.read().to_uint()& 0x40000000)>>0x1E);
        func3.write((instruction.read().to_uint()& 0x7000)>>0xC);
        opcode_6to2.write((instruction.read().to_uint()& 0x7C)>>0x2);

        wait();
        }
    }
};

#endif // INSTRUCTION_H
