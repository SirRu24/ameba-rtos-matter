/*
 *
 *    Copyright (c) 2022 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "platform_opts_matter.h"
#include "matter_core.h"
#include "matter_interaction.h"

#include "app/server/Server.h"
#include "platform/CHIPDeviceLayer.h"
#include <lib/support/CodeUtils.h>

#if defined(CONFIG_ENABLE_CHIP_SHELL) && (CONFIG_ENABLE_CHIP_SHELL == 1)
#include "lib/shell/Engine.h"
#include "lib/shell/commands/Help.h"
#endif // defined(CONFIG_ENABLE_CHIP_SHELL) && (CONFIG_ENABLE_CHIP_SHELL == 1)

using namespace chip;
using namespace chip::app;

#if defined(CONFIG_ENABLE_CHIP_SHELL) && (CONFIG_ENABLE_CHIP_SHELL == 1)
using Shell::Engine;
using Shell::shell_command_t;
using Shell::streamer_get;
using Shell::streamer_printf;
#endif // defined(CONFIG_ENABLE_CHIP_SHELL) && (CONFIG_ENABLE_CHIP_SHELL == 1)

#if defined(CONFIG_ENABLE_CHIP_SHELL) && (CONFIG_ENABLE_CHIP_SHELL == 1) && \
    defined(CONFIG_EXAMPLE_MATTER_CHIPTEST) && (CONFIG_EXAMPLE_MATTER_CHIPTEST == 0) \
    !defined(CONFIG_PLATFORM_8710C) //will not be compiled for non porting layer and ameba Z2, still buggy

/********************************************************
 * Matter Core shell functions, controlled by Ameba Shell
 *********************************************************/

extern "C" void MatterCoreStatusCommandHandler()
{
    if (matter_core_server_is_running())
    {
        ChipLogProgress(DeviceLayer, "Matter device is running!");
    }
    else
    {
        ChipLogProgress(DeviceLayer, "Matter device is not running!");
    }
}

extern "C" void MatterCoreStartCommandHandler()
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    if(matter_core_server_is_running())
    {
        ChipLogProgress(DeviceLayer, "Matter device is already started!");
    }
    else
    {
        ChipLogProgress(DeviceLayer, "MatterCoreStartCommandHandler !!!\n");
        err = matter_core_start();
        if (err != CHIP_NO_ERROR)
            ChipLogProgress(DeviceLayer, "matter_core_start failed!\n");

#if !(defined(CONFIG_EXAMPLE_MATTER_AIRCON) && CONFIG_EXAMPLE_MATTER_AIRCON) // aircon example does not include downlink
        err = matter_interaction_start_downlink();
        if (err != CHIP_NO_ERROR)
            ChipLogProgress(DeviceLayer, "matter_interaction_start_downlink failed!\n");
#endif

        err = matter_interaction_start_uplink();
        if (err != CHIP_NO_ERROR)
            ChipLogProgress(DeviceLayer, "matter_interaction_start_uplink failed!\n");
    }
}

extern "C" void MatterCoreStopCommandHandler()
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    if(matter_core_server_is_running())
    {
        err = matter_core_stop();
        if (err != CHIP_NO_ERROR)
            ChipLogProgress(DeviceLayer, "matter_core_stop failed!\n");

        matter_interaction_clean_up();
    }
    else
    {
        ChipLogProgress(DeviceLayer, "Matter device is already stopped!");
    }
}
#endif // defined(CONFIG_ENABLE_CHIP_SHELL) && (CONFIG_ENABLE_CHIP_SHELL == 1) && \ 
       // defined(CONFIG_EXAMPLE_MATTER_CHIPTEST) && (CONFIG_EXAMPLE_MATTER_CHIPTEST == 0)
       // !defined(CONFIG_PLATFORM_8710C)
