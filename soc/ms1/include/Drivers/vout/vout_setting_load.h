/*******************************************************************
 * 
 *  Copyright C 2008 by Amlogic, Inc. All Rights Reserved.
 *
 *  Description: Header file for VOUT setting load function
 *
 *  Author:  Amlogic 
 *  Created: 07/09/2025
 *
 *******************************************************************/
#ifndef VOUT_SETTING_LOAD_H
#define VOUT_SETTING_LOAD_H

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
int vout_setting_load(void);

#endif /* VOUT_SETTING_LOAD_H */

