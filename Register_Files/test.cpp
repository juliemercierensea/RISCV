/*!
 * \file test.cpp
 * \brief testbench
 * \author Julie Mercier
 * \version 0.1
 * \date 4 May 2022
 *
 * testbench for the Register File.
 */
/*!
 * \fn int sc_main (int argc, char *argv[])\n
 *         function called by the main : create the design and run the simulation, produce the vcd file\n
 *         - first : definition of all the signals and link to the ports\n
 *         - second : start the simulation and  create the vcd files \n
 *         - third : **initialization** : we have to test first if the program works as expected when reset is set to 0 and when it is set to 1,
 *         the value of wdata is the data we want to write in the selected register \n
 *         \n
 *         **wadd** is the address of the register we will write in (rx or ry but we can't write in both register in the same time) \n
 *         **ryadd** and ryadd are the addresses of the resgiters rx and ry. If their address doesn't match the wadd, nothing changes.\n
 *         \n
 *         - 1) no permission at all : **if command is set to 0**, nothing should change \n
 *         - 2) wcommand =1 : we can write \n
 *         - 3) rxcommand =1 : we can write in rx. **If rxcommand is set to 0**, we can't write anything in rx event if wcommand is set to 1 \n
 *         - 4) rycommand =1 : we can write in ry. **If rycommand is set to 0**, we can't write anything in ry event if wcommand is set to 1 \n
 *
 *  \return 0
 */

#include <systemc.h>
#include "all_elements.h"


int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool>   clock;
    sc_signal<bool> RST("RST");
    sc_signal<sc_lv<5>> wadd("Write_address");
    sc_signal<sc_lv<32>> wdata("Write_data");
    sc_signal<sc_lv<1>> wcommand("Write_command");
    sc_signal<sc_lv<5>> rxadd("rx_address");
    sc_signal<sc_lv<1>> rxcommand("rx_command");
    sc_signal<sc_lv<5>> ryadd("ry_address");
    sc_signal<sc_lv<1>> rycommand("rycommand");

    sc_signal<sc_lv<32>> rxout("rxout");
    sc_signal<sc_lv<32>> ryout("ryout");

    scRegister REGFILE("RISCV_Regfile");

    REGFILE.clock(clock);
    REGFILE.reset(RST);
    REGFILE.w_address(wadd);
    REGFILE.w_data(wdata);
    REGFILE.w_command(wcommand);
    REGFILE.rx_address(rxadd);
    REGFILE.rx_command(rxcommand);
    REGFILE.ry_address(ryadd);
    REGFILE.ry_command(rycommand);
    REGFILE.rx(rxout);
    REGFILE.ry(ryout);

    /*!
     *  sc_start()
     *  \brief  start the simulation, run forever
     */
    sc_start();

    /*!
     * sc_trace_file()
     * \brief  Create the traces for GTKwave ie the vcd file
     */
     sc_trace_file *wf = sc_create_vcd_trace_file("Register");

     /*!
      * sc_trace()
      * \brief  Dump the desired signal (name of the signal desired signals as a parameter)
      */
     sc_trace(wf, clock, "clock");
     sc_trace(wf, RST, "reset");
     sc_trace(wf, wadd, "w_address");
     sc_trace(wf, wdata, "w_data");
     sc_trace(wf, wcommand, "w_command");
     sc_trace(wf, rxadd, "rx_address");
     sc_trace(wf, rxcommand, "rx_command");
     sc_trace(wf, ryadd, "ry_address");
     sc_trace(wf, rycommand, "ry_command");
     sc_trace(wf, ryout, "ry");
     sc_trace(wf, rxout, "rx");

     RST.write(0);
     wdata.write(11);
     wadd.write(1);
     rxadd.write(1);
     ryadd.write(1);

     /*
      * no permission at all : if command is set to 0, nothing should change
      */

         for (int i=0;i<3;i++) {
             clock=0;
             sc_start(10, SC_NS);
             clock =0;
             sc_start(10, SC_NS);
             wcommand.write(0);
             rxcommand.write(0);
             rycommand.write(0);
         }

     /*
      * wcommand =1 : we can write
      */

     for (int i=0;i<3;i++) {
         clock = 0;
         sc_start(10, SC_NS);
         clock = 1;
         sc_start(10, SC_NS);
         wcommand.write(1);
     }
     /*
      * rxcommand =1 : we can write in rx. If rxcommand is set to 0, we can't write anything in rx event if wcommand is set to 1
      */

     for (int i=0;i<3;i++) {
         clock = 0;
         sc_start(10, SC_NS);
         clock = 1;
         sc_start(10, SC_NS);
         rxcommand.write(1);
    }
     /*
      * rycommand =1 : we can write in ry.If rycommand is set to 0, we can't write anything in ry event if wcommand is set to 1
      */

     for (int i=0;i<3;i++) {
         clock = 0;
         sc_start(10, SC_NS);
         clock = 1;
         sc_start(10, SC_NS);
         rycommand.write(1);
     }

     /*!
      *  sc_close_vcd_trace_file()
      *  \brief  stop the simulation
      */

     sc_close_vcd_trace_file(wf);
    return 0;
}


