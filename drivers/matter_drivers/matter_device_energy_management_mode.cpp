/*
 *
 *    Copyright (c) 2024 Project CHIP Authors
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

#include <app-common/zap-generated/attributes/Accessors.h>
#include <matter_device_energy_management_mode.h>

using namespace chip::app::Clusters;
using namespace chip::app::Clusters::DeviceEnergyManagementMode;
using chip::Protocols::InteractionModel::Status;
template <typename T>
using List              = chip::app::DataModel::List<T>;
using ModeTagStructType = chip::app::Clusters::detail::Structs::ModeTagStruct::Type;

static std::unique_ptr<DeviceEnergyManagementModeDelegate> gDeviceEnergyManagementModeDelegate;
static std::unique_ptr<ModeBase::Instance> gDeviceEnergyManagementModeInstance;

CHIP_ERROR DeviceEnergyManagementModeDelegate::Init()
{
    return CHIP_NO_ERROR;
}

void DeviceEnergyManagementModeDelegate::HandleChangeToMode(uint8_t NewMode,
                                                            ModeBase::Commands::ChangeToModeResponse::Type & response)
{
    response.status = to_underlying(ModeBase::StatusCode::kSuccess);
}

CHIP_ERROR DeviceEnergyManagementModeDelegate::GetModeLabelByIndex(uint8_t modeIndex, chip::MutableCharSpan & label)
{
    if (modeIndex >= ArraySize(kModeOptions))
    {
        return CHIP_ERROR_PROVIDER_LIST_EXHAUSTED;
    }
    return chip::CopyCharSpanToMutableCharSpan(kModeOptions[modeIndex].label, label);
}

CHIP_ERROR DeviceEnergyManagementModeDelegate::GetModeValueByIndex(uint8_t modeIndex, uint8_t & value)
{
    if (modeIndex >= ArraySize(kModeOptions))
    {
        return CHIP_ERROR_PROVIDER_LIST_EXHAUSTED;
    }
    value = kModeOptions[modeIndex].mode;
    return CHIP_NO_ERROR;
}

CHIP_ERROR DeviceEnergyManagementModeDelegate::GetModeTagsByIndex(uint8_t modeIndex, List<ModeTagStructType> & tags)
{
    if (modeIndex >= ArraySize(kModeOptions))
    {
        return CHIP_ERROR_PROVIDER_LIST_EXHAUSTED;
    }

    if (tags.size() < kModeOptions[modeIndex].modeTags.size())
    {
        return CHIP_ERROR_INVALID_ARGUMENT;
    }

    std::copy(kModeOptions[modeIndex].modeTags.begin(), kModeOptions[modeIndex].modeTags.end(), tags.begin());
    tags.reduce_size(kModeOptions[modeIndex].modeTags.size());

    return CHIP_NO_ERROR;
}

ModeBase::Instance * DeviceEnergyManagementMode::Instance()
{
    return gDeviceEnergyManagementModeInstance.get();
}

void DeviceEnergyManagementMode::Shutdown()
{
    gDeviceEnergyManagementModeInstance.reset();
    gDeviceEnergyManagementModeDelegate.reset();
}

void emberAfDeviceEnergyManagementModeClusterInitCallback(chip::EndpointId endpointId)
{
    VerifyOrDie(endpointId == 1); // this cluster is only enabled for endpoint 1.
    VerifyOrDie(!gDeviceEnergyManagementModeDelegate && !gDeviceEnergyManagementModeInstance);
    gDeviceEnergyManagementModeDelegate = std::make_unique<DeviceEnergyManagementMode::DeviceEnergyManagementModeDelegate>();
    gDeviceEnergyManagementModeInstance =
        std::make_unique<ModeBase::Instance>(gDeviceEnergyManagementModeDelegate.get(), 0x1, DeviceEnergyManagementMode::Id, 0);
    gDeviceEnergyManagementModeInstance->Init();
}
