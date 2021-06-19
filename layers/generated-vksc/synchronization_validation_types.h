// *** THIS FILE IS GENERATED - DO NOT EDIT ***
// See helper_file_generator.py for modifications


/***************************************************************************
 *
 * Copyright (c) 2015-2021 The Khronos Group Inc.
 * Copyright (c) 2015-2021 Valve Corporation
 * Copyright (c) 2015-2021 LunarG, Inc.
 * Copyright (c) 2015-2021 Google Inc.
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
 * Author: Mark Lobodzinski <mark@lunarg.com>
 * Author: Courtney Goeltzenleuchter <courtneygo@google.com>
 * Author: Tobin Ehlis <tobine@google.com>
 * Author: Chris Forbes <chrisforbes@google.com>
 * Author: John Zulauf<jzulauf@lunarg.com>
 *
 ****************************************************************************/

#pragma once

#include <array>
#include <bitset>
#include <map>
#include <stdint.h>
#include <vulkan/vulkan_sc.h>
#include "vk_layer_data.h"
using SyncStageAccessFlags = std::bitset<128>;

// clang-format off

// Unique number for each  stage/access combination
enum SyncStageAccessIndex {
    SYNC_ACCESS_INDEX_NONE = 0,
    SYNC_DRAW_INDIRECT_INDIRECT_COMMAND_READ = 1,
    SYNC_DRAW_INDIRECT_TRANSFORM_FEEDBACK_COUNTER_READ_EXT = 2,
    SYNC_INDEX_INPUT_INDEX_READ = 3,
    SYNC_VERTEX_ATTRIBUTE_INPUT_VERTEX_ATTRIBUTE_READ = 4,
    SYNC_VERTEX_SHADER_SHADER_SAMPLED_READ = 5,
    SYNC_VERTEX_SHADER_SHADER_STORAGE_READ = 6,
    SYNC_VERTEX_SHADER_SHADER_STORAGE_WRITE = 7,
    SYNC_VERTEX_SHADER_UNIFORM_READ = 8,
    SYNC_TESSELLATION_CONTROL_SHADER_SHADER_SAMPLED_READ = 9,
    SYNC_TESSELLATION_CONTROL_SHADER_SHADER_STORAGE_READ = 10,
    SYNC_TESSELLATION_CONTROL_SHADER_SHADER_STORAGE_WRITE = 11,
    SYNC_TESSELLATION_CONTROL_SHADER_UNIFORM_READ = 12,
    SYNC_TESSELLATION_EVALUATION_SHADER_SHADER_SAMPLED_READ = 13,
    SYNC_TESSELLATION_EVALUATION_SHADER_SHADER_STORAGE_READ = 14,
    SYNC_TESSELLATION_EVALUATION_SHADER_SHADER_STORAGE_WRITE = 15,
    SYNC_TESSELLATION_EVALUATION_SHADER_UNIFORM_READ = 16,
    SYNC_GEOMETRY_SHADER_SHADER_SAMPLED_READ = 17,
    SYNC_GEOMETRY_SHADER_SHADER_STORAGE_READ = 18,
    SYNC_GEOMETRY_SHADER_SHADER_STORAGE_WRITE = 19,
    SYNC_GEOMETRY_SHADER_UNIFORM_READ = 20,
    SYNC_TRANSFORM_FEEDBACK_EXT_TRANSFORM_FEEDBACK_COUNTER_READ_EXT = 21,
    SYNC_TRANSFORM_FEEDBACK_EXT_TRANSFORM_FEEDBACK_COUNTER_WRITE_EXT = 22,
    SYNC_TRANSFORM_FEEDBACK_EXT_TRANSFORM_FEEDBACK_WRITE_EXT = 23,
    SYNC_FRAGMENT_DENSITY_PROCESS_EXT_FRAGMENT_DENSITY_MAP_READ_EXT = 24,
    SYNC_TASK_SHADER_NV_SHADER_SAMPLED_READ = 25,
    SYNC_TASK_SHADER_NV_SHADER_STORAGE_READ = 26,
    SYNC_TASK_SHADER_NV_SHADER_STORAGE_WRITE = 27,
    SYNC_TASK_SHADER_NV_UNIFORM_READ = 28,
    SYNC_MESH_SHADER_NV_SHADER_SAMPLED_READ = 29,
    SYNC_MESH_SHADER_NV_SHADER_STORAGE_READ = 30,
    SYNC_MESH_SHADER_NV_SHADER_STORAGE_WRITE = 31,
    SYNC_MESH_SHADER_NV_UNIFORM_READ = 32,
    SYNC_FRAGMENT_SHADING_RATE_ATTACHMENT_FRAGMENT_SHADING_RATE_ATTACHMENT_READ = 33,
    SYNC_EARLY_FRAGMENT_TESTS_DEPTH_STENCIL_ATTACHMENT_READ = 34,
    SYNC_EARLY_FRAGMENT_TESTS_DEPTH_STENCIL_ATTACHMENT_WRITE = 35,
    SYNC_FRAGMENT_SHADER_INPUT_ATTACHMENT_READ = 36,
    SYNC_FRAGMENT_SHADER_SHADER_SAMPLED_READ = 37,
    SYNC_FRAGMENT_SHADER_SHADER_STORAGE_READ = 38,
    SYNC_FRAGMENT_SHADER_SHADER_STORAGE_WRITE = 39,
    SYNC_FRAGMENT_SHADER_UNIFORM_READ = 40,
    SYNC_LATE_FRAGMENT_TESTS_DEPTH_STENCIL_ATTACHMENT_READ = 41,
    SYNC_LATE_FRAGMENT_TESTS_DEPTH_STENCIL_ATTACHMENT_WRITE = 42,
    SYNC_COLOR_ATTACHMENT_OUTPUT_COLOR_ATTACHMENT_READ = 43,
    SYNC_COLOR_ATTACHMENT_OUTPUT_COLOR_ATTACHMENT_READ_NONCOHERENT_EXT = 44,
    SYNC_COLOR_ATTACHMENT_OUTPUT_COLOR_ATTACHMENT_WRITE = 45,
    SYNC_COMPUTE_SHADER_SHADER_SAMPLED_READ = 46,
    SYNC_COMPUTE_SHADER_SHADER_STORAGE_READ = 47,
    SYNC_COMPUTE_SHADER_SHADER_STORAGE_WRITE = 48,
    SYNC_COMPUTE_SHADER_UNIFORM_READ = 49,
    SYNC_COPY_TRANSFER_READ = 50,
    SYNC_COPY_TRANSFER_WRITE = 51,
    SYNC_RESOLVE_TRANSFER_READ = 52,
    SYNC_RESOLVE_TRANSFER_WRITE = 53,
    SYNC_BLIT_TRANSFER_READ = 54,
    SYNC_BLIT_TRANSFER_WRITE = 55,
    SYNC_CLEAR_TRANSFER_WRITE = 56,
    SYNC_COMMAND_PREPROCESS_NV_COMMAND_PREPROCESS_READ_NV = 57,
    SYNC_COMMAND_PREPROCESS_NV_COMMAND_PREPROCESS_WRITE_NV = 58,
    SYNC_CONDITIONAL_RENDERING_EXT_CONDITIONAL_RENDERING_READ_EXT = 59,
    SYNC_RAY_TRACING_SHADER_ACCELERATION_STRUCTURE_READ = 60,
    SYNC_RAY_TRACING_SHADER_SHADER_SAMPLED_READ = 61,
    SYNC_RAY_TRACING_SHADER_SHADER_STORAGE_READ = 62,
    SYNC_RAY_TRACING_SHADER_SHADER_STORAGE_WRITE = 63,
    SYNC_RAY_TRACING_SHADER_UNIFORM_READ = 64,
    SYNC_ACCELERATION_STRUCTURE_BUILD_ACCELERATION_STRUCTURE_READ = 65,
    SYNC_ACCELERATION_STRUCTURE_BUILD_ACCELERATION_STRUCTURE_WRITE = 66,
    SYNC_ACCELERATION_STRUCTURE_BUILD_INDIRECT_COMMAND_READ = 67,
    SYNC_ACCELERATION_STRUCTURE_BUILD_SHADER_SAMPLED_READ = 68,
    SYNC_ACCELERATION_STRUCTURE_BUILD_SHADER_STORAGE_READ = 69,
    SYNC_ACCELERATION_STRUCTURE_BUILD_TRANSFER_READ = 70,
    SYNC_ACCELERATION_STRUCTURE_BUILD_TRANSFER_WRITE = 71,
    SYNC_ACCELERATION_STRUCTURE_BUILD_UNIFORM_READ = 72,
    SYNC_VIDEO_DECODE_VIDEO_DECODE_READ = 73,
    SYNC_VIDEO_DECODE_VIDEO_DECODE_WRITE = 74,
    SYNC_VIDEO_ENCODE_VIDEO_ENCODE_READ = 75,
    SYNC_VIDEO_ENCODE_VIDEO_ENCODE_WRITE = 76,
    SYNC_SUBPASS_SHADING_HUAWEI_INPUT_ATTACHMENT_READ = 77,
    SYNC_HOST_HOST_READ = 78,
    SYNC_HOST_HOST_WRITE = 79,
    SYNC_IMAGE_LAYOUT_TRANSITION = 80,
    SYNC_QUEUE_FAMILY_OWNERSHIP_TRANSFER = 81,
};

