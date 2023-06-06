/*******************************************************************************
* Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*
******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "camera.h"
#include "sccb.h"
#include "asx340_regs.h"
#include "mxc_delay.h"
#include "mxc_device.h"

#define cambus_write(addr, x)      sccb_write_reg16(g_slv_addr, addr, x)
#define cambus_read(addr, x)      sccb_read_reg16(g_slv_addr, addr, x)

#define cambus_writew(addr, x)     sccb_write_reg16w(g_slv_addr, addr, x)
#define cambus_readw(addr, x)      sccb_read_reg16w(g_slv_addr, addr, x)

static int g_slv_addr;
static pixformat_t g_pixelformat = PIXFORMAT_RGB565;

static const uint16_t default_regs[][2] = {

    { 0x098E, 0x4960 },   /*  LOGICAL_ADDRESS_ACCESS [CAM_PGA_PGA_CONTROL]  */
    { 0xC960, 0x0003 },   /*  CAM_PGA_PGA_CONTROL */
    { 0xC870, 0x001E },   /*  CAM_AET_BLACK_CLIPPING_TARGET */
    { 0xAC02, 0x03C8 }, 
    { 0x0040, 0x8606 },
    { 0xC928, 0x0018 },   /*  CAM_LL_START_BRIGHTNESS */
    { 0xC92A, 0x0090 },   /*  CAM_LL_STOP_BRIGHTNESS */
    { 0xC948, 0x0093 },   /*  CAM_LL_START_GAIN_METRIC */
    { 0xC94A, 0x0366 },   /*  CAM_LL_STOP_GAIN_METRIC */
    { 0xC870, 0x0050 },   /*  CAM_AET_BLACK_CLIPPING_TARGET  */
    { 0xC826, 0x008A },   /*  RESERVED_CAM_26 */
    { 0xC972, 0x0023 },   /*  CAM_PORT_PARALLEL_CONTROL */
    { 0xC858, 0x0011 },   /*  CAM_FRAME_SCAN_CONTROL CUSTOM PROGRESSIVE */
    { 0xC972, 0x0025 },   /*  CAM_PORT_PARALLEL_CONTROL */
    { 0x001E, 0x0300 },   /*  PAD_SLEW */
    { 0xC838, 0x0000 },   /*  CAM_CONTROL_READ_MODE */
    { 0xC888, 0x01F2 },   /*  CAM_AWB_CCM_L_0 */
    { 0xC88A, 0xFF6E },   /*  CAM_AWB_CCM_L_1 */
    { 0xC88C, 0xFFA0 },   /*  CAM_AWB_CCM_L_2 */
    { 0xC88E, 0xFF76 },   /*  CAM_AWB_CCM_L_3 */
    { 0xC890, 0x0188 },   /*  CAM_AWB_CCM_L_4 */
    { 0xC892, 0x0002 },   /*  CAM_AWB_CCM_L_5 */
    { 0xC894, 0xFFC2 },   /*  CAM_AWB_CCM_L_6 */
    { 0xC896, 0xFF30 },   /*  CAM_AWB_CCM_L_7 */
    { 0xC898, 0x020E },   /*  CAM_AWB_CCM_L_8 */
    { 0xC89A, 0x01D9 },   /*  CAM_AWB_CCM_M_0 */
    { 0xC89C, 0xFF00 },   /*  CAM_AWB_CCM_M_1 */
    { 0xC89E, 0xFFF3 },   /*  CAM_AWB_CCM_M_2 */
    { 0xC8A0, 0xFF94 },   /*  CAM_AWB_CCM_M_3 */
    { 0xC8A2, 0x0132 },   /*  CAM_AWB_CCM_M_4 */
    { 0xC8A4, 0xFFF3 },   /*  CAM_AWB_CCM_M_5 */
    { 0xC8A6, 0x001A },   /*  CAM_AWB_CCM_M_6 */
    { 0xC8A8, 0xFE73 },   /*  CAM_AWB_CCM_M_7 */
    { 0xC8AA, 0x02C2 },   /*  CAM_AWB_CCM_M_8 */
    { 0xC8AC, 0x01AF },   /*  CAM_AWB_CCM_R_0 */
    { 0xC8AE, 0xFF93 },   /*  CAM_AWB_CCM_R_1 */
    { 0xC8B0, 0xFFBE },   /*  CAM_AWB_CCM_R_2 */
    { 0xC8B2, 0xFF9E },   /*  CAM_AWB_CCM_R_3 */
    { 0xC8B4, 0x0176 },   /*  CAM_AWB_CCM_R_4 */
    { 0xC8B6, 0xFFED },   /*  CAM_AWB_CCM_R_5 */
    { 0xC8B8, 0x0001 },   /*  CAM_AWB_CCM_R_6 */
    { 0xC8BA, 0xFF78 },   /*  CAM_AWB_CCM_R_7 */
    { 0xC8BC, 0x0187 },   /*  CAM_AWB_CCM_R_8 */
    { 0xC8BE, 0x006D },   /*  CAM_AWB_CCM_L_RG_GAIN */
    { 0xC8C0, 0x0148 },   /*  CAM_AWB_CCM_L_BG_GAIN */
    { 0xC8C2, 0x009E },   /*  CAM_AWB_CCM_M_RG_GAIN */
    { 0xC8C4, 0x011D },   /*  CAM_AWB_CCM_M_BG_GAIN */
    { 0xC8C6, 0x00A2 },   /*  CAM_AWB_CCM_R_RG_GAIN */
    { 0xC8C8, 0x00B1 },   /*  CAM_AWB_CCM_R_BG_GAIN */
    { 0xC8CA, 0x0A8C },   /*  CAM_AWB_CCM_L_CTEMP */
    { 0xC8CC, 0x0F0A },   /*  CAM_AWB_CCM_M_CTEMP */
    { 0xC8CE, 0x1964 },   /*  CAM_AWB_CCM_R_CTEMP */
    { 0xC90C, 0x0000 },   /*  CAM_STAT_AWB_CLIP_WINDOW_XSTART */
    { 0xC90E, 0x0000 },   /*  CAM_STAT_AWB_CLIP_WINDOW_YSTART */
    { 0xC910, 0x027F },   /*  CAM_STAT_AWB_CLIP_WINDOW_XEND */
    { 0xC912, 0x01DF },   /*  CAM_STAT_AWB_CLIP_WINDOW_YEND */
    { 0xC8FA, 0x0040 },   /*  CAM_AWB_AWB_XSHIFT_PRE_ADJ */
    { 0xC8FC, 0x0035 },   /*  CAM_AWB_AWB_YSHIFT_PRE_ADJ */  
    { 0xC8EA, 0x5D54 },   /*  CAM_AWB_AWB_WEIGHTS_0 */
    { 0xC8EC, 0xDCCE },   /*  CAM_AWB_AWB_WEIGHTS_1 */
    { 0xC8EE, 0x9A60 },   /*  CAM_AWB_AWB_WEIGHTS_2 */
    { 0xC8F0, 0x812A },   /*  CAM_AWB_AWB_WEIGHTS_3 */
    { 0xC8F2, 0x9C08 },   /*  CAM_AWB_AWB_WEIGHTS_4 */
    { 0xC8F4, 0x989D },   /*  CAM_AWB_AWB_WEIGHTS_5 */
    { 0xC8F6, 0x3D23 },   /*  CAM_AWB_AWB_WEIGHTS_6 */
    { 0xC8F8, 0x0624 },   /*  CAM_AWB_AWB_WEIGHTS_7 */
    { 0xC964, 0x0692 },   /*  CAM_SYSCTL_PLL_DIVIDER_P */
    { 0xC966, 0x0400 },   /*  CAM_SYSCTL_PLL_DIVIDER_P */
    { 0xFFFF, 0xFFFF },

};

