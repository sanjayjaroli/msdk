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

#ifndef LIBRARIES_LWIP_INCLUDE_MAXIM_ARCH_SYS_ARCH_H_
#define LIBRARIES_LWIP_INCLUDE_MAXIM_ARCH_SYS_ARCH_H_

#define SYS_MBOX_NULL   NULL
#define SYS_SEM_NULL    NULL

typedef void *sys_prot_t;
typedef void *sys_mutex_t;
typedef void *sys_sem_t;
typedef void *sys_mbox_t;
typedef void *sys_thread_t;

#define sys_arch_mbox_tryfetch(mbox, msg) sys_arch_mbox_fetch(mbox, msg, 1)

#endif // LIBRARIES_LWIP_INCLUDE_MAXIM_ARCH_SYS_ARCH_H_
