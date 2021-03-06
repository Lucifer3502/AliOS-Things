/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "pinmap.h"
#include "PortNames.h"

/**
 * Configure pin multi-function
 */
void pin_function(PinName pin, int data)
{
    uint32_t pin_index = NU_PINNAME_TO_PIN(pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(pin);
    __IO uint32_t *Px_x_MFP = ((__IO uint32_t *) &SYS->PA_L_MFP) + port_index * 2 + (pin_index / 8);
    uint32_t MFP_Msk = NU_MFP_MSK(pin_index);
    
    // E.g.: SYS->PA_L_MFP  = (SYS->PA_L_MFP & (~SYS_PA_L_MFP_PA0_MFP_Msk) ) | SYS_PA_L_MFP_PA0_MFP_SC0_CD  ;
    *Px_x_MFP  = (*Px_x_MFP & (~MFP_Msk)) | data;
}

/**
 * Configure pin pull-up/pull-down
 */
void pin_mode(PinName pin, PinMode mode)
{
    uint32_t pin_index = NU_PINNAME_TO_PIN(pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(pin);
    GPIO_T *gpio_base = NU_PORT_BASE(port_index);
    
    uint32_t mode_intern = GPIO_PMD_INPUT;
    
    switch (mode) {
        case PullUp:
            mode_intern = GPIO_PMD_INPUT;
            break;
            
        case PullDown:
        case PullNone:
            // NOTE: Not support
            return;
        
        case PushPull:
            mode_intern = GPIO_PMD_OUTPUT;
            break;
            
        case OpenDrain:
            mode_intern = GPIO_PMD_OPEN_DRAIN;
            break;
            
        case Quasi:
            // NOTE: Not support
            break;
    }
    
    GPIO_SetMode(gpio_base, 1 << pin_index, mode_intern);
}

#if 0
void pin_pullctrl(PinName pin, PinPullCtrl pullctl)
{
    uint32_t pin_index;
    uint32_t port_index;
    GPIO_T *gpio_base;
		uint32_t pullctl_intern;

    if ( pin == (PinName)NC )
        halt_on_error_found(__func__, "pin is NC!!", 0);

    pin_index = NU_PINNAME_TO_PIN(pin);
    port_index = NU_PINNAME_TO_PORT(pin);
    gpio_base = NU_PORT_BASE(port_index);

		pullctl_intern = GPIO_PUSEL_PULL_UP;
		
    switch (pullctl) {
			case PullUp:
					pullctl_intern = GPIO_PUSEL_PULL_UP;
					break;

			case PullDown:
					pullctl_intern = GPIO_PUSEL_PULL_DOWN;
					break;
			
			case PullNone:
					pullctl_intern = GPIO_PUSEL_DISABLE;
					break;
			default:
					return;
    }

    GPIO_SetPullCtl(gpio_base, 1 << pin_index, pullctl_intern);
}
#endif
