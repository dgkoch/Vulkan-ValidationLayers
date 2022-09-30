#pragma once
// *** THIS FILE IS GENERATED - DO NOT EDIT ***
// See lvt_file_generator.py for modifications

/*
 * Copyright (c) 2015-2020 The Khronos Group Inc.
 * Copyright (c) 2015-2020 Valve Corporation
 * Copyright (c) 2015-2020 LunarG, Inc.
 * Copyright (c) 2021-2022 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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
 */


#include <vulkan/vulkan_sc.h>
#include "vk_loader_platform.h"

namespace vk {

extern PFN_vkCreateInstance CreateInstance;
extern PFN_vkDestroyInstance DestroyInstance;
extern PFN_vkEnumeratePhysicalDevices EnumeratePhysicalDevices;
extern PFN_vkGetPhysicalDeviceFeatures GetPhysicalDeviceFeatures;
extern PFN_vkGetPhysicalDeviceFormatProperties GetPhysicalDeviceFormatProperties;
extern PFN_vkGetPhysicalDeviceImageFormatProperties GetPhysicalDeviceImageFormatProperties;
extern PFN_vkGetPhysicalDeviceProperties GetPhysicalDeviceProperties;
extern PFN_vkGetPhysicalDeviceQueueFamilyProperties GetPhysicalDeviceQueueFamilyProperties;
extern PFN_vkGetPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties;
extern PFN_vkGetInstanceProcAddr GetInstanceProcAddr;
extern PFN_vkGetDeviceProcAddr GetDeviceProcAddr;
extern PFN_vkCreateDevice CreateDevice;
extern PFN_vkDestroyDevice DestroyDevice;
extern PFN_vkEnumerateInstanceExtensionProperties EnumerateInstanceExtensionProperties;
extern PFN_vkEnumerateDeviceExtensionProperties EnumerateDeviceExtensionProperties;
extern PFN_vkEnumerateInstanceLayerProperties EnumerateInstanceLayerProperties;
extern PFN_vkEnumerateDeviceLayerProperties EnumerateDeviceLayerProperties;
extern PFN_vkGetDeviceQueue GetDeviceQueue;
extern PFN_vkQueueSubmit QueueSubmit;
extern PFN_vkQueueWaitIdle QueueWaitIdle;
extern PFN_vkDeviceWaitIdle DeviceWaitIdle;
extern PFN_vkAllocateMemory AllocateMemory;
extern PFN_vkMapMemory MapMemory;
extern PFN_vkUnmapMemory UnmapMemory;
extern PFN_vkFlushMappedMemoryRanges FlushMappedMemoryRanges;
extern PFN_vkInvalidateMappedMemoryRanges InvalidateMappedMemoryRanges;
extern PFN_vkGetDeviceMemoryCommitment GetDeviceMemoryCommitment;
extern PFN_vkBindBufferMemory BindBufferMemory;
extern PFN_vkBindImageMemory BindImageMemory;
extern PFN_vkGetBufferMemoryRequirements GetBufferMemoryRequirements;
extern PFN_vkGetImageMemoryRequirements GetImageMemoryRequirements;
extern PFN_vkCreateFence CreateFence;
extern PFN_vkDestroyFence DestroyFence;
extern PFN_vkResetFences ResetFences;
extern PFN_vkGetFenceStatus GetFenceStatus;
extern PFN_vkWaitForFences WaitForFences;
extern PFN_vkCreateSemaphore CreateSemaphore;
extern PFN_vkDestroySemaphore DestroySemaphore;
extern PFN_vkCreateEvent CreateEvent;
extern PFN_vkDestroyEvent DestroyEvent;
extern PFN_vkGetEventStatus GetEventStatus;
extern PFN_vkSetEvent SetEvent;
extern PFN_vkResetEvent ResetEvent;
extern PFN_vkCreateQueryPool CreateQueryPool;
extern PFN_vkGetQueryPoolResults GetQueryPoolResults;
extern PFN_vkCreateBuffer CreateBuffer;
extern PFN_vkDestroyBuffer DestroyBuffer;
extern PFN_vkCreateBufferView CreateBufferView;
extern PFN_vkDestroyBufferView DestroyBufferView;
extern PFN_vkCreateImage CreateImage;
extern PFN_vkDestroyImage DestroyImage;
extern PFN_vkGetImageSubresourceLayout GetImageSubresourceLayout;
extern PFN_vkCreateImageView CreateImageView;
extern PFN_vkDestroyImageView DestroyImageView;
extern PFN_vkCreatePipelineCache CreatePipelineCache;
extern PFN_vkDestroyPipelineCache DestroyPipelineCache;
extern PFN_vkCreateGraphicsPipelines CreateGraphicsPipelines;
extern PFN_vkCreateComputePipelines CreateComputePipelines;
extern PFN_vkDestroyPipeline DestroyPipeline;
extern PFN_vkCreatePipelineLayout CreatePipelineLayout;
extern PFN_vkDestroyPipelineLayout DestroyPipelineLayout;
extern PFN_vkCreateSampler CreateSampler;
extern PFN_vkDestroySampler DestroySampler;
extern PFN_vkCreateDescriptorSetLayout CreateDescriptorSetLayout;
extern PFN_vkDestroyDescriptorSetLayout DestroyDescriptorSetLayout;
extern PFN_vkCreateDescriptorPool CreateDescriptorPool;
extern PFN_vkResetDescriptorPool ResetDescriptorPool;
extern PFN_vkAllocateDescriptorSets AllocateDescriptorSets;
extern PFN_vkFreeDescriptorSets FreeDescriptorSets;
extern PFN_vkUpdateDescriptorSets UpdateDescriptorSets;
extern PFN_vkCreateFramebuffer CreateFramebuffer;
extern PFN_vkDestroyFramebuffer DestroyFramebuffer;
extern PFN_vkCreateRenderPass CreateRenderPass;
extern PFN_vkDestroyRenderPass DestroyRenderPass;
extern PFN_vkGetRenderAreaGranularity GetRenderAreaGranularity;
extern PFN_vkCreateCommandPool CreateCommandPool;
extern PFN_vkResetCommandPool ResetCommandPool;
extern PFN_vkAllocateCommandBuffers AllocateCommandBuffers;
extern PFN_vkFreeCommandBuffers FreeCommandBuffers;
extern PFN_vkBeginCommandBuffer BeginCommandBuffer;
extern PFN_vkEndCommandBuffer EndCommandBuffer;
extern PFN_vkResetCommandBuffer ResetCommandBuffer;
extern PFN_vkCmdBindPipeline CmdBindPipeline;
extern PFN_vkCmdSetViewport CmdSetViewport;
extern PFN_vkCmdSetScissor CmdSetScissor;
extern PFN_vkCmdSetLineWidth CmdSetLineWidth;
extern PFN_vkCmdSetDepthBias CmdSetDepthBias;
extern PFN_vkCmdSetBlendConstants CmdSetBlendConstants;
extern PFN_vkCmdSetDepthBounds CmdSetDepthBounds;
extern PFN_vkCmdSetStencilCompareMask CmdSetStencilCompareMask;
extern PFN_vkCmdSetStencilWriteMask CmdSetStencilWriteMask;
extern PFN_vkCmdSetStencilReference CmdSetStencilReference;
extern PFN_vkCmdBindDescriptorSets CmdBindDescriptorSets;
extern PFN_vkCmdBindIndexBuffer CmdBindIndexBuffer;
extern PFN_vkCmdBindVertexBuffers CmdBindVertexBuffers;
extern PFN_vkCmdDraw CmdDraw;
extern PFN_vkCmdDrawIndexed CmdDrawIndexed;
extern PFN_vkCmdDrawIndirect CmdDrawIndirect;
extern PFN_vkCmdDrawIndexedIndirect CmdDrawIndexedIndirect;
extern PFN_vkCmdDispatch CmdDispatch;
extern PFN_vkCmdDispatchIndirect CmdDispatchIndirect;
extern PFN_vkCmdCopyBuffer CmdCopyBuffer;
extern PFN_vkCmdCopyImage CmdCopyImage;
extern PFN_vkCmdBlitImage CmdBlitImage;
extern PFN_vkCmdCopyBufferToImage CmdCopyBufferToImage;
extern PFN_vkCmdCopyImageToBuffer CmdCopyImageToBuffer;
extern PFN_vkCmdUpdateBuffer CmdUpdateBuffer;
extern PFN_vkCmdFillBuffer CmdFillBuffer;
extern PFN_vkCmdClearColorImage CmdClearColorImage;
extern PFN_vkCmdClearDepthStencilImage CmdClearDepthStencilImage;
extern PFN_vkCmdClearAttachments CmdClearAttachments;
extern PFN_vkCmdResolveImage CmdResolveImage;
extern PFN_vkCmdSetEvent CmdSetEvent;
extern PFN_vkCmdResetEvent CmdResetEvent;
extern PFN_vkCmdWaitEvents CmdWaitEvents;
extern PFN_vkCmdPipelineBarrier CmdPipelineBarrier;
extern PFN_vkCmdBeginQuery CmdBeginQuery;
extern PFN_vkCmdEndQuery CmdEndQuery;
extern PFN_vkCmdResetQueryPool CmdResetQueryPool;
extern PFN_vkCmdWriteTimestamp CmdWriteTimestamp;
extern PFN_vkCmdCopyQueryPoolResults CmdCopyQueryPoolResults;
extern PFN_vkCmdPushConstants CmdPushConstants;
extern PFN_vkCmdBeginRenderPass CmdBeginRenderPass;
extern PFN_vkCmdNextSubpass CmdNextSubpass;
extern PFN_vkCmdEndRenderPass CmdEndRenderPass;
extern PFN_vkCmdExecuteCommands CmdExecuteCommands;
extern PFN_vkEnumerateInstanceVersion EnumerateInstanceVersion;
extern PFN_vkBindBufferMemory2 BindBufferMemory2;
extern PFN_vkBindImageMemory2 BindImageMemory2;
extern PFN_vkGetDeviceGroupPeerMemoryFeatures GetDeviceGroupPeerMemoryFeatures;
extern PFN_vkCmdSetDeviceMask CmdSetDeviceMask;
extern PFN_vkCmdDispatchBase CmdDispatchBase;
extern PFN_vkEnumeratePhysicalDeviceGroups EnumeratePhysicalDeviceGroups;
extern PFN_vkGetImageMemoryRequirements2 GetImageMemoryRequirements2;
extern PFN_vkGetBufferMemoryRequirements2 GetBufferMemoryRequirements2;
extern PFN_vkGetPhysicalDeviceFeatures2 GetPhysicalDeviceFeatures2;
extern PFN_vkGetPhysicalDeviceProperties2 GetPhysicalDeviceProperties2;
extern PFN_vkGetPhysicalDeviceFormatProperties2 GetPhysicalDeviceFormatProperties2;
extern PFN_vkGetPhysicalDeviceImageFormatProperties2 GetPhysicalDeviceImageFormatProperties2;
extern PFN_vkGetPhysicalDeviceQueueFamilyProperties2 GetPhysicalDeviceQueueFamilyProperties2;
extern PFN_vkGetPhysicalDeviceMemoryProperties2 GetPhysicalDeviceMemoryProperties2;
extern PFN_vkGetDeviceQueue2 GetDeviceQueue2;
extern PFN_vkCreateSamplerYcbcrConversion CreateSamplerYcbcrConversion;
extern PFN_vkDestroySamplerYcbcrConversion DestroySamplerYcbcrConversion;
extern PFN_vkGetPhysicalDeviceExternalBufferProperties GetPhysicalDeviceExternalBufferProperties;
extern PFN_vkGetPhysicalDeviceExternalFenceProperties GetPhysicalDeviceExternalFenceProperties;
extern PFN_vkGetPhysicalDeviceExternalSemaphoreProperties GetPhysicalDeviceExternalSemaphoreProperties;
extern PFN_vkGetDescriptorSetLayoutSupport GetDescriptorSetLayoutSupport;
extern PFN_vkCmdDrawIndirectCount CmdDrawIndirectCount;
extern PFN_vkCmdDrawIndexedIndirectCount CmdDrawIndexedIndirectCount;
extern PFN_vkCreateRenderPass2 CreateRenderPass2;
extern PFN_vkCmdBeginRenderPass2 CmdBeginRenderPass2;
extern PFN_vkCmdNextSubpass2 CmdNextSubpass2;
extern PFN_vkCmdEndRenderPass2 CmdEndRenderPass2;
extern PFN_vkResetQueryPool ResetQueryPool;
extern PFN_vkGetSemaphoreCounterValue GetSemaphoreCounterValue;
extern PFN_vkWaitSemaphores WaitSemaphores;
extern PFN_vkSignalSemaphore SignalSemaphore;
extern PFN_vkGetBufferDeviceAddress GetBufferDeviceAddress;
extern PFN_vkGetBufferOpaqueCaptureAddress GetBufferOpaqueCaptureAddress;
extern PFN_vkGetDeviceMemoryOpaqueCaptureAddress GetDeviceMemoryOpaqueCaptureAddress;
extern PFN_vkDestroySurfaceKHR DestroySurfaceKHR;
extern PFN_vkGetPhysicalDeviceSurfaceSupportKHR GetPhysicalDeviceSurfaceSupportKHR;
extern PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR GetPhysicalDeviceSurfaceCapabilitiesKHR;
extern PFN_vkGetPhysicalDeviceSurfaceFormatsKHR GetPhysicalDeviceSurfaceFormatsKHR;
extern PFN_vkGetPhysicalDeviceSurfacePresentModesKHR GetPhysicalDeviceSurfacePresentModesKHR;
extern PFN_vkCreateSwapchainKHR CreateSwapchainKHR;
extern PFN_vkGetSwapchainImagesKHR GetSwapchainImagesKHR;
extern PFN_vkAcquireNextImageKHR AcquireNextImageKHR;
extern PFN_vkQueuePresentKHR QueuePresentKHR;
extern PFN_vkGetDeviceGroupPresentCapabilitiesKHR GetDeviceGroupPresentCapabilitiesKHR;
extern PFN_vkGetDeviceGroupSurfacePresentModesKHR GetDeviceGroupSurfacePresentModesKHR;
extern PFN_vkGetPhysicalDevicePresentRectanglesKHR GetPhysicalDevicePresentRectanglesKHR;
extern PFN_vkAcquireNextImage2KHR AcquireNextImage2KHR;
extern PFN_vkGetPhysicalDeviceDisplayPropertiesKHR GetPhysicalDeviceDisplayPropertiesKHR;
extern PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR GetPhysicalDeviceDisplayPlanePropertiesKHR;
extern PFN_vkGetDisplayPlaneSupportedDisplaysKHR GetDisplayPlaneSupportedDisplaysKHR;
extern PFN_vkGetDisplayModePropertiesKHR GetDisplayModePropertiesKHR;
extern PFN_vkCreateDisplayModeKHR CreateDisplayModeKHR;
extern PFN_vkGetDisplayPlaneCapabilitiesKHR GetDisplayPlaneCapabilitiesKHR;
extern PFN_vkCreateDisplayPlaneSurfaceKHR CreateDisplayPlaneSurfaceKHR;

void InitDispatchTable();

} // namespace vk
