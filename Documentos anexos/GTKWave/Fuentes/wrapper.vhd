library ieee;
use ieee.std_logic_1164.all;

entity filtro is
end entity;

architecture wrapper_arq of filtro is
    component sync_rom is
        generic (
            data_width : natural := 16
            );
        port (
            clk :in std_logic;
            ena_fil: out std_logic;
            data_out:out std_logic_vector(data_width-1 downto 0)
        );
    end component;

    constant data_width_aux: natural := 16;
    signal sig_clk : std_logic := '0';
    signal sig_data_out: std_logic_vector(data_width_aux-1 downto 0);

    component FilAvg is
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
    end component;

    constant sig_w_aux: natural := 16;
    constant fil_w_aux: natural := 8;
    signal sig_a_rst: std_logic := '0';
    signal sig_s_rst: std_logic := '0';
    signal sig_ena: std_logic := '0';
    signal sig_sigout: std_logic_vector(sig_w_aux-1 downto 0);

begin

    sig_clk <= not sig_clk after 10 ns; 
    --sig_ena <= '1' after 30 ns;
    sig_a_rst <= '1' after 1000 ns, '0' after 1020 ns;
    sig_s_rst <= '1' after 2000 ns, '0' after 2020 ns;

    generador: sync_rom
        generic map (
            data_width => data_width_aux
        )
        port map (
            clk => sig_clk,
            ena_fil => sig_ena,
            data_out => sig_data_out
        );
    
    FilAvg_1: FilAvg
        generic map (
            sig_w => sig_w_aux,
            fil_w => fil_w_aux
        )
        port map (
            clk     => sig_clk,
            a_rst   => sig_a_rst,
            s_rst   => sig_s_rst,
            ena     => sig_ena,
            sigin   => sig_data_out,
            sigout  => sig_sigout
        );
    
end architecture;