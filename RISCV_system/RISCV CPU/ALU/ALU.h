/*!
 * \file ALU.h
 * \brief Bloc of the ALU gathering and binding the ALU register and the 2 multiplexers before the ALU\n
 * \author Julie Mercier
 * \version 0.1
 * \date  July 15th 2022\n
 */
#ifndef ALU_H
#define ALU_H
#include <systemc.h>
#include "ALU_register.h"
#include "mux2to1.h"
#include "mux4to1.h"
#include "trace.h"

SC_MODULE(scALU){
    sc_in<sc_lv<4>>   selopALU{"selopALU"};
    sc_in<sc_lv<2>>   sel2ALU{"sel2ALU"};
    sc_in<sc_lv<1>>   sel1ALU{"sel1ALU"};
    sc_in<sc_lv<32>>  rs1_value{"rs1_value"};
    sc_in<sc_lv<32>>  PC_value{"PC_value"};
    sc_in<sc_lv<32>>  rs2_value{"rs2_value"};
    sc_in<sc_lv<32>>  I_imm{"I_imm"};
    sc_in<sc_lv<32>>  U_imm{"U_imm"};
    sc_in<sc_lv<32>>  S_imm{"S_imm"};

    sc_out<sc_lv<32>>     ALU_value{"ALUvalue"};

    scALUreg ALU{"RISCV_ALU"};
    mux4to1<32> mux2ALU {"Mux2_ALU"};
    mux2to1<32> mux1ALU {"Mux1_ALU"};

    sc_signal<sc_lv<32>> muxtoop1{"muxtoop1"};
    sc_signal<sc_lv<32>> muxtoop2{"muxtoop2"};


    SC_CTOR(scALU){

        ALU.selop(selopALU);
        ALU.op1(muxtoop1);
        ALU.op2(muxtoop2);
        ALU.res(ALU_value);

        mux1ALU.sel(sel1ALU);
        mux1ALU.i1(PC_value);
        mux1ALU.i0(rs1_value);
        mux1ALU.res(muxtoop1);

        mux2ALU.sel(sel2ALU);
        mux2ALU.i0(rs2_value);
        mux2ALU.i1(I_imm);
        mux2ALU.i2(U_imm);
        mux2ALU.i3(S_imm);
        mux2ALU.res(muxtoop2);

        wf= sc_create_vcd_trace_file("itest_ALU");

        sc_trace(wf,selopALU,"selopALU");
        sc_trace(wf,sel2ALU,"sel2ALU");
        sc_trace(wf,sel1ALU,"sel1ALU");
        sc_trace(wf,rs1_value,"rs1_value");
        sc_trace(wf,PC_value,"PC_value");
        sc_trace(wf,rs2_value,"rs2_value");
        sc_trace(wf,I_imm,"I_imm");
        sc_trace(wf,U_imm,"U_imm");
        sc_trace(wf,S_imm,"S_imm");
        sc_trace(wf,ALU_value,"ALU_value");
        sc_trace(wf,muxtoop1,"muxtoop1");
        sc_trace(wf,muxtoop2,"muxtoop2");
    }
};

#endif // ALU_H
