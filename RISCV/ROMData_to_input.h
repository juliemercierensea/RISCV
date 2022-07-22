/*!
 * \file ROMData_to_input.h
 * \brief Extract the values of waitMEM, EOF and EOI from the the ROM_memory output (Data called ROMData here) (32bits)\n
 *        To do so, we use bitwise masks : we put a 1 at the bit we want to extract to 1, 0 everywhere else and then we shift right the bit so as to put it in first position.\n
 *        waitMEM is bit 0, EOF is bit 30 and EOI is bit 31 of ROMData.
 * \author Julie Mercier
 * \version 0.1
 * \date  17 June 2022\n
 */

#ifndef ROMDATA_TO_INPUT_H
#define ROMDATA_TO_INPUT_H
#include <systemc.h>

SC_MODULE (scSelectfromROMData) {

    sc_in<sc_lv<32>>        ROMData{"ROMData"};

    sc_out<sc_lv<1>>        waitMEM_o{"waitMEM_o"};
    sc_out<sc_lv<1>>        IDMEM{"IDMEM"};
    sc_out<sc_lv<1>>        RD{"RD"};
    sc_out<sc_lv<1>>        WR{"WR"};
    sc_out<sc_lv<1>>        wIR{"wIR"};
    sc_out<sc_lv<1>>        iPC{"iPC"};
    sc_out<sc_lv<1>>        selRD{"selRD"};
    sc_out<sc_lv<1>>        wRD{"wRD"};
    sc_out<sc_lv<4>>        selopALU{"selopALU"};
    sc_out<sc_lv<2>>        sel2ALU{"sel2ALU"};
    sc_out<sc_lv<1>>        sel1ALU{"sel1ALU"};
    sc_out<sc_lv<1>>        sel2PC{"sel2PC"};
    sc_out<sc_lv<1>>        sel1PC{"sel1PC"};
    sc_out<sc_lv<1>>        EOF_o{"EOF_o"};
    sc_out<sc_lv<1>>        EOI_o{"EOI_o"};

    SC_CTOR(scSelectfromROMData) {
        SC_THREAD(selection);
        sensitive << ROMData;
    }
    void selection (){
        while(1){

        waitMEM_o.write(ROMData.read()& 0x1);
        IDMEM.write((ROMData.read()&0b1)>>0x1);
        RD.write((ROMData.read()&0b100)>>0x2);
        WR.write((ROMData.read()&0b1000)>>0x3);
        wIR.write((ROMData.read()&0b10000)>>0x4);
        iPC.write((ROMData.read()&0b100000)>>0x5);
        selRD.write((ROMData.read()&0b1000000)>>0x6);
        wRD.write((ROMData.read()&0b10000000)>>0x7);
        selopALU.write((ROMData.read()&0b111100000000)>>0x8);
        sel2ALU.write((ROMData.read()&0b11000000000000)>>0xC);
        sel1ALU.write((ROMData.read()&0b100000000000000)>>0xE);
        sel2PC.write((ROMData.read()&0b100000000000000000000)>>0x14);
        sel1PC.write((ROMData.read()&0b1000000000000000000000)>>0x15);
        EOF_o.write((ROMData.read()& 0x40000000)>>0x1E);
        EOI_o.write((ROMData.read()& 0x80000000)>>0x1F);

        wait();
        }
    }
};

#endif // ROMDATA_TO_INPUT_H
