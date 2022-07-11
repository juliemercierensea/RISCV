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

    sc_in<sc_lv<32>>     ROMData;

    sc_out<bool>         waitMEM_o;
    sc_out<sc_lv<1>>         EOF_o;
    sc_out<sc_lv<1>>         EOI_o;

    SC_CTOR(scSelectfromROMData) {
        SC_THREAD(selection);
        sensitive << ROMData;
    }
    void selection (){
        while(1){

        waitMEM_o.write(ROMData.read().to_uint()& 0x1);
        EOF_o.write((ROMData.read().to_uint()& 0x40000000)>>0x1E);
        EOI_o.write((ROMData.read().to_uint()& 0x80000000)>>0x1F);

        wait();
        }
    }
};

#endif // ROMDATA_TO_INPUT_H
