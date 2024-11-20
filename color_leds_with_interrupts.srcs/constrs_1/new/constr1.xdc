## RGB LEDs
set_property -dict { PACKAGE_PIN L15   IOSTANDARD LVCMOS33 } [get_ports { rgb_led_tri_o[2] }]; #IO_L22N_T3_AD7N_35 Sch=led0_b
set_property -dict { PACKAGE_PIN G17   IOSTANDARD LVCMOS33 } [get_ports { rgb_led_tri_o[1] }]; #IO_L16P_T2_35 Sch=led0_g
set_property -dict { PACKAGE_PIN N15   IOSTANDARD LVCMOS33 } [get_ports { rgb_led_tri_o[0] }]; #IO_L21P_T3_DQS_AD14P_35 Sch=led0_r
set_property -dict { PACKAGE_PIN G14   IOSTANDARD LVCMOS33 } [get_ports { rgb_led_tri_o[5] }]; #IO_0_35 Sch=led1_b
set_property -dict { PACKAGE_PIN L14   IOSTANDARD LVCMOS33 } [get_ports { rgb_led_tri_o[4] }]; #IO_L22P_T3_AD7P_35 Sch=led1_g
set_property -dict { PACKAGE_PIN M15   IOSTANDARD LVCMOS33 } [get_ports { rgb_led_tri_o[3] }]; #IO_L23N_T3_35 Sch=led1_r

set_property -dict { PACKAGE_PIN D19   IOSTANDARD LVCMOS33 } [get_ports { btns_4bits_tri_io[0] }];
set_property -dict { PACKAGE_PIN D20   IOSTANDARD LVCMOS33 } [get_ports { btns_4bits_tri_io[1] }];
set_property -dict { PACKAGE_PIN L20   IOSTANDARD LVCMOS33 } [get_ports { btns_4bits_tri_io[2] }];
set_property -dict { PACKAGE_PIN L19   IOSTANDARD LVCMOS33 } [get_ports { btns_4bits_tri_io[3] }];

set_property -dict { PACKAGE_PIN R14   IOSTANDARD LVCMOS33 } [get_ports { leds_4bits_tri_io[0] }];
set_property -dict { PACKAGE_PIN P14   IOSTANDARD LVCMOS33 } [get_ports { leds_4bits_tri_io[1] }];
set_property -dict { PACKAGE_PIN N16   IOSTANDARD LVCMOS33 } [get_ports { leds_4bits_tri_io[2] }];
set_property -dict { PACKAGE_PIN M14   IOSTANDARD LVCMOS33 } [get_ports { leds_4bits_tri_io[3] }];

#  <pin index="4" name ="leds_4bits_tri_o_0" iostandard="LVCMOS33" loc="R14"/>
#  <pin index="5" name ="leds_4bits_tri_o_1" iostandard="LVCMOS33" loc="P14"/>
#  <pin index="6" name ="leds_4bits_tri_o_2" iostandard="LVCMOS33" loc="N16"/>
#  <pin index="7" name ="leds_4bits_tri_o_3" iostandard="LVCMOS33" loc="M14"/>

#  <pin index="0" name ="btns_4bits_tri_i_0" iostandard="LVCMOS33" loc="D19"/>
#  <pin index="1" name ="btns_4bits_tri_i_1" iostandard="LVCMOS33" loc="D20"/>
#  <pin index="2" name ="btns_4bits_tri_i_2" iostandard="LVCMOS33" loc="L20"/>
#  <pin index="3" name ="btns_4bits_tri_i_3" iostandard="LVCMOS33" loc="L19"/>