/*******************************************************************
 * 
 *  Copyright C 2008 by Amlogic, Inc. All Rights Reserved.
 *
 *  Description: Load VOUT settings from fpga_sim_setting.txt file
 *
 *  Author:  Amlogic 
 *  Created: 07/09/2025
 *
 *******************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Drivers/vout/register.h"
#include "Drivers/vout/vout.h"
#include "Drivers/vout/vout_setting_load.h"
#include "Drivers/vout/vout_tx_base600_setting.h"

/**
 * @brief Load VOUT settings from fpga_sim_setting.txt file
 * 
 * This function reads the fpga_sim_setting.txt file, parses each line containing
 * an address and value pair, and writes the values to the VOUT registers using
 * the Wr function. The base address 0x10000 in the file is replaced with
 * VOUT_APB_BASE_ADDR.
 * 
 * @return 0 on success, -1 on error
 */
int vout_setting_load(void)
{
    uint32_t  addr, data;
    uint32_t  addr_tmp;
    for (int i=0;i<register_data_length;i=i+1)  {
        addr = register_data[i*2];
        data = register_data[i*2+1];
        if (addr>0x100)
            addr_tmp = VOUT_APB_BASE_ADDR+(addr<<2);
        else
            addr_tmp = VOUT_APB_BASE_ADDR+((0x500+addr)<<2);
        Wr(addr_tmp,data);
      }
}