// Unique bit for each  stage/access combination
static const SyncStageAccessFlags SYNC_DRAW_INDIRECT_INDIRECT_COMMAND_READ_BIT = (SyncStageAccessFlags(1) << SYNC_DRAW_INDIRECT_INDIRECT_COMMAND_READ);
static const SyncStageAccessFlags SYNC_DRAW_INDIRECT_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT = (SyncStageAccessFlags(1) << SYNC_DRAW_INDIRECT_TRANSFORM_FEEDBACK_COUNTER_READ_EXT);
static const SyncStageAccessFlags SYNC_INDEX_INPUT_INDEX_READ_BIT = (SyncStageAccessFlags(1) << SYNC_INDEX_INPUT_INDEX_READ);
static const SyncStageAccessFlags SYNC_VERTEX_ATTRIBUTE_INPUT_VERTEX_ATTRIBUTE_READ_BIT = (SyncStageAccessFlags(1) << SYNC_VERTEX_ATTRIBUTE_INPUT_VERTEX_ATTRIBUTE_READ);
static const SyncStageAccessFlags SYNC_VERTEX_SHADER_SHADER_SAMPLED_READ_BIT = (SyncStageAccessFlags(1) << SYNC_VERTEX_SHADER_SHADER_SAMPLED_READ);
static const SyncStageAccessFlags SYNC_VERTEX_SHADER_SHADER_STORAGE_READ_BIT = (SyncStageAccessFlags(1) << SYNC_VERTEX_SHADER_SHADER_STORAGE_READ);
static const SyncStageAccessFlags SYNC_VERTEX_SHADER_SHADER_STORAGE_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_VERTEX_SHADER_SHADER_STORAGE_WRITE);
static const SyncStageAccessFlags SYNC_VERTEX_SHADER_UNIFORM_READ_BIT = (SyncStageAccessFlags(1) << SYNC_VERTEX_SHADER_UNIFORM_READ);
static const SyncStageAccessFlags SYNC_TESSELLATION_CONTROL_SHADER_SHADER_SAMPLED_READ_BIT = (SyncStageAccessFlags(1) << SYNC_TESSELLATION_CONTROL_SHADER_SHADER_SAMPLED_READ);
static const SyncStageAccessFlags SYNC_TESSELLATION_CONTROL_SHADER_SHADER_STORAGE_READ_BIT = (SyncStageAccessFlags(1) << SYNC_TESSELLATION_CONTROL_SHADER_SHADER_STORAGE_READ);
static const SyncStageAccessFlags SYNC_TESSELLATION_CONTROL_SHADER_SHADER_STORAGE_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_TESSELLATION_CONTROL_SHADER_SHADER_STORAGE_WRITE);
static const SyncStageAccessFlags SYNC_TESSELLATION_CONTROL_SHADER_UNIFORM_READ_BIT = (SyncStageAccessFlags(1) << SYNC_TESSELLATION_CONTROL_SHADER_UNIFORM_READ);
static const SyncStageAccessFlags SYNC_TESSELLATION_EVALUATION_SHADER_SHADER_SAMPLED_READ_BIT = (SyncStageAccessFlags(1) << SYNC_TESSELLATION_EVALUATION_SHADER_SHADER_SAMPLED_READ);
static const SyncStageAccessFlags SYNC_TESSELLATION_EVALUATION_SHADER_SHADER_STORAGE_READ_BIT = (SyncStageAccessFlags(1) << SYNC_TESSELLATION_EVALUATION_SHADER_SHADER_STORAGE_READ);
static const SyncStageAccessFlags SYNC_TESSELLATION_EVALUATION_SHADER_SHADER_STORAGE_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_TESSELLATION_EVALUATION_SHADER_SHADER_STORAGE_WRITE);
static const SyncStageAccessFlags SYNC_TESSELLATION_EVALUATION_SHADER_UNIFORM_READ_BIT = (SyncStageAccessFlags(1) << SYNC_TESSELLATION_EVALUATION_SHADER_UNIFORM_READ);
static const SyncStageAccessFlags SYNC_GEOMETRY_SHADER_SHADER_SAMPLED_READ_BIT = (SyncStageAccessFlags(1) << SYNC_GEOMETRY_SHADER_SHADER_SAMPLED_READ);
static const SyncStageAccessFlags SYNC_GEOMETRY_SHADER_SHADER_STORAGE_READ_BIT = (SyncStageAccessFlags(1) << SYNC_GEOMETRY_SHADER_SHADER_STORAGE_READ);
static const SyncStageAccessFlags SYNC_GEOMETRY_SHADER_SHADER_STORAGE_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_GEOMETRY_SHADER_SHADER_STORAGE_WRITE);
static const SyncStageAccessFlags SYNC_GEOMETRY_SHADER_UNIFORM_READ_BIT = (SyncStageAccessFlags(1) << SYNC_GEOMETRY_SHADER_UNIFORM_READ);
static const SyncStageAccessFlags SYNC_TRANSFORM_FEEDBACK_BIT_EXT_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT = (SyncStageAccessFlags(1) << SYNC_TRANSFORM_FEEDBACK_EXT_TRANSFORM_FEEDBACK_COUNTER_READ_EXT);
static const SyncStageAccessFlags SYNC_TRANSFORM_FEEDBACK_BIT_EXT_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT = (SyncStageAccessFlags(1) << SYNC_TRANSFORM_FEEDBACK_EXT_TRANSFORM_FEEDBACK_COUNTER_WRITE_EXT);
static const SyncStageAccessFlags SYNC_TRANSFORM_FEEDBACK_BIT_EXT_TRANSFORM_FEEDBACK_WRITE_BIT_EXT = (SyncStageAccessFlags(1) << SYNC_TRANSFORM_FEEDBACK_EXT_TRANSFORM_FEEDBACK_WRITE_EXT);
static const SyncStageAccessFlags SYNC_FRAGMENT_DENSITY_PROCESS_BIT_EXT_FRAGMENT_DENSITY_MAP_READ_BIT_EXT = (SyncStageAccessFlags(1) << SYNC_FRAGMENT_DENSITY_PROCESS_EXT_FRAGMENT_DENSITY_MAP_READ_EXT);
static const SyncStageAccessFlags SYNC_TASK_SHADER_NV_SHADER_SAMPLED_READ_BIT = (SyncStageAccessFlags(1) << SYNC_TASK_SHADER_NV_SHADER_SAMPLED_READ);
static const SyncStageAccessFlags SYNC_TASK_SHADER_NV_SHADER_STORAGE_READ_BIT = (SyncStageAccessFlags(1) << SYNC_TASK_SHADER_NV_SHADER_STORAGE_READ);
static const SyncStageAccessFlags SYNC_TASK_SHADER_NV_SHADER_STORAGE_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_TASK_SHADER_NV_SHADER_STORAGE_WRITE);
static const SyncStageAccessFlags SYNC_TASK_SHADER_NV_UNIFORM_READ_BIT = (SyncStageAccessFlags(1) << SYNC_TASK_SHADER_NV_UNIFORM_READ);
static const SyncStageAccessFlags SYNC_MESH_SHADER_NV_SHADER_SAMPLED_READ_BIT = (SyncStageAccessFlags(1) << SYNC_MESH_SHADER_NV_SHADER_SAMPLED_READ);
static const SyncStageAccessFlags SYNC_MESH_SHADER_NV_SHADER_STORAGE_READ_BIT = (SyncStageAccessFlags(1) << SYNC_MESH_SHADER_NV_SHADER_STORAGE_READ);
static const SyncStageAccessFlags SYNC_MESH_SHADER_NV_SHADER_STORAGE_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_MESH_SHADER_NV_SHADER_STORAGE_WRITE);
static const SyncStageAccessFlags SYNC_MESH_SHADER_NV_UNIFORM_READ_BIT = (SyncStageAccessFlags(1) << SYNC_MESH_SHADER_NV_UNIFORM_READ);
static const SyncStageAccessFlags SYNC_FRAGMENT_SHADING_RATE_ATTACHMENT_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT = (SyncStageAccessFlags(1) << SYNC_FRAGMENT_SHADING_RATE_ATTACHMENT_FRAGMENT_SHADING_RATE_ATTACHMENT_READ);
static const SyncStageAccessFlags SYNC_EARLY_FRAGMENT_TESTS_DEPTH_STENCIL_ATTACHMENT_READ_BIT = (SyncStageAccessFlags(1) << SYNC_EARLY_FRAGMENT_TESTS_DEPTH_STENCIL_ATTACHMENT_READ);
static const SyncStageAccessFlags SYNC_EARLY_FRAGMENT_TESTS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_EARLY_FRAGMENT_TESTS_DEPTH_STENCIL_ATTACHMENT_WRITE);
static const SyncStageAccessFlags SYNC_FRAGMENT_SHADER_INPUT_ATTACHMENT_READ_BIT = (SyncStageAccessFlags(1) << SYNC_FRAGMENT_SHADER_INPUT_ATTACHMENT_READ);
static const SyncStageAccessFlags SYNC_FRAGMENT_SHADER_SHADER_SAMPLED_READ_BIT = (SyncStageAccessFlags(1) << SYNC_FRAGMENT_SHADER_SHADER_SAMPLED_READ);
static const SyncStageAccessFlags SYNC_FRAGMENT_SHADER_SHADER_STORAGE_READ_BIT = (SyncStageAccessFlags(1) << SYNC_FRAGMENT_SHADER_SHADER_STORAGE_READ);
static const SyncStageAccessFlags SYNC_FRAGMENT_SHADER_SHADER_STORAGE_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_FRAGMENT_SHADER_SHADER_STORAGE_WRITE);
static const SyncStageAccessFlags SYNC_FRAGMENT_SHADER_UNIFORM_READ_BIT = (SyncStageAccessFlags(1) << SYNC_FRAGMENT_SHADER_UNIFORM_READ);
static const SyncStageAccessFlags SYNC_LATE_FRAGMENT_TESTS_DEPTH_STENCIL_ATTACHMENT_READ_BIT = (SyncStageAccessFlags(1) << SYNC_LATE_FRAGMENT_TESTS_DEPTH_STENCIL_ATTACHMENT_READ);
static const SyncStageAccessFlags SYNC_LATE_FRAGMENT_TESTS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_LATE_FRAGMENT_TESTS_DEPTH_STENCIL_ATTACHMENT_WRITE);
static const SyncStageAccessFlags SYNC_COLOR_ATTACHMENT_OUTPUT_COLOR_ATTACHMENT_READ_BIT = (SyncStageAccessFlags(1) << SYNC_COLOR_ATTACHMENT_OUTPUT_COLOR_ATTACHMENT_READ);
static const SyncStageAccessFlags SYNC_COLOR_ATTACHMENT_OUTPUT_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT = (SyncStageAccessFlags(1) << SYNC_COLOR_ATTACHMENT_OUTPUT_COLOR_ATTACHMENT_READ_NONCOHERENT_EXT);
static const SyncStageAccessFlags SYNC_COLOR_ATTACHMENT_OUTPUT_COLOR_ATTACHMENT_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_COLOR_ATTACHMENT_OUTPUT_COLOR_ATTACHMENT_WRITE);
static const SyncStageAccessFlags SYNC_COMPUTE_SHADER_SHADER_SAMPLED_READ_BIT = (SyncStageAccessFlags(1) << SYNC_COMPUTE_SHADER_SHADER_SAMPLED_READ);
static const SyncStageAccessFlags SYNC_COMPUTE_SHADER_SHADER_STORAGE_READ_BIT = (SyncStageAccessFlags(1) << SYNC_COMPUTE_SHADER_SHADER_STORAGE_READ);
static const SyncStageAccessFlags SYNC_COMPUTE_SHADER_SHADER_STORAGE_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_COMPUTE_SHADER_SHADER_STORAGE_WRITE);
static const SyncStageAccessFlags SYNC_COMPUTE_SHADER_UNIFORM_READ_BIT = (SyncStageAccessFlags(1) << SYNC_COMPUTE_SHADER_UNIFORM_READ);
static const SyncStageAccessFlags SYNC_COPY_TRANSFER_READ_BIT = (SyncStageAccessFlags(1) << SYNC_COPY_TRANSFER_READ);
static const SyncStageAccessFlags SYNC_COPY_TRANSFER_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_COPY_TRANSFER_WRITE);
static const SyncStageAccessFlags SYNC_RESOLVE_TRANSFER_READ_BIT = (SyncStageAccessFlags(1) << SYNC_RESOLVE_TRANSFER_READ);
static const SyncStageAccessFlags SYNC_RESOLVE_TRANSFER_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_RESOLVE_TRANSFER_WRITE);
static const SyncStageAccessFlags SYNC_BLIT_TRANSFER_READ_BIT = (SyncStageAccessFlags(1) << SYNC_BLIT_TRANSFER_READ);
static const SyncStageAccessFlags SYNC_BLIT_TRANSFER_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_BLIT_TRANSFER_WRITE);
static const SyncStageAccessFlags SYNC_CLEAR_TRANSFER_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_CLEAR_TRANSFER_WRITE);
static const SyncStageAccessFlags SYNC_COMMAND_PREPROCESS_BIT_NV_COMMAND_PREPROCESS_READ_BIT_NV = (SyncStageAccessFlags(1) << SYNC_COMMAND_PREPROCESS_NV_COMMAND_PREPROCESS_READ_NV);
static const SyncStageAccessFlags SYNC_COMMAND_PREPROCESS_BIT_NV_COMMAND_PREPROCESS_WRITE_BIT_NV = (SyncStageAccessFlags(1) << SYNC_COMMAND_PREPROCESS_NV_COMMAND_PREPROCESS_WRITE_NV);
static const SyncStageAccessFlags SYNC_CONDITIONAL_RENDERING_BIT_EXT_CONDITIONAL_RENDERING_READ_BIT_EXT = (SyncStageAccessFlags(1) << SYNC_CONDITIONAL_RENDERING_EXT_CONDITIONAL_RENDERING_READ_EXT);
static const SyncStageAccessFlags SYNC_RAY_TRACING_SHADER_ACCELERATION_STRUCTURE_READ_BIT = (SyncStageAccessFlags(1) << SYNC_RAY_TRACING_SHADER_ACCELERATION_STRUCTURE_READ);
static const SyncStageAccessFlags SYNC_RAY_TRACING_SHADER_SHADER_SAMPLED_READ_BIT = (SyncStageAccessFlags(1) << SYNC_RAY_TRACING_SHADER_SHADER_SAMPLED_READ);
static const SyncStageAccessFlags SYNC_RAY_TRACING_SHADER_SHADER_STORAGE_READ_BIT = (SyncStageAccessFlags(1) << SYNC_RAY_TRACING_SHADER_SHADER_STORAGE_READ);
static const SyncStageAccessFlags SYNC_RAY_TRACING_SHADER_SHADER_STORAGE_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_RAY_TRACING_SHADER_SHADER_STORAGE_WRITE);
static const SyncStageAccessFlags SYNC_RAY_TRACING_SHADER_UNIFORM_READ_BIT = (SyncStageAccessFlags(1) << SYNC_RAY_TRACING_SHADER_UNIFORM_READ);
static const SyncStageAccessFlags SYNC_ACCELERATION_STRUCTURE_BUILD_ACCELERATION_STRUCTURE_READ_BIT = (SyncStageAccessFlags(1) << SYNC_ACCELERATION_STRUCTURE_BUILD_ACCELERATION_STRUCTURE_READ);
static const SyncStageAccessFlags SYNC_ACCELERATION_STRUCTURE_BUILD_ACCELERATION_STRUCTURE_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_ACCELERATION_STRUCTURE_BUILD_ACCELERATION_STRUCTURE_WRITE);
static const SyncStageAccessFlags SYNC_ACCELERATION_STRUCTURE_BUILD_INDIRECT_COMMAND_READ_BIT = (SyncStageAccessFlags(1) << SYNC_ACCELERATION_STRUCTURE_BUILD_INDIRECT_COMMAND_READ);
static const SyncStageAccessFlags SYNC_ACCELERATION_STRUCTURE_BUILD_SHADER_SAMPLED_READ_BIT = (SyncStageAccessFlags(1) << SYNC_ACCELERATION_STRUCTURE_BUILD_SHADER_SAMPLED_READ);
static const SyncStageAccessFlags SYNC_ACCELERATION_STRUCTURE_BUILD_SHADER_STORAGE_READ_BIT = (SyncStageAccessFlags(1) << SYNC_ACCELERATION_STRUCTURE_BUILD_SHADER_STORAGE_READ);
static const SyncStageAccessFlags SYNC_ACCELERATION_STRUCTURE_BUILD_TRANSFER_READ_BIT = (SyncStageAccessFlags(1) << SYNC_ACCELERATION_STRUCTURE_BUILD_TRANSFER_READ);
static const SyncStageAccessFlags SYNC_ACCELERATION_STRUCTURE_BUILD_TRANSFER_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_ACCELERATION_STRUCTURE_BUILD_TRANSFER_WRITE);
static const SyncStageAccessFlags SYNC_ACCELERATION_STRUCTURE_BUILD_UNIFORM_READ_BIT = (SyncStageAccessFlags(1) << SYNC_ACCELERATION_STRUCTURE_BUILD_UNIFORM_READ);
static const SyncStageAccessFlags SYNC_VIDEO_DECODE_VIDEO_DECODE_READ_BIT = (SyncStageAccessFlags(1) << SYNC_VIDEO_DECODE_VIDEO_DECODE_READ);
static const SyncStageAccessFlags SYNC_VIDEO_DECODE_VIDEO_DECODE_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_VIDEO_DECODE_VIDEO_DECODE_WRITE);
static const SyncStageAccessFlags SYNC_VIDEO_ENCODE_VIDEO_ENCODE_READ_BIT = (SyncStageAccessFlags(1) << SYNC_VIDEO_ENCODE_VIDEO_ENCODE_READ);
static const SyncStageAccessFlags SYNC_VIDEO_ENCODE_VIDEO_ENCODE_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_VIDEO_ENCODE_VIDEO_ENCODE_WRITE);
static const SyncStageAccessFlags SYNC_SUBPASS_SHADING_HUAWEI_INPUT_ATTACHMENT_READ_BIT = (SyncStageAccessFlags(1) << SYNC_SUBPASS_SHADING_HUAWEI_INPUT_ATTACHMENT_READ);
static const SyncStageAccessFlags SYNC_HOST_HOST_READ_BIT = (SyncStageAccessFlags(1) << SYNC_HOST_HOST_READ);
static const SyncStageAccessFlags SYNC_HOST_HOST_WRITE_BIT = (SyncStageAccessFlags(1) << SYNC_HOST_HOST_WRITE);
static const SyncStageAccessFlags SYNC_IMAGE_LAYOUT_TRANSITION_BIT = (SyncStageAccessFlags(1) << SYNC_IMAGE_LAYOUT_TRANSITION);
static const SyncStageAccessFlags SYNC_QUEUE_FAMILY_OWNERSHIP_TRANSFER_BIT = (SyncStageAccessFlags(1) << SYNC_QUEUE_FAMILY_OWNERSHIP_TRANSFER);

