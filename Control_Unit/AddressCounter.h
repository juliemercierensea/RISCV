/*!
 * \file AddressCounter.h
 * \brief As the name indicates : it's an address counter\n
 * \author Julie Mercier
 * \version 0.1
 * \date  25 May 2022\n
 *  **Operation of the address counter:**\n
 *      - 1)The address counter resets asynchronously to 0x00 (binary) with reset; otherwise,\n
 *      - 2)If the counter is enabled, then on the rising edge of the clock:\n
 *          - a)If EOI = 1 then the address counter resets synchronously to the value 0x01; otherwise,\n
 *          - b)If EOF = 1 then the address counter loads INUM+1; otherwise,\n
 *          - c)If none of the above, the address counter counts up.\n
 *
 *  **The address counter is enabled if:**\n
 *      - There is no ongoing memory operation (waitMEM = 0), or\n
 *      - There is an ongoing memory operation (waitMEM = 1) and the memory interface has signaled this is the last cycle of the operation (membusy = 0).\n
 */

/*! \bug "warning 206 :vector contains 4-value logic" but the compilation works
 */

#ifndef ADDRESSCOUNTER_H
#define ADDRESSCOUNTER_H
#include <systemc.h>

SC_MODULE (scAddressCounter) {
    // ---------------------      Ports      ---------------------

    sc_in_clk           clock{"clock_i"};
    sc_in<bool>         RST{"reset_i"};
    sc_in<bool>         EOI{"restartEOI_i"};
    sc_in<bool>         EOF_i{"LoadEOF_i"};
    sc_in<sc_lv<8>>     LoadVal{"LoadVal_from_InstrNumber_i"};
    sc_in<bool>         waitMEM;
    sc_in<bool>         memBusy;

    sc_out<sc_lv<8>>    Val;
    sc_out<sc_lv<8>>  EOI_i0;


    SC_CTOR(scAddressCounter) {
        SC_THREAD(function);
        sensitive << clock.pos();
        sensitive << RST;
        }

    void function() {

        EOI_i0.write(0);
        while (1){
            Val.write(1);

            if (RST.read() == 1) {
                Val.write(0);
            }
            else{
                if (waitMEM.read()==0){
                    if(EOI.read()== 1){
                        Val.write(1);
                    }
                    else{
                        if(EOF_i.read()== 1){
                            Val.write((LoadVal.read().to_uint())+1);
                        }
                        else{
                            Val.write((Val.read().to_uint())+1);
                           //the address counter counts up
                        }
                    }
                }
                if((waitMEM.read()==1) && (memBusy.read()==0)){
                    if(EOI.read()== 1){
                        Val.write(1);
                    }
                    else{
                        if(EOF_i.read()== 1){
                            Val.write((LoadVal.read().to_uint())+1);
                        }
                        else{
                            Val.write((Val.read().to_uint())+1);
                           //the address counter counts up
                        }
                    }
                }
            }
        wait();
        }
    }
};

#endif // ADDRESSCOUNTER_H
