// See LICENSE for license details.

//-------------------------------------------------
// Define the features in simulatio/fpga/asic
//-------------------------------------------------

`ifndef CHIP_CONFIG_VH
 `define CHIP_CONFIG_VH

 // For some known cases
 `ifdef VERILATOR_SIM
  // when verilator is used for behavioural simulation
  `define SIMULATION
 `endif

 `ifdef VCS
  // when Synopsys VCS is used for behavioural simulation
  `define SIMULATION
 `endif

 `ifdef INCA
  // when Cadence NCSIM is used for behavioural simulation
  `define SIMULATION
 `endif

 `ifdef MODEL_TECH
  // when Mentor Graphic Modelsom is used for behavioural simulation
  `define SIMULATION
 `endif

 // The following should be indicated but can be directly enabled
 // SIMULATION            : simulation
 // FPGA                  : FPGA implementation
 // ASIC                  : ASIC implementation

 // FPGA_FULL             : simulation/implement very ip of FPGA

 //----------- Detailed configuration -------------//

 `define LOWRISC_IO_DAT_WIDTH 32

 `ifdef FPGA

  `ifdef FPGA_FULL
   `define ADD_PHY_DDR
  `endif

  `ifdef NEXYS4_VIDEO
   `define NEXYS4_COMMON
  `endif

  `ifdef NEXYS4
   `define NEXYS4_COMMON
  `endif

  //`define ADD_MINION_SD

 `endif //  `ifdef FPGA

`endif //  `ifndef CHIP_CONFIG_VH

