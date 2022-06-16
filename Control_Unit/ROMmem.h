/*!
 * \file ROMmem.h
 * \brief   It is the ROM memory table:  it's a 256x32 table filled with all the operations to perform. One adress correspond to One operation.
 * \author Julie Mercier
 * \version 0.1
 * \date  2 June 2022
 */

/*! \todo
 *  1) Link the ports to the signals so as to connect with the output of the EOI multiplexer\n
 *  2) Check if it is working until the FENCE instruction, if yes then do 3)\n
 *  3) Fill the table with all the addresses and the corresponding operation\n
 */

/*! \bug
 *  Check if we have to write the values in hexa, binary or deci. For now, they are written as decimals\n
 */

#ifndef ROMMEM_H
#define ROMMEM_H
#include <systemc.h>

SC_MODULE (scROM_Memory) {
    // ---------------------      Ports      ---------------------

    sc_in_clk           clock{"clock_i"};
    sc_in<sc_lv<8>>     Address {"uADDR_i"};
    sc_in<bool>         waitMEM;
    sc_in<bool>         memBusy;

    sc_out<sc_lv<32>>    Data {"Data_uINSTR_control_signals"};

    sc_lv<32>  ROMtab[256];


    SC_CTOR(scROM_Memory) {
        SC_THREAD(function);
        sensitive << clock.pos();
        }

    void function() {
        ROMtab[0x0]=0x04;
        ROMtab[0x1]=0x40000011;
        ROMtab[0x2]=0x80000000;
        ROMtab[0x3]=0x80000000;
        ROMtab[0x4]=0x80000000;
        ROMtab[0x5]=0x80000000;
        ROMtab[0x6]=0x80000000;
        ROMtab[0x7]=0x80000000;
        ROMtab[0x8]=0x0;
        ROMtab[0x9]=0x01004;
        ROMtab[0xA]=0x01;
        ROMtab[0xB]=0x800000C0;
        ROMtab[0xC]=0x0;
        ROMtab[0xD]=0x3008;
        ROMtab[0xE]=0x80000001;
        ROMtab[0xF]=0x80000000;
        for (int i=16;i<256; i++){
            ROMtab[i]=0x80000000; //même valeur que pour FENCE pour l'instant,pour tester
        }
        while (1){
            //first check if enabled or not
            if (((waitMEM.read())==0)||((waitMEM.read()==1) && (memBusy.read()==0))){
                Data.write(ROMtab[Address.read().to_uint()]);
            }
            wait();
        }
    }
};

#endif // ROMMEM_H
