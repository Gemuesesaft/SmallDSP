# SmallDSP
a small, modular and affordable Audio DSP Module

The goal of this Project is to create a line-up of small DSP modules, centred
around an Analog Devices ADAU1463/67.
This Chip is easy to program with Sigma Studio, a node based development 
environment and pretty powerful for its price.

Current status:
- Dual: Rev 1.1: Layout finished, functionality has to be validated.
- Maxi: Rev 1.0: Layout finished, ,prototype ordered, 
    functionality has to be validated.
- Multi: Rev 1.0: Schematic unfinished.
- Mini: Nothing done yet.
- Micro: Nothing done yet.
- Digi: Nothing done yet.


I've planed for 6 different modules:

- A compact one with two analogue input and output channels (SmallDSP Dual).
- A very small with only digital I2S IOs (SmallDSP Digi).
- A very small module with only one analogue input and output (SmallDSPMini).
- A similar than the Mini, but with and Microphone preamp (SmallDSP Micro). 
    Perhaps powered by phantom power.
- One with 16 analogue IOs (SmallDSP Multi).
- And one with 32 analogue IOs (SmallDSP Maxi).

There are three different versions of the used DSP Chip:
- The ADAU1463WBCPZ150. This is a slower version (max. 150MHz) with 48kWords 
    data memory and 16kWords program memory. Optimal for the smaller Boards
    like the Dual, Mini or Micro.
- The ADAU1463WBCPZ300. This is a faster version (max. 300MHz) with the same
    48kWords data memory and 16kWords program memory. Well suited for heavy
    processing or many channels, for example the Multi or Maxi.
- And the ADAU1467WBCPZ300. This is also a faster version (max. 300MHz) with
    more memory 80kWords data and 24kWords program memory. The expanded memory
    is useful if long delays on multiple channels are required.
All versions are pin-compatible, so one can use the version, best suited for 
the use case. 

The Dual, Digi, Mini and if phantom power isn't possible, the Micro, shall be
powered by a single output wall brick power supply. Both Multi and Maxi are 
using a +-5V power supply and feature a PE connection. They are therefore
designed for a metal enclosure such a 19" Rack enclosure. 

I try to maximize the components covered by JLCPCBs Basic Parts library, so
the boards can be manufactured cheaply and easily. Unfortunately this doesn't
cover all components, so there is still some hand soldering necessary. 

I tried to avoid the use of a Microcontroller, because the DSP can do pretty
much everything on itself. Although the DSP can act as a I2C or SPI master, I 
used ADCs and DAC that are configured directly by setting specific pins,
because the usage of the I2C/SPI master feature an ADAU DSPs is, at least
to my knowledge, not very user-friendly. Please correct me if I'm wrong there 
I would very much like to know an easy way to use this feature.
The Maxi is an exception in that, as it has a STM32 MCU on board. That is 
because the used Codecs are only controllable over I2C/SPI and are way cheaper
than pin-controllable alternatives.

A detailed documentation is planned, but the current focus lies on finishing
the PCBs and the testing of those. Detailed performance measurements will
be published here as well, as soon as they are made.