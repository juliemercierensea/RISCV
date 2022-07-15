/*!
 * \file PC.h
 * \brief PC register acts like an up counter (+4 instead of +1) when Inc is set to 1 (this must be the test with the higher priority level)
 * \author Julie Mercier
 * \version 0.1
 * \date  16 June 2022\n
 */

#ifndef PCREGISTER_H
#define PCREGISTER_H
#include <systemc.h>

SC_MODULE (scPCregister) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<32>>        LoadVal{"LoadVal_targetPC_i"};
    //sc_in<sc_lv<32>>    Load{"Load_wPC_i"};
    sc_lv<32>               Load=0;
    sc_in<sc_lv<1>>         Inc{"iPC_i"};
    sc_in<bool>             RST{"reset_i"};
    sc_in_clk               clock{"clock_i"};

    sc_out<sc_lv<32>>    Count{"Count_o"};

    SC_CTOR(scPCregister) {
        SC_THREAD(PClogic);

        sensitive << clock.pos();
        sensitive << RST;
    }

    void PClogic() {

        while(1){
            Count.write(0); //on initialise

            if (RST.read() == true) {
                Count.write(0); // on réinitialise
            }

            if(Inc.read()==1){//on fait +4 tant que Inc=1
                Count.write(Count.read().to_uint()+4);
                }

            else {
                if(Load==1){
                    Count.write(LoadVal);
                }
            }
            wait();
        }
    }

};

#endif // PCREGISTER_H