static struct camera_reg default_regs8[] = {
    { 0xC931, 0x07 },   /*  CAM_LL_START_AP_GAIN */
    { 0xC934, 0x02 },   /*  CAM_LL_STOP_AP_GAIN */
    { 0xC932, 0x02 },   /*  CAM_LL_START_AP_THRESH */
    { 0xC935, 0x1F },   /*  CAM_LL_STOP_AP_THRESH */
    { 0xC946, 0x28 },   /*  CAM_LL_START_CONTRAST_LUMA_PERCENTAGE */
    { 0xC947, 0x1E },   /*  CAM_LL_STOP_CONTRAST_LUMA_PERCENTAGE */
    { 0xC864, 0x00 },   /*  CAM_ZOOM_FACTOR */
    { 0xC85C, 0x00 },   /*   CAM_FOV_CALIB_X_OFFSET   */
    { 0xC85D, 0x00 },   /*   CAM_FOV_CALIB_Y_OFFSET   */
    { 0xC8E8, 0x03 },   /*   CAM_AWB_AWB_XSCALE   */
    { 0xC8E9, 0x02 },   /*   CAM_AWB_AWB_YSCALE   */     
    { 0xFFFF, 0xFF },
};

static const uint16_t tuning_regs[][2] = {
    { 0xC988, 0x0001},   /*  AUTO FLICKER CORRECTION */
    { 0x9406, 0x00A4},   /*  RESERVED_NTSC_06 */
    { 0x9806, 0x00A4},   /*  RESERVED_PAL_06 */
    { 0xC80E, 0x00A4},   /*  CAM_SENSOR_CFG_FINE_INTEG_TIME_MAX */
    { 0xC872, 0x00A4},    /*  RESERVED_CAM_72 */
    { 0xFFFF, 0xFFFF},
};

