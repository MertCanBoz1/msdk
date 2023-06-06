/******************************************************************************
 * Copyright (C) 2023 Maxim Integrated Products, Inc., All Rights Reserved.
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

#ifndef LIBRARIES_MISCDRIVERS_CAMERA_ASX340_REGS_H_
#define LIBRARIES_MISCDRIVERS_CAMERA_ASX340_REGS_H_

// clang-format off

#define DATA_WIDTH                  (0x02u)       /* Register width */
#define DATA_WIDTH_BYTE             (0x1u)        /* Byte Register width */

#define CHIP_ID                     (0x0000u)     /* Chip ID register */

#define CHIP_ID_VAL                 (0x2285u)     /* Chip ID register Value*/

#define OUTPUT_FORMAT               (0xC96Cu)     /* Camera output format */

#define AE_ALGO                     (0xA804u)     /* AGC,AEC,AWB,Exposure control */
#define DIGITAL_GAIN                (0xC84Cu)     /* Auto frame rate and digital gain control */
#define ANALOG_GAIN                 (0xC83Au)     /* Analog Gain */

#define COARSE_INT_TIME             (0xC840)      /* Coarse Integration time */
#define FINE_INT_TIME               (0xC842)      /* Fine Integration time */

#define PREFIXED_VALUE              (0xFFu)       /* To fill with known value */

#define WIDTH                       (0xC968u)     /* Width  register*/
#define HEIGHT                      (0xC96Au)     /* Height register*/

#define COLORBAR                    (0xC850u)     /* Color bar register*/
#define COLORBAR_ENABLE_MASK        (0x0200u)     /* Color bar enable mask */
#define COLORBAR_DISABLE_MASK       (0xFDFFu)     /* Color bar disable mask */

#define REG_LO_ASX340_FRAMERATE_DIV            (0xC808u)     /* clock divisior */
#define REG_HI_ASX340_FRAMERATE_DIV            (0xC80Au)
#define FRAMERATE_LINES             (0xC810u)     /* cam_sensor_cfg_frame_length_lines */
#define FRAMERATE_PCK               (0xC812u)     /* cam_sensor_cfg_line_length_pck */

#define FRAMERATE_LINES_30FPS       (525)
#define FRAMERATE_PCK_30FPS         (1520)
#define FRAMERATE_LINES_15FPS       (500)
#define FRAMERATE_PCK_15FPS         (1600)

#define REG_LO_ASX340_FRAMERATE_30FPS          (0x019B)      /* No scaling; Full frame rate */
#define REG_HI_ASX340_FRAMERATE_30FPS          (0xFCC0u)
#define REG_LO_ASX340_FRAMERATE_15FPS          (0xCDu)       /* scaling by 2; half frame rate */
#define REG_HI_ASX340_FRAMERATE_15FPS          (0xFE60u)
#define FRAMERATE_15FPS                        (0x0Fu)       /* 15 fps */
#define FRAMERATE_30FPS                        (0x1Eu)       /* 30 fps */

#define COMMAND_HANDLER         (0xFC00)                  
#define COMMAND_REGISTER        (0x0040)        


#define Y_ADDR_START            (0xC800)
#define X_ADDR_START            (0xC802)
#define Y_ADDR_END              (0xC804)
#define X_ADDR_END              (0xC806)

#define Y_ADDR_PIXEL_START_OFFSET           (280)
#define X_ADDR_PIXEL_START_OFFSET           (373)
#define Y_ADDR_PIXEL_END_OFFSET             (287)
#define X_ADDR_PIXEL_END_OFFSET             (380)



#define COLOR_RGB565    (0x0100)
#define COLOR_RGB444    (0x3100)
#define COLOR_YUV422    (0x0000)
#define COLOR_BAYER     (0x0004)

/* TWI register address width */
#define ASX340_REG_ADDR_WIDTH                  (2u)
// clang-format on

#endif // LIBRARIES_MISCDRIVERS_CAMERA_PAG7920_REGS_H_
