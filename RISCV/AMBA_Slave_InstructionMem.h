#ifndef AMBA_SLAVE_INSTRUCTIONMEM_H
#define AMBA_SLAVE_INSTRUCTIONMEM_H
#include <systemc.h>
//#include "IHex.cpp"
#include "IHex.h"
#include "trace.h"

SC_MODULE (scIMEM) {

    // ---------------------      Ports      ---------------------

    sc_in_clk            PCLK{"clock_i"};
    sc_in<sc_lv<8>>      PADDR{"PADDR"};
    sc_in<sc_lv<4>>      PSTRB{"PSTRB"};
    sc_in<sc_lv<1>>      PENABLE{"PENABLE_super"};
    sc_in<sc_lv<2>>      PSEL{"PSEL"};

    sc_out<sc_lv<32>>    PRDATA{"PRDATA"};
    sc_out<sc_lv<1>>     PREADY{"PREADY"};

    sc_lv<32> contents [256]{"contents"};

    typedef scIMEM SC_CURRENT_USER_MODULE;
    scIMEM(const char *fname, ::sc_core::sc_module_name = "")
    {
        IHex::IHexFile ihexfile(fname);
        ihexfile.hasError();
        ihexfile.exportSystemC(0,256,contents);

        SC_THREAD(decodeHex);
        sensitive << PCLK.pos();
        sensitive << PADDR;
        sensitive << PSTRB;
        sensitive << PENABLE;
        sensitive << PSEL;

        wf= sc_create_vcd_trace_file("IMEM");
        sc_trace(wf,PCLK,"PCLK");
        sc_trace(wf,PADDR,"PADDR");
        sc_trace(wf,PSTRB,"PSTRB");
        sc_trace(wf,PENABLE,"PENABLE");
        sc_trace(wf,PSEL,"PSEL");
        sc_trace(wf,PRDATA,"PRDATA");
        sc_trace(wf,PREADY,"PREADY");
        }


    void decodeHex() {
        while (1){

            PREADY.write(PENABLE.read());

#if 1
            if((PSEL.read()&0b1)==1){
                PRDATA.write(0);

                if(PSTRB.read()==0b0){
                    PRDATA.write(0);
                }
                else{
                    if(PSTRB.read()==0b0001){
                        PRDATA.write((contents[PADDR.read().to_uint()])&0b11111111);
                    }
                    else{
                        if(PSTRB.read()==0b0010){
                            PRDATA.write((contents[PADDR.read().to_uint()])&0b1111111100000000);
                        }
                        else{
                            if(PSTRB.read()==0b0011){
                                PRDATA.write((contents[PADDR.read().to_uint()])&0b1111111111111111);
                            }
                            else{
                                if(PSTRB.read()==0b0100){
                                    PRDATA.write((contents[PADDR.read().to_uint()])&0b111111110000000000000000);
                                }
                                else{
                                    if(PSTRB.read()==0b0101){
                                        PRDATA.write((contents[PADDR.read().to_uint()])&0b111111110000000011111111);
                                    }
                                    else{
                                        if(PSTRB.read()==0b0110){
                                            PRDATA.write((contents[PADDR.read().to_uint()])&0b111111111111111100000000);
                                        }
                                        else {
                                            if(PSTRB.read()==0b0111){
                                                PRDATA.write((contents[PADDR.read().to_uint()])&0b111111111111111111111111);
                                            }
                                            else{
                                                if(PSTRB.read()==0b1000){
                                                    PRDATA.write((contents[PADDR.read().to_uint()])&0b11111111000000000000000000000000);
                                                }
                                                else{
                                                    if(PSTRB.read()==0b1001){
                                                        PRDATA.write((contents[PADDR.read().to_uint()])&0b11111111000000000000000011111111);
                                                    }
                                                    else{
                                                        if(PSTRB.read()==0b1010){
                                                            PRDATA.write((contents[PADDR.read().to_uint()])&0b11111111000000001111111100000000);
                                                        }
                                                        else{
                                                            if(PSTRB.read()==0b1011){
                                                                PRDATA.write((contents[PADDR.read().to_uint()])&0b11111111000000001111111111111111);
                                                            }
                                                            else{
                                                                if(PSTRB.read()==0b1100){
                                                                    PRDATA.write((contents[PADDR.read().to_uint()])&0b11111111111111110000000000000000);
                                                                }
                                                                else{
                                                                    if(PSTRB.read()==0b1101){
                                                                        PRDATA.write((contents[PADDR.read().to_uint()])&0b11111111111111110000000011111111);
                                                                    }
                                                                    else{
                                                                        if(PSTRB.read()==0b1110){
                                                                            PRDATA.write((contents[PADDR.read().to_uint()])&0b11111111111111111111111100000000);
                                                                        }
                                                                        else{
                                                                            if(PSTRB.read()==0b1111){
                                                                                PRDATA.write(contents[PADDR.read().to_uint()]);
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }


#endif
        wait();
        }
    }

};


#endif // AMBA_SLAVE_INSTRUCTIONMEM_H
