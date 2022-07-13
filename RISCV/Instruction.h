/*!
 * \file Instruction.h
 * \brief Extract the values of func12_0, func7_5, func3 and opcode_6to2 from the instruction signal (32bits)\n
 *        To do so, we use bitwise masks : we put a 1 at the bit we want to extract to 1, 0 everywhere else.\n
 *         If it's a single bit, then we shift right the bit so as to put it in first position.\n
 *         If not, be carefull that the shift amount is linked to the lowest bit you want to extract.\n
 *         func12_0 is bit 20, func7_5 is bit 30 \n
 *         func3 is bit 14 to 12 (included) and opcode_6to2 is bit 6 to 2 (included)\n
 * \author Julie Mercier
 * \version 0.1
 * \date  9 June 2022\n
 */

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <systemc.h>

SC_MODULE (scInstruction) {

    sc_in<sc_lv<32>>     instruction{"instruction"};

    sc_out<bool>         func12_0{"func12_0"};
    sc_out<bool>         func7_5{"func7_5"};
    sc_out<sc_lv<3>>     func3{"func3"};
    sc_out<sc_lv<5>>     opcode_6to2{"opcode_6to2"};

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