const mxc_gpio_cfg_t cyfi013_reset_pin = {MXC_GPIO2, MXC_GPIO_PIN_4, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH};

/******************************** Static Functions ***************************/
static int doorbell(void)
{
    uint16_t regValue = 0xFFFF;
    do
    {
        MXC_Delay(5000);
        cambus_readw(COMMAND_REGISTER,&regValue);
    }while(regValue & 0x8000);
    return 0;
}
static int change_config(void)
{
    int ret = 0;
    ret |= cambus_writew(COMMAND_HANDLER, 0x2800);
    MXC_Delay(5000);
    doorbell();
    ret |= cambus_writew(COMMAND_REGISTER, 0x8100);
    MXC_Delay(5000);
    doorbell();
    ret |= cambus_writew(COMMAND_REGISTER,0x8101);
    MXC_Delay(5000);
    doorbell();
    uint8_t count = 2u;
    uint16_t regValue = 0xFFFF;
    while((regValue != 0x3100) && (count>0)){
    	cambus_readw(COMMAND_HANDLER, &regValue);
    	count--;
    }
    return 0;
}
static int init(void)
{
    int ret = 0;

    g_slv_addr = 0x48;

    if (g_slv_addr == -1) {
        return -1;
    }

    return ret;
}

static int get_slave_address(void)
{
    return g_slv_addr;
}

static int get_product_id(int* id)
{
    int ret = 0;
    uint16_t rev;

    ret |= cambus_readw(CHIP_ID, &rev);
    *id = (int)rev;
    return ret;
}

static int get_manufacture_id(int* id)
{
    int ret = 0;

    uint16_t wrd;
    ret = cambus_readw(CHIP_ID, &wrd);
    *id = wrd;

    return ret;
}

