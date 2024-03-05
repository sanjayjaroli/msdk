/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by 
 * Analog Devices, Inc.),
 * Copyright (C) 2023-2024 Analog Devices, Inc.
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
 *
 ******************************************************************************/

#ifndef EXAMPLES_MAX32655_TFT_DEMO_INCLUDE_UTILS_H_
#define EXAMPLES_MAX32655_TFT_DEMO_INCLUDE_UTILS_H_

/*****************************     MACROS    *********************************/
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

/*****************************     VARIABLES *********************************/

/*****************************     FUNCTIONS *********************************/
unsigned int utils_get_time_ms(void);
unsigned int utils_get_time_tick(void);
void utils_delay_ms(unsigned int ms);

#endif // EXAMPLES_MAX32655_TFT_DEMO_INCLUDE_UTILS_H_
