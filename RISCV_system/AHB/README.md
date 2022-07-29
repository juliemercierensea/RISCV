Files of the AHB folder :
  - testbench : gather the testbench written for the elements of AHB
  - AMBA.h : gather and bind all the elements of the design (slaves, decoder and master)
  - AMBA_Slave_InstructionMem.h : it's the first slave and it corresponds to the Intsruction Memory
  - AMBA_Slave1_dataMEM.h : it's the second slave, the data memory. This file is almost empty.
  - AMBA_SlaveDecoder.h : slave decoder that allows us to select the good slave and send the accurate signals for PREADY and PRDATA through multiplexers to the master (ie RV_1.h)
  - IHex.h : contains the code which give the hex file access
  - IHex.c : same as IHex.h
  - test1.hex : the hex file with the IMEM contents
  - test1.DISASM : content of the hex file but in disasembler language
