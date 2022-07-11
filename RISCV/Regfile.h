#ifndef REGFILE_H
#define REGFILE_H

/*!
 * \file Regfile.h
 * \brief gather all the elements needed to define the register
 * \author Julie Mercier
 * \version 0.1
 * \date 2 May 2022
 *
 * Register File sensitive to the rising edges of the clock.\n
 * - ** INPUTS**
 *
 *   **clock** :Falling-edge active clock input \n
 *   **reset** :Active-high synchronous reset input\n
 *   **w_address, rx_address and ry_address** : 5 bits signal (input) because it is all it takes to count until 32\n
 *   **w_data** : 32 bit data input: the data we write in the registers\n
 *   **w_command,rx_command and ry_command** : 1 bit signals to allow the writting (1) or not (0)\n
 *   **rx and ry** : 32 bit data output : the content of register rx and ry\n
 *   **Local variable** : value : array of 32 with the internal value 'value'\n
 *   \n
 * - **METHOD SC_THREAD** sensitive to reset and positive edges of the clock\n
 * - **function void store()** : the function that defines the scRegister logic\n
 *                      Works with an infinite loop and an a wait \n
 *                      **first** : **if** the reset is active at every edge of the clock : for loop to reset all value[i]=0 \n
 *                      **If not** : if commands are true and that the register's address is not 0 (because no writing allowed in R0),
 *         load the corresponding value in wdata and write it in rx or ry if rxcommand and rycommand are set to 1. \n
 *                      **Else**,
 *                      The register takes the value 0.
 */

#include <systemc.h>


SC_MODULE (scRegfile) {
    sc_in_clk           clock{"clock"};
    sc_in<bool>         reset{"reset_i"};
    sc_in<sc_lv<5>>     WAdd{"w_address_i"};
    sc_in<sc_lv<32>>    WVal{"w_data_i"};
    sc_in<sc_lv<1>>     WCmd{"w_command_i"};
    sc_in<sc_lv<5>>     R0Add{"rx_address_i"};
    //sc_in<sc_lv<1>>     rx_command{"rx_command_i"};
    sc_in<sc_lv<5>>     R1Add{"ry_address_i"};
    //sc_in<sc_lv<1>>     ry_command{"ry_command_i"};

    sc_out<sc_lv<32>>   R0Val{"R0_val"};
    sc_out<sc_lv<32>>   R1Val{"R1_val"};

    sc_lv<32>  value[32]{"value"};

    SC_CTOR(scRegfile) {
        SC_THREAD(store);

        sensitive << reset;
        sensitive << clock.pos();
    }
    void store () {
        while(1){

            if (reset.read() == true) {

                for (int i=0;i<=31; i++){
                    value[i]=0;
                }
            } else {
                if (WCmd.read() == true) {
                    value[WAdd.read().to_int()] = WVal.read();
                }
                if (R0Add.read().to_int()!=0) {
                    R0Val.write(value[R0Add.read().to_int()]);
                } else {
                    R0Val.write(0);
                }
                if (R1Add.read().to_int()!=0) {
                    R1Val.write(value[R1Add.read().to_int()]);
                } else {
                    R1Val.write(0);
                }
            }
            wait();
        }
    }

};

#endif // REGFILE_H
