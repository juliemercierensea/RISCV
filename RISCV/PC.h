#ifndef PC_H
#define PC_H
#include <systemc.h>
#include "mux2to1.h"
#include "PCregister.h"
#include "TargetAdd.h"

SC_MODULE(scPC){

    sc_in<sc_lv<32>>    rs1_value{"rs1_value"};
    sc_in<sc_lv<32>>    B_imm{"B_imm"};
    sc_in<sc_lv<32>>    J_imm{"J_imm"};
    sc_in<sc_lv<1>>     sel1PC{"sel1PC"};
    sc_in<sc_lv<1>>     sel2PC{"sel2PC"};
    sc_in<sc_lv<1>>     iPC{"iPC"};
    sc_in<bool>         RST{"reset"};
    sc_in_clk           clock{"clock"};

    sc_out<sc_lv<32>>   PC_value{"PC_value"};

    scPCregister PC{"PC_PCreg"};
    mux2to1<32> mux1PC{"mux1PC"};
    mux2to1<32> mux2PC{"mux2PC"};
    scTargetAdd TargetAdd {"RISCV_Target_Address"};

    sc_signal<sc_lv<32>>    mux1_to_targetAdd{"mux1_to_targetAdd"};
    sc_signal<sc_lv<32>>    mux2_to_targetAdd{"mux2_to_targetAdd"};
    sc_signal<sc_lv<32>>    targetPC{"targetPC"};

    SC_CTOR(scPC){
        mux1PC.sel(sel1PC);
        mux1PC.i1(rs1_value);
        mux1PC.i0(PC_value);
        mux1PC.res(mux1_to_targetAdd);

        mux2PC.sel(sel2PC);
        mux2PC.i0(B_imm);
        mux2PC.i1(J_imm);
        mux2PC.res(mux2_to_targetAdd);

        PC.LoadVal(targetPC);
        //PC.Load(wPC);
        PC.Inc(iPC);
        PC.Count(PC_value);
        PC.clock(clock);
        PC.RST(RST);

        TargetAdd.op1(mux1_to_targetAdd);
        TargetAdd.op2(mux2_to_targetAdd);
        TargetAdd.targetPC(targetPC);
    }
};


#endif // PC_H
