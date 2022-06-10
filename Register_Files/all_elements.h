/*!
 * \file all_elements.h
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

#ifndef ALL_ELEMENTS_H
#define ALL_ELEMENTS_H

#include <systemc.h>


SC_MODULE (scRegister) {
    sc_in_clk           clock{"clock"};
    sc_in<bool>         reset{"reset_i"};
    sc_in<sc_lv<5>>    w_address{"w_address_i"};
    sc_in<sc_lv<32>>    w_data{"w_data_i"};
    sc_in<sc_lv<1>>     w_command{"w_command_i"};
    sc_in<sc_lv<5>>    rx_address{"rx_address_i"};
    sc_in<sc_lv<1>>     rx_command{"rx_command_i"};
    sc_in<sc_lv<5>>    ry_address{"ry_address_i"};
    sc_in<sc_lv<1>>     ry_command{"ry_command_i"};

    sc_out<sc_lv<32>>   rx{"rx_o"};
    sc_out<sc_lv<32>>   ry{"ry_o"};

    sc_lv<32>  value[32]{"value"};

    SC_CTOR(scRegister) {
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
                if (w_command.read() == true) {
                    value[w_address.read().to_int()] = w_data.read();
                }
                if ((rx_command.read() == true) && (rx_address.read().to_int()!=0)) {
                    rx.write(value[rx_address.read().to_int()]);
                } else {
                    rx.write(0);
                }
                if ((ry_command.read() == true) && (ry_address.read().to_int()!=0)) {
                    ry.write(value[ry_address.read().to_int()]);
                } else {
                    ry.write(0);
                }
            }
            wait();
        }
    }

};

#endif // ALL_ELEMENTS_H
