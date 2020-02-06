# Read and write pRU register by TCP transmission

![alt text](figure/800px-Bus_tree.png)

|         Module        | Base address  |  Module range |   Total address space     |     Num moduels   |   ID range    |
|:---------------------:|:-------------:|:-------------:|:-------------------------:|:-----------------:|:-------------:|
|   global_regs         |   0x20000000  | 0x000 - 0xFFF |  0x20000000 - 0x20000FFF  |       1           |       0       |
|   offload             |   0x20001000  | 0x000 - 0xFFF |  0x20001000 - 0x20004FFF  |       4           |     0 - 3     |
|   trigger_manager     |   0x24220000  | 0x000 - 0xFFF |  0x24220000 - 0x24220FFF  |       1           |       0       |
|   alpide_control      |   0x24110000  | 0x000 - 0xFFF |  0x24110000 - 0x2411BFFF  |       12          |     0 - 11    |
|   alpide_data         |   0x22000000  | 0x000 - 0xFFF |  0x22000000 - 0x2206BFFF  |       108         |     0 - 107   |

## build project
    mkdir build
    cd build
    cmake ..
    make

## run program
    ./registers_rw
### read option
> The previous run program is set to read all modules (global register, offload, trigger manager, alpide ctrl, 
> and alpide date) with module id 0.

    ./registers_rw -r<pRU id> -<m or r> <modules name>-<module id> or <registers name>-<module id>

pRU id is the number to set readout unit, 
`./registers_rw -r` and `./registers_rw -r0` are the same as `./registers_rw`

The IP address to connect pRU is **192.168.0.100** + pRU id.
The `-<module name>` posfix is optional for module id 0. 

> -m following by \<module names\> to read single or multiple modules of registers
&nbsp;`-m global_regs offload-2 ...`

Module name : global_regs, offload, trigger, alpide_ctrl, alpide_date

> -r following by \<register names\> to read single or multiple registers
&nbsp;`-r date_code mode write_ctrl-11 ...`

Register's name can be found in json file of &nbsp; *json/* &nbsp; directory

### write option

    ./registers_rw -w<pRU id> 

> Interative mode writing register input register's name to set the value

or
#### write modules

    ./registers_rw -w<pRU id> -m <module's name>-<module's id>
The module's id 0 is optional. When the command is given the program will enter to interaction mode and 
fetch only read-write register to ask input value from the console.

##### example:
    ./registers_rw -w0 -m global_regs offload-1 alpide_control-11 ...

#### write registers
    ./registers_rw -w<pRU id> -m <register's name>-<module's id>=<register's value>
The module's id 0 is optional. When the command is given the program will fill the register's value which
its argument from command was given value, on the other hand non-given-value register will ask to fill its
value from console.

##### example:
    ./registers_rw -w0 -r led_blinky=0 write_ctrl-1 write_ctrl-11=0

### Concentration:
The every word in command mustn't contain any space.
