/*!
 * \file test.cpp
 * \brief testbench
 * \author Julie Mercier
 * \version 0.1
 * \date 13 May 2022
 *
 * testbench for the ALU.
 */
/*!
 * \fn int sc_main (int argc, char *argv[])
 *         function called by the main : create the design and run the simulation, produce the vcd file\n
 *         **first** : definition of all the signals and link to the ports\n
 *                     /!\ the output of both multiplexers must be connected to the input signals op1 and op2 of the ALU \n
 *         **second** : start the simulation and  create the vcd files \n
 *         **third** : test if the program works as expected.
 *  \param[in] int argc\n
 *  \param[in] char *argv\n
 *  \return 0
 */

#include <systemc.h>
#include "alu.h"
#include "mux2to1ALU.h"
#include "mux4to1ALU.h"


int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<sc_lv<4>> selop("selop");
    sc_signal<sc_lv<32>> op1("operand1");
    sc_signal<sc_lv<32>> op2("operand2");
    sc_signal<sc_lv<32>> res("result");
    //sc_signal<sc_lv<32>>  res2("result_mux2");
    //sc_signal<sc_lv<32>>  res1 ("result_mux1");
    sc_signal<sc_lv<3>> flags("flags");
    sc_signal<bool> sel1("sel_mux1");
    sc_signal<sc_lv<2>> sel2("sel_mux2");
    sc_signal<sc_lv<32>> rs1("rs1_value");
    sc_signal<sc_lv<32>> PC_value("PC_value");
    sc_signal<sc_lv<32>> rs2("rs2_value");
    sc_signal<sc_lv<32>> I_imm("I_imm");
    sc_signal<sc_lv<32>> U_imm("U_imm");
    sc_signal<sc_lv<32>> S_imm("S_imm");

    scALU ALU("RISCV_ALU");
    mux4to1ALU mux2ALU ("Mux2_ALU");
    mux2to1ALU mux1ALU ("Mux1_ALU");

    ALU.selop(selop);
    ALU.op1(op1);
    ALU.op2(op2);
    ALU.res(res);
    ALU.flags(flags);

    mux1ALU.sel(sel1);
    mux1ALU.i1(PC_value);
    mux1ALU.i0(rs1);
    mux1ALU.res(op1);

    mux2ALU.sel(sel2);
    mux2ALU.i0(rs2);
    mux2ALU.i1(I_imm);
    mux2ALU.i2(U_imm);
    mux2ALU.i3(S_imm);
    mux2ALU.res(op2);

     sc_start();
     sc_trace_file *wf = sc_create_vcd_trace_file("ALU");

     sc_trace(wf,selop,"opsel");
     sc_trace(wf,op1,"op1");
     sc_trace(wf,op2,"op2");
     sc_trace(wf,res,"res");
     sc_trace(wf,sel1,"sel1");
     sc_trace(wf,sel2,"sel2");
     sc_trace(wf,rs2,"rs2");
     sc_trace(wf,I_imm,"I_imm");
     sc_trace(wf,U_imm,"U_imm");
     sc_trace(wf,S_imm,"S_imm");
     sc_trace(wf,rs1,"rs1");
     sc_trace(wf,PC_value,"PC_value");
     //sc_trace(wf,flags,"flags");

     sel1.write(1);
     sel2.write(1);
     rs2.write(1);
     I_imm.write(2147483650);
     U_imm.write(3);
     S_imm.write(4);
     rs1.write(5);
     PC_value.write(101);
     op1.write(10);
     op2.write(2);

     for (int i=0;i<2;i++) {

         selop.write(1);
         sc_start(10, SC_NS);
     }

     for (int i=0;i<2;i++) {

         selop.write(2);
         sc_start(10, SC_NS);
    }

     for (int i=0;i<2;i++) {

         selop.write(3);
         sc_start(10, SC_NS);
     }

     for (int i=0;i<2;i++) {

         selop.write(4);
         sc_start(10, SC_NS);
     }

     for (int i=0;i<2;i++) {

         selop.write(5);
         sc_start(10, SC_NS);
     }

     for (int i=0;i<2;i++) {

         selop.write(6);
         sc_start(10, SC_NS);
     }

     for (int i=0;i<2;i++) {

         selop.write(7);
         sc_start(10, SC_NS);
    }

     for (int i=0;i<2;i++) {

         selop.write(8);
         sc_start(10, SC_NS);
     }

     for (int i=0;i<2;i++) {

         selop.write(9);
         sc_start(10, SC_NS);
     }

     for (int i=0;i<2;i++) {

         selop.write(10);
         sc_start(10, SC_NS);
     }

     sc_close_vcd_trace_file(wf);
     return 0;
}


