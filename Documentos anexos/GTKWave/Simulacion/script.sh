ghdl -a ../Fuentes/sync_rom.vhd ../Fuentes/FilAvg.vhd ../Fuentes/wrapper.vhd
ghdl -s ../Fuentes/sync_rom.vhd ../Fuentes/FilAvg.vhd ../Fuentes/wrapper.vhd
ghdl -e filtro
ghdl -r filtro --vcd=filtro.vcd --stop-time=50000ns

gtkwave filtro.vcd
