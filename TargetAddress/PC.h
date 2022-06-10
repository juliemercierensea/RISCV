#ifndef WRITEPC_H
#define WRITEPC_H
#include <systemc.h>

SC_MODULE (scPC) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<32>>    targetPC{"LoadVal_targetPC_i"};
    sc_in<sc_lv<32>>    wPC{"Load_wPC_i"};
    sc_in<bool>    iPC{"Inc"};
    sc_in<bool>    RST{"reset"};
    sc_clock    clk;

    sc_out<sc_lv<32>>    PC_value{"Count_o"};

    SC_CTOR(scPC) {
        SC_THREAD(name);
        sensitive << targetPC;
        sensitive << wPC;
        sensitive << clk;
        sensitive << RST;
        sensitive << iPC;
    }

    void name() {
        PC_value.write(0); //on initialise à 0 (début fetch)
        while(1){

            if (RST.read() == true) {
                PC_value.write(0); // est ce qu'il ne se passe que ça?
                }

            if(wPC.read().to_uint()==0){//on fait +4 tant que ? absence de wPC ie wPC==0 (?)
               PC_value.write(PC_value.read().to_uint()+4);
               }
            else {
               PC_value.write(wPC.read()); //PC_value prend la valeur de wPC pour prendre le branchement
               }

            wait();
        }
//autre compréhension :
        PC_value.write(0); //on initialise
        while(1){

            if (RST.read() == true) {
                PC_value.write(targetPC.read()); // on initialise à la valeur targetPC

                if(wPC.read().to_uint()==0){//on fait +4 tant que ? absence de wPC ie wPC==0 (?)
                    PC_value.write(PC_value.read().to_uint()+4);
                }
                }
            else {
                if(wPC.read().to_uint()==0){//on fait +4 tant que ? absence de wPC ie wPC==0 (?)
                    PC_value.write(PC_value.read().to_uint()+4);
                }
            }
            wait();
        }

     }
};

#endif // WRITEPC_H
