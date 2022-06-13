library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity FilAvg is
    generic (
        sig_w : integer := 16;
        fil_w : integer := 8
    );
    port (
        clk     : in std_logic;
        a_rst   : in std_logic;
        s_rst   : in std_logic;
        ena     : in std_logic;
        sigin   : in std_logic_vector(sig_w-1 downto 0);
        sigout  : out std_logic_vector(sig_w-1 downto 0)
    );
end entity;

architecture FilAvg_arq of FilAvg is
    
    type buff_fil is array (0 to fil_w-1) of unsigned(sig_w-1 downto 0); -- buffer del filtro
    signal fil_buff : buff_fil;
    signal en_reg: std_logic;

    function bitadd (data_w: natural) return natural is
    begin
        for index in 1 to (31-sig_w) loop    -- planteo un máximo de 32 bits
            if (data_w <= 2**index) then
                return index;
            end if;
        end loop;
        return (32-sig_w);
    end function;

begin

filtrar: process (clk, a_rst)
    variable sum_aux : unsigned(sig_w+bitadd(fil_w)-1 downto 0);
    variable en_reg_aux : std_logic := '0';
    variable en_reg_aux2 : std_logic := '0';

begin
    if (a_rst = '1') then
        en_reg <= '0';
        fil_buff <= (others => (others => '0'));
        sum_aux := (others => '0');
        sigout <= (others => '0');
    elsif rising_edge(clk) then
        if (s_rst = '1') then
            en_reg <= '0';
            fil_buff <= (others => (others => '0'));
            sum_aux := (others => '0');
            sigout <= (others => '0');
        else
            en_reg <= ena;
            en_reg_aux2 := en_reg_aux;
            en_reg_aux := en_reg;
            
            if (not en_reg_aux2 and en_reg_aux)='1' then
                fil_buff(0) <= unsigned(sigin);
                for i in 1 to fil_w-1 loop
                    fil_buff(i) <= fil_buff(i-1);
                end loop;
                sum_aux := (others => '0');
                for i in 0 to fil_w-1 loop
                    sum_aux := sum_aux + resize(fil_buff(i), sum_aux'length);
                end loop;
            sigout <= std_logic_vector(sum_aux(sig_w+bitadd(fil_w)-1 downto bitadd(fil_w)));
            end if;
        end if;
    end if;
end process;
end architecture;