// Map of the StageAccessIndices from the StageAccess Bit
extern const layer_data::unordered_map<SyncStageAccessFlags, SyncStageAccessIndex> syncStageAccessIndexByStageAccessBit;

struct SyncStageAccessInfoType {
    const char *name;
    VkPipelineStageFlags2KHR stage_mask;
    VkAccessFlags2KHR access_mask;
    SyncStageAccessIndex stage_access_index;
    SyncStageAccessFlags stage_access_bit;
};

// Array of text names and component masks for each stage/access index
extern const std::array<SyncStageAccessInfoType, 82> syncStageAccessInfoByStageAccessIndex;

// Constants defining the mask of all read and write stage_access states
static const SyncStageAccessFlags syncStageAccessReadMask = ( //  Mask of all read StageAccess bits
    SYNC_DRAW_INDIRECT_INDIRECT_COMMAND_READ_BIT |
    SYNC_DRAW_INDIRECT_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT |
    SYNC_INDEX_INPUT_INDEX_READ_BIT |
    SYNC_VERTEX_ATTRIBUTE_INPUT_VERTEX_ATTRIBUTE_READ_BIT |
    SYNC_VERTEX_SHADER_SHADER_SAMPLED_READ_BIT |
    SYNC_VERTEX_SHADER_SHADER_STORAGE_READ_BIT |
    SYNC_VERTEX_SHADER_UNIFORM_READ_BIT |
    SYNC_TESSELLATION_CONTROL_SHADER_SHADER_SAMPLED_READ_BIT |
    SYNC_TESSELLATION_CONTROL_SHADER_SHADER_STORAGE_READ_BIT |
    SYNC_TESSELLATION_CONTROL_SHADER_UNIFORM_READ_BIT |
    SYNC_TESSELLATION_EVALUATION_SHADER_SHADER_SAMPLED_READ_BIT |
    SYNC_TESSELLATION_EVALUATION_SHADER_SHADER_STORAGE_READ_BIT |
    SYNC_TESSELLATION_EVALUATION_SHADER_UNIFORM_READ_BIT |
    SYNC_GEOMETRY_SHADER_SHADER_SAMPLED_READ_BIT |
    SYNC_GEOMETRY_SHADER_SHADER_STORAGE_READ_BIT |
    SYNC_GEOMETRY_SHADER_UNIFORM_READ_BIT |
    SYNC_TRANSFORM_FEEDBACK_BIT_EXT_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT |
    SYNC_FRAGMENT_DENSITY_PROCESS_BIT_EXT_FRAGMENT_DENSITY_MAP_READ_BIT_EXT |
    SYNC_TASK_SHADER_NV_SHADER_SAMPLED_READ_BIT |
    SYNC_TASK_SHADER_NV_SHADER_STORAGE_READ_BIT |
    SYNC_TASK_SHADER_NV_UNIFORM_READ_BIT |
    SYNC_MESH_SHADER_NV_SHADER_SAMPLED_READ_BIT |
    SYNC_MESH_SHADER_NV_SHADER_STORAGE_READ_BIT |
    SYNC_MESH_SHADER_NV_UNIFORM_READ_BIT |
    SYNC_FRAGMENT_SHADING_RATE_ATTACHMENT_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT |
    SYNC_EARLY_FRAGMENT_TESTS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
    SYNC_FRAGMENT_SHADER_INPUT_ATTACHMENT_READ_BIT |
    SYNC_FRAGMENT_SHADER_SHADER_SAMPLED_READ_BIT |
    SYNC_FRAGMENT_SHADER_SHADER_STORAGE_READ_BIT |
    SYNC_FRAGMENT_SHADER_UNIFORM_READ_BIT |
    SYNC_LATE_FRAGMENT_TESTS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
    SYNC_COLOR_ATTACHMENT_OUTPUT_COLOR_ATTACHMENT_READ_BIT |
    SYNC_COLOR_ATTACHMENT_OUTPUT_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT |
    SYNC_COMPUTE_SHADER_SHADER_SAMPLED_READ_BIT |
    SYNC_COMPUTE_SHADER_SHADER_STORAGE_READ_BIT |
    SYNC_COMPUTE_SHADER_UNIFORM_READ_BIT |
    SYNC_COPY_TRANSFER_READ_BIT |
    SYNC_RESOLVE_TRANSFER_READ_BIT |
    SYNC_BLIT_TRANSFER_READ_BIT |
    SYNC_COMMAND_PREPROCESS_BIT_NV_COMMAND_PREPROCESS_READ_BIT_NV |
    SYNC_CONDITIONAL_RENDERING_BIT_EXT_CONDITIONAL_RENDERING_READ_BIT_EXT |
    SYNC_RAY_TRACING_SHADER_ACCELERATION_STRUCTURE_READ_BIT |
    SYNC_RAY_TRACING_SHADER_SHADER_SAMPLED_READ_BIT |
    SYNC_RAY_TRACING_SHADER_SHADER_STORAGE_READ_BIT |
    SYNC_RAY_TRACING_SHADER_UNIFORM_READ_BIT |
    SYNC_ACCELERATION_STRUCTURE_BUILD_ACCELERATION_STRUCTURE_READ_BIT |
    SYNC_ACCELERATION_STRUCTURE_BUILD_INDIRECT_COMMAND_READ_BIT |
    SYNC_ACCELERATION_STRUCTURE_BUILD_SHADER_SAMPLED_READ_BIT |
    SYNC_ACCELERATION_STRUCTURE_BUILD_SHADER_STORAGE_READ_BIT |
    SYNC_ACCELERATION_STRUCTURE_BUILD_TRANSFER_READ_BIT |
    SYNC_ACCELERATION_STRUCTURE_BUILD_UNIFORM_READ_BIT |
    SYNC_VIDEO_DECODE_VIDEO_DECODE_READ_BIT |
    SYNC_VIDEO_ENCODE_VIDEO_ENCODE_READ_BIT |
    SYNC_SUBPASS_SHADING_HUAWEI_INPUT_ATTACHMENT_READ_BIT |
    SYNC_HOST_HOST_READ_BIT
);

