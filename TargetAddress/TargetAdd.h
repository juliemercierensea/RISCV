#ifndef TARGETADD_H
#define TARGETADD_H
#include <systemc.h>

SC_MODULE (scTargetAdd) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<32>>    op1{"operand1_i"};
    sc_in<sc_lv<32>>    op2{"operand2_i"};

    sc_out<sc_lv<32>>    targetPC{"targetPC_o"};


    SC_CTOR(scTargetAdd) {
        SC_THREAD(operation);
        sensitive << op1;
        sensitive << op2;
    }

    void operation() {
        while (1){
            targetPC.write(op1.read().to_uint64()+op2.read().to_uint64());

        wait();
        }
     }
};

#endif // TARGETADD_H
