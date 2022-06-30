#include <systemc.h>
#include "SlaveDecoder.h"
#include "mux2to1.h"
#include "master.h"
#include "Slave0.h"
#include "Slave1.h"

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool> clock;
    sc_signal<sc_lv<32>> PRDATA;
    sc_signal<bool> numSlave;
    sc_signal<bool> RST("reset");
    sc_signal<sc_lv<32>> PRDATA0;
    sc_signal<sc_lv<32>> PRDATA1;
    sc_signal<sc_lv<1>> PREADY;
    sc_signal<sc_lv<1>> PREADY0;
    sc_signal<sc_lv<1>> PREADY1;
    sc_signal<sc_lv<32>> PADDR;
    sc_signal<sc_lv<4>> PSTRB;
    sc_signal<sc_lv<1>> PWRITE;
    sc_signal<sc_lv<32>> PWDATA;
    sc_signal<sc_lv<1>> PENABLE;
    sc_signal<sc_lv<1>> PREQ;
    sc_signal<sc_lv<1>> PSEL;
    sc_signal<sc_lv<1>> PSEL0;
    sc_signal<sc_lv<1>> PSEL1;

    scMaster master("AMBA_master");
    scSlave0 slave0 ("slave0");
    scSlave1 slave1("slave1");
    mux2to1<32> mux0 ("Mux0");
    mux2to1<1> mux1 ("Mux_1");
    scDecoder decoder ("AMBA_decoder");

    decoder.address(PADDR);
    decoder.enable(PREQ);
    decoder.numSlave(numSlave);
    decoder.sel(PSEL);

    master.PCLK(clock);
    master.PRSTn(RST);
    master.PRDATA(PRDATA);
    master.PREADY(PREADY);
    master.PADDR(PADDR);
    master.PSTRB(PSTRB);
    master.PWDATA(PWDATA);
    master.PWDATA(PWDATA);
    master.PWRITE(PWRITE);
    master.PENABLE(PENABLE);
    master.PREQ(PREQ);

    slave0.PCLK(clock);
    slave0.PRSTn(RST);
    slave0.PADDR(PADDR);
    slave0.PSTRB(PSTRB);
    slave0.PWDATA(PWDATA);
    slave0.PWRITE(PWRITE);
    slave0.PENABLE(PENABLE);
    slave0.PSEL(PSEL0);
    slave0.PRDATA(PRDATA0);
    slave0.PREADY(PREADY0);

    slave1.PCLK(clock);
    slave1.PRSTn(RST);
    slave1.PADDR(PADDR);
    slave1.PSTRB(PSTRB);
    slave1.PWDATA(PWDATA);
    slave1.PWRITE(PWRITE);
    slave1.PENABLE(PENABLE);
    slave1.PSEL(PSEL1);
    slave1.PRDATA(PRDATA1);
    slave1.PREADY(PREADY1);

    mux0.sel(numSlave);
    mux0.i0(PRDATA0);
    mux0.i1(PRDATA1);
    mux0.res(PRDATA);

    mux1.sel(numSlave);
    mux1.i0(PREADY0);
    mux1.i1(PREADY1);
    mux1.res(PREADY);

     sc_start();
     sc_trace_file *wf = sc_create_vcd_trace_file("ControlUnit_tbSTORE");

     sc_trace(wf,clock,"clock");



     sc_close_vcd_trace_file(wf);
     return 0;
}