static int dump_registers(void)
{
    int ret = 0;
    uint16_t wrd = 0;
    uint32_t i;
    uint8_t buf[80] = {0};
    uint8_t* ptr = buf;

    for (i = 0x8000; i < 0xA000 ; i++) {

        if ((i != 0) && !(i % 16)) {
            *ptr = '\0';
            printf("%04X:%s\n", i - 16, buf);
            ptr = buf;
        }

        if (i == 0xFC0E) {
            break;
        }

        ret = cambus_readw(i, &wrd);
        ret = 0;
        if (ret == 0) {
            ret = sprintf((char*)ptr, " %04hX", wrd);

            if (ret < 0) {
                return ret;
            }

            ptr += 5;// XX + space
        }
        else {
            *ptr++ = '!';
            *ptr++ = '!';
            *ptr++ = ' ';
        }
    }

    return ret;
}

static int reset(void)
{
    int ret = 0;

    printf("Resetting the image sensor\n");
    if (MXC_GPIO_Config(&cyfi013_reset_pin) != E_NO_ERROR) {
        return -1;
    }

    MXC_GPIO_OutSet(cyfi013_reset_pin.port, cyfi013_reset_pin.mask);
    MXC_Delay(50000);
    MXC_GPIO_OutClr(cyfi013_reset_pin.port, cyfi013_reset_pin.mask);
    MXC_Delay(50000);
    MXC_GPIO_OutSet(cyfi013_reset_pin.port, cyfi013_reset_pin.mask);
    MXC_Delay(50000);
    cambus_writew(0x001A,0x0001);
    MXC_Delay(2000);
    cambus_writew(0x001A,0x0000);
    MXC_Delay(2000);
    doorbell();

    for (int i = 0; (default_regs[i][0] != 0xFFFF); i++) {
        ret |= cambus_writew(default_regs[i][0], (uint16_t)default_regs[i][1]);
        MXC_Delay(50);
		doorbell();
    }
    change_config();

    for (int i = 0; (default_regs8[i].addr != 0xFFFF); i++) {
        ret |= cambus_write(default_regs8[i].addr, (uint8_t)default_regs8[i].val);
        MXC_Delay(50);
    }
    change_config();

    for (int i = 0; (tuning_regs[i][0] != 0xFFFF); i++) {
        ret |= cambus_writew(tuning_regs[i][0], (uint16_t)tuning_regs[i][1]);
        MXC_Delay(50);
		doorbell();
    }          
    change_config();

    return ret;
}

static int sleep(int enable)
{
    int ret = 0;
    
    return ret;
}

static int read_reg(uint16_t reg_addr, uint16_t* reg_data)
{
    *reg_data = 0xff;

    if (cambus_readw(reg_addr, reg_data) != 0) {
        return -1;
    }

    return 0;
}

static int write_reg(uint16_t reg_addr, uint16_t reg_data)
{
    return cambus_writew(reg_addr, reg_data);
}

static int set_pixformat(pixformat_t pixformat)
{
    int ret = 0;
    uint16_t reg;
    g_pixelformat = pixformat;
    switch (pixformat) {
    case PIXFORMAT_YUV422:
        ret |= cambus_readw(OUTPUT_FORMAT, &reg);
        reg |= COLOR_YUV422;
        ret |= cambus_writew(OUTPUT_FORMAT, reg);
        break;
    case PIXFORMAT_RGB444:
        ret |= cambus_readw(OUTPUT_FORMAT, &reg);
        reg |= COLOR_RGB444;
        ret |= cambus_writew(OUTPUT_FORMAT, reg);
        break;
    case PIXFORMAT_RGB565:
        ret |= cambus_readw(OUTPUT_FORMAT, &reg);
        reg |= COLOR_RGB565;
        ret |= cambus_writew(OUTPUT_FORMAT, reg);
        break;
    case PIXFORMAT_RGB888:
        ret = -1;
        break;
    case PIXFORMAT_BAYER:
        ret |= cambus_readw(OUTPUT_FORMAT, &reg);
        reg |= COLOR_BAYER;
        ret |= cambus_writew(OUTPUT_FORMAT, reg);
        break;
    default:
        ret = -1;
        break;
    }
    ret |= change_config();
    return ret;
}

