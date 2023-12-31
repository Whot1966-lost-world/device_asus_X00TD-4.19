/*
 * Copyright (C) 2019 The LineageOS Project
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

#define LOG_TAG "TouchscreenGestureService"

#include <unordered_map>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <fstream>
#include "TouchscreenGesture.h"

namespace {
typedef struct {
    int32_t keycode;
    const char* name;
} GestureInfo;

// id -> info
const std::unordered_map<int32_t, GestureInfo> kGestureInfoMap = {
    {0, {748, "Gesture C"}},
    {1, {749, "Gesture e"}},
    {2, {750, "Gesture M"}},
    {3, {751, "Gesture O"}},
    {4, {752, "Gesture S"}},
    {5, {753, "Gesture V"}},
    {6, {754, "Gesture W"}},
    {7, {755, "Gesture Z"}},
    {8, {756, "Gesture Swipe Up"}},
    {9, {757, "Gesture Swipe Down"}},
    {10, {758, "Gesture Swipe Left"}},
    {11, {759, "Gesture Swipe Right"}},
};
}  // anonymous namespace

namespace vendor {
namespace lineage {
namespace touch {
namespace V1_0 {
namespace implementation {

static constexpr const char* kGestureNodePath =
    "/sys/kernel/touchpanel/gesture_node";

Return<void> TouchscreenGesture::getSupportedGestures(getSupportedGestures_cb resultCb) {
    std::vector<Gesture> gestures;

    for (const auto& entry : kGestureInfoMap) {
        gestures.push_back({entry.first, entry.second.name, entry.second.keycode});
    }
    resultCb(gestures);

    return Void();
}

Return<bool> TouchscreenGesture::setGestureEnabled(
    const ::vendor::lineage::touch::V1_0::Gesture& gesture, bool enabled) {
    const auto entry = kGestureInfoMap.find(gesture.id);
    if (entry == kGestureInfoMap.end()) {
        return false;
    }

    std::ofstream file(kGestureNodePath);
    file << (enabled ? "1" : "0");
    LOG(DEBUG) << "Wrote file " << kGestureNodePath << " fail " << file.fail();
    return !file.fail();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor
