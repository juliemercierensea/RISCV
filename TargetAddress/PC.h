#ifndef WRITEPC_H
#define WRITEPC_H
#include <systemc.h>

SC_MODULE (scPC) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<32>>    LoadVal{"LoadVal_targetPC_i"};
    sc_in<sc_lv<32>>    Load{"Load_wPC_i"};
    sc_in<bool>         Inc{"iPC_i"};
    sc_in<bool>         RST{"reset_i"};
    sc_in_clk           clock{"clock_i"};

    sc_out<sc_lv<32>>    Count{"Count_o"};

    SC_CTOR(scPC) {
        SC_THREAD(PClogic);

        sensitive << clock;
        sensitive << RST;
    }

    void PClogic() {

        //Count.write(0); //on initialise
        while(1){
            Count.write(0); //on initialise

            if (RST.read() == true) {
                Count.write(0); // on réinitialise
            }

            if(Inc.read()==1){//on fait +4 tant que Inc=1
                Count.write(Count.read().to_uint()+4);
                }

            else {
                if(Load.read()==1){
                    Count.write(LoadVal);
                }
            }
            wait();
        }
    }

};

#endif // WRITEPC_H
