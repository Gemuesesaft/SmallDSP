# SmallDSP
a small, modular and affordable Audio DSP Module

The goal of this Project is to create a line-up of small DSP modules, centred
around an Analog Devices ADAU1463.
This Chip is easy to program with Sigma Studio, a node based development 
environment and pretty powerful for its price.

I've planed for 4 different modules:

- One relatively small with two analogue input and output channels (SmallDSP).
- One smaller with only digital IOs(SmallDSPDigi).
- A very small module with only one analogue input and output(SmallDSPMicro).
- And one with 16 analogue IOs(SmallDSPMulti).

On every module, besides the Micro, all the pins of the ADAU are connected to
pin headers, so that it can be used in other projects and its functionality can
be easily expanded.

I try to maximize the components covered by JLCPCBs Basic Parts library, so
the boards can be manufactured cheap and easy, but if done so, there is still 
some hand soldering necessary. 