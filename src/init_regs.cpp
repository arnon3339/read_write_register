#include "init_regs.hpp"
#include "pru_register.hpp"

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* init global register *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
void    init_globreg(pru_register *reg)
{
    reg[0].name    = "date_code";              reg[1].name    = "hash_code";
    reg[2].name    = "led_blinky";             reg[3].name    = "reset_bank_bitslice";
    reg[4].name    = "enable_alpide_clock";    reg[5].name    = "ru_id";
    reg[6].name    = "max_size";               reg[7].name    = "max_wait_time";
    reg[8].name    = "filter_data_word";       reg[9].name    = "empty_frame_compression";
    reg[10].name   = "check_id";               reg[11].name   = "DPA_delta";
    reg[12].name   = "tabs";

    reg[0].addr    = 0x00;  reg[1].addr    = 0x04;
    reg[2].addr    = 0x08;  reg[3].addr    = 0x0C;
    reg[4].addr    = 0x10;  reg[5].addr    = 0x14;
    reg[6].addr    = 0x18;  reg[7].addr    = 0x1C;
    reg[8].addr    = 0x20;  reg[9].addr    = 0x24;
    reg[10].addr   = 0x28;  reg[11].addr   = 0x2C;
    reg[12].addr   = 0x30;

    reg[0].length    = 0x20;  reg[1].length    = 0x20;
    reg[2].length    = 0x01;  reg[3].length    = 0x05;
    reg[4].length    = 0x01;  reg[5].length    = 0x06;
    reg[6].length    = 0x20;  reg[7].length    = 0x20;
    reg[8].length    = 0x02;  reg[9].length    = 0x10;
    reg[10].length   = 0x01;  reg[11].length   = 0x07;
    reg[12].length   = 0x09;
    
}

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* init offload register *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
void    init_offld(pru_register *reg)
{
    reg[0].name    = "enable_offload";          reg[1].name    = "reset_offload_fifo";
    reg[2].name    = "frame_based_offload";     reg[3].name    = "idle";
    reg[4].name    = "pDTP_TX_status";          reg[5].name    = "test_mode";
    reg[6].name    = "num_test_words";          reg[7].name    = "num_wait";
    reg[8].name    = "tlast_threshold";         reg[9].name    = "assert_tlast_when_empty";
    reg[10].name   = "flush_buffer";               

    reg[0].addr    = 0x00;  reg[1].addr    = 0x04;
    reg[2].addr    = 0x08;  reg[3].addr    = 0x0C;
    reg[4].addr    = 0x10;  reg[5].addr    = 0x14;
    reg[6].addr    = 0x18;  reg[7].addr    = 0x1C;
    reg[8].addr    = 0x20;  reg[9].addr    = 0x24;
    reg[10].addr   = 0x28;

    reg[0].length    = 0x01;  reg[1].length    = 0x01;
    reg[2].length    = 0x01;  reg[3].length    = 0x01;
    reg[4].length    = 0x1C;  reg[5].length    = 0x20;
    reg[6].length    = 0x20;  reg[7].length    = 0x20;
    reg[8].length    = 0x20;  reg[9].length    = 0x01;
    reg[10].length   = 0x01; 
}

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* init trigger manager *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
void    init_trgg(pru_register *reg)
{
    reg[0].name     = "status";              reg[1].name    = "mode";
    reg[2].name     = "num_triggers";        reg[3].name    = "pre_cmd_delay";
    reg[4].name     = "trigger_delay";       reg[5].name    = "pulse_delay";
    reg[6].name     = "pulse_trigger_delay"; reg[7].name    = "trigger_init";
    reg[8].name     = "num_trains";          reg[9].name    = "trains_delay";
    reg[10].name    = "alpide_grst";         reg[11].name   = "alpide_prst";
    reg[12].name    = "alpide_bcrst";        reg[13].name   = "alpide_rorst";
    reg[14].name    = "trigger_source";     reg[15].name    = "alpide_mode";
    reg[16].name    = "absolute_time";      reg[17].name    = "reset_time";
    reg[18].name    = "spill_id";           reg[19].name    = "increment_spill_id";
    reg[20].name    = "reset_spill_id";       
    

    reg[0].addr     = 0x00;  reg[1].addr    = 0x04;
    reg[2].addr     = 0x08;  reg[3].addr    = 0x0C;
    reg[4].addr     = 0x10;  reg[5].addr    = 0x14;
    reg[6].addr     = 0x18;  reg[7].addr    = 0x1C;
    reg[8].addr     = 0x20;  reg[9].addr    = 0x24;
    reg[10].addr    = 0x28;  reg[11].addr   = 0x2C;
    reg[12].addr    = 0x30;  reg[13].addr   = 0x34;
    reg[14].addr    = 0x38;  reg[15].addr   = 0x3C;
    reg[16].addr    = 0x40;  reg[17].addr   = 0x44;
    reg[18].addr    = 0x48;  reg[19].addr   = 0x4C;
    reg[20].addr    = 0x50;

    reg[0].length       = 0x02;  reg[1].length      = 0x02;
    reg[2].length       = 0x20;  reg[3].length      = 0x20;
    reg[4].length       = 0x20;  reg[5].length      = 0x20;
    reg[6].length       = 0x20;  reg[7].length      = 0x01;
    reg[8].length       = 0x20;  reg[9].length      = 0x20;
    reg[10].length      = 0x01;  reg[11].length     = 0x01;
    reg[12].length      = 0x01;  reg[13].length     = 0x01;
    reg[14].length      = 0x02;  reg[15].length     = 0x01;
    reg[16].length      = 0x20;  reg[17].length     = 0x01;
    reg[18].length      = 0x10;  reg[19].length     = 0x01;
    reg[20].length      = 0x01;
}