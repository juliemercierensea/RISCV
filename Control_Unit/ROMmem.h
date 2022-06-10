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
    sc_in<sc_lv<8>>     uADDR {"Address"};
    sc_in<bool>         waitMEM;
    sc_in<bool>         memBusy;

    sc_out<sc_lv<32>>    uINSTR {"Data_uINSTR_control_signals"};

    sc_lv<32>  ROMtab[256];


    SC_CTOR(scROM_Memory) {
        SC_THREAD(function);
        sensitive << clock.pos();
        sensitive << uADDR;
        sensitive << waitMEM;
        sensitive << memBusy;
    }

    void function() {
        ROMtab[0]=4; //0x04 hexa
        ROMtab[1]=1073741841; //0x40000011 hexa
        ROMtab[2]=2147483648; //0x80000000 hexa
        ROMtab[3]=2147483648; //0x80000000 hexa
        ROMtab[4]=2147483648; //0x80000000 hexa
        ROMtab[5]=2147483648; //0x80000000 hexa
        ROMtab[6]=2147483648; //0x80000000 hexa
        ROMtab[7]=2147483648; //0x80000000 hexa
        ROMtab[8]=0;
        ROMtab[9]=4100; //0x01004 hexa
        ROMtab[10]=1;
        ROMtab[11]=2147483840; //0x800000C0 hexa
        ROMtab[12]=0;
        ROMtab[13]=12296; //0x3008 hexa
        ROMtab[14]=2147483649; //0x80000001 hexa
        ROMtab[15]=2147483648; //0x80000000 hexa

        for (int i=16;i<=256; i++){
            ROMtab[i]=2147483648; //0x80000000 hexa (même valeur que pour FENCE pour l'instant,pour tester)
        }
        while (1){
            //first check if enabled or not
            uINSTR.write(ROMtab[uADDR.read().to_int()]);

        }

        wait();
    }

};

#endif // ROMMEM_H
