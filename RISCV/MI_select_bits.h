#include <systemc.h>
#ifndef MI_SELECT_BITS_H
#define MI_SELECT_BITS_H

SC_MODULE (scSel) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<8>>         bytestrb{"bytestrb"};
    sc_out<sc_lv<4>>        bytestrb_3to0{"bytestrb_3to0"};
    sc_out<sc_lv<4>>        bytestrb_7to4{"bytestrb_7to4"};

    sc_in<sc_lv<8>>         reg_bytestrb_output{"reg_bytestrb_output"};
    sc_out<sc_lv<4>>        reg_bytestrb_output_3to0{"reg_bytestrb_output_3to0"};
    sc_out<sc_lv<4>>        reg_bytestrb_output_7to4{"reg_bytestrb_output_7to4"};

    sc_in<sc_lv<64>>        wdata64{"wdata64"};
    sc_out<sc_lv<32>>       wdata64_31to0{"wdata64_31to0"};

    sc_in<sc_lv<32>>        addr_i{"addr_i"};
    sc_out<sc_lv<30>>       wordaddr{"wordaddr"};
    sc_out<sc_lv<2>>        alignment{"alignment"};

    sc_in<sc_lv<32>>        rdata64b_63to32{"rdata64b_63to32"};
    sc_in<sc_lv<32>>        rdata64b_31to0{"rdata64b_1to0"};
    sc_out<sc_lv<64>>       rdata64b{"rdata64b"};
    sc_lv<64>               pre{"pre"};

    sc_in<sc_lv<32>>        in0{"in0"};
    sc_in<sc_lv<64>>        in1{"in1"};
    sc_in<sc_lv<64>>        in2{"in2"};
    sc_in<sc_lv<64>>        in3{"in3"};
    sc_in<sc_lv<64>>        in4{"in4"};
    sc_in<sc_lv<32>>        in5{"in5"};

    sc_out<sc_lv<32>>       out0{"out0"};
    sc_out<sc_lv<32>>       out1{"out1"};
    sc_out<sc_lv<32>>       out2{"out2"};
    sc_out<sc_lv<32>>       out3{"out3"};
    sc_out<sc_lv<32>>       out4{"out4"};
    sc_out<sc_lv<16>>       out6{"out6"};
    sc_out<sc_lv<8>>        out5{"out5"};

    sc_in<sc_lv<30>>    paddr30{"paddr30"};
    sc_out<sc_lv<32>>   paddr32{"paddr32"};


    SC_CTOR(scSel) {
        SC_THREAD(select);
        sensitive<<bytestrb;
        sensitive<<reg_bytestrb_output;
        sensitive<<wdata64;
        sensitive<<addr_i;
        }

    void select() {
        while (1){
            bytestrb_3to0.write(bytestrb.read().to_uint()& 0b1111);
            bytestrb_7to4.write((bytestrb.read().to_uint()&0b11110000)>>0x4);

            reg_bytestrb_output_3to0.write((reg_bytestrb_output.read().to_uint())& 0b1111);
            reg_bytestrb_output_7to4.write((reg_bytestrb_output.read().to_uint()& 0b11110000)>>0x4);

            wdata64_31to0.write(wdata64.read().to_uint()& 0b11111111111111111111111111111111);

            wordaddr.write(((addr_i.read().to_uint())&0b11111111111111111111111111111110)>>0x2);
            alignment.write(addr_i.read().to_uint()&0b11);

            pre=rdata64b_63to32.read();
            pre=pre<<0b100000;          //vérifier que ça fait bien l'opération souhaitée
            rdata64b.write(pre|rdata64b_31to0.read());

            out0.write(in0.read()); //rdata_unbuff_o

            out1.write(in1.read()&0b11111111111111111111111111111111);
            out2.write(in2.read()&0b11111111111111111111111111111111);
            out3.write(in3.read()&0b11111111111111111111111111111111);
            out4.write(in4.read()&0b11111111111111111111111111111111);

            out5.write(in5.read()&0b11111111); //muxtoextend_7to0
            out6.write(in5.read()&0b1111111111111111); //muxtoextend_15to0

            paddr32.write(paddr30.read());

        wait();
        }
    }
};

#endif // MI_SELECT_BITS_H
