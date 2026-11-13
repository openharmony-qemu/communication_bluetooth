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
#define LOG_TAG "bt_napi_module_connection"
#endif

#include "bluetooth_log.h"
#include "napi_bluetooth_connection.h"
#include "hitrace_meter.h"

namespace OHOS {
namespace Bluetooth {
static napi_status RegisterControlEnums(napi_env env, napi_value exports)
{
    struct Member { const char *name; int32_t value; };
    static const Member object[] = {
        {"LEFT_EAR", ControlObject::LEFT_EAR},
        {"RIGHT_EAR", ControlObject::RIGHT_EAR},
        {"LEFT_RIGHT_EAR", ControlObject::LEFT_RIGHT_EAR},
    };
    static const Member type[] = {
        {"PLAY", ControlType::PLAY}, {"VIBRATE", ControlType::VIBRATE},
        {"FLASH", ControlType::FLASH}, {"LOCK", ControlType::LOCK}, {"ERASE", ControlType::ERASE},
    };
    static const Member value[] = {
        {"DISABLE", ControlTypeVal::DISABLE}, {"ENABLE", ControlTypeVal::ENABLE}, {"QUERY", ControlTypeVal::QUERY},
    };
    struct Enum { const char *name; const Member *members; size_t count; };
    static const Enum enums[] = {
        {"ControlObject", object, sizeof(object) / sizeof(object[0])},
        {"ControlType", type, sizeof(type) / sizeof(type[0])},
        {"ControlTypeValue", value, sizeof(value) / sizeof(value[0])},
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
    if (RegisterControlEnums(env, exports) != napi_ok) {
        return nullptr;
    }
    HITRACE_METER_NAME(HITRACE_TAG_OHOS, "connection init");
    HILOGD("-----connection Init start------");
    napi_property_descriptor desc[] = {};

    napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);

    DefineConnectionFunctions(env, exports);

    HILOGD("-----connection Init end------");
    return exports;
}
EXTERN_C_END
/*
 * Module define
 */
static napi_module bluetoothConnectionModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = NULL,
    .nm_register_func = Init,
    .nm_modname = "bluetooth.connection",
    .nm_priv = ((void *)0),
    .reserved = {0}};
/*
 * Module register function
 */
extern "C" __attribute__((constructor)) void RegisterModule(void)
{
    HILOGI("Register bluetoothConnectionModule nm_modname:%{public}s", bluetoothConnectionModule.nm_modname);
    napi_module_register(&bluetoothConnectionModule);
}
}  // namespace Bluetooth
}  // namespace OHOS
