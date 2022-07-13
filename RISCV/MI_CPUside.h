#include <systemc.h>
#ifndef MI_CPUSIDE_H
#define MI_CPUSIDE_H

SC_MODULE (scCPU) {
    sc_in<sc_lv<32>>    rs2_value{"rs2_value"};
    sc_out<sc_lv<32>>   wdata_i{"wdata_i"};
    sc_in<sc_lv<3>>     func3{"func3"};
    sc_out<sc_lv<2>>    size_i{"size_i"};
    sc_out<sc_lv<1>>    unsigned_i{"unsigned_i"};

    SC_CTOR(scCPU) {
        sensitive <<rs2_value;
        sensitive << func3;
        }

    void link() {
        while(1){

            wdata_i.write(rs2_value.read());
            size_i.write(func3.read()&0b11);
            unsigned_i.write(func3.read()&0b100);

            wait();
        }
    }
};

#endif // MI_CPUSIDE_H
