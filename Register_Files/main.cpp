/*!
 * \file main.cpp
 * \brief main function
 * \author Julie Mercier
 * \version 0.1
 * \date 4 May 2022
 *
 * main function : launch the simulation\n
 * \param[in] int argc \n
 * \param[in] char *argv[] \n
 *
 */
#include <systemc.h>

int main(int argc, char *argv[])
{
    return sc_core::sc_elab_and_sim(argc, argv);
}
