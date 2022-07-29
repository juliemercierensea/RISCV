Contains : 
  - an excel file with the instruction number table (with the detail of the value of the bits for func3, func12 and func7)
  - testbench
  - AddressCounter.h : counts the address
  - InstrNumber.h : check the value of opcode_6to2, func12_0, func7_5 and func3 in order to write the address of the instruction to be executed in the result INUM
  - Instruction.h : create opcode, func3, func12 and func7 from the instruction signal
  - ROMData_to_input.h : uses the output of ROMmem to produce signals linked to the interesting bits of the ROM data memory.
  - ROMmem.h : gives the value of the ROM memory according to the input (the address)
  - xmlucrom files and configengine files : needed to access the xml file

