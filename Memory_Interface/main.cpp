#include <systemc.h>
#include "IHex.h"

int main(int argc, char *argv[])
{
    sc_lv<32> contents [256];

    IHex::IHexFile ihexfile ("test1.hex");
    ihexfile.hasError();
    ihexfile.exportSystemC(0,256,contents);
    return sc_core::sc_elab_and_sim(argc, argv);

}
