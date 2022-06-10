/*!
 * \file main.cpp
 * \brief main function
 * \author Julie Mercier
 * \version 0.1
 * \date 13 May 2022
 *
 * launch the simulation
 */

#include <systemc.h>

int main(int argc, char *argv[])
{

    return sc_core::sc_elab_and_sim(argc, argv);

}
