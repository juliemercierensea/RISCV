#ifndef WRITEPC_H
#define WRITEPC_H
#include <systemc.h>

SC_MODULE (scWritePC) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<32>>    JB{"Jump_or_branch_i"};
    sc_in<sc_lv<32>>    XZ;
    sc_in<sc_lv<32>>    XN;
    sc_in<sc_lv<32>>    XF;
    //sc_in<bool>    Z;
    //sc_in<bool>    N;
    //sc_in<bool>    C;

    sc_out<sc_lv<32>>    wPC{"Write_to_PC_o"};

    SC_CTOR(scWritePC) {
        SC_THREAD(name);
        sensitive << JB;
        sensitive << XZ;
        sensitive << XN;
        sensitive << XF;
    }

    void name() {
        while(1){

            wait();
        }

     }
};


#endif // WRITEPC_H
