#ifndef WRITEPC_H
#define WRITEPC_H
#include <systemc.h>

SC_MODULE (scPC) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<32>>    LoadVal{"LoadVal_targetPC_i"};
    sc_in<sc_lv<32>>    Load{"Load_wPC_i"};
    sc_in<bool>    Inc{"iPC_i"};
    sc_in<bool>    RST{"reset_i"};
    sc_clock    clk;

    sc_out<sc_lv<32>>    PC_value{"Count_o"};

    SC_CTOR(scPC) {
        SC_THREAD(PClogic);
        sensitive << LoadVal;
        sensitive << Load;
        sensitive << clk;
        sensitive << RST;
        sensitive << Inc;
    }

    void PClogic() {

        PC_value.write(0); //on initialise
        while(1){

            if (RST.read() == true) {
                PC_value.write(0); // on réinitialise
            }

            if(Inc.read()==1){//on fait +4 tant que Inc=1
                PC_value.write(PC_value.read().to_uint()+4);
                }

            else {
                if(Load.read()==1){
                    PC_value.write(LoadVal);
                }
            }
            wait();
        }
    }

};

#endif // WRITEPC_H
