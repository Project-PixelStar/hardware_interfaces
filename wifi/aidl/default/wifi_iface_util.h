/*
 * Copyright (C) 2022 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WIFI_IFACE_UTIL_H_
#define WIFI_IFACE_UTIL_H_

#include <aidl/android/hardware/wifi/IWifi.h>
#include <wifi_system/interface_tool.h>

#include "wifi_legacy_hal.h"

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace iface_util {

// Iface event handlers.
struct IfaceEventHandlers {
    // Callback to be invoked when the iface is set down & up for MAC address
    // change.
    std::function<void(const std::string& iface_name)> on_state_toggle_off_on;
};

/**
 * Util class for common iface operations.
 */
class WifiIfaceUtil {
  public:
    WifiIfaceUtil(const std::weak_ptr<::android::wifi_system::InterfaceTool> iface_tool,
                  const std::weak_ptr<legacy_hal::WifiLegacyHal> legacy_hal);
    virtual ~WifiIfaceUtil() = default;

    virtual std::array<uint8_t, 6> getFactoryMacAddress(const std::string& iface_name);
    virtual bool setMacAddress(const std::string& iface_name, const std::array<uint8_t, 6>& mac);
    // Get or create a random MAC address. The MAC address returned from
    // this method will remain the same throughout the lifetime of the HAL
    // daemon. (So, changes on every reboot)
    virtual std::array<uint8_t, 6> getOrCreateRandomMacAddress();

    // Register for any iface event callbacks for the provided interface.
    virtual void registerIfaceEventHandlers(const std::string& iface_name,
                                            IfaceEventHandlers handlers);
    virtual void unregisterIfaceEventHandlers(const std::string& iface_name);
    virtual bool setUpState(const std::string& iface_name, bool request_up);
    virtual unsigned ifNameToIndex(const std::string& iface_name);

    virtual bool createBridge(const std::string& br_name);

    virtual bool deleteBridge(const std::string& br_name);

    virtual bool addIfaceToBridge(const std::string& br_name, const std::string& if_name);

    virtual bool removeIfaceFromBridge(const std::string& br_name, const std::string& if_name);
    // Get a random MAC address.
    virtual std::array<uint8_t, 6> createRandomMacAddress();

  private:
    std::weak_ptr<::android::wifi_system::InterfaceTool> iface_tool_;
    std::weak_ptr<legacy_hal::WifiLegacyHal> legacy_hal_;
    std::unique_ptr<std::array<uint8_t, 6>> random_mac_address_;
    std::map<std::string, IfaceEventHandlers> event_handlers_map_;
};

}  // namespace iface_util
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl

#endif  // WIFI_IFACE_UTIL_H_