static const SyncStageAccessFlags syncStageAccessWriteMask = ( //  Mask of all write StageAccess bits
    SYNC_VERTEX_SHADER_SHADER_STORAGE_WRITE_BIT |
    SYNC_TESSELLATION_CONTROL_SHADER_SHADER_STORAGE_WRITE_BIT |
    SYNC_TESSELLATION_EVALUATION_SHADER_SHADER_STORAGE_WRITE_BIT |
    SYNC_GEOMETRY_SHADER_SHADER_STORAGE_WRITE_BIT |
    SYNC_TRANSFORM_FEEDBACK_BIT_EXT_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT |
    SYNC_TRANSFORM_FEEDBACK_BIT_EXT_TRANSFORM_FEEDBACK_WRITE_BIT_EXT |
    SYNC_TASK_SHADER_NV_SHADER_STORAGE_WRITE_BIT |
    SYNC_MESH_SHADER_NV_SHADER_STORAGE_WRITE_BIT |
    SYNC_EARLY_FRAGMENT_TESTS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
    SYNC_FRAGMENT_SHADER_SHADER_STORAGE_WRITE_BIT |
    SYNC_LATE_FRAGMENT_TESTS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
    SYNC_COLOR_ATTACHMENT_OUTPUT_COLOR_ATTACHMENT_WRITE_BIT |
    SYNC_COMPUTE_SHADER_SHADER_STORAGE_WRITE_BIT |
    SYNC_COPY_TRANSFER_WRITE_BIT |
    SYNC_RESOLVE_TRANSFER_WRITE_BIT |
    SYNC_BLIT_TRANSFER_WRITE_BIT |
    SYNC_CLEAR_TRANSFER_WRITE_BIT |
    SYNC_COMMAND_PREPROCESS_BIT_NV_COMMAND_PREPROCESS_WRITE_BIT_NV |
    SYNC_RAY_TRACING_SHADER_SHADER_STORAGE_WRITE_BIT |
    SYNC_ACCELERATION_STRUCTURE_BUILD_ACCELERATION_STRUCTURE_WRITE_BIT |
    SYNC_ACCELERATION_STRUCTURE_BUILD_TRANSFER_WRITE_BIT |
    SYNC_VIDEO_DECODE_VIDEO_DECODE_WRITE_BIT |
    SYNC_VIDEO_ENCODE_VIDEO_ENCODE_WRITE_BIT |
    SYNC_HOST_HOST_WRITE_BIT |
    SYNC_IMAGE_LAYOUT_TRANSITION_BIT |
    SYNC_QUEUE_FAMILY_OWNERSHIP_TRANSFER_BIT
);

