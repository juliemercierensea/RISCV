/*!
 * \file ALU.h
 * \brief Arithmetic Logic Unit of the RISC-V design
 * \author Julie Mercier
 * \version 0.1
 * \date 13 May 2022
 *
 * ALU design with :\n
 *      - 3 inputs signals : op1, op2 and selop (selects the operation to be performed. Alternative 2 with 11 different operations)\n
 *      - 2 output signals : res (result of the operation between op1 and op2) and flags (implementation 2)\n
 * NB : for the simulation, the SC_THREAD mustbe sensitive to all the input signals or it will never update \n
 *\n
 * **fonction** void operations ():\n
 *      - Defines the kind of operation to be performed according to the value of selop, writes the **result in res** and produces flags.\n
 *      - There are 10 working operations : +, - (signed or unsigned), bitwise operations (^ | &), op1, op2, right and left shift on op1 where the shift amount is the less 5 bits of op2 \n
 */

/*! \bug There is no difference between the signed and unsigned operation while there should be : the result is the same no matter the values of op1 and op2 \n
 */

/*! \todo
 *        - Add the code corresponding to the flags Z, N and C \n
 *        - Find a way to code the shift "<<*"\n
 */

#ifndef ALU_H
#define ALU_H

#include <systemc.h>

SC_MODULE (scALU) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<32>>    op1{"operand1_i"};
    sc_in<sc_lv<32>>    op2{"operand2_i"};
    sc_in<sc_lv<4>>    selop{"opselect_i"};

    sc_out<sc_lv<32>>    res{"result_data_o"};
    sc_out<sc_lv<3>>    flags{"comp_branches_o"};

    SC_CTOR(scALU) {
        SC_THREAD(operations);
        sensitive << op1;
        sensitive << op2;
        sensitive << selop;
    }

    void operations() {
        while (1){

        if (selop.read()==1){ //+ operation

            res.write((op1.read().to_uint64())+(op2.read().to_uint64()));
                        }

        if (selop.read()==2){ // -(signed) operation
            res.write((op1.read().to_int64()) - (op2.read().to_int64()));
            //flags.write(0); //Correct ?
            }

        if (selop.read()==3){ // -(unsigned) operation
            res.write((op1.read().to_uint64()) - (op2.read().to_uint64()));
            //flags.write(0);
            }

        if (selop.read()==4){ // & operation
            res.write((op1.read().to_uint64()) & (op2.read().to_uint64()));
                        }

        if (selop.read()==5){ // | operation
            res.write((op1.read().to_uint64()) | (op2.read().to_uint64()));
                        }

        if (selop.read()==6){ // ^ operation
            res.write((op1.read().to_uint64()) ^ (op2.read().to_uint64()));
                        }

        if (selop.read()==7){ // operand1 operation
            res.write(op1.read());
                        }

        if (selop.read()==8){ // operand2 operation
            res.write(op2.read());
                        }

        if (selop.read()==9){ // << operation
            res.write(op1.read().to_uint64()<<(op2.read().to_uint64()& 0x001F));
                        }

        if (selop.read()==10){ // >> operation
            res.write(op1.read().to_uint64()>>(op2.read().to_uint64()& 0x001F));
                        }
        wait();
        }
     }
};
#endif // ALU_H
