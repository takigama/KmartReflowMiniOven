# KMART REFLOW MINI OVEN

its only just started and the code is ugly as sin.

The goal is to produce a codebase that might be useful for any reflow oven where:
 - heat control is done via SSR (100hz cycle)
 - Temperature via a thermistor (similar to 3d printer)
 - Lilygo t-display controller (with a daughter board), but this might be changed later for something more "modular"

## General Thoughts...

Much of the code will be similar to 3d printers because alot of the features are similar, plus to make it module it
sort of makes sense to use the 3d printer board base as it already has all the modularity for things like tft screens, 
thermistors, etc.

## TODO
 - the web interface (im terrible at making pretty ui's)
 - design the daughter board for the lilygo t-display to 4 temperature sensors and 2 SSR controls

## DONE
 - add wifimanager - done