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
#include "matter_prov.h"

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

Engine sShellMatterProvisioningSubCommands;
#endif // defined(CONFIG_ENABLE_CHIP_SHELL) && (CONFIG_ENABLE_CHIP_SHELL == 1)

#if defined(CONFIG_ENABLE_CHIP_SHELL) && (CONFIG_ENABLE_CHIP_SHELL == 1)
/********************************************************
 * Matter Provisioning shell functions
 *********************************************************/

CHIP_ERROR MatterProvisioningCommandHelpHandler(int argc, char ** argv)
{
    sShellMatterProvisioningSubCommands.ForEachCommand(Shell::PrintCommandHelp, nullptr);
    return CHIP_NO_ERROR;
}

CHIP_ERROR MatterProvisioningStatusCommandHandler(int argc, char ** argv)
{
    if (argc != 0)
    {
        return MatterProvisioningCommandHelpHandler(argc, argv);
    }

    if (matter_prov_is_provisioned())
    {
        ChipLogProgress(DeviceLayer, "Matter device is provisioned!");
    }
    else
    {
        ChipLogProgress(DeviceLayer, "Matter device is not provisioned!");
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR MatterProvisioningEnterProvModeCommandHandler(int argc, char ** argv)
{
    if (argc != 0)
    {
        return MatterProvisioningCommandHelpHandler(argc, argv);
    }

    return matter_prov_start();
}

CHIP_ERROR MatterProvisioningExitProvModeCommandHandler(int argc, char ** argv)
{
    if (argc != 0)
    {
        return MatterProvisioningCommandHelpHandler(argc, argv);
    }

    return matter_prov_stop();
}

CHIP_ERROR MatterProvisioningPrintProvTableCommandHandler(int argc, char ** argv)
{
    if (argc != 0)
    {
        return MatterProvisioningCommandHelpHandler(argc, argv);
    }

    return matter_prov_print_fabric_table();
}

CHIP_ERROR MatterProvisioningEraseRowCommandHandler(int argc, char ** argv)
{
    if (argc != 1)
    {
        return MatterProvisioningCommandHelpHandler(argc, argv);
    }

    uint8_t row = atoi(argv[0]);
    return matter_prov_delete_fabric_table_row(row);
}
#endif // defined(CONFIG_ENABLE_CHIP_SHELL) && (CONFIG_ENABLE_CHIP_SHELL == 1)