static int get_pixformat(pixformat_t* pixformat)
{
    int ret = 0;
    *pixformat = g_pixelformat;
    return ret;
}

static int set_framesize(int width, int height)
{
    int ret = 0;
    ret |= cambus_writew(WIDTH, width);
    ret |= cambus_writew(HEIGHT, height);
    ret |= cambus_writew(Y_ADDR_START, Y_ADDR_PIXEL_START_OFFSET- (height/2));   
    ret |= cambus_writew(X_ADDR_START, X_ADDR_PIXEL_START_OFFSET - (width/2));
    ret |= cambus_writew(Y_ADDR_END, Y_ADDR_PIXEL_END_OFFSET + (height/2));
    ret |= cambus_writew(X_ADDR_END, X_ADDR_PIXEL_END_OFFSET + (width/2));

    change_config();

    return ret;
}

static int set_windowing(int width, int height, int hsize, int vsize)
{
    /* Note: width and height is used to control scaling size of the image
       width: horizontal input size
       height: vertical input size
       hsize: horizontal size of cropped image
       vsize: vertical size of cropped image
    */
    int ret = 0;

    if (width < hsize || height < vsize) {
        ret = -1;
    }

    return ret;
}

static int set_contrast(int level)
{
    int ret = 0;

    return ret;
}

static int set_brightness(int level)
{
    int ret = 0;

    return ret;
}

static int set_saturation(int level)
{
    int ret = 0;

    return ret;
}

static int set_gainceiling(gainceiling_t gainceiling)
{
    int ret = 0;

    return ret;
}

static int set_colorbar(int enable)
{
    int ret = 0;

    return ret;
}

static int set_hmirror(int enable)
{
    int ret = 0;
    uint16_t reg;


    cambus_readw( 0xC838, &reg);   /*  CAM_CONTROL_READ_MODE */
    if (enable) {
        reg |= 0x0001;
    }
    else {
        reg &= ~0x0001;
    }

    ret |= cambus_writew(0xC838, reg);
    change_config();

    return ret;
}

static int set_negateimage(int enable)
{
    int ret = 0;
    ret |= cambus_write(0xC868,0x03); /*  CAM_SFX_CONTROL */
    change_config();     
    return ret;
}

static int set_vflip(int enable)
{
    int ret = 0;
    uint16_t reg;

    cambus_readw( 0xC838, &reg);   /*  CAM_CONTROL_READ_MODE */
    if (enable) {
        reg |= 0x0002;
    }
    else {
        reg &= ~0x0002;
    }

    ret |= cambus_writew(0xC838, reg);
    change_config();

    return ret;
}

static  int get_luminance(int* lum)
{
    int ret = 0;

    return ret;
}

/******************************** Public Functions ***************************/
int sensor_register(camera_t* camera)
{
    // Initialize sensor structure.
    camera->init                = init;
    camera->get_slave_address   = get_slave_address;
    camera->get_product_id      = get_product_id;
    camera->get_manufacture_id  = get_manufacture_id;
    camera->dump_registers      = dump_registers;
    camera->reset               = reset;
    camera->sleep               = sleep;
    camera->read_reg            = read_reg;
    camera->write_reg           = write_reg;
    camera->set_pixformat       = set_pixformat;
    camera->get_pixformat       = get_pixformat;
    camera->set_framesize       = set_framesize;
    camera->set_windowing       = set_windowing;
    camera->set_contrast        = set_contrast;
    camera->set_brightness      = set_brightness;
    camera->set_saturation      = set_saturation;
    camera->set_gainceiling     = set_gainceiling;
    camera->set_colorbar        = set_colorbar;
    camera->set_hmirror         = set_hmirror;
    camera->set_vflip           = set_vflip;
    camera->set_negateimage     = set_negateimage;
    camera->get_luminance       = get_luminance;
    camera->change_config       = change_config;
    return 0;
}