// Bit order mask of stage_access bit for each stage
extern const std::map<VkPipelineStageFlags2KHR, SyncStageAccessFlags> syncStageAccessMaskByStageBit;

// Bit order mask of stage_access bit for each access
extern const std::map<VkAccessFlags2KHR, SyncStageAccessFlags> syncStageAccessMaskByAccessBit;

// stage_access index for each stage and access
extern const std::map<VkPipelineStageFlags2KHR, std::map<VkAccessFlags2KHR, SyncStageAccessIndex>> syncStageAccessIndexByStageAndAccess;

// Direct VkPipelineStageFlags to valid VkAccessFlags lookup table
extern const std::map<VkPipelineStageFlags2KHR, VkAccessFlags2KHR> syncDirectStageToAccessMask;

// Pipeline stages corresponding to VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR for each VkQueueFlagBits
extern const std::map<VkQueueFlagBits, VkPipelineStageFlags2KHR> syncAllCommandStagesByQueueFlags;

// Masks of logically earlier stage flags for a given stage flag
extern const std::map<VkPipelineStageFlags2KHR, VkPipelineStageFlags2KHR> syncLogicallyEarlierStages;

// Masks of logically later stage flags for a given stage flag
extern const std::map<VkPipelineStageFlags2KHR, VkPipelineStageFlags2KHR> syncLogicallyLaterStages;

// Lookup table of stage orderings
extern const std::map<VkPipelineStageFlags2KHR, int> syncStageOrder;

struct SyncShaderStageAccess {
    SyncStageAccessIndex sampled_read;
    SyncStageAccessIndex storage_read;
    SyncStageAccessIndex storage_write;
    SyncStageAccessIndex uniform_read;
};

extern const std::map<VkShaderStageFlagBits, SyncShaderStageAccess> syncStageAccessMaskByShaderStage;
