//extern void set_display_combo_dphy(int phy0_func,int phy1_func,int phy2_func);
//extern void set_display_combo_dphy01_prbs();
//extern void set_display_combo_dphy2_prbs();
//extern void set_combo_dphy_out_high();
//extern void set_combo_dphy_out_low();
//extern void chk_combo_dphy_reg();
typedef enum {
    COMBO_VX1          = 0,
    COMBO_LVDS_SINGLE  = 1,
    COMBO_LVDS_DUAL    = 2,
    COMBO_EDP          = 3,
    COMBO_DSI          = 4,
    COMBO_NO           = 5,
}COMBO_DPHY_TYPE_E;
extern void set_edp_lvds_dig_phy(COMBO_DPHY_TYPE_E a_dphy_type, COMBO_DPHY_TYPE_E  b_dphy_type,COMBO_DPHY_TYPE_E  lvds_phy_type);

