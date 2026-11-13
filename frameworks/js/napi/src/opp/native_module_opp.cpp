/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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
#ifndef LOG_TAG
#define LOG_TAG "bt_napi_native_module_opp"
#endif

#include "bluetooth_log.h"
#include "napi_bluetooth_opp.h"
#include "hitrace_meter.h"

namespace OHOS {
namespace Bluetooth {
static napi_status RegisterOppEnums(napi_env env, napi_value exports)
{
    // Public wire values carried unchanged by BluetoothOppTransferInformation.
    struct Member { const char *name; int32_t value; };
    static const Member direction[] = {{"OUTBOUND", 0}, {"INBOUND", 1}};
    static const Member statusValues[] = {{"PENDING", 0}, {"RUNNING", 1}, {"FINISH", 2}};
    static const Member result[] = {
        {"SUCCESS", 0}, {"ERROR_UNSUPPORTED_TYPE", 1}, {"ERROR_BAD_REQUEST", 2},
        {"ERROR_NOT_ACCEPTABLE", 3}, {"ERROR_CANCELED", 4}, {"ERROR_CONNECTION_FAILED", 5},
        {"ERROR_TRANSFER_FAILED", 6}, {"ERROR_UNKNOWN", 7},
    };
    struct Enum { const char *name; const Member *members; size_t count; };
    static const Enum enums[] = {
        {"DirectionType", direction, sizeof(direction) / sizeof(direction[0])},
        {"TransferStatus", statusValues, sizeof(statusValues) / sizeof(statusValues[0])},
        {"TransferResult", result, sizeof(result) / sizeof(result[0])},
    };
    for (const auto &entry : enums) {
        napi_value object = nullptr;
        napi_status status = napi_create_object(env, &object);
        if (status != napi_ok) {
            return status;
        }
        for (size_t index = 0; index < entry.count; ++index) {
            napi_value value = nullptr;
            status = napi_create_int32(env, entry.members[index].value, &value);
            if (status != napi_ok) {
                return status;
            }
            status = napi_set_named_property(env, object, entry.members[index].name, value);
            if (status != napi_ok) {
                return status;
            }
        }
        status = napi_set_named_property(env, exports, entry.name, object);
        if (status != napi_ok) {
            return status;
        }
    }
    return napi_ok;
}

EXTERN_C_START
/*
 * Module initialization function
 */
static napi_value Init(napi_env env, napi_value exports)
{
    if (RegisterOppEnums(env, exports) != napi_ok) {
        return nullptr;
    }
    HITRACE_METER_NAME(HITRACE_TAG_OHOS, "opp init");
    HILOGI("-----Opp Init start------");
    napi_property_descriptor desc[] = {};

    napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);

    NapiBluetoothOpp::DefineOppJSClass(env, exports);

    HILOGI("-----Opp Init end------");
    return exports;
}
EXTERN_C_END
/*
 * Module define
 */
static napi_module bluetoothOppModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = NULL,
    .nm_register_func = Init,
    .nm_modname = "bluetooth.opp",
    .nm_priv = ((void *)0),
    .reserved = {0}};
/*
 * Module register function
 */
extern "C" __attribute__((constructor)) void RegisterModule(void)
{
    HILOGI("Register bluetoothModule nm_modname:%{public}s", bluetoothOppModule.nm_modname);
    napi_module_register(&bluetoothOppModule);
}
}  // namespace Bluetooth
}  // namespace OHOS
