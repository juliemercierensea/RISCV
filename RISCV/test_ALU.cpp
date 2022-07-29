/*!
 * \file test_ALU.cpp
 * \brief testbench for the ALU\n
 *        function called by the main : create the design and run the simulation, produce the vcd file\n
 *         **first** : definition of all the signals and link to the ports\n
 *                     /!\ the output of both multiplexers must be connected to the input signals op1 and op2 of the ALU \n
 *         **second** : start the simulation and  create the vcd files \n
 *         **third** : test if the program works as expected.
 * \author Julie Mercier
 * \version 0.1
 * \date 13 May 2022
 *
 */

#include <systemc.h>
#include "ALU.h"
#include "trace.h"

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<sc_lv<4>>   selopALU{"selopALU"};
    sc_signal<sc_lv<2>>   sel2ALU{"sel2ALU"};
    sc_signal<sc_lv<1>>   sel1ALU{"sel1ALU"};
    sc_signal<sc_lv<32>>  rs1_value{"rs1_value"};
    sc_signal<sc_lv<32>>  PC_value{"PC_value"};
    sc_signal<sc_lv<32>>  rs2_value{"rs2_value"};
    sc_signal<sc_lv<32>>  I_imm{"I_imm"};
    sc_signal<sc_lv<32>>  U_imm{"U_imm"};
    sc_signal<sc_lv<32>>  S_imm{"S_imm"};

    sc_signal<sc_lv<32>>     ALU_value{"ALUvalue"};

    scALU ALU("RISCV_ALU");

    ALU.selopALU(selopALU);
    ALU.sel2ALU(sel2ALU);
    ALU.sel1ALU(sel1ALU);
    ALU.rs1_value(rs1_value);
    ALU.PC_value(PC_value);
    ALU.rs2_value(rs2_value);
    ALU.I_imm(I_imm);
    ALU.U_imm(U_imm);
    ALU.S_imm(S_imm);
    ALU.ALU_value(ALU_value);

     sc_start();
     sc_trace_file *wf = sc_create_vcd_trace_file("ALU");

     sc_trace(wf,selopALU,"selopALU");
     sc_trace(wf,ALU_value,"ALU_value");
     sc_trace(wf,sel1ALU,"sel1ALU");
     sc_trace(wf,sel2ALU,"sel2ALU");
     sc_trace(wf,rs2_value,"rs2_value");
     sc_trace(wf,I_imm,"I_imm");
     sc_trace(wf,U_imm,"U_imm");
     sc_trace(wf,S_imm,"S_imm");
     sc_trace(wf,rs1_value,"rs1_value");
     sc_trace(wf,PC_value,"PC_value");

     sel1ALU.write(1);
     sel2ALU.write(1);
     rs2_value.write(1);
     I_imm.write(2147483650);
     U_imm.write(3);
     S_imm.write(4);
     rs1_value.write(5);
     PC_value.write(101);

     for (int i=0;i<2;i++) {

         selopALU.write(1);
         sc_start(10, SC_NS);
     }

     for (int i=0;i<2;i++) {

         selopALU.write(2);
         sc_start(10, SC_NS);
    }

     for (int i=0;i<2;i++) {

         selopALU.write(3);
         sc_start(10, SC_NS);
     }

     for (int i=0;i<2;i++) {

         selopALU.write(4);
         sc_start(10, SC_NS);
     }

     for (int i=0;i<2;i++) {

         selopALU.write(5);
         sc_start(10, SC_NS);
     }

     for (int i=0;i<2;i++) {

         selopALU.write(6);
         sc_start(10, SC_NS);
     }

     for (int i=0;i<2;i++) {

         selopALU.write(7);
         sc_start(10, SC_NS);
    }

     for (int i=0;i<2;i++) {

         selopALU.write(8);
         sc_start(10, SC_NS);
     }

     for (int i=0;i<2;i++) {

         selopALU.write(9);
         sc_start(10, SC_NS);
     }

     for (int i=0;i<2;i++) {

         selopALU.write(10);
         sc_start(10, SC_NS);
     }

     sc_close_vcd_trace_file(wf);
     return 0;
}
