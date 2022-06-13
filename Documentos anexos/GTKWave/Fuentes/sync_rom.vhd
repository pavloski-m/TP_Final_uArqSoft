library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity sync_rom is
    generic (
        data_width : natural := 16;
        addr_length : natural := 4);
    port (
        clk :in std_logic;
        ena_fil: out std_logic;
        data_out:out std_logic_vector(data_width-1 downto 0)
    );
end sync_rom;

architecture synth of sync_rom is
    
    constant mem_size : natural := 2**addr_length;
    type mem_type is array (mem_size-1 downto 0) of std_logic_vector (data_width-1 downto 0);
    
    constant mem : mem_type :=
        (0 => X"0000" ,
        1 => X"348e" ,
        2 => X"611b" ,
        3 => X"7ee0" ,
        4 => X"8955" ,
        5 => X"7ee0" ,
        6 => X"611b" ,
        7 => X"348e" ,
        8 => X"0000" ,
        9 => X"348e" ,
        10 => X"611b" ,
        11 => X"7ee0" ,
        12 => X"8955" ,
        13 => X"7ee0" ,
        14 => X"611b" ,
        15 => X"348e");


begin

    
    rom : process (clk)
    variable i: natural := 0;
    variable j: natural := 0;
    begin
        if rising_edge(clk) then  
               
            if (3 = j) then
                ena_fil <= '1';
                data_out <=mem(i);
                if i = 15 then      -- 15 hardcodeado porque no es el largo de la rom si no el período de la señal
                    i := 0;
                else
                    i := i + 1;
                end if;
                j:=0;
            else
                ena_fil <= '0';
            end if;
           
        j:=j+1;            
        end if;
    end process rom;

end architecture synth;
