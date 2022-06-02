
// This file is ***GENERATED***.  Do Not Edit.
// See layer_chassis_dispatch_generator.py for modifications.

/* Copyright (c) 2015-2021 The Khronos Group Inc.
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
 */

#include <mutex>
#include "chassis.h"
#include "layer_chassis_dispatch.h"
#include "vk_layer_utils.h"
#include "vk_safe_struct.h"

ReadWriteLock dispatch_lock;

// Unique Objects pNext extension handling function
void WrapPnextChainHandles(ValidationObject *layer_data, const void *pNext) {
    void *cur_pnext = const_cast<void *>(pNext);
    while (cur_pnext != NULL) {
        VkBaseOutStructure *header = reinterpret_cast<VkBaseOutStructure *>(cur_pnext);

        switch (header->sType) {
            case VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO: {
                    safe_VkMemoryDedicatedAllocateInfo *safe_struct = reinterpret_cast<safe_VkMemoryDedicatedAllocateInfo *>(cur_pnext);
                    if (safe_struct->image) {
                        safe_struct->image = layer_data->Unwrap(safe_struct->image);
                    }
                    if (safe_struct->buffer) {
                        safe_struct->buffer = layer_data->Unwrap(safe_struct->buffer);
                    }
                } break;

            case VK_STRUCTURE_TYPE_IMAGE_SWAPCHAIN_CREATE_INFO_KHR: {
                    safe_VkImageSwapchainCreateInfoKHR *safe_struct = reinterpret_cast<safe_VkImageSwapchainCreateInfoKHR *>(cur_pnext);
                    if (safe_struct->swapchain) {
                        safe_struct->swapchain = layer_data->Unwrap(safe_struct->swapchain);
                    }
                } break;

            case VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_INFO: {
                    safe_VkSamplerYcbcrConversionInfo *safe_struct = reinterpret_cast<safe_VkSamplerYcbcrConversionInfo *>(cur_pnext);
                    if (safe_struct->conversion) {
                        safe_struct->conversion = layer_data->Unwrap(safe_struct->conversion);
                    }
                } break;

            case VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO: {
                    safe_VkRenderPassAttachmentBeginInfo *safe_struct = reinterpret_cast<safe_VkRenderPassAttachmentBeginInfo *>(cur_pnext);
                    if (safe_struct->pAttachments) {
                        for (uint32_t index0 = 0; index0 < safe_struct->attachmentCount; ++index0) {
                            safe_struct->pAttachments[index0] = layer_data->Unwrap(safe_struct->pAttachments[index0]);
                        }
                    }
                } break;

            case VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_SWAPCHAIN_INFO_KHR: {
                    safe_VkBindImageMemorySwapchainInfoKHR *safe_struct = reinterpret_cast<safe_VkBindImageMemorySwapchainInfoKHR *>(cur_pnext);
                    if (safe_struct->swapchain) {
                        safe_struct->swapchain = layer_data->Unwrap(safe_struct->swapchain);
                    }
                } break;

            default:
                break;
        }

        // Process the next structure in the chain
        cur_pnext = header->pNext;
    }
}


// Manually written Dispatch routines


#define DISPATCH_MAX_STACK_ALLOCATIONS 32

#if defined(VK_EXT_pipeline_creation_feedback)
// The VK_EXT_pipeline_creation_feedback extension returns data from the driver -- we've created a copy of the pnext chain, so
// copy the returned data to the caller before freeing the copy's data.
void CopyCreatePipelineFeedbackData(const void *src_chain, const void *dst_chain) {
    auto src_feedback_struct = LvlFindInChain<VkPipelineCreationFeedbackCreateInfoEXT>(src_chain);
    if (!src_feedback_struct) return;
    auto dst_feedback_struct = const_cast<VkPipelineCreationFeedbackCreateInfoEXT *>(
        LvlFindInChain<VkPipelineCreationFeedbackCreateInfoEXT>(dst_chain));
    *dst_feedback_struct->pPipelineCreationFeedback = *src_feedback_struct->pPipelineCreationFeedback;
    for (uint32_t i = 0; i < src_feedback_struct->pipelineStageCreationFeedbackCount; i++) {
        dst_feedback_struct->pPipelineStageCreationFeedbacks[i] = src_feedback_struct->pPipelineStageCreationFeedbacks[i];
    }
}
#endif

VkResult DispatchCreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount,
                                         const VkGraphicsPipelineCreateInfo *pCreateInfos,
                                         const VkAllocationCallbacks *pAllocator, VkPipeline *pPipelines) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreateGraphicsPipelines(device, pipelineCache, createInfoCount,
                                                                                           pCreateInfos, pAllocator, pPipelines);
    safe_VkGraphicsPipelineCreateInfo *local_pCreateInfos = nullptr;
    if (pCreateInfos) {
        local_pCreateInfos = new safe_VkGraphicsPipelineCreateInfo[createInfoCount];
        ReadLockGuard lock(dispatch_lock);
        for (uint32_t idx0 = 0; idx0 < createInfoCount; ++idx0) {
            bool uses_color_attachment = false;
            bool uses_depthstencil_attachment = false;
            {
                const auto subpasses_uses_it = layer_data->renderpasses_states.find(layer_data->Unwrap(pCreateInfos[idx0].renderPass));
                if (subpasses_uses_it != layer_data->renderpasses_states.end()) {
                    const auto &subpasses_uses = subpasses_uses_it->second;
                    if (subpasses_uses.subpasses_using_color_attachment.count(pCreateInfos[idx0].subpass))
                        uses_color_attachment = true;
                    if (subpasses_uses.subpasses_using_depthstencil_attachment.count(pCreateInfos[idx0].subpass))
                        uses_depthstencil_attachment = true;
                }
            }

#if defined(VK_KHR_dynamic_rendering)
            auto dynamic_rendering = LvlFindInChain<VkPipelineRenderingCreateInfoKHR>(pCreateInfos[idx0].pNext);
            if (dynamic_rendering) {
                uses_color_attachment        = (dynamic_rendering->colorAttachmentCount > 0);
                uses_depthstencil_attachment = (dynamic_rendering->depthAttachmentFormat != VK_FORMAT_UNDEFINED);
            }
#endif

            local_pCreateInfos[idx0].initialize(&pCreateInfos[idx0], uses_color_attachment, uses_depthstencil_attachment);

            if (pCreateInfos[idx0].basePipelineHandle) {
                local_pCreateInfos[idx0].basePipelineHandle = layer_data->Unwrap(pCreateInfos[idx0].basePipelineHandle);
            }
            if (pCreateInfos[idx0].layout) {
                local_pCreateInfos[idx0].layout = layer_data->Unwrap(pCreateInfos[idx0].layout);
            }
            if (pCreateInfos[idx0].pStages) {
                for (uint32_t idx1 = 0; idx1 < pCreateInfos[idx0].stageCount; ++idx1) {
                    if (pCreateInfos[idx0].pStages[idx1].module) {
                        local_pCreateInfos[idx0].pStages[idx1].module = layer_data->Unwrap(pCreateInfos[idx0].pStages[idx1].module);
                    }
                }
            }
            if (pCreateInfos[idx0].renderPass) {
                local_pCreateInfos[idx0].renderPass = layer_data->Unwrap(pCreateInfos[idx0].renderPass);
            }
        }
    }
    if (pipelineCache) {
        pipelineCache = layer_data->Unwrap(pipelineCache);
    }

    VkResult result = layer_data->device_dispatch_table.CreateGraphicsPipelines(device, pipelineCache, createInfoCount,
                                                                                local_pCreateInfos->ptr(), pAllocator, pPipelines);
#if defined(VK_EXT_pipeline_creation_feedback)
    for (uint32_t i = 0; i < createInfoCount; ++i) {
        if (pCreateInfos[i].pNext != VK_NULL_HANDLE) {
            CopyCreatePipelineFeedbackData(local_pCreateInfos[i].pNext, pCreateInfos[i].pNext);
        }
    }
#endif

    delete[] local_pCreateInfos;
    {
        for (uint32_t i = 0; i < createInfoCount; ++i) {
            if (pPipelines[i] != VK_NULL_HANDLE) {
                pPipelines[i] = layer_data->WrapNew(pPipelines[i]);
            }
        }
    }
    return result;
}

template <typename T>
static void UpdateCreateRenderPassState(ValidationObject *layer_data, const T *pCreateInfo, VkRenderPass renderPass) {
    auto &renderpass_state = layer_data->renderpasses_states[renderPass];

    for (uint32_t subpass = 0; subpass < pCreateInfo->subpassCount; ++subpass) {
        bool uses_color = false;
        for (uint32_t i = 0; i < pCreateInfo->pSubpasses[subpass].colorAttachmentCount && !uses_color; ++i)
            if (pCreateInfo->pSubpasses[subpass].pColorAttachments[i].attachment != VK_ATTACHMENT_UNUSED) uses_color = true;

        bool uses_depthstencil = false;
        if (pCreateInfo->pSubpasses[subpass].pDepthStencilAttachment)
            if (pCreateInfo->pSubpasses[subpass].pDepthStencilAttachment->attachment != VK_ATTACHMENT_UNUSED)
                uses_depthstencil = true;

        if (uses_color) renderpass_state.subpasses_using_color_attachment.insert(subpass);
        if (uses_depthstencil) renderpass_state.subpasses_using_depthstencil_attachment.insert(subpass);
    }
}

VkResult DispatchCreateRenderPass(VkDevice device, const VkRenderPassCreateInfo *pCreateInfo,
                                  const VkAllocationCallbacks *pAllocator, VkRenderPass *pRenderPass) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    VkResult result = layer_data->device_dispatch_table.CreateRenderPass(device, pCreateInfo, pAllocator, pRenderPass);
    if (!wrap_handles) return result;
    if (VK_SUCCESS == result) {
        WriteLockGuard lock(dispatch_lock);
        UpdateCreateRenderPassState(layer_data, pCreateInfo, *pRenderPass);
        *pRenderPass = layer_data->WrapNew(*pRenderPass);
    }
    return result;
}

#if defined(VK_KHR_create_renderpass2)
VkResult DispatchCreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2 *pCreateInfo,
                                      const VkAllocationCallbacks *pAllocator, VkRenderPass *pRenderPass) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    VkResult result = layer_data->device_dispatch_table.CreateRenderPass2KHR(device, pCreateInfo, pAllocator, pRenderPass);
    if (!wrap_handles) return result;
    if (VK_SUCCESS == result) {
        WriteLockGuard lock(dispatch_lock);
        UpdateCreateRenderPassState(layer_data, pCreateInfo, *pRenderPass);
        *pRenderPass = layer_data->WrapNew(*pRenderPass);
    }
    return result;
}
#endif

VkResult DispatchCreateRenderPass2(VkDevice device, const VkRenderPassCreateInfo2 *pCreateInfo,
                                      const VkAllocationCallbacks *pAllocator, VkRenderPass *pRenderPass) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    VkResult result = layer_data->device_dispatch_table.CreateRenderPass2(device, pCreateInfo, pAllocator, pRenderPass);
    if (!wrap_handles) return result;
    if (VK_SUCCESS == result) {
        WriteLockGuard lock(dispatch_lock);
        UpdateCreateRenderPassState(layer_data, pCreateInfo, *pRenderPass);
        *pRenderPass = layer_data->WrapNew(*pRenderPass);
    }
    return result;
}

void DispatchDestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks *pAllocator) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroyRenderPass(device, renderPass, pAllocator);
    uint64_t renderPass_id = reinterpret_cast<uint64_t &>(renderPass);

    auto iter = unique_id_mapping.pop(renderPass_id);
    if (iter != unique_id_mapping.end()) {
        renderPass = (VkRenderPass)iter->second;
    } else {
        renderPass = (VkRenderPass)0;
    }

    layer_data->device_dispatch_table.DestroyRenderPass(device, renderPass, pAllocator);

    WriteLockGuard lock(dispatch_lock);
    layer_data->renderpasses_states.erase(renderPass);
}

VkResult DispatchCreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR *pCreateInfo,
                                    const VkAllocationCallbacks *pAllocator, VkSwapchainKHR *pSwapchain) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);
    safe_VkSwapchainCreateInfoKHR *local_pCreateInfo = NULL;
    if (pCreateInfo) {
        local_pCreateInfo = new safe_VkSwapchainCreateInfoKHR(pCreateInfo);
        local_pCreateInfo->oldSwapchain = layer_data->Unwrap(pCreateInfo->oldSwapchain);
        // Surface is instance-level object
        local_pCreateInfo->surface = layer_data->Unwrap(pCreateInfo->surface);
    }

    VkResult result = layer_data->device_dispatch_table.CreateSwapchainKHR(device, local_pCreateInfo->ptr(), pAllocator, pSwapchain);
    delete local_pCreateInfo;

    if (VK_SUCCESS == result) {
        *pSwapchain = layer_data->WrapNew(*pSwapchain);
    }
    return result;
}

VkResult DispatchCreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR *pCreateInfos,
                                           const VkAllocationCallbacks *pAllocator, VkSwapchainKHR *pSwapchains) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles)
        return layer_data->device_dispatch_table.CreateSharedSwapchainsKHR(device, swapchainCount, pCreateInfos, pAllocator,
                                                                           pSwapchains);
    safe_VkSwapchainCreateInfoKHR *local_pCreateInfos = NULL;
    {
        if (pCreateInfos) {
            local_pCreateInfos = new safe_VkSwapchainCreateInfoKHR[swapchainCount];
            for (uint32_t i = 0; i < swapchainCount; ++i) {
                local_pCreateInfos[i].initialize(&pCreateInfos[i]);
                if (pCreateInfos[i].surface) {
                    // Surface is instance-level object
                    local_pCreateInfos[i].surface = layer_data->Unwrap(pCreateInfos[i].surface);
                }
                if (pCreateInfos[i].oldSwapchain) {
                    local_pCreateInfos[i].oldSwapchain = layer_data->Unwrap(pCreateInfos[i].oldSwapchain);
                }
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.CreateSharedSwapchainsKHR(device, swapchainCount, local_pCreateInfos->ptr(),
                                                                                  pAllocator, pSwapchains);
    delete[] local_pCreateInfos;
    if (VK_SUCCESS == result) {
        for (uint32_t i = 0; i < swapchainCount; i++) {
            pSwapchains[i] = layer_data->WrapNew(pSwapchains[i]);
        }
    }
    return result;
}

VkResult DispatchGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t *pSwapchainImageCount,
                                       VkImage *pSwapchainImages) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles)
        return layer_data->device_dispatch_table.GetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
    VkSwapchainKHR wrapped_swapchain_handle = swapchain;
    if (VK_NULL_HANDLE != swapchain) {
        swapchain = layer_data->Unwrap(swapchain);
    }
    VkResult result =
        layer_data->device_dispatch_table.GetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
    if ((VK_SUCCESS == result) || (VK_INCOMPLETE == result)) {
        if ((*pSwapchainImageCount > 0) && pSwapchainImages) {
            WriteLockGuard lock(dispatch_lock);
            auto &wrapped_swapchain_image_handles = layer_data->swapchain_wrapped_image_handle_map[wrapped_swapchain_handle];
            for (uint32_t i = static_cast<uint32_t>(wrapped_swapchain_image_handles.size()); i < *pSwapchainImageCount; i++) {
                wrapped_swapchain_image_handles.emplace_back(layer_data->WrapNew(pSwapchainImages[i]));
            }
            for (uint32_t i = 0; i < *pSwapchainImageCount; i++) {
                pSwapchainImages[i] = wrapped_swapchain_image_handles[i];
            }
        }
    }
    return result;
}

#if !defined(VULKANSC)
void DispatchDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks *pAllocator) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroySwapchainKHR(device, swapchain, pAllocator);
    WriteLockGuard lock(dispatch_lock);

    auto &image_array = layer_data->swapchain_wrapped_image_handle_map[swapchain];
    for (auto &image_handle : image_array) {
        unique_id_mapping.erase(HandleToUint64(image_handle));
    }
    layer_data->swapchain_wrapped_image_handle_map.erase(swapchain);
    lock.unlock();

    uint64_t swapchain_id = HandleToUint64(swapchain);

    auto iter = unique_id_mapping.pop(swapchain_id);
    if (iter != unique_id_mapping.end()) {
        swapchain = (VkSwapchainKHR)iter->second;
    } else {
        swapchain = (VkSwapchainKHR)0;
    }

    layer_data->device_dispatch_table.DestroySwapchainKHR(device, swapchain, pAllocator);
}
#endif

VkResult DispatchQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR *pPresentInfo) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(queue), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.QueuePresentKHR(queue, pPresentInfo);
    safe_VkPresentInfoKHR *local_pPresentInfo = NULL;
    {
        if (pPresentInfo) {
            local_pPresentInfo = new safe_VkPresentInfoKHR(pPresentInfo);
            if (local_pPresentInfo->pWaitSemaphores) {
                for (uint32_t index1 = 0; index1 < local_pPresentInfo->waitSemaphoreCount; ++index1) {
                    local_pPresentInfo->pWaitSemaphores[index1] = layer_data->Unwrap(pPresentInfo->pWaitSemaphores[index1]);
                }
            }
            if (local_pPresentInfo->pSwapchains) {
                for (uint32_t index1 = 0; index1 < local_pPresentInfo->swapchainCount; ++index1) {
                    local_pPresentInfo->pSwapchains[index1] = layer_data->Unwrap(pPresentInfo->pSwapchains[index1]);
                }
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.QueuePresentKHR(queue, local_pPresentInfo->ptr());

    // pResults is an output array embedded in a structure. The code generator neglects to copy back from the safe_* version,
    // so handle it as a special case here:
    if (pPresentInfo && pPresentInfo->pResults) {
        for (uint32_t i = 0; i < pPresentInfo->swapchainCount; i++) {
            pPresentInfo->pResults[i] = local_pPresentInfo->pResults[i];
        }
    }
    delete local_pPresentInfo;
    return result;
}

#if !defined(VULKANSC)
void DispatchDestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks *pAllocator) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroyDescriptorPool(device, descriptorPool, pAllocator);
    WriteLockGuard lock(dispatch_lock);

    // remove references to implicitly freed descriptor sets
    for(auto descriptor_set : layer_data->pool_descriptor_sets_map[descriptorPool]) {
        unique_id_mapping.erase(reinterpret_cast<uint64_t &>(descriptor_set));
    }
    layer_data->pool_descriptor_sets_map.erase(descriptorPool);
    lock.unlock();

    uint64_t descriptorPool_id = reinterpret_cast<uint64_t &>(descriptorPool);

    auto iter = unique_id_mapping.pop(descriptorPool_id);
    if (iter != unique_id_mapping.end()) {
        descriptorPool = (VkDescriptorPool)iter->second;
    } else {
        descriptorPool = (VkDescriptorPool)0;
    }

    layer_data->device_dispatch_table.DestroyDescriptorPool(device, descriptorPool, pAllocator);
}
#endif

VkResult DispatchResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.ResetDescriptorPool(device, descriptorPool, flags);
    VkDescriptorPool local_descriptor_pool = VK_NULL_HANDLE;
    {
        local_descriptor_pool = layer_data->Unwrap(descriptorPool);
    }
    VkResult result = layer_data->device_dispatch_table.ResetDescriptorPool(device, local_descriptor_pool, flags);
    if (VK_SUCCESS == result) {
        WriteLockGuard lock(dispatch_lock);
        // remove references to implicitly freed descriptor sets
        for(auto descriptor_set : layer_data->pool_descriptor_sets_map[descriptorPool]) {
            unique_id_mapping.erase(reinterpret_cast<uint64_t &>(descriptor_set));
        }
        layer_data->pool_descriptor_sets_map[descriptorPool].clear();
    }

    return result;
}

VkResult DispatchAllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo *pAllocateInfo,
                                        VkDescriptorSet *pDescriptorSets) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.AllocateDescriptorSets(device, pAllocateInfo, pDescriptorSets);
    safe_VkDescriptorSetAllocateInfo *local_pAllocateInfo = NULL;
    {
        if (pAllocateInfo) {
            local_pAllocateInfo = new safe_VkDescriptorSetAllocateInfo(pAllocateInfo);
            if (pAllocateInfo->descriptorPool) {
                local_pAllocateInfo->descriptorPool = layer_data->Unwrap(pAllocateInfo->descriptorPool);
            }
            if (local_pAllocateInfo->pSetLayouts) {
                for (uint32_t index1 = 0; index1 < local_pAllocateInfo->descriptorSetCount; ++index1) {
                    local_pAllocateInfo->pSetLayouts[index1] = layer_data->Unwrap(local_pAllocateInfo->pSetLayouts[index1]);
                }
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.AllocateDescriptorSets(
        device, (const VkDescriptorSetAllocateInfo *)local_pAllocateInfo, pDescriptorSets);
    if (local_pAllocateInfo) {
        delete local_pAllocateInfo;
    }
    if (VK_SUCCESS == result) {
        WriteLockGuard lock(dispatch_lock);
        auto &pool_descriptor_sets = layer_data->pool_descriptor_sets_map[pAllocateInfo->descriptorPool];
        for (uint32_t index0 = 0; index0 < pAllocateInfo->descriptorSetCount; index0++) {
            pDescriptorSets[index0] = layer_data->WrapNew(pDescriptorSets[index0]);
            pool_descriptor_sets.insert(pDescriptorSets[index0]);
        }
    }
    return result;
}

VkResult DispatchFreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount,
                                    const VkDescriptorSet *pDescriptorSets) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles)
        return layer_data->device_dispatch_table.FreeDescriptorSets(device, descriptorPool, descriptorSetCount, pDescriptorSets);
    VkDescriptorSet *local_pDescriptorSets = NULL;
    VkDescriptorPool local_descriptor_pool = VK_NULL_HANDLE;
    {
        local_descriptor_pool = layer_data->Unwrap(descriptorPool);
        if (pDescriptorSets) {
            local_pDescriptorSets = new VkDescriptorSet[descriptorSetCount];
            for (uint32_t index0 = 0; index0 < descriptorSetCount; ++index0) {
                local_pDescriptorSets[index0] = layer_data->Unwrap(pDescriptorSets[index0]);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.FreeDescriptorSets(device, local_descriptor_pool, descriptorSetCount,
                                                                           (const VkDescriptorSet *)local_pDescriptorSets);
    if (local_pDescriptorSets) delete[] local_pDescriptorSets;
    if ((VK_SUCCESS == result) && (pDescriptorSets)) {
        WriteLockGuard lock(dispatch_lock);
        auto &pool_descriptor_sets = layer_data->pool_descriptor_sets_map[descriptorPool];
        for (uint32_t index0 = 0; index0 < descriptorSetCount; index0++) {
            VkDescriptorSet handle = pDescriptorSets[index0];
            pool_descriptor_sets.erase(handle);
            uint64_t unique_id = reinterpret_cast<uint64_t &>(handle);
            unique_id_mapping.erase(unique_id);
        }
    }
    return result;
}

#if !defined(VULKANSC)
// This is the core version of this routine.  The extension version is below.
VkResult DispatchCreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo *pCreateInfo,
                                                const VkAllocationCallbacks *pAllocator,
                                                VkDescriptorUpdateTemplate *pDescriptorUpdateTemplate) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles)
        return layer_data->device_dispatch_table.CreateDescriptorUpdateTemplate(device, pCreateInfo, pAllocator,
                                                                                pDescriptorUpdateTemplate);
    safe_VkDescriptorUpdateTemplateCreateInfo var_local_pCreateInfo;
    safe_VkDescriptorUpdateTemplateCreateInfo *local_pCreateInfo = NULL;
    if (pCreateInfo) {
        local_pCreateInfo = &var_local_pCreateInfo;
        local_pCreateInfo->initialize(pCreateInfo);
        if (pCreateInfo->templateType == VK_DESCRIPTOR_UPDATE_TEMPLATE_TYPE_DESCRIPTOR_SET) {
            local_pCreateInfo->descriptorSetLayout = layer_data->Unwrap(pCreateInfo->descriptorSetLayout);
        }
        if (pCreateInfo->templateType == VK_DESCRIPTOR_UPDATE_TEMPLATE_TYPE_PUSH_DESCRIPTORS_KHR) {
            local_pCreateInfo->pipelineLayout = layer_data->Unwrap(pCreateInfo->pipelineLayout);
        }
    }
    VkResult result = layer_data->device_dispatch_table.CreateDescriptorUpdateTemplate(device, local_pCreateInfo->ptr(), pAllocator,
                                                                                       pDescriptorUpdateTemplate);
    if (VK_SUCCESS == result) {
        *pDescriptorUpdateTemplate = layer_data->WrapNew(*pDescriptorUpdateTemplate);

        // Shadow template createInfo for later updates
        if (local_pCreateInfo) {
            WriteLockGuard lock(dispatch_lock);
            std::unique_ptr<TEMPLATE_STATE> template_state(new TEMPLATE_STATE(*pDescriptorUpdateTemplate, local_pCreateInfo));
            layer_data->desc_template_createinfo_map[(uint64_t)*pDescriptorUpdateTemplate] = std::move(template_state);
        }
    }
    return result;
}
#endif

#if defined(VK_KHR_descriptor_update_template)
// This is the extension version of this routine.  The core version is above.
VkResult DispatchCreateDescriptorUpdateTemplateKHR(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo *pCreateInfo,
                                                   const VkAllocationCallbacks *pAllocator,
                                                   VkDescriptorUpdateTemplate *pDescriptorUpdateTemplate) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles)
        return layer_data->device_dispatch_table.CreateDescriptorUpdateTemplateKHR(device, pCreateInfo, pAllocator,
                                                                                   pDescriptorUpdateTemplate);
    safe_VkDescriptorUpdateTemplateCreateInfo var_local_pCreateInfo;
    safe_VkDescriptorUpdateTemplateCreateInfo *local_pCreateInfo = NULL;
    if (pCreateInfo) {
        local_pCreateInfo = &var_local_pCreateInfo;
        local_pCreateInfo->initialize(pCreateInfo);
        if (pCreateInfo->templateType == VK_DESCRIPTOR_UPDATE_TEMPLATE_TYPE_DESCRIPTOR_SET) {
            local_pCreateInfo->descriptorSetLayout = layer_data->Unwrap(pCreateInfo->descriptorSetLayout);
        }
        if (pCreateInfo->templateType == VK_DESCRIPTOR_UPDATE_TEMPLATE_TYPE_PUSH_DESCRIPTORS_KHR) {
            local_pCreateInfo->pipelineLayout = layer_data->Unwrap(pCreateInfo->pipelineLayout);
        }
    }
    VkResult result = layer_data->device_dispatch_table.CreateDescriptorUpdateTemplateKHR(device, local_pCreateInfo->ptr(),
                                                                                          pAllocator, pDescriptorUpdateTemplate);

    if (VK_SUCCESS == result) {
        *pDescriptorUpdateTemplate = layer_data->WrapNew(*pDescriptorUpdateTemplate);

        // Shadow template createInfo for later updates
        if (local_pCreateInfo) {
            WriteLockGuard lock(dispatch_lock);
            std::unique_ptr<TEMPLATE_STATE> template_state(new TEMPLATE_STATE(*pDescriptorUpdateTemplate, local_pCreateInfo));
            layer_data->desc_template_createinfo_map[(uint64_t)*pDescriptorUpdateTemplate] = std::move(template_state);
        }
    }
    return result;
}
#endif

#if !defined(VULKANSC)
// This is the core version of this routine.  The extension version is below.
void DispatchDestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate,
                                             const VkAllocationCallbacks *pAllocator) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles)
        return layer_data->device_dispatch_table.DestroyDescriptorUpdateTemplate(device, descriptorUpdateTemplate, pAllocator);
    WriteLockGuard lock(dispatch_lock);
    uint64_t descriptor_update_template_id = reinterpret_cast<uint64_t &>(descriptorUpdateTemplate);
    layer_data->desc_template_createinfo_map.erase(descriptor_update_template_id);
    lock.unlock();

    auto iter = unique_id_mapping.pop(descriptor_update_template_id);
    if (iter != unique_id_mapping.end()) {
        descriptorUpdateTemplate = (VkDescriptorUpdateTemplate)iter->second;
    } else {
        descriptorUpdateTemplate = (VkDescriptorUpdateTemplate)0;
    }

    layer_data->device_dispatch_table.DestroyDescriptorUpdateTemplate(device, descriptorUpdateTemplate, pAllocator);
}
#endif

#if defined(VK_KHR_descriptor_update_template)
// This is the extension version of this routine.  The core version is above.
void DispatchDestroyDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate,
                                                const VkAllocationCallbacks *pAllocator) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles)
        return layer_data->device_dispatch_table.DestroyDescriptorUpdateTemplateKHR(device, descriptorUpdateTemplate, pAllocator);
    WriteLockGuard lock(dispatch_lock);
    uint64_t descriptor_update_template_id = reinterpret_cast<uint64_t &>(descriptorUpdateTemplate);
    layer_data->desc_template_createinfo_map.erase(descriptor_update_template_id);
    lock.unlock();

    auto iter = unique_id_mapping.pop(descriptor_update_template_id);
    if (iter != unique_id_mapping.end()) {
        descriptorUpdateTemplate = (VkDescriptorUpdateTemplate)iter->second;
    } else {
        descriptorUpdateTemplate = (VkDescriptorUpdateTemplate)0;
    }

    layer_data->device_dispatch_table.DestroyDescriptorUpdateTemplateKHR(device, descriptorUpdateTemplate, pAllocator);
}
#endif

#if !defined(VULKANSC)
void *BuildUnwrappedUpdateTemplateBuffer(ValidationObject *layer_data, uint64_t descriptorUpdateTemplate, const void *pData) {
    auto const template_map_entry = layer_data->desc_template_createinfo_map.find(descriptorUpdateTemplate);
    auto const &create_info = template_map_entry->second->create_info;
    size_t allocation_size = 0;
    std::vector<std::tuple<size_t, VulkanObjectType, uint64_t, size_t>> template_entries;

    for (uint32_t i = 0; i < create_info.descriptorUpdateEntryCount; i++) {
        for (uint32_t j = 0; j < create_info.pDescriptorUpdateEntries[i].descriptorCount; j++) {
            size_t offset = create_info.pDescriptorUpdateEntries[i].offset + j * create_info.pDescriptorUpdateEntries[i].stride;
            char *update_entry = (char *)(pData) + offset;

            switch (create_info.pDescriptorUpdateEntries[i].descriptorType) {
                case VK_DESCRIPTOR_TYPE_SAMPLER:
                case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
                case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
                case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: {
                    auto image_entry = reinterpret_cast<VkDescriptorImageInfo *>(update_entry);
                    allocation_size = std::max(allocation_size, offset + sizeof(VkDescriptorImageInfo));

                    VkDescriptorImageInfo *wrapped_entry = new VkDescriptorImageInfo(*image_entry);
                    wrapped_entry->sampler = layer_data->Unwrap(image_entry->sampler);
                    wrapped_entry->imageView = layer_data->Unwrap(image_entry->imageView);
                    template_entries.emplace_back(offset, kVulkanObjectTypeImage, CastToUint64(wrapped_entry), 0);
                } break;

                case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
                case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
                case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: {
                    auto buffer_entry = reinterpret_cast<VkDescriptorBufferInfo *>(update_entry);
                    allocation_size = std::max(allocation_size, offset + sizeof(VkDescriptorBufferInfo));

                    VkDescriptorBufferInfo *wrapped_entry = new VkDescriptorBufferInfo(*buffer_entry);
                    wrapped_entry->buffer = layer_data->Unwrap(buffer_entry->buffer);
                    template_entries.emplace_back(offset, kVulkanObjectTypeBuffer, CastToUint64(wrapped_entry), 0);
                } break;

                case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
                case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: {
                    auto buffer_view_handle = reinterpret_cast<VkBufferView *>(update_entry);
                    allocation_size = std::max(allocation_size, offset + sizeof(VkBufferView));

                    VkBufferView wrapped_entry = layer_data->Unwrap(*buffer_view_handle);
                    template_entries.emplace_back(offset, kVulkanObjectTypeBufferView, CastToUint64(wrapped_entry), 0);
                } break;
                case VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT: {
                    size_t numBytes = create_info.pDescriptorUpdateEntries[i].descriptorCount;
                    allocation_size = std::max(allocation_size, offset + numBytes);
                    // nothing to unwrap, just plain data
                    template_entries.emplace_back(offset, kVulkanObjectTypeUnknown, CastToUint64(update_entry),
                                                  numBytes);
                    // to break out of the loop
                    j = create_info.pDescriptorUpdateEntries[i].descriptorCount;
                } break;
                case VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV:{
                    auto accstruct_nv_handle = reinterpret_cast<VkAccelerationStructureNV *>(update_entry);
                    allocation_size = std::max(allocation_size, offset + sizeof(VkAccelerationStructureNV ));

                    VkAccelerationStructureNV  wrapped_entry = layer_data->Unwrap(*accstruct_nv_handle);
                    template_entries.emplace_back(offset, kVulkanObjectTypeAccelerationStructureNV, CastToUint64(wrapped_entry), 0);
                } break;
                case VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR: {
                    auto accstruct_khr_handle = reinterpret_cast<VkAccelerationStructureKHR *>(update_entry);
                    allocation_size = std::max(allocation_size, offset + sizeof(VkAccelerationStructureKHR ));

                    VkAccelerationStructureKHR  wrapped_entry = layer_data->Unwrap(*accstruct_khr_handle);
                    template_entries.emplace_back(offset, kVulkanObjectTypeAccelerationStructureKHR, CastToUint64(wrapped_entry), 0);
                } break;
                default:
                    assert(0);
                    break;
            }
        }
    }
    // Allocate required buffer size and populate with source/unwrapped data
    void *unwrapped_data = malloc(allocation_size);
    for (auto &this_entry : template_entries) {
        VulkanObjectType type = std::get<1>(this_entry);
        void *destination = (char *)unwrapped_data + std::get<0>(this_entry);
        uint64_t source = std::get<2>(this_entry);
        size_t size = std::get<3>(this_entry);

        if (size != 0) {
            assert(type == kVulkanObjectTypeUnknown);
            memcpy(destination, CastFromUint64<void *>(source), size);
        } else {
            switch (type) {
                case kVulkanObjectTypeImage:
                    *(reinterpret_cast<VkDescriptorImageInfo *>(destination)) =
                        *(reinterpret_cast<VkDescriptorImageInfo *>(source));
                    delete CastFromUint64<VkDescriptorImageInfo *>(source);
                    break;
                case kVulkanObjectTypeBuffer:
                    *(reinterpret_cast<VkDescriptorBufferInfo *>(destination)) =
                        *(CastFromUint64<VkDescriptorBufferInfo *>(source));
                    delete CastFromUint64<VkDescriptorBufferInfo *>(source);
                    break;
                case kVulkanObjectTypeBufferView:
                    *(reinterpret_cast<VkBufferView *>(destination)) = CastFromUint64<VkBufferView>(source);
                    break;
                case kVulkanObjectTypeAccelerationStructureKHR:
                    *(reinterpret_cast<VkAccelerationStructureKHR *>(destination)) = CastFromUint64<VkAccelerationStructureKHR>(source);
                    break;
                case kVulkanObjectTypeAccelerationStructureNV:
                    *(reinterpret_cast<VkAccelerationStructureNV *>(destination)) = CastFromUint64<VkAccelerationStructureNV>(source);
                    break;
                default:
                    assert(0);
                    break;
            }
        }
    }
    return (void *)unwrapped_data;
}

void DispatchUpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet,
                                             VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void *pData) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles)
        return layer_data->device_dispatch_table.UpdateDescriptorSetWithTemplate(device, descriptorSet, descriptorUpdateTemplate,
                                                                                 pData);
    uint64_t template_handle = reinterpret_cast<uint64_t &>(descriptorUpdateTemplate);
    void *unwrapped_buffer = nullptr;
    {
        ReadLockGuard lock(dispatch_lock);
        descriptorSet = layer_data->Unwrap(descriptorSet);
        descriptorUpdateTemplate = (VkDescriptorUpdateTemplate)layer_data->Unwrap(descriptorUpdateTemplate);
        unwrapped_buffer = BuildUnwrappedUpdateTemplateBuffer(layer_data, template_handle, pData);
    }
    layer_data->device_dispatch_table.UpdateDescriptorSetWithTemplate(device, descriptorSet, descriptorUpdateTemplate, unwrapped_buffer);
    free(unwrapped_buffer);
}
#endif

#if defined(VK_KHR_descriptor_update_template)
void DispatchUpdateDescriptorSetWithTemplateKHR(VkDevice device, VkDescriptorSet descriptorSet,
                                                VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void *pData) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles)
        return layer_data->device_dispatch_table.UpdateDescriptorSetWithTemplateKHR(device, descriptorSet, descriptorUpdateTemplate,
                                                                                    pData);
    uint64_t template_handle = reinterpret_cast<uint64_t &>(descriptorUpdateTemplate);
    void *unwrapped_buffer = nullptr;
    {
        ReadLockGuard lock(dispatch_lock);
        descriptorSet = layer_data->Unwrap(descriptorSet);
        descriptorUpdateTemplate = layer_data->Unwrap(descriptorUpdateTemplate);
        unwrapped_buffer = BuildUnwrappedUpdateTemplateBuffer(layer_data, template_handle, pData);
    }
    layer_data->device_dispatch_table.UpdateDescriptorSetWithTemplateKHR(device, descriptorSet, descriptorUpdateTemplate, unwrapped_buffer);
    free(unwrapped_buffer);
}

void DispatchCmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer,
                                                 VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout,
                                                 uint32_t set, const void *pData) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles)
        return layer_data->device_dispatch_table.CmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate,
                                                                                     layout, set, pData);
    uint64_t template_handle = reinterpret_cast<uint64_t &>(descriptorUpdateTemplate);
    void *unwrapped_buffer = nullptr;
    {
        ReadLockGuard lock(dispatch_lock);
        descriptorUpdateTemplate = layer_data->Unwrap(descriptorUpdateTemplate);
        layout = layer_data->Unwrap(layout);
        unwrapped_buffer = BuildUnwrappedUpdateTemplateBuffer(layer_data, template_handle, pData);
    }
    layer_data->device_dispatch_table.CmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate, layout, set,
                                                                 unwrapped_buffer);
    free(unwrapped_buffer);
}
#endif

VkResult DispatchGetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t *pPropertyCount,
                                                       VkDisplayPropertiesKHR *pProperties) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    VkResult result =
        layer_data->instance_dispatch_table.GetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, pPropertyCount, pProperties);
    if (!wrap_handles) return result;
    if ((result == VK_SUCCESS || result == VK_INCOMPLETE) && pProperties) {
        for (uint32_t idx0 = 0; idx0 < *pPropertyCount; ++idx0) {
            pProperties[idx0].display = layer_data->MaybeWrapDisplay(pProperties[idx0].display, layer_data);
        }
    }
    return result;
}

VkResult DispatchGetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t *pPropertyCount,
                                                        VkDisplayProperties2KHR *pProperties) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    VkResult result =
        layer_data->instance_dispatch_table.GetPhysicalDeviceDisplayProperties2KHR(physicalDevice, pPropertyCount, pProperties);
    if (!wrap_handles) return result;
    if ((result == VK_SUCCESS || result == VK_INCOMPLETE) && pProperties) {
        for (uint32_t idx0 = 0; idx0 < *pPropertyCount; ++idx0) {
            pProperties[idx0].displayProperties.display =
                layer_data->MaybeWrapDisplay(pProperties[idx0].displayProperties.display, layer_data);
        }
    }
    return result;
}

VkResult DispatchGetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t *pPropertyCount,
                                                            VkDisplayPlanePropertiesKHR *pProperties) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    VkResult result =
        layer_data->instance_dispatch_table.GetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, pPropertyCount, pProperties);
    if (!wrap_handles) return result;
    if ((result == VK_SUCCESS || result == VK_INCOMPLETE) && pProperties) {
        for (uint32_t idx0 = 0; idx0 < *pPropertyCount; ++idx0) {
            VkDisplayKHR &opt_display = pProperties[idx0].currentDisplay;
            if (opt_display) opt_display = layer_data->MaybeWrapDisplay(opt_display, layer_data);
        }
    }
    return result;
}

VkResult DispatchGetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t *pPropertyCount,
                                                             VkDisplayPlaneProperties2KHR *pProperties) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    VkResult result = layer_data->instance_dispatch_table.GetPhysicalDeviceDisplayPlaneProperties2KHR(physicalDevice,
                                                                                                      pPropertyCount, pProperties);
    if (!wrap_handles) return result;
    if ((result == VK_SUCCESS || result == VK_INCOMPLETE) && pProperties) {
        for (uint32_t idx0 = 0; idx0 < *pPropertyCount; ++idx0) {
            VkDisplayKHR &opt_display = pProperties[idx0].displayPlaneProperties.currentDisplay;
            if (opt_display) opt_display = layer_data->MaybeWrapDisplay(opt_display, layer_data);
        }
    }
    return result;
}

VkResult DispatchGetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t *pDisplayCount,
                                                     VkDisplayKHR *pDisplays) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    VkResult result = layer_data->instance_dispatch_table.GetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex,
                                                                                              pDisplayCount, pDisplays);
    if ((result == VK_SUCCESS || result == VK_INCOMPLETE) && pDisplays) {
    if (!wrap_handles) return result;
        for (uint32_t i = 0; i < *pDisplayCount; ++i) {
            if (pDisplays[i]) pDisplays[i] = layer_data->MaybeWrapDisplay(pDisplays[i], layer_data);
        }
    }
    return result;
}

VkResult DispatchGetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t *pPropertyCount,
                                             VkDisplayModePropertiesKHR *pProperties) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    if (!wrap_handles)
        return layer_data->instance_dispatch_table.GetDisplayModePropertiesKHR(physicalDevice, display, pPropertyCount,
                                                                               pProperties);
    {
        display = layer_data->Unwrap(display);
    }

    VkResult result = layer_data->instance_dispatch_table.GetDisplayModePropertiesKHR(physicalDevice, display, pPropertyCount, pProperties);
    if ((result == VK_SUCCESS || result == VK_INCOMPLETE) && pProperties) {
        for (uint32_t idx0 = 0; idx0 < *pPropertyCount; ++idx0) {
            pProperties[idx0].displayMode = layer_data->WrapNew(pProperties[idx0].displayMode);
        }
    }
    return result;
}

VkResult DispatchGetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t *pPropertyCount,
                                              VkDisplayModeProperties2KHR *pProperties) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    if (!wrap_handles)
        return layer_data->instance_dispatch_table.GetDisplayModeProperties2KHR(physicalDevice, display, pPropertyCount,
                                                                                pProperties);
    {
        display = layer_data->Unwrap(display);
    }

    VkResult result =
        layer_data->instance_dispatch_table.GetDisplayModeProperties2KHR(physicalDevice, display, pPropertyCount, pProperties);
    if ((result == VK_SUCCESS || result == VK_INCOMPLETE) && pProperties) {
        for (uint32_t idx0 = 0; idx0 < *pPropertyCount; ++idx0) {
            pProperties[idx0].displayModeProperties.displayMode = layer_data->WrapNew(pProperties[idx0].displayModeProperties.displayMode);
        }
    }
    return result;
}

#if defined(VK_EXT_debug_marker)
VkResult DispatchDebugMarkerSetObjectTagEXT(VkDevice device, const VkDebugMarkerObjectTagInfoEXT *pTagInfo) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DebugMarkerSetObjectTagEXT(device, pTagInfo);
    safe_VkDebugMarkerObjectTagInfoEXT local_tag_info(pTagInfo);
    {
        auto it = unique_id_mapping.find(reinterpret_cast<uint64_t &>(local_tag_info.object));
        if (it != unique_id_mapping.end()) {
            local_tag_info.object = it->second;
        }
    }
    VkResult result = layer_data->device_dispatch_table.DebugMarkerSetObjectTagEXT(device,
                                                                                   reinterpret_cast<VkDebugMarkerObjectTagInfoEXT *>(&local_tag_info));
    return result;
}

VkResult DispatchDebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT *pNameInfo) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DebugMarkerSetObjectNameEXT(device, pNameInfo);
    safe_VkDebugMarkerObjectNameInfoEXT local_name_info(pNameInfo);
    {
        auto it = unique_id_mapping.find(reinterpret_cast<uint64_t &>(local_name_info.object));
        if (it != unique_id_mapping.end()) {
            local_name_info.object = it->second;
        }
    }
    VkResult result = layer_data->device_dispatch_table.DebugMarkerSetObjectNameEXT(
        device, reinterpret_cast<VkDebugMarkerObjectNameInfoEXT *>(&local_name_info));
    return result;
}
#endif

// VK_EXT_debug_utils
VkResult DispatchSetDebugUtilsObjectTagEXT(VkDevice device, const VkDebugUtilsObjectTagInfoEXT *pTagInfo) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.SetDebugUtilsObjectTagEXT(device, pTagInfo);
    safe_VkDebugUtilsObjectTagInfoEXT local_tag_info(pTagInfo);
    {
        auto it = unique_id_mapping.find(reinterpret_cast<uint64_t &>(local_tag_info.objectHandle));
        if (it != unique_id_mapping.end()) {
            local_tag_info.objectHandle = it->second;
        }
    }
    VkResult result = layer_data->device_dispatch_table.SetDebugUtilsObjectTagEXT(
        device, reinterpret_cast<const VkDebugUtilsObjectTagInfoEXT *>(&local_tag_info));
    return result;
}

VkResult DispatchSetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT *pNameInfo) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.SetDebugUtilsObjectNameEXT(device, pNameInfo);
    safe_VkDebugUtilsObjectNameInfoEXT local_name_info(pNameInfo);
    {
        auto it = unique_id_mapping.find(reinterpret_cast<uint64_t &>(local_name_info.objectHandle));
        if (it != unique_id_mapping.end()) {
            local_name_info.objectHandle = it->second;
        }
    }
    VkResult result = layer_data->device_dispatch_table.SetDebugUtilsObjectNameEXT(
        device, reinterpret_cast<const VkDebugUtilsObjectNameInfoEXT *>(&local_name_info));
    return result;
}

#if defined(VK_EXT_tooling_info)
VkResult DispatchGetPhysicalDeviceToolPropertiesEXT(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pToolCount,
    VkPhysicalDeviceToolPropertiesEXT*          pToolProperties)
{
    VkResult result = VK_SUCCESS;
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    if (layer_data->instance_dispatch_table.GetPhysicalDeviceToolPropertiesEXT == nullptr) {
        // This layer is the terminator. Set pToolCount to zero.
        *pToolCount = 0;
    } else {
        result = layer_data->instance_dispatch_table.GetPhysicalDeviceToolPropertiesEXT(physicalDevice, pToolCount, pToolProperties);
    }

    return result;
}
#endif

bool NotDispatchableHandle(VkObjectType object_type) {
    bool not_dispatchable = true;
    if ((object_type == VK_OBJECT_TYPE_INSTANCE)        ||
        (object_type == VK_OBJECT_TYPE_PHYSICAL_DEVICE) ||
        (object_type == VK_OBJECT_TYPE_DEVICE)          ||
        (object_type == VK_OBJECT_TYPE_QUEUE)           ||
        (object_type == VK_OBJECT_TYPE_COMMAND_BUFFER)) {
        not_dispatchable = false;
    }
    return not_dispatchable;
}

#if defined(VK_EXT_private_data)
VkResult DispatchSetPrivateDataEXT(
    VkDevice                                    device,
    VkObjectType                                objectType,
    uint64_t                                    objectHandle,
    VkPrivateDataSlotEXT                        privateDataSlot,
    uint64_t                                    data)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.SetPrivateDataEXT(device, objectType, objectHandle, privateDataSlot, data);
    privateDataSlot = layer_data->Unwrap(privateDataSlot);
    if (NotDispatchableHandle(objectType)) {
        objectHandle = layer_data->Unwrap(objectHandle);
    }
    VkResult result = layer_data->device_dispatch_table.SetPrivateDataEXT(device, objectType, objectHandle, privateDataSlot, data);
    return result;
}

void DispatchGetPrivateDataEXT(
    VkDevice                                    device,
    VkObjectType                                objectType,
    uint64_t                                    objectHandle,
    VkPrivateDataSlotEXT                        privateDataSlot,
    uint64_t*                                   pData)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetPrivateDataEXT(device, objectType, objectHandle, privateDataSlot, pData);
    privateDataSlot = layer_data->Unwrap(privateDataSlot);
    if (NotDispatchableHandle(objectType)) {
        objectHandle = layer_data->Unwrap(objectHandle);
    }
    layer_data->device_dispatch_table.GetPrivateDataEXT(device, objectType, objectHandle, privateDataSlot, pData);
}
#endif

layer_data::unordered_map<VkCommandBuffer, VkCommandPool> secondary_cb_map{};

ReadWriteLock dispatch_secondary_cb_map_mutex;

ReadLockGuard dispatch_cb_read_lock() {
    return ReadLockGuard(dispatch_secondary_cb_map_mutex);
}

WriteLockGuard dispatch_cb_write_lock() {
    return WriteLockGuard(dispatch_secondary_cb_map_mutex);
}

VkResult DispatchAllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.AllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
    safe_VkCommandBufferAllocateInfo var_local_pAllocateInfo;
    safe_VkCommandBufferAllocateInfo *local_pAllocateInfo = NULL;
    if (pAllocateInfo) {
        local_pAllocateInfo = &var_local_pAllocateInfo;
        local_pAllocateInfo->initialize(pAllocateInfo);
        if (pAllocateInfo->commandPool) {
            local_pAllocateInfo->commandPool = layer_data->Unwrap(pAllocateInfo->commandPool);
        }
    }
    VkResult result = layer_data->device_dispatch_table.AllocateCommandBuffers(device, (const VkCommandBufferAllocateInfo*)local_pAllocateInfo, pCommandBuffers);
    if ((result == VK_SUCCESS) && pAllocateInfo && (pAllocateInfo->level == VK_COMMAND_BUFFER_LEVEL_SECONDARY)) {
        auto lock = dispatch_cb_write_lock();
        for (uint32_t cb_index = 0; cb_index < pAllocateInfo->commandBufferCount; cb_index++) {
            secondary_cb_map.emplace(pCommandBuffers[cb_index], pAllocateInfo->commandPool);
        }
    }
    return result;
}

void DispatchFreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.FreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
    commandPool = layer_data->Unwrap(commandPool);
    layer_data->device_dispatch_table.FreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
    auto lock = dispatch_cb_write_lock();
    for (uint32_t cb_index = 0; cb_index < commandBufferCount; cb_index++) {
        secondary_cb_map.erase(pCommandBuffers[cb_index]);
    }
}

#if !defined(VULKANSC)
void DispatchDestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) {
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroyCommandPool(device, commandPool, pAllocator);
    uint64_t commandPool_id = reinterpret_cast<uint64_t &>(commandPool);
    auto iter = unique_id_mapping.pop(commandPool_id);
    if (iter != unique_id_mapping.end()) {
        commandPool = (VkCommandPool)iter->second;
    } else {
        commandPool = (VkCommandPool)0;
    }
    layer_data->device_dispatch_table.DestroyCommandPool(device, commandPool, pAllocator);
    auto lock = dispatch_cb_write_lock();
    for (auto item = secondary_cb_map.begin(); item != secondary_cb_map.end();) {
        if (item->second == commandPool) {
            item = secondary_cb_map.erase(item);
        } else {
            ++item;
        }
    }
}
#endif

VkResult DispatchBeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) {
    bool cb_is_primary;
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    {
        auto lock = dispatch_cb_read_lock();
        cb_is_primary = (secondary_cb_map.find(commandBuffer) == secondary_cb_map.end());
    }
    if (!wrap_handles || cb_is_primary) return layer_data->device_dispatch_table.BeginCommandBuffer(commandBuffer, pBeginInfo);
    safe_VkCommandBufferBeginInfo var_local_pBeginInfo;
    safe_VkCommandBufferBeginInfo *local_pBeginInfo = NULL;
    if (pBeginInfo) {
        local_pBeginInfo = &var_local_pBeginInfo;
        local_pBeginInfo->initialize(pBeginInfo);
        if (local_pBeginInfo->pInheritanceInfo) {
            if (pBeginInfo->pInheritanceInfo->renderPass) {
                local_pBeginInfo->pInheritanceInfo->renderPass = layer_data->Unwrap(pBeginInfo->pInheritanceInfo->renderPass);
            }
            if (pBeginInfo->pInheritanceInfo->framebuffer) {
                local_pBeginInfo->pInheritanceInfo->framebuffer = layer_data->Unwrap(pBeginInfo->pInheritanceInfo->framebuffer);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.BeginCommandBuffer(commandBuffer, (const VkCommandBufferBeginInfo*)local_pBeginInfo);
    return result;
}



// Skip vkCreateInstance dispatch, manually generated

// Skip vkDestroyInstance dispatch, manually generated

VkResult DispatchEnumeratePhysicalDevices(
    VkInstance                                  instance,
    uint32_t*                                   pPhysicalDeviceCount,
    VkPhysicalDevice*                           pPhysicalDevices)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(instance), layer_data_map);
    VkResult result = layer_data->instance_dispatch_table.EnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);

    return result;
}

void DispatchGetPhysicalDeviceFeatures(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceFeatures*                   pFeatures)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    layer_data->instance_dispatch_table.GetPhysicalDeviceFeatures(physicalDevice, pFeatures);

}

void DispatchGetPhysicalDeviceFormatProperties(
    VkPhysicalDevice                            physicalDevice,
    VkFormat                                    format,
    VkFormatProperties*                         pFormatProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    layer_data->instance_dispatch_table.GetPhysicalDeviceFormatProperties(physicalDevice, format, pFormatProperties);

}

VkResult DispatchGetPhysicalDeviceImageFormatProperties(
    VkPhysicalDevice                            physicalDevice,
    VkFormat                                    format,
    VkImageType                                 type,
    VkImageTiling                               tiling,
    VkImageUsageFlags                           usage,
    VkImageCreateFlags                          flags,
    VkImageFormatProperties*                    pImageFormatProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    VkResult result = layer_data->instance_dispatch_table.GetPhysicalDeviceImageFormatProperties(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);

    return result;
}

void DispatchGetPhysicalDeviceProperties(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceProperties*                 pProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    layer_data->instance_dispatch_table.GetPhysicalDeviceProperties(physicalDevice, pProperties);

}

void DispatchGetPhysicalDeviceQueueFamilyProperties(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pQueueFamilyPropertyCount,
    VkQueueFamilyProperties*                    pQueueFamilyProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    layer_data->instance_dispatch_table.GetPhysicalDeviceQueueFamilyProperties(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);

}

void DispatchGetPhysicalDeviceMemoryProperties(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceMemoryProperties*           pMemoryProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    layer_data->instance_dispatch_table.GetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);

}

PFN_vkVoidFunction DispatchGetInstanceProcAddr(
    VkInstance                                  instance,
    const char*                                 pName)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(instance), layer_data_map);
    PFN_vkVoidFunction result = layer_data->instance_dispatch_table.GetInstanceProcAddr(instance, pName);

    return result;
}

PFN_vkVoidFunction DispatchGetDeviceProcAddr(
    VkDevice                                    device,
    const char*                                 pName)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    PFN_vkVoidFunction result = layer_data->device_dispatch_table.GetDeviceProcAddr(device, pName);

    return result;
}

// Skip vkCreateDevice dispatch, manually generated

// Skip vkDestroyDevice dispatch, manually generated

// Skip vkEnumerateInstanceExtensionProperties dispatch, manually generated

// Skip vkEnumerateDeviceExtensionProperties dispatch, manually generated

// Skip vkEnumerateInstanceLayerProperties dispatch, manually generated

// Skip vkEnumerateDeviceLayerProperties dispatch, manually generated

void DispatchGetDeviceQueue(
    VkDevice                                    device,
    uint32_t                                    queueFamilyIndex,
    uint32_t                                    queueIndex,
    VkQueue*                                    pQueue)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    layer_data->device_dispatch_table.GetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);

}

VkResult DispatchQueueSubmit(
    VkQueue                                     queue,
    uint32_t                                    submitCount,
    const VkSubmitInfo*                         pSubmits,
    VkFence                                     fence)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(queue), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.QueueSubmit(queue, submitCount, pSubmits, fence);
    safe_VkSubmitInfo *local_pSubmits = NULL;
    {
        if (pSubmits) {
            local_pSubmits = new safe_VkSubmitInfo[submitCount];
            for (uint32_t index0 = 0; index0 < submitCount; ++index0) {
                local_pSubmits[index0].initialize(&pSubmits[index0]);
                if (local_pSubmits[index0].pWaitSemaphores) {
                    for (uint32_t index1 = 0; index1 < local_pSubmits[index0].waitSemaphoreCount; ++index1) {
                        local_pSubmits[index0].pWaitSemaphores[index1] = layer_data->Unwrap(local_pSubmits[index0].pWaitSemaphores[index1]);
                    }
                }
                if (local_pSubmits[index0].pSignalSemaphores) {
                    for (uint32_t index1 = 0; index1 < local_pSubmits[index0].signalSemaphoreCount; ++index1) {
                        local_pSubmits[index0].pSignalSemaphores[index1] = layer_data->Unwrap(local_pSubmits[index0].pSignalSemaphores[index1]);
                    }
                }
            }
        }
        fence = layer_data->Unwrap(fence);
    }
    VkResult result = layer_data->device_dispatch_table.QueueSubmit(queue, submitCount, (const VkSubmitInfo*)local_pSubmits, fence);
    if (local_pSubmits) {
        delete[] local_pSubmits;
    }
    return result;
}

VkResult DispatchQueueWaitIdle(
    VkQueue                                     queue)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(queue), layer_data_map);
    VkResult result = layer_data->device_dispatch_table.QueueWaitIdle(queue);

    return result;
}

VkResult DispatchDeviceWaitIdle(
    VkDevice                                    device)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    VkResult result = layer_data->device_dispatch_table.DeviceWaitIdle(device);

    return result;
}

VkResult DispatchAllocateMemory(
    VkDevice                                    device,
    const VkMemoryAllocateInfo*                 pAllocateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDeviceMemory*                             pMemory)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.AllocateMemory(device, pAllocateInfo, pAllocator, pMemory);
    safe_VkMemoryAllocateInfo var_local_pAllocateInfo;
    safe_VkMemoryAllocateInfo *local_pAllocateInfo = NULL;
    {
        if (pAllocateInfo) {
            local_pAllocateInfo = &var_local_pAllocateInfo;
            local_pAllocateInfo->initialize(pAllocateInfo);
            WrapPnextChainHandles(layer_data, local_pAllocateInfo->pNext);
        }
    }
    VkResult result = layer_data->device_dispatch_table.AllocateMemory(device, (const VkMemoryAllocateInfo*)local_pAllocateInfo, pAllocator, pMemory);
    if (VK_SUCCESS == result) {
        *pMemory = layer_data->WrapNew(*pMemory);
    }
    return result;
}

VkResult DispatchMapMemory(
    VkDevice                                    device,
    VkDeviceMemory                              memory,
    VkDeviceSize                                offset,
    VkDeviceSize                                size,
    VkMemoryMapFlags                            flags,
    void**                                      ppData)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.MapMemory(device, memory, offset, size, flags, ppData);
    {
        memory = layer_data->Unwrap(memory);
    }
    VkResult result = layer_data->device_dispatch_table.MapMemory(device, memory, offset, size, flags, ppData);

    return result;
}

void DispatchUnmapMemory(
    VkDevice                                    device,
    VkDeviceMemory                              memory)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.UnmapMemory(device, memory);
    {
        memory = layer_data->Unwrap(memory);
    }
    layer_data->device_dispatch_table.UnmapMemory(device, memory);

}

VkResult DispatchFlushMappedMemoryRanges(
    VkDevice                                    device,
    uint32_t                                    memoryRangeCount,
    const VkMappedMemoryRange*                  pMemoryRanges)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.FlushMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
    safe_VkMappedMemoryRange *local_pMemoryRanges = NULL;
    {
        if (pMemoryRanges) {
            local_pMemoryRanges = new safe_VkMappedMemoryRange[memoryRangeCount];
            for (uint32_t index0 = 0; index0 < memoryRangeCount; ++index0) {
                local_pMemoryRanges[index0].initialize(&pMemoryRanges[index0]);
                if (pMemoryRanges[index0].memory) {
                    local_pMemoryRanges[index0].memory = layer_data->Unwrap(pMemoryRanges[index0].memory);
                }
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.FlushMappedMemoryRanges(device, memoryRangeCount, (const VkMappedMemoryRange*)local_pMemoryRanges);
    if (local_pMemoryRanges) {
        delete[] local_pMemoryRanges;
    }
    return result;
}

VkResult DispatchInvalidateMappedMemoryRanges(
    VkDevice                                    device,
    uint32_t                                    memoryRangeCount,
    const VkMappedMemoryRange*                  pMemoryRanges)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.InvalidateMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
    safe_VkMappedMemoryRange *local_pMemoryRanges = NULL;
    {
        if (pMemoryRanges) {
            local_pMemoryRanges = new safe_VkMappedMemoryRange[memoryRangeCount];
            for (uint32_t index0 = 0; index0 < memoryRangeCount; ++index0) {
                local_pMemoryRanges[index0].initialize(&pMemoryRanges[index0]);
                if (pMemoryRanges[index0].memory) {
                    local_pMemoryRanges[index0].memory = layer_data->Unwrap(pMemoryRanges[index0].memory);
                }
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.InvalidateMappedMemoryRanges(device, memoryRangeCount, (const VkMappedMemoryRange*)local_pMemoryRanges);
    if (local_pMemoryRanges) {
        delete[] local_pMemoryRanges;
    }
    return result;
}

void DispatchGetDeviceMemoryCommitment(
    VkDevice                                    device,
    VkDeviceMemory                              memory,
    VkDeviceSize*                               pCommittedMemoryInBytes)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetDeviceMemoryCommitment(device, memory, pCommittedMemoryInBytes);
    {
        memory = layer_data->Unwrap(memory);
    }
    layer_data->device_dispatch_table.GetDeviceMemoryCommitment(device, memory, pCommittedMemoryInBytes);

}

VkResult DispatchBindBufferMemory(
    VkDevice                                    device,
    VkBuffer                                    buffer,
    VkDeviceMemory                              memory,
    VkDeviceSize                                memoryOffset)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.BindBufferMemory(device, buffer, memory, memoryOffset);
    {
        buffer = layer_data->Unwrap(buffer);
        memory = layer_data->Unwrap(memory);
    }
    VkResult result = layer_data->device_dispatch_table.BindBufferMemory(device, buffer, memory, memoryOffset);

    return result;
}

VkResult DispatchBindImageMemory(
    VkDevice                                    device,
    VkImage                                     image,
    VkDeviceMemory                              memory,
    VkDeviceSize                                memoryOffset)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.BindImageMemory(device, image, memory, memoryOffset);
    {
        image = layer_data->Unwrap(image);
        memory = layer_data->Unwrap(memory);
    }
    VkResult result = layer_data->device_dispatch_table.BindImageMemory(device, image, memory, memoryOffset);

    return result;
}

void DispatchGetBufferMemoryRequirements(
    VkDevice                                    device,
    VkBuffer                                    buffer,
    VkMemoryRequirements*                       pMemoryRequirements)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
    {
        buffer = layer_data->Unwrap(buffer);
    }
    layer_data->device_dispatch_table.GetBufferMemoryRequirements(device, buffer, pMemoryRequirements);

}

void DispatchGetImageMemoryRequirements(
    VkDevice                                    device,
    VkImage                                     image,
    VkMemoryRequirements*                       pMemoryRequirements)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetImageMemoryRequirements(device, image, pMemoryRequirements);
    {
        image = layer_data->Unwrap(image);
    }
    layer_data->device_dispatch_table.GetImageMemoryRequirements(device, image, pMemoryRequirements);

}

VkResult DispatchCreateFence(
    VkDevice                                    device,
    const VkFenceCreateInfo*                    pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFence*                                    pFence)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreateFence(device, pCreateInfo, pAllocator, pFence);
    VkResult result = layer_data->device_dispatch_table.CreateFence(device, pCreateInfo, pAllocator, pFence);
    if (VK_SUCCESS == result) {
        *pFence = layer_data->WrapNew(*pFence);
    }
    return result;
}

void DispatchDestroyFence(
    VkDevice                                    device,
    VkFence                                     fence,
    const VkAllocationCallbacks*                pAllocator)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroyFence(device, fence, pAllocator);
    uint64_t fence_id = reinterpret_cast<uint64_t &>(fence);
    auto iter = unique_id_mapping.pop(fence_id);
    if (iter != unique_id_mapping.end()) {
        fence = (VkFence)iter->second;
    } else {
        fence = (VkFence)0;
    }
    layer_data->device_dispatch_table.DestroyFence(device, fence, pAllocator);

}

VkResult DispatchResetFences(
    VkDevice                                    device,
    uint32_t                                    fenceCount,
    const VkFence*                              pFences)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.ResetFences(device, fenceCount, pFences);
    VkFence var_local_pFences[DISPATCH_MAX_STACK_ALLOCATIONS];
    VkFence *local_pFences = NULL;
    {
        if (pFences) {
            local_pFences = fenceCount > DISPATCH_MAX_STACK_ALLOCATIONS ? new VkFence[fenceCount] : var_local_pFences;
            for (uint32_t index0 = 0; index0 < fenceCount; ++index0) {
                local_pFences[index0] = layer_data->Unwrap(pFences[index0]);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.ResetFences(device, fenceCount, (const VkFence*)local_pFences);
    if (local_pFences != var_local_pFences)
        delete[] local_pFences;
    return result;
}

VkResult DispatchGetFenceStatus(
    VkDevice                                    device,
    VkFence                                     fence)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetFenceStatus(device, fence);
    {
        fence = layer_data->Unwrap(fence);
    }
    VkResult result = layer_data->device_dispatch_table.GetFenceStatus(device, fence);

    return result;
}

VkResult DispatchWaitForFences(
    VkDevice                                    device,
    uint32_t                                    fenceCount,
    const VkFence*                              pFences,
    VkBool32                                    waitAll,
    uint64_t                                    timeout)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.WaitForFences(device, fenceCount, pFences, waitAll, timeout);
    VkFence var_local_pFences[DISPATCH_MAX_STACK_ALLOCATIONS];
    VkFence *local_pFences = NULL;
    {
        if (pFences) {
            local_pFences = fenceCount > DISPATCH_MAX_STACK_ALLOCATIONS ? new VkFence[fenceCount] : var_local_pFences;
            for (uint32_t index0 = 0; index0 < fenceCount; ++index0) {
                local_pFences[index0] = layer_data->Unwrap(pFences[index0]);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.WaitForFences(device, fenceCount, (const VkFence*)local_pFences, waitAll, timeout);
    if (local_pFences != var_local_pFences)
        delete[] local_pFences;
    return result;
}

VkResult DispatchCreateSemaphore(
    VkDevice                                    device,
    const VkSemaphoreCreateInfo*                pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSemaphore*                                pSemaphore)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreateSemaphore(device, pCreateInfo, pAllocator, pSemaphore);
    VkResult result = layer_data->device_dispatch_table.CreateSemaphore(device, pCreateInfo, pAllocator, pSemaphore);
    if (VK_SUCCESS == result) {
        *pSemaphore = layer_data->WrapNew(*pSemaphore);
    }
    return result;
}

void DispatchDestroySemaphore(
    VkDevice                                    device,
    VkSemaphore                                 semaphore,
    const VkAllocationCallbacks*                pAllocator)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroySemaphore(device, semaphore, pAllocator);
    uint64_t semaphore_id = reinterpret_cast<uint64_t &>(semaphore);
    auto iter = unique_id_mapping.pop(semaphore_id);
    if (iter != unique_id_mapping.end()) {
        semaphore = (VkSemaphore)iter->second;
    } else {
        semaphore = (VkSemaphore)0;
    }
    layer_data->device_dispatch_table.DestroySemaphore(device, semaphore, pAllocator);

}

VkResult DispatchCreateEvent(
    VkDevice                                    device,
    const VkEventCreateInfo*                    pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkEvent*                                    pEvent)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreateEvent(device, pCreateInfo, pAllocator, pEvent);
    VkResult result = layer_data->device_dispatch_table.CreateEvent(device, pCreateInfo, pAllocator, pEvent);
    if (VK_SUCCESS == result) {
        *pEvent = layer_data->WrapNew(*pEvent);
    }
    return result;
}

void DispatchDestroyEvent(
    VkDevice                                    device,
    VkEvent                                     event,
    const VkAllocationCallbacks*                pAllocator)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroyEvent(device, event, pAllocator);
    uint64_t event_id = reinterpret_cast<uint64_t &>(event);
    auto iter = unique_id_mapping.pop(event_id);
    if (iter != unique_id_mapping.end()) {
        event = (VkEvent)iter->second;
    } else {
        event = (VkEvent)0;
    }
    layer_data->device_dispatch_table.DestroyEvent(device, event, pAllocator);

}

VkResult DispatchGetEventStatus(
    VkDevice                                    device,
    VkEvent                                     event)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetEventStatus(device, event);
    {
        event = layer_data->Unwrap(event);
    }
    VkResult result = layer_data->device_dispatch_table.GetEventStatus(device, event);

    return result;
}

VkResult DispatchSetEvent(
    VkDevice                                    device,
    VkEvent                                     event)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.SetEvent(device, event);
    {
        event = layer_data->Unwrap(event);
    }
    VkResult result = layer_data->device_dispatch_table.SetEvent(device, event);

    return result;
}

VkResult DispatchResetEvent(
    VkDevice                                    device,
    VkEvent                                     event)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.ResetEvent(device, event);
    {
        event = layer_data->Unwrap(event);
    }
    VkResult result = layer_data->device_dispatch_table.ResetEvent(device, event);

    return result;
}

VkResult DispatchCreateQueryPool(
    VkDevice                                    device,
    const VkQueryPoolCreateInfo*                pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkQueryPool*                                pQueryPool)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreateQueryPool(device, pCreateInfo, pAllocator, pQueryPool);
    VkResult result = layer_data->device_dispatch_table.CreateQueryPool(device, pCreateInfo, pAllocator, pQueryPool);
    if (VK_SUCCESS == result) {
        *pQueryPool = layer_data->WrapNew(*pQueryPool);
    }
    return result;
}

VkResult DispatchGetQueryPoolResults(
    VkDevice                                    device,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount,
    size_t                                      dataSize,
    void*                                       pData,
    VkDeviceSize                                stride,
    VkQueryResultFlags                          flags)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
    {
        queryPool = layer_data->Unwrap(queryPool);
    }
    VkResult result = layer_data->device_dispatch_table.GetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);

    return result;
}

VkResult DispatchCreateBuffer(
    VkDevice                                    device,
    const VkBufferCreateInfo*                   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkBuffer*                                   pBuffer)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
    VkResult result = layer_data->device_dispatch_table.CreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
    if (VK_SUCCESS == result) {
        *pBuffer = layer_data->WrapNew(*pBuffer);
    }
    return result;
}

void DispatchDestroyBuffer(
    VkDevice                                    device,
    VkBuffer                                    buffer,
    const VkAllocationCallbacks*                pAllocator)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroyBuffer(device, buffer, pAllocator);
    uint64_t buffer_id = reinterpret_cast<uint64_t &>(buffer);
    auto iter = unique_id_mapping.pop(buffer_id);
    if (iter != unique_id_mapping.end()) {
        buffer = (VkBuffer)iter->second;
    } else {
        buffer = (VkBuffer)0;
    }
    layer_data->device_dispatch_table.DestroyBuffer(device, buffer, pAllocator);

}

VkResult DispatchCreateBufferView(
    VkDevice                                    device,
    const VkBufferViewCreateInfo*               pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkBufferView*                               pView)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreateBufferView(device, pCreateInfo, pAllocator, pView);
    safe_VkBufferViewCreateInfo var_local_pCreateInfo;
    safe_VkBufferViewCreateInfo *local_pCreateInfo = NULL;
    {
        if (pCreateInfo) {
            local_pCreateInfo = &var_local_pCreateInfo;
            local_pCreateInfo->initialize(pCreateInfo);
            if (pCreateInfo->buffer) {
                local_pCreateInfo->buffer = layer_data->Unwrap(pCreateInfo->buffer);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.CreateBufferView(device, (const VkBufferViewCreateInfo*)local_pCreateInfo, pAllocator, pView);
    if (VK_SUCCESS == result) {
        *pView = layer_data->WrapNew(*pView);
    }
    return result;
}

void DispatchDestroyBufferView(
    VkDevice                                    device,
    VkBufferView                                bufferView,
    const VkAllocationCallbacks*                pAllocator)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroyBufferView(device, bufferView, pAllocator);
    uint64_t bufferView_id = reinterpret_cast<uint64_t &>(bufferView);
    auto iter = unique_id_mapping.pop(bufferView_id);
    if (iter != unique_id_mapping.end()) {
        bufferView = (VkBufferView)iter->second;
    } else {
        bufferView = (VkBufferView)0;
    }
    layer_data->device_dispatch_table.DestroyBufferView(device, bufferView, pAllocator);

}

VkResult DispatchCreateImage(
    VkDevice                                    device,
    const VkImageCreateInfo*                    pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkImage*                                    pImage)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreateImage(device, pCreateInfo, pAllocator, pImage);
    safe_VkImageCreateInfo var_local_pCreateInfo;
    safe_VkImageCreateInfo *local_pCreateInfo = NULL;
    {
        if (pCreateInfo) {
            local_pCreateInfo = &var_local_pCreateInfo;
            local_pCreateInfo->initialize(pCreateInfo);
            WrapPnextChainHandles(layer_data, local_pCreateInfo->pNext);
        }
    }
    VkResult result = layer_data->device_dispatch_table.CreateImage(device, (const VkImageCreateInfo*)local_pCreateInfo, pAllocator, pImage);
    if (VK_SUCCESS == result) {
        *pImage = layer_data->WrapNew(*pImage);
    }
    return result;
}

void DispatchDestroyImage(
    VkDevice                                    device,
    VkImage                                     image,
    const VkAllocationCallbacks*                pAllocator)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroyImage(device, image, pAllocator);
    uint64_t image_id = reinterpret_cast<uint64_t &>(image);
    auto iter = unique_id_mapping.pop(image_id);
    if (iter != unique_id_mapping.end()) {
        image = (VkImage)iter->second;
    } else {
        image = (VkImage)0;
    }
    layer_data->device_dispatch_table.DestroyImage(device, image, pAllocator);

}

void DispatchGetImageSubresourceLayout(
    VkDevice                                    device,
    VkImage                                     image,
    const VkImageSubresource*                   pSubresource,
    VkSubresourceLayout*                        pLayout)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetImageSubresourceLayout(device, image, pSubresource, pLayout);
    {
        image = layer_data->Unwrap(image);
    }
    layer_data->device_dispatch_table.GetImageSubresourceLayout(device, image, pSubresource, pLayout);

}

VkResult DispatchCreateImageView(
    VkDevice                                    device,
    const VkImageViewCreateInfo*                pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkImageView*                                pView)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreateImageView(device, pCreateInfo, pAllocator, pView);
    safe_VkImageViewCreateInfo var_local_pCreateInfo;
    safe_VkImageViewCreateInfo *local_pCreateInfo = NULL;
    {
        if (pCreateInfo) {
            local_pCreateInfo = &var_local_pCreateInfo;
            local_pCreateInfo->initialize(pCreateInfo);
            if (pCreateInfo->image) {
                local_pCreateInfo->image = layer_data->Unwrap(pCreateInfo->image);
            }
            WrapPnextChainHandles(layer_data, local_pCreateInfo->pNext);
        }
    }
    VkResult result = layer_data->device_dispatch_table.CreateImageView(device, (const VkImageViewCreateInfo*)local_pCreateInfo, pAllocator, pView);
    if (VK_SUCCESS == result) {
        *pView = layer_data->WrapNew(*pView);
    }
    return result;
}

void DispatchDestroyImageView(
    VkDevice                                    device,
    VkImageView                                 imageView,
    const VkAllocationCallbacks*                pAllocator)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroyImageView(device, imageView, pAllocator);
    uint64_t imageView_id = reinterpret_cast<uint64_t &>(imageView);
    auto iter = unique_id_mapping.pop(imageView_id);
    if (iter != unique_id_mapping.end()) {
        imageView = (VkImageView)iter->second;
    } else {
        imageView = (VkImageView)0;
    }
    layer_data->device_dispatch_table.DestroyImageView(device, imageView, pAllocator);

}

VkResult DispatchCreatePipelineCache(
    VkDevice                                    device,
    const VkPipelineCacheCreateInfo*            pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkPipelineCache*                            pPipelineCache)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreatePipelineCache(device, pCreateInfo, pAllocator, pPipelineCache);
    VkResult result = layer_data->device_dispatch_table.CreatePipelineCache(device, pCreateInfo, pAllocator, pPipelineCache);
    if (VK_SUCCESS == result) {
        *pPipelineCache = layer_data->WrapNew(*pPipelineCache);
    }
    return result;
}

void DispatchDestroyPipelineCache(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    const VkAllocationCallbacks*                pAllocator)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroyPipelineCache(device, pipelineCache, pAllocator);
    uint64_t pipelineCache_id = reinterpret_cast<uint64_t &>(pipelineCache);
    auto iter = unique_id_mapping.pop(pipelineCache_id);
    if (iter != unique_id_mapping.end()) {
        pipelineCache = (VkPipelineCache)iter->second;
    } else {
        pipelineCache = (VkPipelineCache)0;
    }
    layer_data->device_dispatch_table.DestroyPipelineCache(device, pipelineCache, pAllocator);

}

// Skip vkCreateGraphicsPipelines dispatch, manually generated

VkResult DispatchCreateComputePipelines(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    uint32_t                                    createInfoCount,
    const VkComputePipelineCreateInfo*          pCreateInfos,
    const VkAllocationCallbacks*                pAllocator,
    VkPipeline*                                 pPipelines)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    safe_VkComputePipelineCreateInfo *local_pCreateInfos = NULL;
    {
        pipelineCache = layer_data->Unwrap(pipelineCache);
        if (pCreateInfos) {
            local_pCreateInfos = new safe_VkComputePipelineCreateInfo[createInfoCount];
            for (uint32_t index0 = 0; index0 < createInfoCount; ++index0) {
                local_pCreateInfos[index0].initialize(&pCreateInfos[index0]);
                if (pCreateInfos[index0].stage.module) {
                    local_pCreateInfos[index0].stage.module = layer_data->Unwrap(pCreateInfos[index0].stage.module);
                }
                if (pCreateInfos[index0].layout) {
                    local_pCreateInfos[index0].layout = layer_data->Unwrap(pCreateInfos[index0].layout);
                }
                if (pCreateInfos[index0].basePipelineHandle) {
                    local_pCreateInfos[index0].basePipelineHandle = layer_data->Unwrap(pCreateInfos[index0].basePipelineHandle);
                }
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.CreateComputePipelines(device, pipelineCache, createInfoCount, (const VkComputePipelineCreateInfo*)local_pCreateInfos, pAllocator, pPipelines);
    if (local_pCreateInfos) {
        delete[] local_pCreateInfos;
    }
    {
        for (uint32_t index0 = 0; index0 < createInfoCount; index0++) {
            if (pPipelines[index0] != VK_NULL_HANDLE) {
                pPipelines[index0] = layer_data->WrapNew(pPipelines[index0]);
            }
        }
    }
    return result;
}

void DispatchDestroyPipeline(
    VkDevice                                    device,
    VkPipeline                                  pipeline,
    const VkAllocationCallbacks*                pAllocator)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroyPipeline(device, pipeline, pAllocator);
    uint64_t pipeline_id = reinterpret_cast<uint64_t &>(pipeline);
    auto iter = unique_id_mapping.pop(pipeline_id);
    if (iter != unique_id_mapping.end()) {
        pipeline = (VkPipeline)iter->second;
    } else {
        pipeline = (VkPipeline)0;
    }
    layer_data->device_dispatch_table.DestroyPipeline(device, pipeline, pAllocator);

}

VkResult DispatchCreatePipelineLayout(
    VkDevice                                    device,
    const VkPipelineLayoutCreateInfo*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkPipelineLayout*                           pPipelineLayout)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreatePipelineLayout(device, pCreateInfo, pAllocator, pPipelineLayout);
    safe_VkPipelineLayoutCreateInfo var_local_pCreateInfo;
    safe_VkPipelineLayoutCreateInfo *local_pCreateInfo = NULL;
    {
        if (pCreateInfo) {
            local_pCreateInfo = &var_local_pCreateInfo;
            local_pCreateInfo->initialize(pCreateInfo);
            if (local_pCreateInfo->pSetLayouts) {
                for (uint32_t index1 = 0; index1 < local_pCreateInfo->setLayoutCount; ++index1) {
                    local_pCreateInfo->pSetLayouts[index1] = layer_data->Unwrap(local_pCreateInfo->pSetLayouts[index1]);
                }
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.CreatePipelineLayout(device, (const VkPipelineLayoutCreateInfo*)local_pCreateInfo, pAllocator, pPipelineLayout);
    if (VK_SUCCESS == result) {
        *pPipelineLayout = layer_data->WrapNew(*pPipelineLayout);
    }
    return result;
}

void DispatchDestroyPipelineLayout(
    VkDevice                                    device,
    VkPipelineLayout                            pipelineLayout,
    const VkAllocationCallbacks*                pAllocator)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroyPipelineLayout(device, pipelineLayout, pAllocator);
    uint64_t pipelineLayout_id = reinterpret_cast<uint64_t &>(pipelineLayout);
    auto iter = unique_id_mapping.pop(pipelineLayout_id);
    if (iter != unique_id_mapping.end()) {
        pipelineLayout = (VkPipelineLayout)iter->second;
    } else {
        pipelineLayout = (VkPipelineLayout)0;
    }
    layer_data->device_dispatch_table.DestroyPipelineLayout(device, pipelineLayout, pAllocator);

}

VkResult DispatchCreateSampler(
    VkDevice                                    device,
    const VkSamplerCreateInfo*                  pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSampler*                                  pSampler)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreateSampler(device, pCreateInfo, pAllocator, pSampler);
    safe_VkSamplerCreateInfo var_local_pCreateInfo;
    safe_VkSamplerCreateInfo *local_pCreateInfo = NULL;
    {
        if (pCreateInfo) {
            local_pCreateInfo = &var_local_pCreateInfo;
            local_pCreateInfo->initialize(pCreateInfo);
            WrapPnextChainHandles(layer_data, local_pCreateInfo->pNext);
        }
    }
    VkResult result = layer_data->device_dispatch_table.CreateSampler(device, (const VkSamplerCreateInfo*)local_pCreateInfo, pAllocator, pSampler);
    if (VK_SUCCESS == result) {
        *pSampler = layer_data->WrapNew(*pSampler);
    }
    return result;
}

void DispatchDestroySampler(
    VkDevice                                    device,
    VkSampler                                   sampler,
    const VkAllocationCallbacks*                pAllocator)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroySampler(device, sampler, pAllocator);
    uint64_t sampler_id = reinterpret_cast<uint64_t &>(sampler);
    auto iter = unique_id_mapping.pop(sampler_id);
    if (iter != unique_id_mapping.end()) {
        sampler = (VkSampler)iter->second;
    } else {
        sampler = (VkSampler)0;
    }
    layer_data->device_dispatch_table.DestroySampler(device, sampler, pAllocator);

}

VkResult DispatchCreateDescriptorSetLayout(
    VkDevice                                    device,
    const VkDescriptorSetLayoutCreateInfo*      pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDescriptorSetLayout*                      pSetLayout)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreateDescriptorSetLayout(device, pCreateInfo, pAllocator, pSetLayout);
    safe_VkDescriptorSetLayoutCreateInfo var_local_pCreateInfo;
    safe_VkDescriptorSetLayoutCreateInfo *local_pCreateInfo = NULL;
    {
        if (pCreateInfo) {
            local_pCreateInfo = &var_local_pCreateInfo;
            local_pCreateInfo->initialize(pCreateInfo);
            if (local_pCreateInfo->pBindings) {
                for (uint32_t index1 = 0; index1 < local_pCreateInfo->bindingCount; ++index1) {
                    if (local_pCreateInfo->pBindings[index1].pImmutableSamplers) {
                        for (uint32_t index2 = 0; index2 < local_pCreateInfo->pBindings[index1].descriptorCount; ++index2) {
                            local_pCreateInfo->pBindings[index1].pImmutableSamplers[index2] = layer_data->Unwrap(local_pCreateInfo->pBindings[index1].pImmutableSamplers[index2]);
                        }
                    }
                }
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.CreateDescriptorSetLayout(device, (const VkDescriptorSetLayoutCreateInfo*)local_pCreateInfo, pAllocator, pSetLayout);
    if (VK_SUCCESS == result) {
        *pSetLayout = layer_data->WrapNew(*pSetLayout);
    }
    return result;
}

void DispatchDestroyDescriptorSetLayout(
    VkDevice                                    device,
    VkDescriptorSetLayout                       descriptorSetLayout,
    const VkAllocationCallbacks*                pAllocator)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);
    uint64_t descriptorSetLayout_id = reinterpret_cast<uint64_t &>(descriptorSetLayout);
    auto iter = unique_id_mapping.pop(descriptorSetLayout_id);
    if (iter != unique_id_mapping.end()) {
        descriptorSetLayout = (VkDescriptorSetLayout)iter->second;
    } else {
        descriptorSetLayout = (VkDescriptorSetLayout)0;
    }
    layer_data->device_dispatch_table.DestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);

}

VkResult DispatchCreateDescriptorPool(
    VkDevice                                    device,
    const VkDescriptorPoolCreateInfo*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDescriptorPool*                           pDescriptorPool)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreateDescriptorPool(device, pCreateInfo, pAllocator, pDescriptorPool);
    VkResult result = layer_data->device_dispatch_table.CreateDescriptorPool(device, pCreateInfo, pAllocator, pDescriptorPool);
    if (VK_SUCCESS == result) {
        *pDescriptorPool = layer_data->WrapNew(*pDescriptorPool);
    }
    return result;
}

// Skip vkResetDescriptorPool dispatch, manually generated

// Skip vkAllocateDescriptorSets dispatch, manually generated

// Skip vkFreeDescriptorSets dispatch, manually generated

void DispatchUpdateDescriptorSets(
    VkDevice                                    device,
    uint32_t                                    descriptorWriteCount,
    const VkWriteDescriptorSet*                 pDescriptorWrites,
    uint32_t                                    descriptorCopyCount,
    const VkCopyDescriptorSet*                  pDescriptorCopies)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.UpdateDescriptorSets(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
    safe_VkWriteDescriptorSet *local_pDescriptorWrites = NULL;
    safe_VkCopyDescriptorSet *local_pDescriptorCopies = NULL;
    {
        if (pDescriptorWrites) {
            local_pDescriptorWrites = new safe_VkWriteDescriptorSet[descriptorWriteCount];
            for (uint32_t index0 = 0; index0 < descriptorWriteCount; ++index0) {
                local_pDescriptorWrites[index0].initialize(&pDescriptorWrites[index0]);
                if (pDescriptorWrites[index0].dstSet) {
                    local_pDescriptorWrites[index0].dstSet = layer_data->Unwrap(pDescriptorWrites[index0].dstSet);
                }
                if (local_pDescriptorWrites[index0].pImageInfo) {
                    for (uint32_t index1 = 0; index1 < local_pDescriptorWrites[index0].descriptorCount; ++index1) {
                        if (pDescriptorWrites[index0].pImageInfo[index1].sampler) {
                            local_pDescriptorWrites[index0].pImageInfo[index1].sampler = layer_data->Unwrap(pDescriptorWrites[index0].pImageInfo[index1].sampler);
                        }
                        if (pDescriptorWrites[index0].pImageInfo[index1].imageView) {
                            local_pDescriptorWrites[index0].pImageInfo[index1].imageView = layer_data->Unwrap(pDescriptorWrites[index0].pImageInfo[index1].imageView);
                        }
                    }
                }
                if (local_pDescriptorWrites[index0].pBufferInfo) {
                    for (uint32_t index1 = 0; index1 < local_pDescriptorWrites[index0].descriptorCount; ++index1) {
                        if (pDescriptorWrites[index0].pBufferInfo[index1].buffer) {
                            local_pDescriptorWrites[index0].pBufferInfo[index1].buffer = layer_data->Unwrap(pDescriptorWrites[index0].pBufferInfo[index1].buffer);
                        }
                    }
                }
                if (local_pDescriptorWrites[index0].pTexelBufferView) {
                    for (uint32_t index1 = 0; index1 < local_pDescriptorWrites[index0].descriptorCount; ++index1) {
                        local_pDescriptorWrites[index0].pTexelBufferView[index1] = layer_data->Unwrap(local_pDescriptorWrites[index0].pTexelBufferView[index1]);
                    }
                }
            }
        }
        if (pDescriptorCopies) {
            local_pDescriptorCopies = new safe_VkCopyDescriptorSet[descriptorCopyCount];
            for (uint32_t index0 = 0; index0 < descriptorCopyCount; ++index0) {
                local_pDescriptorCopies[index0].initialize(&pDescriptorCopies[index0]);
                if (pDescriptorCopies[index0].srcSet) {
                    local_pDescriptorCopies[index0].srcSet = layer_data->Unwrap(pDescriptorCopies[index0].srcSet);
                }
                if (pDescriptorCopies[index0].dstSet) {
                    local_pDescriptorCopies[index0].dstSet = layer_data->Unwrap(pDescriptorCopies[index0].dstSet);
                }
            }
        }
    }
    layer_data->device_dispatch_table.UpdateDescriptorSets(device, descriptorWriteCount, (const VkWriteDescriptorSet*)local_pDescriptorWrites, descriptorCopyCount, (const VkCopyDescriptorSet*)local_pDescriptorCopies);
    if (local_pDescriptorWrites) {
        delete[] local_pDescriptorWrites;
    }
    if (local_pDescriptorCopies) {
        delete[] local_pDescriptorCopies;
    }
}

VkResult DispatchCreateFramebuffer(
    VkDevice                                    device,
    const VkFramebufferCreateInfo*              pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFramebuffer*                              pFramebuffer)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreateFramebuffer(device, pCreateInfo, pAllocator, pFramebuffer);
    safe_VkFramebufferCreateInfo var_local_pCreateInfo;
    safe_VkFramebufferCreateInfo *local_pCreateInfo = NULL;
    {
        if (pCreateInfo) {
            local_pCreateInfo = &var_local_pCreateInfo;
            local_pCreateInfo->initialize(pCreateInfo);
            if (pCreateInfo->renderPass) {
                local_pCreateInfo->renderPass = layer_data->Unwrap(pCreateInfo->renderPass);
            }
            if (local_pCreateInfo->pAttachments) {
                for (uint32_t index1 = 0; index1 < local_pCreateInfo->attachmentCount; ++index1) {
                    local_pCreateInfo->pAttachments[index1] = layer_data->Unwrap(local_pCreateInfo->pAttachments[index1]);
                }
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.CreateFramebuffer(device, (const VkFramebufferCreateInfo*)local_pCreateInfo, pAllocator, pFramebuffer);
    if (VK_SUCCESS == result) {
        *pFramebuffer = layer_data->WrapNew(*pFramebuffer);
    }
    return result;
}

void DispatchDestroyFramebuffer(
    VkDevice                                    device,
    VkFramebuffer                               framebuffer,
    const VkAllocationCallbacks*                pAllocator)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroyFramebuffer(device, framebuffer, pAllocator);
    uint64_t framebuffer_id = reinterpret_cast<uint64_t &>(framebuffer);
    auto iter = unique_id_mapping.pop(framebuffer_id);
    if (iter != unique_id_mapping.end()) {
        framebuffer = (VkFramebuffer)iter->second;
    } else {
        framebuffer = (VkFramebuffer)0;
    }
    layer_data->device_dispatch_table.DestroyFramebuffer(device, framebuffer, pAllocator);

}

// Skip vkCreateRenderPass dispatch, manually generated

// Skip vkDestroyRenderPass dispatch, manually generated

void DispatchGetRenderAreaGranularity(
    VkDevice                                    device,
    VkRenderPass                                renderPass,
    VkExtent2D*                                 pGranularity)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetRenderAreaGranularity(device, renderPass, pGranularity);
    {
        renderPass = layer_data->Unwrap(renderPass);
    }
    layer_data->device_dispatch_table.GetRenderAreaGranularity(device, renderPass, pGranularity);

}

VkResult DispatchCreateCommandPool(
    VkDevice                                    device,
    const VkCommandPoolCreateInfo*              pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkCommandPool*                              pCommandPool)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
    VkResult result = layer_data->device_dispatch_table.CreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
    if (VK_SUCCESS == result) {
        *pCommandPool = layer_data->WrapNew(*pCommandPool);
    }
    return result;
}

VkResult DispatchResetCommandPool(
    VkDevice                                    device,
    VkCommandPool                               commandPool,
    VkCommandPoolResetFlags                     flags)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.ResetCommandPool(device, commandPool, flags);
    {
        commandPool = layer_data->Unwrap(commandPool);
    }
    VkResult result = layer_data->device_dispatch_table.ResetCommandPool(device, commandPool, flags);

    return result;
}

// Skip vkAllocateCommandBuffers dispatch, manually generated

// Skip vkFreeCommandBuffers dispatch, manually generated

// Skip vkBeginCommandBuffer dispatch, manually generated

VkResult DispatchEndCommandBuffer(
    VkCommandBuffer                             commandBuffer)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    VkResult result = layer_data->device_dispatch_table.EndCommandBuffer(commandBuffer);

    return result;
}

VkResult DispatchResetCommandBuffer(
    VkCommandBuffer                             commandBuffer,
    VkCommandBufferResetFlags                   flags)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    VkResult result = layer_data->device_dispatch_table.ResetCommandBuffer(commandBuffer, flags);

    return result;
}

void DispatchCmdBindPipeline(
    VkCommandBuffer                             commandBuffer,
    VkPipelineBindPoint                         pipelineBindPoint,
    VkPipeline                                  pipeline)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
    {
        pipeline = layer_data->Unwrap(pipeline);
    }
    layer_data->device_dispatch_table.CmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);

}

void DispatchCmdSetViewport(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstViewport,
    uint32_t                                    viewportCount,
    const VkViewport*                           pViewports)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);

}

void DispatchCmdSetScissor(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstScissor,
    uint32_t                                    scissorCount,
    const VkRect2D*                             pScissors)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);

}

void DispatchCmdSetLineWidth(
    VkCommandBuffer                             commandBuffer,
    float                                       lineWidth)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetLineWidth(commandBuffer, lineWidth);

}

void DispatchCmdSetDepthBias(
    VkCommandBuffer                             commandBuffer,
    float                                       depthBiasConstantFactor,
    float                                       depthBiasClamp,
    float                                       depthBiasSlopeFactor)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);

}

void DispatchCmdSetBlendConstants(
    VkCommandBuffer                             commandBuffer,
    const float                                 blendConstants[4])
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetBlendConstants(commandBuffer, blendConstants);

}

void DispatchCmdSetDepthBounds(
    VkCommandBuffer                             commandBuffer,
    float                                       minDepthBounds,
    float                                       maxDepthBounds)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);

}

void DispatchCmdSetStencilCompareMask(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    compareMask)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);

}

void DispatchCmdSetStencilWriteMask(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    writeMask)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);

}

void DispatchCmdSetStencilReference(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    reference)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetStencilReference(commandBuffer, faceMask, reference);

}

void DispatchCmdBindDescriptorSets(
    VkCommandBuffer                             commandBuffer,
    VkPipelineBindPoint                         pipelineBindPoint,
    VkPipelineLayout                            layout,
    uint32_t                                    firstSet,
    uint32_t                                    descriptorSetCount,
    const VkDescriptorSet*                      pDescriptorSets,
    uint32_t                                    dynamicOffsetCount,
    const uint32_t*                             pDynamicOffsets)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
    VkDescriptorSet var_local_pDescriptorSets[DISPATCH_MAX_STACK_ALLOCATIONS];
    VkDescriptorSet *local_pDescriptorSets = NULL;
    {
        layout = layer_data->Unwrap(layout);
        if (pDescriptorSets) {
            local_pDescriptorSets = descriptorSetCount > DISPATCH_MAX_STACK_ALLOCATIONS ? new VkDescriptorSet[descriptorSetCount] : var_local_pDescriptorSets;
            for (uint32_t index0 = 0; index0 < descriptorSetCount; ++index0) {
                local_pDescriptorSets[index0] = layer_data->Unwrap(pDescriptorSets[index0]);
            }
        }
    }
    layer_data->device_dispatch_table.CmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, (const VkDescriptorSet*)local_pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
    if (local_pDescriptorSets != var_local_pDescriptorSets)
        delete[] local_pDescriptorSets;
}

void DispatchCmdBindIndexBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkIndexType                                 indexType)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
    {
        buffer = layer_data->Unwrap(buffer);
    }
    layer_data->device_dispatch_table.CmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);

}

void DispatchCmdBindVertexBuffers(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstBinding,
    uint32_t                                    bindingCount,
    const VkBuffer*                             pBuffers,
    const VkDeviceSize*                         pOffsets)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
    VkBuffer var_local_pBuffers[DISPATCH_MAX_STACK_ALLOCATIONS];
    VkBuffer *local_pBuffers = NULL;
    {
        if (pBuffers) {
            local_pBuffers = bindingCount > DISPATCH_MAX_STACK_ALLOCATIONS ? new VkBuffer[bindingCount] : var_local_pBuffers;
            for (uint32_t index0 = 0; index0 < bindingCount; ++index0) {
                local_pBuffers[index0] = layer_data->Unwrap(pBuffers[index0]);
            }
        }
    }
    layer_data->device_dispatch_table.CmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, (const VkBuffer*)local_pBuffers, pOffsets);
    if (local_pBuffers != var_local_pBuffers)
        delete[] local_pBuffers;
}

void DispatchCmdDraw(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    vertexCount,
    uint32_t                                    instanceCount,
    uint32_t                                    firstVertex,
    uint32_t                                    firstInstance)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);

}

void DispatchCmdDrawIndexed(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    indexCount,
    uint32_t                                    instanceCount,
    uint32_t                                    firstIndex,
    int32_t                                     vertexOffset,
    uint32_t                                    firstInstance)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);

}

void DispatchCmdDrawIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    uint32_t                                    drawCount,
    uint32_t                                    stride)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
    {
        buffer = layer_data->Unwrap(buffer);
    }
    layer_data->device_dispatch_table.CmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);

}

void DispatchCmdDrawIndexedIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    uint32_t                                    drawCount,
    uint32_t                                    stride)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
    {
        buffer = layer_data->Unwrap(buffer);
    }
    layer_data->device_dispatch_table.CmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);

}

void DispatchCmdDispatch(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    groupCountX,
    uint32_t                                    groupCountY,
    uint32_t                                    groupCountZ)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);

}

void DispatchCmdDispatchIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdDispatchIndirect(commandBuffer, buffer, offset);
    {
        buffer = layer_data->Unwrap(buffer);
    }
    layer_data->device_dispatch_table.CmdDispatchIndirect(commandBuffer, buffer, offset);

}

void DispatchCmdCopyBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    srcBuffer,
    VkBuffer                                    dstBuffer,
    uint32_t                                    regionCount,
    const VkBufferCopy*                         pRegions)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
    {
        srcBuffer = layer_data->Unwrap(srcBuffer);
        dstBuffer = layer_data->Unwrap(dstBuffer);
    }
    layer_data->device_dispatch_table.CmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);

}

void DispatchCmdCopyImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkImageCopy*                          pRegions)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
    {
        srcImage = layer_data->Unwrap(srcImage);
        dstImage = layer_data->Unwrap(dstImage);
    }
    layer_data->device_dispatch_table.CmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);

}

void DispatchCmdBlitImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkImageBlit*                          pRegions,
    VkFilter                                    filter)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
    {
        srcImage = layer_data->Unwrap(srcImage);
        dstImage = layer_data->Unwrap(dstImage);
    }
    layer_data->device_dispatch_table.CmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);

}

void DispatchCmdCopyBufferToImage(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    srcBuffer,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkBufferImageCopy*                    pRegions)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
    {
        srcBuffer = layer_data->Unwrap(srcBuffer);
        dstImage = layer_data->Unwrap(dstImage);
    }
    layer_data->device_dispatch_table.CmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);

}

void DispatchCmdCopyImageToBuffer(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkBuffer                                    dstBuffer,
    uint32_t                                    regionCount,
    const VkBufferImageCopy*                    pRegions)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
    {
        srcImage = layer_data->Unwrap(srcImage);
        dstBuffer = layer_data->Unwrap(dstBuffer);
    }
    layer_data->device_dispatch_table.CmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);

}

void DispatchCmdUpdateBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    VkDeviceSize                                dataSize,
    const void*                                 pData)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
    {
        dstBuffer = layer_data->Unwrap(dstBuffer);
    }
    layer_data->device_dispatch_table.CmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);

}

void DispatchCmdFillBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    VkDeviceSize                                size,
    uint32_t                                    data)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
    {
        dstBuffer = layer_data->Unwrap(dstBuffer);
    }
    layer_data->device_dispatch_table.CmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);

}

void DispatchCmdClearColorImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     image,
    VkImageLayout                               imageLayout,
    const VkClearColorValue*                    pColor,
    uint32_t                                    rangeCount,
    const VkImageSubresourceRange*              pRanges)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
    {
        image = layer_data->Unwrap(image);
    }
    layer_data->device_dispatch_table.CmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);

}

void DispatchCmdClearDepthStencilImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     image,
    VkImageLayout                               imageLayout,
    const VkClearDepthStencilValue*             pDepthStencil,
    uint32_t                                    rangeCount,
    const VkImageSubresourceRange*              pRanges)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
    {
        image = layer_data->Unwrap(image);
    }
    layer_data->device_dispatch_table.CmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);

}

void DispatchCmdClearAttachments(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    attachmentCount,
    const VkClearAttachment*                    pAttachments,
    uint32_t                                    rectCount,
    const VkClearRect*                          pRects)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);

}

void DispatchCmdResolveImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkImageResolve*                       pRegions)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
    {
        srcImage = layer_data->Unwrap(srcImage);
        dstImage = layer_data->Unwrap(dstImage);
    }
    layer_data->device_dispatch_table.CmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);

}

void DispatchCmdSetEvent(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    VkPipelineStageFlags                        stageMask)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdSetEvent(commandBuffer, event, stageMask);
    {
        event = layer_data->Unwrap(event);
    }
    layer_data->device_dispatch_table.CmdSetEvent(commandBuffer, event, stageMask);

}

void DispatchCmdResetEvent(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    VkPipelineStageFlags                        stageMask)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdResetEvent(commandBuffer, event, stageMask);
    {
        event = layer_data->Unwrap(event);
    }
    layer_data->device_dispatch_table.CmdResetEvent(commandBuffer, event, stageMask);

}

void DispatchCmdWaitEvents(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    eventCount,
    const VkEvent*                              pEvents,
    VkPipelineStageFlags                        srcStageMask,
    VkPipelineStageFlags                        dstStageMask,
    uint32_t                                    memoryBarrierCount,
    const VkMemoryBarrier*                      pMemoryBarriers,
    uint32_t                                    bufferMemoryBarrierCount,
    const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
    uint32_t                                    imageMemoryBarrierCount,
    const VkImageMemoryBarrier*                 pImageMemoryBarriers)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
    VkEvent var_local_pEvents[DISPATCH_MAX_STACK_ALLOCATIONS];
    VkEvent *local_pEvents = NULL;
    safe_VkBufferMemoryBarrier *local_pBufferMemoryBarriers = NULL;
    safe_VkImageMemoryBarrier *local_pImageMemoryBarriers = NULL;
    {
        if (pEvents) {
            local_pEvents = eventCount > DISPATCH_MAX_STACK_ALLOCATIONS ? new VkEvent[eventCount] : var_local_pEvents;
            for (uint32_t index0 = 0; index0 < eventCount; ++index0) {
                local_pEvents[index0] = layer_data->Unwrap(pEvents[index0]);
            }
        }
        if (pBufferMemoryBarriers) {
            local_pBufferMemoryBarriers = new safe_VkBufferMemoryBarrier[bufferMemoryBarrierCount];
            for (uint32_t index0 = 0; index0 < bufferMemoryBarrierCount; ++index0) {
                local_pBufferMemoryBarriers[index0].initialize(&pBufferMemoryBarriers[index0]);
                if (pBufferMemoryBarriers[index0].buffer) {
                    local_pBufferMemoryBarriers[index0].buffer = layer_data->Unwrap(pBufferMemoryBarriers[index0].buffer);
                }
            }
        }
        if (pImageMemoryBarriers) {
            local_pImageMemoryBarriers = new safe_VkImageMemoryBarrier[imageMemoryBarrierCount];
            for (uint32_t index0 = 0; index0 < imageMemoryBarrierCount; ++index0) {
                local_pImageMemoryBarriers[index0].initialize(&pImageMemoryBarriers[index0]);
                if (pImageMemoryBarriers[index0].image) {
                    local_pImageMemoryBarriers[index0].image = layer_data->Unwrap(pImageMemoryBarriers[index0].image);
                }
            }
        }
    }
    layer_data->device_dispatch_table.CmdWaitEvents(commandBuffer, eventCount, (const VkEvent*)local_pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, (const VkBufferMemoryBarrier*)local_pBufferMemoryBarriers, imageMemoryBarrierCount, (const VkImageMemoryBarrier*)local_pImageMemoryBarriers);
    if (local_pEvents != var_local_pEvents)
        delete[] local_pEvents;
    if (local_pBufferMemoryBarriers) {
        delete[] local_pBufferMemoryBarriers;
    }
    if (local_pImageMemoryBarriers) {
        delete[] local_pImageMemoryBarriers;
    }
}

void DispatchCmdPipelineBarrier(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlags                        srcStageMask,
    VkPipelineStageFlags                        dstStageMask,
    VkDependencyFlags                           dependencyFlags,
    uint32_t                                    memoryBarrierCount,
    const VkMemoryBarrier*                      pMemoryBarriers,
    uint32_t                                    bufferMemoryBarrierCount,
    const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
    uint32_t                                    imageMemoryBarrierCount,
    const VkImageMemoryBarrier*                 pImageMemoryBarriers)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
    safe_VkBufferMemoryBarrier *local_pBufferMemoryBarriers = NULL;
    safe_VkImageMemoryBarrier *local_pImageMemoryBarriers = NULL;
    {
        if (pBufferMemoryBarriers) {
            local_pBufferMemoryBarriers = new safe_VkBufferMemoryBarrier[bufferMemoryBarrierCount];
            for (uint32_t index0 = 0; index0 < bufferMemoryBarrierCount; ++index0) {
                local_pBufferMemoryBarriers[index0].initialize(&pBufferMemoryBarriers[index0]);
                if (pBufferMemoryBarriers[index0].buffer) {
                    local_pBufferMemoryBarriers[index0].buffer = layer_data->Unwrap(pBufferMemoryBarriers[index0].buffer);
                }
            }
        }
        if (pImageMemoryBarriers) {
            local_pImageMemoryBarriers = new safe_VkImageMemoryBarrier[imageMemoryBarrierCount];
            for (uint32_t index0 = 0; index0 < imageMemoryBarrierCount; ++index0) {
                local_pImageMemoryBarriers[index0].initialize(&pImageMemoryBarriers[index0]);
                if (pImageMemoryBarriers[index0].image) {
                    local_pImageMemoryBarriers[index0].image = layer_data->Unwrap(pImageMemoryBarriers[index0].image);
                }
            }
        }
    }
    layer_data->device_dispatch_table.CmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, (const VkBufferMemoryBarrier*)local_pBufferMemoryBarriers, imageMemoryBarrierCount, (const VkImageMemoryBarrier*)local_pImageMemoryBarriers);
    if (local_pBufferMemoryBarriers) {
        delete[] local_pBufferMemoryBarriers;
    }
    if (local_pImageMemoryBarriers) {
        delete[] local_pImageMemoryBarriers;
    }
}

void DispatchCmdBeginQuery(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    query,
    VkQueryControlFlags                         flags)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdBeginQuery(commandBuffer, queryPool, query, flags);
    {
        queryPool = layer_data->Unwrap(queryPool);
    }
    layer_data->device_dispatch_table.CmdBeginQuery(commandBuffer, queryPool, query, flags);

}

void DispatchCmdEndQuery(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    query)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdEndQuery(commandBuffer, queryPool, query);
    {
        queryPool = layer_data->Unwrap(queryPool);
    }
    layer_data->device_dispatch_table.CmdEndQuery(commandBuffer, queryPool, query);

}

void DispatchCmdResetQueryPool(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
    {
        queryPool = layer_data->Unwrap(queryPool);
    }
    layer_data->device_dispatch_table.CmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);

}

void DispatchCmdWriteTimestamp(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlagBits                     pipelineStage,
    VkQueryPool                                 queryPool,
    uint32_t                                    query)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
    {
        queryPool = layer_data->Unwrap(queryPool);
    }
    layer_data->device_dispatch_table.CmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);

}

void DispatchCmdCopyQueryPoolResults(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    VkDeviceSize                                stride,
    VkQueryResultFlags                          flags)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
    {
        queryPool = layer_data->Unwrap(queryPool);
        dstBuffer = layer_data->Unwrap(dstBuffer);
    }
    layer_data->device_dispatch_table.CmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);

}

void DispatchCmdPushConstants(
    VkCommandBuffer                             commandBuffer,
    VkPipelineLayout                            layout,
    VkShaderStageFlags                          stageFlags,
    uint32_t                                    offset,
    uint32_t                                    size,
    const void*                                 pValues)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
    {
        layout = layer_data->Unwrap(layout);
    }
    layer_data->device_dispatch_table.CmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);

}

void DispatchCmdBeginRenderPass(
    VkCommandBuffer                             commandBuffer,
    const VkRenderPassBeginInfo*                pRenderPassBegin,
    VkSubpassContents                           contents)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
    safe_VkRenderPassBeginInfo var_local_pRenderPassBegin;
    safe_VkRenderPassBeginInfo *local_pRenderPassBegin = NULL;
    {
        if (pRenderPassBegin) {
            local_pRenderPassBegin = &var_local_pRenderPassBegin;
            local_pRenderPassBegin->initialize(pRenderPassBegin);
            if (pRenderPassBegin->renderPass) {
                local_pRenderPassBegin->renderPass = layer_data->Unwrap(pRenderPassBegin->renderPass);
            }
            if (pRenderPassBegin->framebuffer) {
                local_pRenderPassBegin->framebuffer = layer_data->Unwrap(pRenderPassBegin->framebuffer);
            }
            WrapPnextChainHandles(layer_data, local_pRenderPassBegin->pNext);
        }
    }
    layer_data->device_dispatch_table.CmdBeginRenderPass(commandBuffer, (const VkRenderPassBeginInfo*)local_pRenderPassBegin, contents);

}

void DispatchCmdNextSubpass(
    VkCommandBuffer                             commandBuffer,
    VkSubpassContents                           contents)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdNextSubpass(commandBuffer, contents);

}

void DispatchCmdEndRenderPass(
    VkCommandBuffer                             commandBuffer)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdEndRenderPass(commandBuffer);

}

void DispatchCmdExecuteCommands(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    commandBufferCount,
    const VkCommandBuffer*                      pCommandBuffers)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);

}

// Skip vkEnumerateInstanceVersion dispatch, manually generated

VkResult DispatchBindBufferMemory2(
    VkDevice                                    device,
    uint32_t                                    bindInfoCount,
    const VkBindBufferMemoryInfo*               pBindInfos)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.BindBufferMemory2(device, bindInfoCount, pBindInfos);
    safe_VkBindBufferMemoryInfo *local_pBindInfos = NULL;
    {
        if (pBindInfos) {
            local_pBindInfos = new safe_VkBindBufferMemoryInfo[bindInfoCount];
            for (uint32_t index0 = 0; index0 < bindInfoCount; ++index0) {
                local_pBindInfos[index0].initialize(&pBindInfos[index0]);
                if (pBindInfos[index0].buffer) {
                    local_pBindInfos[index0].buffer = layer_data->Unwrap(pBindInfos[index0].buffer);
                }
                if (pBindInfos[index0].memory) {
                    local_pBindInfos[index0].memory = layer_data->Unwrap(pBindInfos[index0].memory);
                }
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.BindBufferMemory2(device, bindInfoCount, (const VkBindBufferMemoryInfo*)local_pBindInfos);
    if (local_pBindInfos) {
        delete[] local_pBindInfos;
    }
    return result;
}

VkResult DispatchBindImageMemory2(
    VkDevice                                    device,
    uint32_t                                    bindInfoCount,
    const VkBindImageMemoryInfo*                pBindInfos)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.BindImageMemory2(device, bindInfoCount, pBindInfos);
    safe_VkBindImageMemoryInfo *local_pBindInfos = NULL;
    {
        if (pBindInfos) {
            local_pBindInfos = new safe_VkBindImageMemoryInfo[bindInfoCount];
            for (uint32_t index0 = 0; index0 < bindInfoCount; ++index0) {
                local_pBindInfos[index0].initialize(&pBindInfos[index0]);
                WrapPnextChainHandles(layer_data, local_pBindInfos[index0].pNext);
                if (pBindInfos[index0].image) {
                    local_pBindInfos[index0].image = layer_data->Unwrap(pBindInfos[index0].image);
                }
                if (pBindInfos[index0].memory) {
                    local_pBindInfos[index0].memory = layer_data->Unwrap(pBindInfos[index0].memory);
                }
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.BindImageMemory2(device, bindInfoCount, (const VkBindImageMemoryInfo*)local_pBindInfos);
    if (local_pBindInfos) {
        delete[] local_pBindInfos;
    }
    return result;
}

void DispatchGetDeviceGroupPeerMemoryFeatures(
    VkDevice                                    device,
    uint32_t                                    heapIndex,
    uint32_t                                    localDeviceIndex,
    uint32_t                                    remoteDeviceIndex,
    VkPeerMemoryFeatureFlags*                   pPeerMemoryFeatures)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    layer_data->device_dispatch_table.GetDeviceGroupPeerMemoryFeatures(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);

}

void DispatchCmdSetDeviceMask(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    deviceMask)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetDeviceMask(commandBuffer, deviceMask);

}

void DispatchCmdDispatchBase(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    baseGroupX,
    uint32_t                                    baseGroupY,
    uint32_t                                    baseGroupZ,
    uint32_t                                    groupCountX,
    uint32_t                                    groupCountY,
    uint32_t                                    groupCountZ)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdDispatchBase(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);

}

VkResult DispatchEnumeratePhysicalDeviceGroups(
    VkInstance                                  instance,
    uint32_t*                                   pPhysicalDeviceGroupCount,
    VkPhysicalDeviceGroupProperties*            pPhysicalDeviceGroupProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(instance), layer_data_map);
    VkResult result = layer_data->instance_dispatch_table.EnumeratePhysicalDeviceGroups(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);

    return result;
}

void DispatchGetImageMemoryRequirements2(
    VkDevice                                    device,
    const VkImageMemoryRequirementsInfo2*       pInfo,
    VkMemoryRequirements2*                      pMemoryRequirements)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetImageMemoryRequirements2(device, pInfo, pMemoryRequirements);
    safe_VkImageMemoryRequirementsInfo2 var_local_pInfo;
    safe_VkImageMemoryRequirementsInfo2 *local_pInfo = NULL;
    {
        if (pInfo) {
            local_pInfo = &var_local_pInfo;
            local_pInfo->initialize(pInfo);
            if (pInfo->image) {
                local_pInfo->image = layer_data->Unwrap(pInfo->image);
            }
        }
    }
    layer_data->device_dispatch_table.GetImageMemoryRequirements2(device, (const VkImageMemoryRequirementsInfo2*)local_pInfo, pMemoryRequirements);

}

void DispatchGetBufferMemoryRequirements2(
    VkDevice                                    device,
    const VkBufferMemoryRequirementsInfo2*      pInfo,
    VkMemoryRequirements2*                      pMemoryRequirements)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetBufferMemoryRequirements2(device, pInfo, pMemoryRequirements);
    safe_VkBufferMemoryRequirementsInfo2 var_local_pInfo;
    safe_VkBufferMemoryRequirementsInfo2 *local_pInfo = NULL;
    {
        if (pInfo) {
            local_pInfo = &var_local_pInfo;
            local_pInfo->initialize(pInfo);
            if (pInfo->buffer) {
                local_pInfo->buffer = layer_data->Unwrap(pInfo->buffer);
            }
        }
    }
    layer_data->device_dispatch_table.GetBufferMemoryRequirements2(device, (const VkBufferMemoryRequirementsInfo2*)local_pInfo, pMemoryRequirements);

}

void DispatchGetPhysicalDeviceFeatures2(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceFeatures2*                  pFeatures)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    layer_data->instance_dispatch_table.GetPhysicalDeviceFeatures2(physicalDevice, pFeatures);

}

void DispatchGetPhysicalDeviceProperties2(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceProperties2*                pProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    layer_data->instance_dispatch_table.GetPhysicalDeviceProperties2(physicalDevice, pProperties);

}

void DispatchGetPhysicalDeviceFormatProperties2(
    VkPhysicalDevice                            physicalDevice,
    VkFormat                                    format,
    VkFormatProperties2*                        pFormatProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    layer_data->instance_dispatch_table.GetPhysicalDeviceFormatProperties2(physicalDevice, format, pFormatProperties);

}

VkResult DispatchGetPhysicalDeviceImageFormatProperties2(
    VkPhysicalDevice                            physicalDevice,
    const VkPhysicalDeviceImageFormatInfo2*     pImageFormatInfo,
    VkImageFormatProperties2*                   pImageFormatProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    VkResult result = layer_data->instance_dispatch_table.GetPhysicalDeviceImageFormatProperties2(physicalDevice, pImageFormatInfo, pImageFormatProperties);

    return result;
}

void DispatchGetPhysicalDeviceQueueFamilyProperties2(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pQueueFamilyPropertyCount,
    VkQueueFamilyProperties2*                   pQueueFamilyProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    layer_data->instance_dispatch_table.GetPhysicalDeviceQueueFamilyProperties2(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);

}

void DispatchGetPhysicalDeviceMemoryProperties2(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceMemoryProperties2*          pMemoryProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    layer_data->instance_dispatch_table.GetPhysicalDeviceMemoryProperties2(physicalDevice, pMemoryProperties);

}

void DispatchGetDeviceQueue2(
    VkDevice                                    device,
    const VkDeviceQueueInfo2*                   pQueueInfo,
    VkQueue*                                    pQueue)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    layer_data->device_dispatch_table.GetDeviceQueue2(device, pQueueInfo, pQueue);

}

VkResult DispatchCreateSamplerYcbcrConversion(
    VkDevice                                    device,
    const VkSamplerYcbcrConversionCreateInfo*   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSamplerYcbcrConversion*                   pYcbcrConversion)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CreateSamplerYcbcrConversion(device, pCreateInfo, pAllocator, pYcbcrConversion);
    VkResult result = layer_data->device_dispatch_table.CreateSamplerYcbcrConversion(device, pCreateInfo, pAllocator, pYcbcrConversion);
    if (VK_SUCCESS == result) {
        *pYcbcrConversion = layer_data->WrapNew(*pYcbcrConversion);
    }
    return result;
}

void DispatchDestroySamplerYcbcrConversion(
    VkDevice                                    device,
    VkSamplerYcbcrConversion                    ycbcrConversion,
    const VkAllocationCallbacks*                pAllocator)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DestroySamplerYcbcrConversion(device, ycbcrConversion, pAllocator);
    uint64_t ycbcrConversion_id = reinterpret_cast<uint64_t &>(ycbcrConversion);
    auto iter = unique_id_mapping.pop(ycbcrConversion_id);
    if (iter != unique_id_mapping.end()) {
        ycbcrConversion = (VkSamplerYcbcrConversion)iter->second;
    } else {
        ycbcrConversion = (VkSamplerYcbcrConversion)0;
    }
    layer_data->device_dispatch_table.DestroySamplerYcbcrConversion(device, ycbcrConversion, pAllocator);

}

void DispatchGetPhysicalDeviceExternalBufferProperties(
    VkPhysicalDevice                            physicalDevice,
    const VkPhysicalDeviceExternalBufferInfo*   pExternalBufferInfo,
    VkExternalBufferProperties*                 pExternalBufferProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    layer_data->instance_dispatch_table.GetPhysicalDeviceExternalBufferProperties(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);

}

void DispatchGetPhysicalDeviceExternalFenceProperties(
    VkPhysicalDevice                            physicalDevice,
    const VkPhysicalDeviceExternalFenceInfo*    pExternalFenceInfo,
    VkExternalFenceProperties*                  pExternalFenceProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    layer_data->instance_dispatch_table.GetPhysicalDeviceExternalFenceProperties(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);

}

void DispatchGetPhysicalDeviceExternalSemaphoreProperties(
    VkPhysicalDevice                            physicalDevice,
    const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo,
    VkExternalSemaphoreProperties*              pExternalSemaphoreProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    layer_data->instance_dispatch_table.GetPhysicalDeviceExternalSemaphoreProperties(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);

}

void DispatchGetDescriptorSetLayoutSupport(
    VkDevice                                    device,
    const VkDescriptorSetLayoutCreateInfo*      pCreateInfo,
    VkDescriptorSetLayoutSupport*               pSupport)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetDescriptorSetLayoutSupport(device, pCreateInfo, pSupport);
    safe_VkDescriptorSetLayoutCreateInfo var_local_pCreateInfo;
    safe_VkDescriptorSetLayoutCreateInfo *local_pCreateInfo = NULL;
    {
        if (pCreateInfo) {
            local_pCreateInfo = &var_local_pCreateInfo;
            local_pCreateInfo->initialize(pCreateInfo);
            if (local_pCreateInfo->pBindings) {
                for (uint32_t index1 = 0; index1 < local_pCreateInfo->bindingCount; ++index1) {
                    if (local_pCreateInfo->pBindings[index1].pImmutableSamplers) {
                        for (uint32_t index2 = 0; index2 < local_pCreateInfo->pBindings[index1].descriptorCount; ++index2) {
                            local_pCreateInfo->pBindings[index1].pImmutableSamplers[index2] = layer_data->Unwrap(local_pCreateInfo->pBindings[index1].pImmutableSamplers[index2]);
                        }
                    }
                }
            }
        }
    }
    layer_data->device_dispatch_table.GetDescriptorSetLayoutSupport(device, (const VkDescriptorSetLayoutCreateInfo*)local_pCreateInfo, pSupport);

}

void DispatchCmdDrawIndirectCount(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkBuffer                                    countBuffer,
    VkDeviceSize                                countBufferOffset,
    uint32_t                                    maxDrawCount,
    uint32_t                                    stride)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdDrawIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
    {
        buffer = layer_data->Unwrap(buffer);
        countBuffer = layer_data->Unwrap(countBuffer);
    }
    layer_data->device_dispatch_table.CmdDrawIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);

}

void DispatchCmdDrawIndexedIndirectCount(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkBuffer                                    countBuffer,
    VkDeviceSize                                countBufferOffset,
    uint32_t                                    maxDrawCount,
    uint32_t                                    stride)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdDrawIndexedIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
    {
        buffer = layer_data->Unwrap(buffer);
        countBuffer = layer_data->Unwrap(countBuffer);
    }
    layer_data->device_dispatch_table.CmdDrawIndexedIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);

}

// Skip vkCreateRenderPass2 dispatch, manually generated

void DispatchCmdBeginRenderPass2(
    VkCommandBuffer                             commandBuffer,
    const VkRenderPassBeginInfo*                pRenderPassBegin,
    const VkSubpassBeginInfo*                   pSubpassBeginInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdBeginRenderPass2(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
    safe_VkRenderPassBeginInfo var_local_pRenderPassBegin;
    safe_VkRenderPassBeginInfo *local_pRenderPassBegin = NULL;
    {
        if (pRenderPassBegin) {
            local_pRenderPassBegin = &var_local_pRenderPassBegin;
            local_pRenderPassBegin->initialize(pRenderPassBegin);
            if (pRenderPassBegin->renderPass) {
                local_pRenderPassBegin->renderPass = layer_data->Unwrap(pRenderPassBegin->renderPass);
            }
            if (pRenderPassBegin->framebuffer) {
                local_pRenderPassBegin->framebuffer = layer_data->Unwrap(pRenderPassBegin->framebuffer);
            }
            WrapPnextChainHandles(layer_data, local_pRenderPassBegin->pNext);
        }
    }
    layer_data->device_dispatch_table.CmdBeginRenderPass2(commandBuffer, (const VkRenderPassBeginInfo*)local_pRenderPassBegin, pSubpassBeginInfo);

}

void DispatchCmdNextSubpass2(
    VkCommandBuffer                             commandBuffer,
    const VkSubpassBeginInfo*                   pSubpassBeginInfo,
    const VkSubpassEndInfo*                     pSubpassEndInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdNextSubpass2(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);

}

void DispatchCmdEndRenderPass2(
    VkCommandBuffer                             commandBuffer,
    const VkSubpassEndInfo*                     pSubpassEndInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdEndRenderPass2(commandBuffer, pSubpassEndInfo);

}

void DispatchResetQueryPool(
    VkDevice                                    device,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.ResetQueryPool(device, queryPool, firstQuery, queryCount);
    {
        queryPool = layer_data->Unwrap(queryPool);
    }
    layer_data->device_dispatch_table.ResetQueryPool(device, queryPool, firstQuery, queryCount);

}

VkResult DispatchGetSemaphoreCounterValue(
    VkDevice                                    device,
    VkSemaphore                                 semaphore,
    uint64_t*                                   pValue)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetSemaphoreCounterValue(device, semaphore, pValue);
    {
        semaphore = layer_data->Unwrap(semaphore);
    }
    VkResult result = layer_data->device_dispatch_table.GetSemaphoreCounterValue(device, semaphore, pValue);

    return result;
}

VkResult DispatchWaitSemaphores(
    VkDevice                                    device,
    const VkSemaphoreWaitInfo*                  pWaitInfo,
    uint64_t                                    timeout)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.WaitSemaphores(device, pWaitInfo, timeout);
    safe_VkSemaphoreWaitInfo var_local_pWaitInfo;
    safe_VkSemaphoreWaitInfo *local_pWaitInfo = NULL;
    {
        if (pWaitInfo) {
            local_pWaitInfo = &var_local_pWaitInfo;
            local_pWaitInfo->initialize(pWaitInfo);
            if (local_pWaitInfo->pSemaphores) {
                for (uint32_t index1 = 0; index1 < local_pWaitInfo->semaphoreCount; ++index1) {
                    local_pWaitInfo->pSemaphores[index1] = layer_data->Unwrap(local_pWaitInfo->pSemaphores[index1]);
                }
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.WaitSemaphores(device, (const VkSemaphoreWaitInfo*)local_pWaitInfo, timeout);

    return result;
}

VkResult DispatchSignalSemaphore(
    VkDevice                                    device,
    const VkSemaphoreSignalInfo*                pSignalInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.SignalSemaphore(device, pSignalInfo);
    safe_VkSemaphoreSignalInfo var_local_pSignalInfo;
    safe_VkSemaphoreSignalInfo *local_pSignalInfo = NULL;
    {
        if (pSignalInfo) {
            local_pSignalInfo = &var_local_pSignalInfo;
            local_pSignalInfo->initialize(pSignalInfo);
            if (pSignalInfo->semaphore) {
                local_pSignalInfo->semaphore = layer_data->Unwrap(pSignalInfo->semaphore);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.SignalSemaphore(device, (const VkSemaphoreSignalInfo*)local_pSignalInfo);

    return result;
}

VkDeviceAddress DispatchGetBufferDeviceAddress(
    VkDevice                                    device,
    const VkBufferDeviceAddressInfo*            pInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetBufferDeviceAddress(device, pInfo);
    safe_VkBufferDeviceAddressInfo var_local_pInfo;
    safe_VkBufferDeviceAddressInfo *local_pInfo = NULL;
    {
        if (pInfo) {
            local_pInfo = &var_local_pInfo;
            local_pInfo->initialize(pInfo);
            if (pInfo->buffer) {
                local_pInfo->buffer = layer_data->Unwrap(pInfo->buffer);
            }
        }
    }
    VkDeviceAddress result = layer_data->device_dispatch_table.GetBufferDeviceAddress(device, (const VkBufferDeviceAddressInfo*)local_pInfo);

    return result;
}

uint64_t DispatchGetBufferOpaqueCaptureAddress(
    VkDevice                                    device,
    const VkBufferDeviceAddressInfo*            pInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetBufferOpaqueCaptureAddress(device, pInfo);
    safe_VkBufferDeviceAddressInfo var_local_pInfo;
    safe_VkBufferDeviceAddressInfo *local_pInfo = NULL;
    {
        if (pInfo) {
            local_pInfo = &var_local_pInfo;
            local_pInfo->initialize(pInfo);
            if (pInfo->buffer) {
                local_pInfo->buffer = layer_data->Unwrap(pInfo->buffer);
            }
        }
    }
    uint64_t result = layer_data->device_dispatch_table.GetBufferOpaqueCaptureAddress(device, (const VkBufferDeviceAddressInfo*)local_pInfo);

    return result;
}

uint64_t DispatchGetDeviceMemoryOpaqueCaptureAddress(
    VkDevice                                    device,
    const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetDeviceMemoryOpaqueCaptureAddress(device, pInfo);
    safe_VkDeviceMemoryOpaqueCaptureAddressInfo var_local_pInfo;
    safe_VkDeviceMemoryOpaqueCaptureAddressInfo *local_pInfo = NULL;
    {
        if (pInfo) {
            local_pInfo = &var_local_pInfo;
            local_pInfo->initialize(pInfo);
            if (pInfo->memory) {
                local_pInfo->memory = layer_data->Unwrap(pInfo->memory);
            }
        }
    }
    uint64_t result = layer_data->device_dispatch_table.GetDeviceMemoryOpaqueCaptureAddress(device, (const VkDeviceMemoryOpaqueCaptureAddressInfo*)local_pInfo);

    return result;
}

void DispatchGetCommandPoolMemoryConsumption(
    VkDevice                                    device,
    VkCommandPool                               commandPool,
    VkCommandBuffer                             commandBuffer,
    VkCommandPoolMemoryConsumption*             pConsumption)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetCommandPoolMemoryConsumption(device, commandPool, commandBuffer, pConsumption);
    {
        commandPool = layer_data->Unwrap(commandPool);
    }
    layer_data->device_dispatch_table.GetCommandPoolMemoryConsumption(device, commandPool, commandBuffer, pConsumption);

}

VkResult DispatchGetFaultData(
    VkDevice                                    device,
    VkFaultQueryBehavior                        faultQueryBehavior,
    VkBool32*                                   pUnrecordedFaults,
    uint32_t*                                   pFaultCount,
    VkFaultData*                                pFaults)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    VkResult result = layer_data->device_dispatch_table.GetFaultData(device, faultQueryBehavior, pUnrecordedFaults, pFaultCount, pFaults);

    return result;
}

void DispatchDestroySurfaceKHR(
    VkInstance                                  instance,
    VkSurfaceKHR                                surface,
    const VkAllocationCallbacks*                pAllocator)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(instance), layer_data_map);
    if (!wrap_handles) return layer_data->instance_dispatch_table.DestroySurfaceKHR(instance, surface, pAllocator);
    uint64_t surface_id = reinterpret_cast<uint64_t &>(surface);
    auto iter = unique_id_mapping.pop(surface_id);
    if (iter != unique_id_mapping.end()) {
        surface = (VkSurfaceKHR)iter->second;
    } else {
        surface = (VkSurfaceKHR)0;
    }
    layer_data->instance_dispatch_table.DestroySurfaceKHR(instance, surface, pAllocator);

}

VkResult DispatchGetPhysicalDeviceSurfaceSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex,
    VkSurfaceKHR                                surface,
    VkBool32*                                   pSupported)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    if (!wrap_handles) return layer_data->instance_dispatch_table.GetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
    {
        surface = layer_data->Unwrap(surface);
    }
    VkResult result = layer_data->instance_dispatch_table.GetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);

    return result;
}

VkResult DispatchGetPhysicalDeviceSurfaceCapabilitiesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    VkSurfaceCapabilitiesKHR*                   pSurfaceCapabilities)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    if (!wrap_handles) return layer_data->instance_dispatch_table.GetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
    {
        surface = layer_data->Unwrap(surface);
    }
    VkResult result = layer_data->instance_dispatch_table.GetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);

    return result;
}

VkResult DispatchGetPhysicalDeviceSurfaceFormatsKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    uint32_t*                                   pSurfaceFormatCount,
    VkSurfaceFormatKHR*                         pSurfaceFormats)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    if (!wrap_handles) return layer_data->instance_dispatch_table.GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
    {
        surface = layer_data->Unwrap(surface);
    }
    VkResult result = layer_data->instance_dispatch_table.GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);

    return result;
}

VkResult DispatchGetPhysicalDeviceSurfacePresentModesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    uint32_t*                                   pPresentModeCount,
    VkPresentModeKHR*                           pPresentModes)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    if (!wrap_handles) return layer_data->instance_dispatch_table.GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
    {
        surface = layer_data->Unwrap(surface);
    }
    VkResult result = layer_data->instance_dispatch_table.GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);

    return result;
}

// Skip vkCreateSwapchainKHR dispatch, manually generated

// Skip vkGetSwapchainImagesKHR dispatch, manually generated

VkResult DispatchAcquireNextImageKHR(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    uint64_t                                    timeout,
    VkSemaphore                                 semaphore,
    VkFence                                     fence,
    uint32_t*                                   pImageIndex)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.AcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
    {
        swapchain = layer_data->Unwrap(swapchain);
        semaphore = layer_data->Unwrap(semaphore);
        fence = layer_data->Unwrap(fence);
    }
    VkResult result = layer_data->device_dispatch_table.AcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);

    return result;
}

// Skip vkQueuePresentKHR dispatch, manually generated

VkResult DispatchGetDeviceGroupPresentCapabilitiesKHR(
    VkDevice                                    device,
    VkDeviceGroupPresentCapabilitiesKHR*        pDeviceGroupPresentCapabilities)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    VkResult result = layer_data->device_dispatch_table.GetDeviceGroupPresentCapabilitiesKHR(device, pDeviceGroupPresentCapabilities);

    return result;
}

VkResult DispatchGetDeviceGroupSurfacePresentModesKHR(
    VkDevice                                    device,
    VkSurfaceKHR                                surface,
    VkDeviceGroupPresentModeFlagsKHR*           pModes)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetDeviceGroupSurfacePresentModesKHR(device, surface, pModes);
    {
        surface = layer_data->Unwrap(surface);
    }
    VkResult result = layer_data->device_dispatch_table.GetDeviceGroupSurfacePresentModesKHR(device, surface, pModes);

    return result;
}

VkResult DispatchGetPhysicalDevicePresentRectanglesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    uint32_t*                                   pRectCount,
    VkRect2D*                                   pRects)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    if (!wrap_handles) return layer_data->instance_dispatch_table.GetPhysicalDevicePresentRectanglesKHR(physicalDevice, surface, pRectCount, pRects);
    {
        surface = layer_data->Unwrap(surface);
    }
    VkResult result = layer_data->instance_dispatch_table.GetPhysicalDevicePresentRectanglesKHR(physicalDevice, surface, pRectCount, pRects);

    return result;
}

VkResult DispatchAcquireNextImage2KHR(
    VkDevice                                    device,
    const VkAcquireNextImageInfoKHR*            pAcquireInfo,
    uint32_t*                                   pImageIndex)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.AcquireNextImage2KHR(device, pAcquireInfo, pImageIndex);
    safe_VkAcquireNextImageInfoKHR var_local_pAcquireInfo;
    safe_VkAcquireNextImageInfoKHR *local_pAcquireInfo = NULL;
    {
        if (pAcquireInfo) {
            local_pAcquireInfo = &var_local_pAcquireInfo;
            local_pAcquireInfo->initialize(pAcquireInfo);
            if (pAcquireInfo->swapchain) {
                local_pAcquireInfo->swapchain = layer_data->Unwrap(pAcquireInfo->swapchain);
            }
            if (pAcquireInfo->semaphore) {
                local_pAcquireInfo->semaphore = layer_data->Unwrap(pAcquireInfo->semaphore);
            }
            if (pAcquireInfo->fence) {
                local_pAcquireInfo->fence = layer_data->Unwrap(pAcquireInfo->fence);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.AcquireNextImage2KHR(device, (const VkAcquireNextImageInfoKHR*)local_pAcquireInfo, pImageIndex);

    return result;
}

// Skip vkGetPhysicalDeviceDisplayPropertiesKHR dispatch, manually generated

// Skip vkGetPhysicalDeviceDisplayPlanePropertiesKHR dispatch, manually generated

// Skip vkGetDisplayPlaneSupportedDisplaysKHR dispatch, manually generated

// Skip vkGetDisplayModePropertiesKHR dispatch, manually generated

VkResult DispatchCreateDisplayModeKHR(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayKHR                                display,
    const VkDisplayModeCreateInfoKHR*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDisplayModeKHR*                           pMode)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    if (!wrap_handles) return layer_data->instance_dispatch_table.CreateDisplayModeKHR(physicalDevice, display, pCreateInfo, pAllocator, pMode);
    {
        display = layer_data->Unwrap(display);
    }
    VkResult result = layer_data->instance_dispatch_table.CreateDisplayModeKHR(physicalDevice, display, pCreateInfo, pAllocator, pMode);
    if (VK_SUCCESS == result) {
        *pMode = layer_data->WrapNew(*pMode);
    }
    return result;
}

VkResult DispatchGetDisplayPlaneCapabilitiesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayModeKHR                            mode,
    uint32_t                                    planeIndex,
    VkDisplayPlaneCapabilitiesKHR*              pCapabilities)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    if (!wrap_handles) return layer_data->instance_dispatch_table.GetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, pCapabilities);
    {
        mode = layer_data->Unwrap(mode);
    }
    VkResult result = layer_data->instance_dispatch_table.GetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, pCapabilities);

    return result;
}

VkResult DispatchCreateDisplayPlaneSurfaceKHR(
    VkInstance                                  instance,
    const VkDisplaySurfaceCreateInfoKHR*        pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(instance), layer_data_map);
    if (!wrap_handles) return layer_data->instance_dispatch_table.CreateDisplayPlaneSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    safe_VkDisplaySurfaceCreateInfoKHR var_local_pCreateInfo;
    safe_VkDisplaySurfaceCreateInfoKHR *local_pCreateInfo = NULL;
    {
        if (pCreateInfo) {
            local_pCreateInfo = &var_local_pCreateInfo;
            local_pCreateInfo->initialize(pCreateInfo);
            if (pCreateInfo->displayMode) {
                local_pCreateInfo->displayMode = layer_data->Unwrap(pCreateInfo->displayMode);
            }
        }
    }
    VkResult result = layer_data->instance_dispatch_table.CreateDisplayPlaneSurfaceKHR(instance, (const VkDisplaySurfaceCreateInfoKHR*)local_pCreateInfo, pAllocator, pSurface);
    if (VK_SUCCESS == result) {
        *pSurface = layer_data->WrapNew(*pSurface);
    }
    return result;
}

// Skip vkCreateSharedSwapchainsKHR dispatch, manually generated

VkResult DispatchGetMemoryFdKHR(
    VkDevice                                    device,
    const VkMemoryGetFdInfoKHR*                 pGetFdInfo,
    int*                                        pFd)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetMemoryFdKHR(device, pGetFdInfo, pFd);
    safe_VkMemoryGetFdInfoKHR var_local_pGetFdInfo;
    safe_VkMemoryGetFdInfoKHR *local_pGetFdInfo = NULL;
    {
        if (pGetFdInfo) {
            local_pGetFdInfo = &var_local_pGetFdInfo;
            local_pGetFdInfo->initialize(pGetFdInfo);
            if (pGetFdInfo->memory) {
                local_pGetFdInfo->memory = layer_data->Unwrap(pGetFdInfo->memory);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.GetMemoryFdKHR(device, (const VkMemoryGetFdInfoKHR*)local_pGetFdInfo, pFd);

    return result;
}

VkResult DispatchGetMemoryFdPropertiesKHR(
    VkDevice                                    device,
    VkExternalMemoryHandleTypeFlagBits          handleType,
    int                                         fd,
    VkMemoryFdPropertiesKHR*                    pMemoryFdProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    VkResult result = layer_data->device_dispatch_table.GetMemoryFdPropertiesKHR(device, handleType, fd, pMemoryFdProperties);

    return result;
}

VkResult DispatchImportSemaphoreFdKHR(
    VkDevice                                    device,
    const VkImportSemaphoreFdInfoKHR*           pImportSemaphoreFdInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.ImportSemaphoreFdKHR(device, pImportSemaphoreFdInfo);
    safe_VkImportSemaphoreFdInfoKHR var_local_pImportSemaphoreFdInfo;
    safe_VkImportSemaphoreFdInfoKHR *local_pImportSemaphoreFdInfo = NULL;
    {
        if (pImportSemaphoreFdInfo) {
            local_pImportSemaphoreFdInfo = &var_local_pImportSemaphoreFdInfo;
            local_pImportSemaphoreFdInfo->initialize(pImportSemaphoreFdInfo);
            if (pImportSemaphoreFdInfo->semaphore) {
                local_pImportSemaphoreFdInfo->semaphore = layer_data->Unwrap(pImportSemaphoreFdInfo->semaphore);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.ImportSemaphoreFdKHR(device, (const VkImportSemaphoreFdInfoKHR*)local_pImportSemaphoreFdInfo);

    return result;
}

VkResult DispatchGetSemaphoreFdKHR(
    VkDevice                                    device,
    const VkSemaphoreGetFdInfoKHR*              pGetFdInfo,
    int*                                        pFd)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetSemaphoreFdKHR(device, pGetFdInfo, pFd);
    safe_VkSemaphoreGetFdInfoKHR var_local_pGetFdInfo;
    safe_VkSemaphoreGetFdInfoKHR *local_pGetFdInfo = NULL;
    {
        if (pGetFdInfo) {
            local_pGetFdInfo = &var_local_pGetFdInfo;
            local_pGetFdInfo->initialize(pGetFdInfo);
            if (pGetFdInfo->semaphore) {
                local_pGetFdInfo->semaphore = layer_data->Unwrap(pGetFdInfo->semaphore);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.GetSemaphoreFdKHR(device, (const VkSemaphoreGetFdInfoKHR*)local_pGetFdInfo, pFd);

    return result;
}

VkResult DispatchGetSwapchainStatusKHR(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetSwapchainStatusKHR(device, swapchain);
    {
        swapchain = layer_data->Unwrap(swapchain);
    }
    VkResult result = layer_data->device_dispatch_table.GetSwapchainStatusKHR(device, swapchain);

    return result;
}

VkResult DispatchImportFenceFdKHR(
    VkDevice                                    device,
    const VkImportFenceFdInfoKHR*               pImportFenceFdInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.ImportFenceFdKHR(device, pImportFenceFdInfo);
    safe_VkImportFenceFdInfoKHR var_local_pImportFenceFdInfo;
    safe_VkImportFenceFdInfoKHR *local_pImportFenceFdInfo = NULL;
    {
        if (pImportFenceFdInfo) {
            local_pImportFenceFdInfo = &var_local_pImportFenceFdInfo;
            local_pImportFenceFdInfo->initialize(pImportFenceFdInfo);
            if (pImportFenceFdInfo->fence) {
                local_pImportFenceFdInfo->fence = layer_data->Unwrap(pImportFenceFdInfo->fence);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.ImportFenceFdKHR(device, (const VkImportFenceFdInfoKHR*)local_pImportFenceFdInfo);

    return result;
}

VkResult DispatchGetFenceFdKHR(
    VkDevice                                    device,
    const VkFenceGetFdInfoKHR*                  pGetFdInfo,
    int*                                        pFd)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetFenceFdKHR(device, pGetFdInfo, pFd);
    safe_VkFenceGetFdInfoKHR var_local_pGetFdInfo;
    safe_VkFenceGetFdInfoKHR *local_pGetFdInfo = NULL;
    {
        if (pGetFdInfo) {
            local_pGetFdInfo = &var_local_pGetFdInfo;
            local_pGetFdInfo->initialize(pGetFdInfo);
            if (pGetFdInfo->fence) {
                local_pGetFdInfo->fence = layer_data->Unwrap(pGetFdInfo->fence);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.GetFenceFdKHR(device, (const VkFenceGetFdInfoKHR*)local_pGetFdInfo, pFd);

    return result;
}

VkResult DispatchEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex,
    uint32_t*                                   pCounterCount,
    VkPerformanceCounterKHR*                    pCounters,
    VkPerformanceCounterDescriptionKHR*         pCounterDescriptions)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    VkResult result = layer_data->instance_dispatch_table.EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(physicalDevice, queueFamilyIndex, pCounterCount, pCounters, pCounterDescriptions);

    return result;
}

void DispatchGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(
    VkPhysicalDevice                            physicalDevice,
    const VkQueryPoolPerformanceCreateInfoKHR*  pPerformanceQueryCreateInfo,
    uint32_t*                                   pNumPasses)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    layer_data->instance_dispatch_table.GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(physicalDevice, pPerformanceQueryCreateInfo, pNumPasses);

}

VkResult DispatchAcquireProfilingLockKHR(
    VkDevice                                    device,
    const VkAcquireProfilingLockInfoKHR*        pInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    VkResult result = layer_data->device_dispatch_table.AcquireProfilingLockKHR(device, pInfo);

    return result;
}

void DispatchReleaseProfilingLockKHR(
    VkDevice                                    device)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    layer_data->device_dispatch_table.ReleaseProfilingLockKHR(device);

}

VkResult DispatchGetPhysicalDeviceSurfaceCapabilities2KHR(
    VkPhysicalDevice                            physicalDevice,
    const VkPhysicalDeviceSurfaceInfo2KHR*      pSurfaceInfo,
    VkSurfaceCapabilities2KHR*                  pSurfaceCapabilities)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    if (!wrap_handles) return layer_data->instance_dispatch_table.GetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
    safe_VkPhysicalDeviceSurfaceInfo2KHR var_local_pSurfaceInfo;
    safe_VkPhysicalDeviceSurfaceInfo2KHR *local_pSurfaceInfo = NULL;
    {
        if (pSurfaceInfo) {
            local_pSurfaceInfo = &var_local_pSurfaceInfo;
            local_pSurfaceInfo->initialize(pSurfaceInfo);
            if (pSurfaceInfo->surface) {
                local_pSurfaceInfo->surface = layer_data->Unwrap(pSurfaceInfo->surface);
            }
        }
    }
    VkResult result = layer_data->instance_dispatch_table.GetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice, (const VkPhysicalDeviceSurfaceInfo2KHR*)local_pSurfaceInfo, pSurfaceCapabilities);

    return result;
}

VkResult DispatchGetPhysicalDeviceSurfaceFormats2KHR(
    VkPhysicalDevice                            physicalDevice,
    const VkPhysicalDeviceSurfaceInfo2KHR*      pSurfaceInfo,
    uint32_t*                                   pSurfaceFormatCount,
    VkSurfaceFormat2KHR*                        pSurfaceFormats)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    if (!wrap_handles) return layer_data->instance_dispatch_table.GetPhysicalDeviceSurfaceFormats2KHR(physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
    safe_VkPhysicalDeviceSurfaceInfo2KHR var_local_pSurfaceInfo;
    safe_VkPhysicalDeviceSurfaceInfo2KHR *local_pSurfaceInfo = NULL;
    {
        if (pSurfaceInfo) {
            local_pSurfaceInfo = &var_local_pSurfaceInfo;
            local_pSurfaceInfo->initialize(pSurfaceInfo);
            if (pSurfaceInfo->surface) {
                local_pSurfaceInfo->surface = layer_data->Unwrap(pSurfaceInfo->surface);
            }
        }
    }
    VkResult result = layer_data->instance_dispatch_table.GetPhysicalDeviceSurfaceFormats2KHR(physicalDevice, (const VkPhysicalDeviceSurfaceInfo2KHR*)local_pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);

    return result;
}

// Skip vkGetPhysicalDeviceDisplayProperties2KHR dispatch, manually generated

// Skip vkGetPhysicalDeviceDisplayPlaneProperties2KHR dispatch, manually generated

// Skip vkGetDisplayModeProperties2KHR dispatch, manually generated

VkResult DispatchGetDisplayPlaneCapabilities2KHR(
    VkPhysicalDevice                            physicalDevice,
    const VkDisplayPlaneInfo2KHR*               pDisplayPlaneInfo,
    VkDisplayPlaneCapabilities2KHR*             pCapabilities)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    if (!wrap_handles) return layer_data->instance_dispatch_table.GetDisplayPlaneCapabilities2KHR(physicalDevice, pDisplayPlaneInfo, pCapabilities);
    safe_VkDisplayPlaneInfo2KHR var_local_pDisplayPlaneInfo;
    safe_VkDisplayPlaneInfo2KHR *local_pDisplayPlaneInfo = NULL;
    {
        if (pDisplayPlaneInfo) {
            local_pDisplayPlaneInfo = &var_local_pDisplayPlaneInfo;
            local_pDisplayPlaneInfo->initialize(pDisplayPlaneInfo);
            if (pDisplayPlaneInfo->mode) {
                local_pDisplayPlaneInfo->mode = layer_data->Unwrap(pDisplayPlaneInfo->mode);
            }
        }
    }
    VkResult result = layer_data->instance_dispatch_table.GetDisplayPlaneCapabilities2KHR(physicalDevice, (const VkDisplayPlaneInfo2KHR*)local_pDisplayPlaneInfo, pCapabilities);

    return result;
}

VkResult DispatchGetPhysicalDeviceFragmentShadingRatesKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pFragmentShadingRateCount,
    VkPhysicalDeviceFragmentShadingRateKHR*     pFragmentShadingRates)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    VkResult result = layer_data->instance_dispatch_table.GetPhysicalDeviceFragmentShadingRatesKHR(physicalDevice, pFragmentShadingRateCount, pFragmentShadingRates);

    return result;
}

void DispatchCmdSetFragmentShadingRateKHR(
    VkCommandBuffer                             commandBuffer,
    const VkExtent2D*                           pFragmentSize,
    const VkFragmentShadingRateCombinerOpKHR    combinerOps[2])
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetFragmentShadingRateKHR(commandBuffer, pFragmentSize, combinerOps);

}

void DispatchCmdRefreshObjectsKHR(
    VkCommandBuffer                             commandBuffer,
    const VkRefreshObjectListKHR*               pRefreshObjects)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdRefreshObjectsKHR(commandBuffer, pRefreshObjects);

}

VkResult DispatchGetPhysicalDeviceRefreshableObjectTypesKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pRefreshableObjectTypeCount,
    VkObjectType*                               pRefreshableObjectTypes)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    VkResult result = layer_data->instance_dispatch_table.GetPhysicalDeviceRefreshableObjectTypesKHR(physicalDevice, pRefreshableObjectTypeCount, pRefreshableObjectTypes);

    return result;
}

void DispatchCmdSetEvent2KHR(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    const VkDependencyInfoKHR*                  pDependencyInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdSetEvent2KHR(commandBuffer, event, pDependencyInfo);
    safe_VkDependencyInfoKHR var_local_pDependencyInfo;
    safe_VkDependencyInfoKHR *local_pDependencyInfo = NULL;
    {
        event = layer_data->Unwrap(event);
        if (pDependencyInfo) {
            local_pDependencyInfo = &var_local_pDependencyInfo;
            local_pDependencyInfo->initialize(pDependencyInfo);
            if (local_pDependencyInfo->pBufferMemoryBarriers) {
                for (uint32_t index1 = 0; index1 < local_pDependencyInfo->bufferMemoryBarrierCount; ++index1) {
                    if (pDependencyInfo->pBufferMemoryBarriers[index1].buffer) {
                        local_pDependencyInfo->pBufferMemoryBarriers[index1].buffer = layer_data->Unwrap(pDependencyInfo->pBufferMemoryBarriers[index1].buffer);
                    }
                }
            }
            if (local_pDependencyInfo->pImageMemoryBarriers) {
                for (uint32_t index1 = 0; index1 < local_pDependencyInfo->imageMemoryBarrierCount; ++index1) {
                    if (pDependencyInfo->pImageMemoryBarriers[index1].image) {
                        local_pDependencyInfo->pImageMemoryBarriers[index1].image = layer_data->Unwrap(pDependencyInfo->pImageMemoryBarriers[index1].image);
                    }
                }
            }
        }
    }
    layer_data->device_dispatch_table.CmdSetEvent2KHR(commandBuffer, event, (const VkDependencyInfoKHR*)local_pDependencyInfo);

}

void DispatchCmdResetEvent2KHR(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    VkPipelineStageFlags2KHR                    stageMask)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdResetEvent2KHR(commandBuffer, event, stageMask);
    {
        event = layer_data->Unwrap(event);
    }
    layer_data->device_dispatch_table.CmdResetEvent2KHR(commandBuffer, event, stageMask);

}

void DispatchCmdWaitEvents2KHR(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    eventCount,
    const VkEvent*                              pEvents,
    const VkDependencyInfoKHR*                  pDependencyInfos)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdWaitEvents2KHR(commandBuffer, eventCount, pEvents, pDependencyInfos);
    VkEvent var_local_pEvents[DISPATCH_MAX_STACK_ALLOCATIONS];
    VkEvent *local_pEvents = NULL;
    safe_VkDependencyInfoKHR *local_pDependencyInfos = NULL;
    {
        if (pEvents) {
            local_pEvents = eventCount > DISPATCH_MAX_STACK_ALLOCATIONS ? new VkEvent[eventCount] : var_local_pEvents;
            for (uint32_t index0 = 0; index0 < eventCount; ++index0) {
                local_pEvents[index0] = layer_data->Unwrap(pEvents[index0]);
            }
        }
        if (pDependencyInfos) {
            local_pDependencyInfos = new safe_VkDependencyInfoKHR[eventCount];
            for (uint32_t index0 = 0; index0 < eventCount; ++index0) {
                local_pDependencyInfos[index0].initialize(&pDependencyInfos[index0]);
                if (local_pDependencyInfos[index0].pBufferMemoryBarriers) {
                    for (uint32_t index1 = 0; index1 < local_pDependencyInfos[index0].bufferMemoryBarrierCount; ++index1) {
                        if (pDependencyInfos[index0].pBufferMemoryBarriers[index1].buffer) {
                            local_pDependencyInfos[index0].pBufferMemoryBarriers[index1].buffer = layer_data->Unwrap(pDependencyInfos[index0].pBufferMemoryBarriers[index1].buffer);
                        }
                    }
                }
                if (local_pDependencyInfos[index0].pImageMemoryBarriers) {
                    for (uint32_t index1 = 0; index1 < local_pDependencyInfos[index0].imageMemoryBarrierCount; ++index1) {
                        if (pDependencyInfos[index0].pImageMemoryBarriers[index1].image) {
                            local_pDependencyInfos[index0].pImageMemoryBarriers[index1].image = layer_data->Unwrap(pDependencyInfos[index0].pImageMemoryBarriers[index1].image);
                        }
                    }
                }
            }
        }
    }
    layer_data->device_dispatch_table.CmdWaitEvents2KHR(commandBuffer, eventCount, (const VkEvent*)local_pEvents, (const VkDependencyInfoKHR*)local_pDependencyInfos);
    if (local_pEvents != var_local_pEvents)
        delete[] local_pEvents;
    if (local_pDependencyInfos) {
        delete[] local_pDependencyInfos;
    }
}

void DispatchCmdPipelineBarrier2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkDependencyInfoKHR*                  pDependencyInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdPipelineBarrier2KHR(commandBuffer, pDependencyInfo);
    safe_VkDependencyInfoKHR var_local_pDependencyInfo;
    safe_VkDependencyInfoKHR *local_pDependencyInfo = NULL;
    {
        if (pDependencyInfo) {
            local_pDependencyInfo = &var_local_pDependencyInfo;
            local_pDependencyInfo->initialize(pDependencyInfo);
            if (local_pDependencyInfo->pBufferMemoryBarriers) {
                for (uint32_t index1 = 0; index1 < local_pDependencyInfo->bufferMemoryBarrierCount; ++index1) {
                    if (pDependencyInfo->pBufferMemoryBarriers[index1].buffer) {
                        local_pDependencyInfo->pBufferMemoryBarriers[index1].buffer = layer_data->Unwrap(pDependencyInfo->pBufferMemoryBarriers[index1].buffer);
                    }
                }
            }
            if (local_pDependencyInfo->pImageMemoryBarriers) {
                for (uint32_t index1 = 0; index1 < local_pDependencyInfo->imageMemoryBarrierCount; ++index1) {
                    if (pDependencyInfo->pImageMemoryBarriers[index1].image) {
                        local_pDependencyInfo->pImageMemoryBarriers[index1].image = layer_data->Unwrap(pDependencyInfo->pImageMemoryBarriers[index1].image);
                    }
                }
            }
        }
    }
    layer_data->device_dispatch_table.CmdPipelineBarrier2KHR(commandBuffer, (const VkDependencyInfoKHR*)local_pDependencyInfo);

}

void DispatchCmdWriteTimestamp2KHR(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlags2KHR                    stage,
    VkQueryPool                                 queryPool,
    uint32_t                                    query)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdWriteTimestamp2KHR(commandBuffer, stage, queryPool, query);
    {
        queryPool = layer_data->Unwrap(queryPool);
    }
    layer_data->device_dispatch_table.CmdWriteTimestamp2KHR(commandBuffer, stage, queryPool, query);

}

VkResult DispatchQueueSubmit2KHR(
    VkQueue                                     queue,
    uint32_t                                    submitCount,
    const VkSubmitInfo2KHR*                     pSubmits,
    VkFence                                     fence)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(queue), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.QueueSubmit2KHR(queue, submitCount, pSubmits, fence);
    safe_VkSubmitInfo2KHR *local_pSubmits = NULL;
    {
        if (pSubmits) {
            local_pSubmits = new safe_VkSubmitInfo2KHR[submitCount];
            for (uint32_t index0 = 0; index0 < submitCount; ++index0) {
                local_pSubmits[index0].initialize(&pSubmits[index0]);
                if (local_pSubmits[index0].pWaitSemaphoreInfos) {
                    for (uint32_t index1 = 0; index1 < local_pSubmits[index0].waitSemaphoreInfoCount; ++index1) {
                        if (pSubmits[index0].pWaitSemaphoreInfos[index1].semaphore) {
                            local_pSubmits[index0].pWaitSemaphoreInfos[index1].semaphore = layer_data->Unwrap(pSubmits[index0].pWaitSemaphoreInfos[index1].semaphore);
                        }
                    }
                }
                if (local_pSubmits[index0].pSignalSemaphoreInfos) {
                    for (uint32_t index1 = 0; index1 < local_pSubmits[index0].signalSemaphoreInfoCount; ++index1) {
                        if (pSubmits[index0].pSignalSemaphoreInfos[index1].semaphore) {
                            local_pSubmits[index0].pSignalSemaphoreInfos[index1].semaphore = layer_data->Unwrap(pSubmits[index0].pSignalSemaphoreInfos[index1].semaphore);
                        }
                    }
                }
            }
        }
        fence = layer_data->Unwrap(fence);
    }
    VkResult result = layer_data->device_dispatch_table.QueueSubmit2KHR(queue, submitCount, (const VkSubmitInfo2KHR*)local_pSubmits, fence);
    if (local_pSubmits) {
        delete[] local_pSubmits;
    }
    return result;
}

void DispatchCmdWriteBufferMarker2AMD(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlags2KHR                    stage,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    uint32_t                                    marker)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdWriteBufferMarker2AMD(commandBuffer, stage, dstBuffer, dstOffset, marker);
    {
        dstBuffer = layer_data->Unwrap(dstBuffer);
    }
    layer_data->device_dispatch_table.CmdWriteBufferMarker2AMD(commandBuffer, stage, dstBuffer, dstOffset, marker);

}

void DispatchGetQueueCheckpointData2NV(
    VkQueue                                     queue,
    uint32_t*                                   pCheckpointDataCount,
    VkCheckpointData2NV*                        pCheckpointData)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(queue), layer_data_map);
    layer_data->device_dispatch_table.GetQueueCheckpointData2NV(queue, pCheckpointDataCount, pCheckpointData);

}

void DispatchCmdCopyBuffer2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkCopyBufferInfo2KHR*                 pCopyBufferInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdCopyBuffer2KHR(commandBuffer, pCopyBufferInfo);
    safe_VkCopyBufferInfo2KHR var_local_pCopyBufferInfo;
    safe_VkCopyBufferInfo2KHR *local_pCopyBufferInfo = NULL;
    {
        if (pCopyBufferInfo) {
            local_pCopyBufferInfo = &var_local_pCopyBufferInfo;
            local_pCopyBufferInfo->initialize(pCopyBufferInfo);
            if (pCopyBufferInfo->srcBuffer) {
                local_pCopyBufferInfo->srcBuffer = layer_data->Unwrap(pCopyBufferInfo->srcBuffer);
            }
            if (pCopyBufferInfo->dstBuffer) {
                local_pCopyBufferInfo->dstBuffer = layer_data->Unwrap(pCopyBufferInfo->dstBuffer);
            }
        }
    }
    layer_data->device_dispatch_table.CmdCopyBuffer2KHR(commandBuffer, (const VkCopyBufferInfo2KHR*)local_pCopyBufferInfo);

}

void DispatchCmdCopyImage2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkCopyImageInfo2KHR*                  pCopyImageInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdCopyImage2KHR(commandBuffer, pCopyImageInfo);
    safe_VkCopyImageInfo2KHR var_local_pCopyImageInfo;
    safe_VkCopyImageInfo2KHR *local_pCopyImageInfo = NULL;
    {
        if (pCopyImageInfo) {
            local_pCopyImageInfo = &var_local_pCopyImageInfo;
            local_pCopyImageInfo->initialize(pCopyImageInfo);
            if (pCopyImageInfo->srcImage) {
                local_pCopyImageInfo->srcImage = layer_data->Unwrap(pCopyImageInfo->srcImage);
            }
            if (pCopyImageInfo->dstImage) {
                local_pCopyImageInfo->dstImage = layer_data->Unwrap(pCopyImageInfo->dstImage);
            }
        }
    }
    layer_data->device_dispatch_table.CmdCopyImage2KHR(commandBuffer, (const VkCopyImageInfo2KHR*)local_pCopyImageInfo);

}

void DispatchCmdCopyBufferToImage2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkCopyBufferToImageInfo2KHR*          pCopyBufferToImageInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdCopyBufferToImage2KHR(commandBuffer, pCopyBufferToImageInfo);
    safe_VkCopyBufferToImageInfo2KHR var_local_pCopyBufferToImageInfo;
    safe_VkCopyBufferToImageInfo2KHR *local_pCopyBufferToImageInfo = NULL;
    {
        if (pCopyBufferToImageInfo) {
            local_pCopyBufferToImageInfo = &var_local_pCopyBufferToImageInfo;
            local_pCopyBufferToImageInfo->initialize(pCopyBufferToImageInfo);
            if (pCopyBufferToImageInfo->srcBuffer) {
                local_pCopyBufferToImageInfo->srcBuffer = layer_data->Unwrap(pCopyBufferToImageInfo->srcBuffer);
            }
            if (pCopyBufferToImageInfo->dstImage) {
                local_pCopyBufferToImageInfo->dstImage = layer_data->Unwrap(pCopyBufferToImageInfo->dstImage);
            }
        }
    }
    layer_data->device_dispatch_table.CmdCopyBufferToImage2KHR(commandBuffer, (const VkCopyBufferToImageInfo2KHR*)local_pCopyBufferToImageInfo);

}

void DispatchCmdCopyImageToBuffer2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkCopyImageToBufferInfo2KHR*          pCopyImageToBufferInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdCopyImageToBuffer2KHR(commandBuffer, pCopyImageToBufferInfo);
    safe_VkCopyImageToBufferInfo2KHR var_local_pCopyImageToBufferInfo;
    safe_VkCopyImageToBufferInfo2KHR *local_pCopyImageToBufferInfo = NULL;
    {
        if (pCopyImageToBufferInfo) {
            local_pCopyImageToBufferInfo = &var_local_pCopyImageToBufferInfo;
            local_pCopyImageToBufferInfo->initialize(pCopyImageToBufferInfo);
            if (pCopyImageToBufferInfo->srcImage) {
                local_pCopyImageToBufferInfo->srcImage = layer_data->Unwrap(pCopyImageToBufferInfo->srcImage);
            }
            if (pCopyImageToBufferInfo->dstBuffer) {
                local_pCopyImageToBufferInfo->dstBuffer = layer_data->Unwrap(pCopyImageToBufferInfo->dstBuffer);
            }
        }
    }
    layer_data->device_dispatch_table.CmdCopyImageToBuffer2KHR(commandBuffer, (const VkCopyImageToBufferInfo2KHR*)local_pCopyImageToBufferInfo);

}

void DispatchCmdBlitImage2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkBlitImageInfo2KHR*                  pBlitImageInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdBlitImage2KHR(commandBuffer, pBlitImageInfo);
    safe_VkBlitImageInfo2KHR var_local_pBlitImageInfo;
    safe_VkBlitImageInfo2KHR *local_pBlitImageInfo = NULL;
    {
        if (pBlitImageInfo) {
            local_pBlitImageInfo = &var_local_pBlitImageInfo;
            local_pBlitImageInfo->initialize(pBlitImageInfo);
            if (pBlitImageInfo->srcImage) {
                local_pBlitImageInfo->srcImage = layer_data->Unwrap(pBlitImageInfo->srcImage);
            }
            if (pBlitImageInfo->dstImage) {
                local_pBlitImageInfo->dstImage = layer_data->Unwrap(pBlitImageInfo->dstImage);
            }
        }
    }
    layer_data->device_dispatch_table.CmdBlitImage2KHR(commandBuffer, (const VkBlitImageInfo2KHR*)local_pBlitImageInfo);

}

void DispatchCmdResolveImage2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkResolveImageInfo2KHR*               pResolveImageInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdResolveImage2KHR(commandBuffer, pResolveImageInfo);
    safe_VkResolveImageInfo2KHR var_local_pResolveImageInfo;
    safe_VkResolveImageInfo2KHR *local_pResolveImageInfo = NULL;
    {
        if (pResolveImageInfo) {
            local_pResolveImageInfo = &var_local_pResolveImageInfo;
            local_pResolveImageInfo->initialize(pResolveImageInfo);
            if (pResolveImageInfo->srcImage) {
                local_pResolveImageInfo->srcImage = layer_data->Unwrap(pResolveImageInfo->srcImage);
            }
            if (pResolveImageInfo->dstImage) {
                local_pResolveImageInfo->dstImage = layer_data->Unwrap(pResolveImageInfo->dstImage);
            }
        }
    }
    layer_data->device_dispatch_table.CmdResolveImage2KHR(commandBuffer, (const VkResolveImageInfo2KHR*)local_pResolveImageInfo);

}

VkResult DispatchReleaseDisplayEXT(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayKHR                                display)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    if (!wrap_handles) return layer_data->instance_dispatch_table.ReleaseDisplayEXT(physicalDevice, display);
    {
        display = layer_data->Unwrap(display);
    }
    VkResult result = layer_data->instance_dispatch_table.ReleaseDisplayEXT(physicalDevice, display);

    return result;
}

VkResult DispatchGetPhysicalDeviceSurfaceCapabilities2EXT(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    VkSurfaceCapabilities2EXT*                  pSurfaceCapabilities)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    if (!wrap_handles) return layer_data->instance_dispatch_table.GetPhysicalDeviceSurfaceCapabilities2EXT(physicalDevice, surface, pSurfaceCapabilities);
    {
        surface = layer_data->Unwrap(surface);
    }
    VkResult result = layer_data->instance_dispatch_table.GetPhysicalDeviceSurfaceCapabilities2EXT(physicalDevice, surface, pSurfaceCapabilities);

    return result;
}

VkResult DispatchDisplayPowerControlEXT(
    VkDevice                                    device,
    VkDisplayKHR                                display,
    const VkDisplayPowerInfoEXT*                pDisplayPowerInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.DisplayPowerControlEXT(device, display, pDisplayPowerInfo);
    {
        display = layer_data->Unwrap(display);
    }
    VkResult result = layer_data->device_dispatch_table.DisplayPowerControlEXT(device, display, pDisplayPowerInfo);

    return result;
}

VkResult DispatchRegisterDeviceEventEXT(
    VkDevice                                    device,
    const VkDeviceEventInfoEXT*                 pDeviceEventInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFence*                                    pFence)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.RegisterDeviceEventEXT(device, pDeviceEventInfo, pAllocator, pFence);
    VkResult result = layer_data->device_dispatch_table.RegisterDeviceEventEXT(device, pDeviceEventInfo, pAllocator, pFence);
    if (VK_SUCCESS == result) {
        *pFence = layer_data->WrapNew(*pFence);
    }
    return result;
}

VkResult DispatchRegisterDisplayEventEXT(
    VkDevice                                    device,
    VkDisplayKHR                                display,
    const VkDisplayEventInfoEXT*                pDisplayEventInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFence*                                    pFence)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.RegisterDisplayEventEXT(device, display, pDisplayEventInfo, pAllocator, pFence);
    {
        display = layer_data->Unwrap(display);
    }
    VkResult result = layer_data->device_dispatch_table.RegisterDisplayEventEXT(device, display, pDisplayEventInfo, pAllocator, pFence);
    if (VK_SUCCESS == result) {
        *pFence = layer_data->WrapNew(*pFence);
    }
    return result;
}

VkResult DispatchGetSwapchainCounterEXT(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    VkSurfaceCounterFlagBitsEXT                 counter,
    uint64_t*                                   pCounterValue)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetSwapchainCounterEXT(device, swapchain, counter, pCounterValue);
    {
        swapchain = layer_data->Unwrap(swapchain);
    }
    VkResult result = layer_data->device_dispatch_table.GetSwapchainCounterEXT(device, swapchain, counter, pCounterValue);

    return result;
}

void DispatchCmdSetDiscardRectangleEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstDiscardRectangle,
    uint32_t                                    discardRectangleCount,
    const VkRect2D*                             pDiscardRectangles)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetDiscardRectangleEXT(commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);

}

void DispatchSetHdrMetadataEXT(
    VkDevice                                    device,
    uint32_t                                    swapchainCount,
    const VkSwapchainKHR*                       pSwapchains,
    const VkHdrMetadataEXT*                     pMetadata)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.SetHdrMetadataEXT(device, swapchainCount, pSwapchains, pMetadata);
    VkSwapchainKHR var_local_pSwapchains[DISPATCH_MAX_STACK_ALLOCATIONS];
    VkSwapchainKHR *local_pSwapchains = NULL;
    {
        if (pSwapchains) {
            local_pSwapchains = swapchainCount > DISPATCH_MAX_STACK_ALLOCATIONS ? new VkSwapchainKHR[swapchainCount] : var_local_pSwapchains;
            for (uint32_t index0 = 0; index0 < swapchainCount; ++index0) {
                local_pSwapchains[index0] = layer_data->Unwrap(pSwapchains[index0]);
            }
        }
    }
    layer_data->device_dispatch_table.SetHdrMetadataEXT(device, swapchainCount, (const VkSwapchainKHR*)local_pSwapchains, pMetadata);
    if (local_pSwapchains != var_local_pSwapchains)
        delete[] local_pSwapchains;
}

// Skip vkSetDebugUtilsObjectNameEXT dispatch, manually generated

// Skip vkSetDebugUtilsObjectTagEXT dispatch, manually generated

void DispatchQueueBeginDebugUtilsLabelEXT(
    VkQueue                                     queue,
    const VkDebugUtilsLabelEXT*                 pLabelInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(queue), layer_data_map);
    layer_data->device_dispatch_table.QueueBeginDebugUtilsLabelEXT(queue, pLabelInfo);

}

void DispatchQueueEndDebugUtilsLabelEXT(
    VkQueue                                     queue)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(queue), layer_data_map);
    layer_data->device_dispatch_table.QueueEndDebugUtilsLabelEXT(queue);

}

void DispatchQueueInsertDebugUtilsLabelEXT(
    VkQueue                                     queue,
    const VkDebugUtilsLabelEXT*                 pLabelInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(queue), layer_data_map);
    layer_data->device_dispatch_table.QueueInsertDebugUtilsLabelEXT(queue, pLabelInfo);

}

void DispatchCmdBeginDebugUtilsLabelEXT(
    VkCommandBuffer                             commandBuffer,
    const VkDebugUtilsLabelEXT*                 pLabelInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);

}

void DispatchCmdEndDebugUtilsLabelEXT(
    VkCommandBuffer                             commandBuffer)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdEndDebugUtilsLabelEXT(commandBuffer);

}

void DispatchCmdInsertDebugUtilsLabelEXT(
    VkCommandBuffer                             commandBuffer,
    const VkDebugUtilsLabelEXT*                 pLabelInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdInsertDebugUtilsLabelEXT(commandBuffer, pLabelInfo);

}

VkResult DispatchCreateDebugUtilsMessengerEXT(
    VkInstance                                  instance,
    const VkDebugUtilsMessengerCreateInfoEXT*   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDebugUtilsMessengerEXT*                   pMessenger)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(instance), layer_data_map);
    if (!wrap_handles) return layer_data->instance_dispatch_table.CreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
    VkResult result = layer_data->instance_dispatch_table.CreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
    if (VK_SUCCESS == result) {
        *pMessenger = layer_data->WrapNew(*pMessenger);
    }
    return result;
}

void DispatchDestroyDebugUtilsMessengerEXT(
    VkInstance                                  instance,
    VkDebugUtilsMessengerEXT                    messenger,
    const VkAllocationCallbacks*                pAllocator)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(instance), layer_data_map);
    if (!wrap_handles) return layer_data->instance_dispatch_table.DestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
    uint64_t messenger_id = reinterpret_cast<uint64_t &>(messenger);
    auto iter = unique_id_mapping.pop(messenger_id);
    if (iter != unique_id_mapping.end()) {
        messenger = (VkDebugUtilsMessengerEXT)iter->second;
    } else {
        messenger = (VkDebugUtilsMessengerEXT)0;
    }
    layer_data->instance_dispatch_table.DestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);

}

void DispatchSubmitDebugUtilsMessageEXT(
    VkInstance                                  instance,
    VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(instance), layer_data_map);
    layer_data->instance_dispatch_table.SubmitDebugUtilsMessageEXT(instance, messageSeverity, messageTypes, pCallbackData);

}

void DispatchCmdSetSampleLocationsEXT(
    VkCommandBuffer                             commandBuffer,
    const VkSampleLocationsInfoEXT*             pSampleLocationsInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);

}

void DispatchGetPhysicalDeviceMultisamplePropertiesEXT(
    VkPhysicalDevice                            physicalDevice,
    VkSampleCountFlagBits                       samples,
    VkMultisamplePropertiesEXT*                 pMultisampleProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    layer_data->instance_dispatch_table.GetPhysicalDeviceMultisamplePropertiesEXT(physicalDevice, samples, pMultisampleProperties);

}

VkResult DispatchGetImageDrmFormatModifierPropertiesEXT(
    VkDevice                                    device,
    VkImage                                     image,
    VkImageDrmFormatModifierPropertiesEXT*      pProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetImageDrmFormatModifierPropertiesEXT(device, image, pProperties);
    {
        image = layer_data->Unwrap(image);
    }
    VkResult result = layer_data->device_dispatch_table.GetImageDrmFormatModifierPropertiesEXT(device, image, pProperties);

    return result;
}

VkResult DispatchGetMemoryHostPointerPropertiesEXT(
    VkDevice                                    device,
    VkExternalMemoryHandleTypeFlagBits          handleType,
    const void*                                 pHostPointer,
    VkMemoryHostPointerPropertiesEXT*           pMemoryHostPointerProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    VkResult result = layer_data->device_dispatch_table.GetMemoryHostPointerPropertiesEXT(device, handleType, pHostPointer, pMemoryHostPointerProperties);

    return result;
}

VkResult DispatchGetPhysicalDeviceCalibrateableTimeDomainsEXT(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pTimeDomainCount,
    VkTimeDomainEXT*                            pTimeDomains)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    VkResult result = layer_data->instance_dispatch_table.GetPhysicalDeviceCalibrateableTimeDomainsEXT(physicalDevice, pTimeDomainCount, pTimeDomains);

    return result;
}

VkResult DispatchGetCalibratedTimestampsEXT(
    VkDevice                                    device,
    uint32_t                                    timestampCount,
    const VkCalibratedTimestampInfoEXT*         pTimestampInfos,
    uint64_t*                                   pTimestamps,
    uint64_t*                                   pMaxDeviation)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    VkResult result = layer_data->device_dispatch_table.GetCalibratedTimestampsEXT(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);

    return result;
}

VkResult DispatchCreateHeadlessSurfaceEXT(
    VkInstance                                  instance,
    const VkHeadlessSurfaceCreateInfoEXT*       pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(instance), layer_data_map);
    if (!wrap_handles) return layer_data->instance_dispatch_table.CreateHeadlessSurfaceEXT(instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = layer_data->instance_dispatch_table.CreateHeadlessSurfaceEXT(instance, pCreateInfo, pAllocator, pSurface);
    if (VK_SUCCESS == result) {
        *pSurface = layer_data->WrapNew(*pSurface);
    }
    return result;
}

void DispatchCmdSetLineStippleEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    lineStippleFactor,
    uint16_t                                    lineStipplePattern)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);

}

void DispatchCmdSetCullModeEXT(
    VkCommandBuffer                             commandBuffer,
    VkCullModeFlags                             cullMode)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetCullModeEXT(commandBuffer, cullMode);

}

void DispatchCmdSetFrontFaceEXT(
    VkCommandBuffer                             commandBuffer,
    VkFrontFace                                 frontFace)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetFrontFaceEXT(commandBuffer, frontFace);

}

void DispatchCmdSetPrimitiveTopologyEXT(
    VkCommandBuffer                             commandBuffer,
    VkPrimitiveTopology                         primitiveTopology)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetPrimitiveTopologyEXT(commandBuffer, primitiveTopology);

}

void DispatchCmdSetViewportWithCountEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    viewportCount,
    const VkViewport*                           pViewports)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetViewportWithCountEXT(commandBuffer, viewportCount, pViewports);

}

void DispatchCmdSetScissorWithCountEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    scissorCount,
    const VkRect2D*                             pScissors)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetScissorWithCountEXT(commandBuffer, scissorCount, pScissors);

}

void DispatchCmdBindVertexBuffers2EXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstBinding,
    uint32_t                                    bindingCount,
    const VkBuffer*                             pBuffers,
    const VkDeviceSize*                         pOffsets,
    const VkDeviceSize*                         pSizes,
    const VkDeviceSize*                         pStrides)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.CmdBindVertexBuffers2EXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
    VkBuffer var_local_pBuffers[DISPATCH_MAX_STACK_ALLOCATIONS];
    VkBuffer *local_pBuffers = NULL;
    {
        if (pBuffers) {
            local_pBuffers = bindingCount > DISPATCH_MAX_STACK_ALLOCATIONS ? new VkBuffer[bindingCount] : var_local_pBuffers;
            for (uint32_t index0 = 0; index0 < bindingCount; ++index0) {
                local_pBuffers[index0] = layer_data->Unwrap(pBuffers[index0]);
            }
        }
    }
    layer_data->device_dispatch_table.CmdBindVertexBuffers2EXT(commandBuffer, firstBinding, bindingCount, (const VkBuffer*)local_pBuffers, pOffsets, pSizes, pStrides);
    if (local_pBuffers != var_local_pBuffers)
        delete[] local_pBuffers;
}

void DispatchCmdSetDepthTestEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    depthTestEnable)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetDepthTestEnableEXT(commandBuffer, depthTestEnable);

}

void DispatchCmdSetDepthWriteEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    depthWriteEnable)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetDepthWriteEnableEXT(commandBuffer, depthWriteEnable);

}

void DispatchCmdSetDepthCompareOpEXT(
    VkCommandBuffer                             commandBuffer,
    VkCompareOp                                 depthCompareOp)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetDepthCompareOpEXT(commandBuffer, depthCompareOp);

}

void DispatchCmdSetDepthBoundsTestEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    depthBoundsTestEnable)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetDepthBoundsTestEnableEXT(commandBuffer, depthBoundsTestEnable);

}

void DispatchCmdSetStencilTestEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    stencilTestEnable)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetStencilTestEnableEXT(commandBuffer, stencilTestEnable);

}

void DispatchCmdSetStencilOpEXT(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    VkStencilOp                                 failOp,
    VkStencilOp                                 passOp,
    VkStencilOp                                 depthFailOp,
    VkCompareOp                                 compareOp)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetStencilOpEXT(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);

}

void DispatchCmdSetVertexInputEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    vertexBindingDescriptionCount,
    const VkVertexInputBindingDescription2EXT*  pVertexBindingDescriptions,
    uint32_t                                    vertexAttributeDescriptionCount,
    const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetVertexInputEXT(commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);

}

#ifdef VK_USE_PLATFORM_SCI

VkResult DispatchGetFenceSciSyncFenceNV(
    VkDevice                                    device,
    const VkFenceGetSciSyncInfoNV*              pGetSciSyncHandleInfo,
    void*                                       pHandle)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetFenceSciSyncFenceNV(device, pGetSciSyncHandleInfo, pHandle);
    safe_VkFenceGetSciSyncInfoNV var_local_pGetSciSyncHandleInfo;
    safe_VkFenceGetSciSyncInfoNV *local_pGetSciSyncHandleInfo = NULL;
    {
        if (pGetSciSyncHandleInfo) {
            local_pGetSciSyncHandleInfo = &var_local_pGetSciSyncHandleInfo;
            local_pGetSciSyncHandleInfo->initialize(pGetSciSyncHandleInfo);
            if (pGetSciSyncHandleInfo->fence) {
                local_pGetSciSyncHandleInfo->fence = layer_data->Unwrap(pGetSciSyncHandleInfo->fence);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.GetFenceSciSyncFenceNV(device, (const VkFenceGetSciSyncInfoNV*)local_pGetSciSyncHandleInfo, pHandle);

    return result;
}
#endif // VK_USE_PLATFORM_SCI

#ifdef VK_USE_PLATFORM_SCI

VkResult DispatchGetFenceSciSyncObjNV(
    VkDevice                                    device,
    const VkFenceGetSciSyncInfoNV*              pGetSciSyncHandleInfo,
    void*                                       pHandle)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetFenceSciSyncObjNV(device, pGetSciSyncHandleInfo, pHandle);
    safe_VkFenceGetSciSyncInfoNV var_local_pGetSciSyncHandleInfo;
    safe_VkFenceGetSciSyncInfoNV *local_pGetSciSyncHandleInfo = NULL;
    {
        if (pGetSciSyncHandleInfo) {
            local_pGetSciSyncHandleInfo = &var_local_pGetSciSyncHandleInfo;
            local_pGetSciSyncHandleInfo->initialize(pGetSciSyncHandleInfo);
            if (pGetSciSyncHandleInfo->fence) {
                local_pGetSciSyncHandleInfo->fence = layer_data->Unwrap(pGetSciSyncHandleInfo->fence);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.GetFenceSciSyncObjNV(device, (const VkFenceGetSciSyncInfoNV*)local_pGetSciSyncHandleInfo, pHandle);

    return result;
}
#endif // VK_USE_PLATFORM_SCI

#ifdef VK_USE_PLATFORM_SCI

VkResult DispatchImportFenceSciSyncFenceNV(
    VkDevice                                    device,
    const VkImportFenceSciSyncInfoNV*           pImportFenceSciSyncInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.ImportFenceSciSyncFenceNV(device, pImportFenceSciSyncInfo);
    safe_VkImportFenceSciSyncInfoNV var_local_pImportFenceSciSyncInfo;
    safe_VkImportFenceSciSyncInfoNV *local_pImportFenceSciSyncInfo = NULL;
    {
        if (pImportFenceSciSyncInfo) {
            local_pImportFenceSciSyncInfo = &var_local_pImportFenceSciSyncInfo;
            local_pImportFenceSciSyncInfo->initialize(pImportFenceSciSyncInfo);
            if (pImportFenceSciSyncInfo->fence) {
                local_pImportFenceSciSyncInfo->fence = layer_data->Unwrap(pImportFenceSciSyncInfo->fence);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.ImportFenceSciSyncFenceNV(device, (const VkImportFenceSciSyncInfoNV*)local_pImportFenceSciSyncInfo);

    return result;
}
#endif // VK_USE_PLATFORM_SCI

#ifdef VK_USE_PLATFORM_SCI

VkResult DispatchImportFenceSciSyncObjNV(
    VkDevice                                    device,
    const VkImportFenceSciSyncInfoNV*           pImportFenceSciSyncInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.ImportFenceSciSyncObjNV(device, pImportFenceSciSyncInfo);
    safe_VkImportFenceSciSyncInfoNV var_local_pImportFenceSciSyncInfo;
    safe_VkImportFenceSciSyncInfoNV *local_pImportFenceSciSyncInfo = NULL;
    {
        if (pImportFenceSciSyncInfo) {
            local_pImportFenceSciSyncInfo = &var_local_pImportFenceSciSyncInfo;
            local_pImportFenceSciSyncInfo->initialize(pImportFenceSciSyncInfo);
            if (pImportFenceSciSyncInfo->fence) {
                local_pImportFenceSciSyncInfo->fence = layer_data->Unwrap(pImportFenceSciSyncInfo->fence);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.ImportFenceSciSyncObjNV(device, (const VkImportFenceSciSyncInfoNV*)local_pImportFenceSciSyncInfo);

    return result;
}
#endif // VK_USE_PLATFORM_SCI

#ifdef VK_USE_PLATFORM_SCI

VkResult DispatchGetPhysicalDeviceSciSyncAttributesNV(
    VkPhysicalDevice                            physicalDevice,
    const VkSciSyncAttributesInfoNV*            pSciSyncAttributesInfo,
    NvSciSyncAttrList                           pAttributes)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    VkResult result = layer_data->instance_dispatch_table.GetPhysicalDeviceSciSyncAttributesNV(physicalDevice, pSciSyncAttributesInfo, pAttributes);

    return result;
}
#endif // VK_USE_PLATFORM_SCI

#ifdef VK_USE_PLATFORM_SCI

VkResult DispatchGetSemaphoreSciSyncObjNV(
    VkDevice                                    device,
    const VkSemaphoreGetSciSyncInfoNV*          pGetSciSyncInfo,
    void*                                       pHandle)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetSemaphoreSciSyncObjNV(device, pGetSciSyncInfo, pHandle);
    safe_VkSemaphoreGetSciSyncInfoNV var_local_pGetSciSyncInfo;
    safe_VkSemaphoreGetSciSyncInfoNV *local_pGetSciSyncInfo = NULL;
    {
        if (pGetSciSyncInfo) {
            local_pGetSciSyncInfo = &var_local_pGetSciSyncInfo;
            local_pGetSciSyncInfo->initialize(pGetSciSyncInfo);
            if (pGetSciSyncInfo->semaphore) {
                local_pGetSciSyncInfo->semaphore = layer_data->Unwrap(pGetSciSyncInfo->semaphore);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.GetSemaphoreSciSyncObjNV(device, (const VkSemaphoreGetSciSyncInfoNV*)local_pGetSciSyncInfo, pHandle);

    return result;
}
#endif // VK_USE_PLATFORM_SCI

#ifdef VK_USE_PLATFORM_SCI

VkResult DispatchImportSemaphoreSciSyncObjNV(
    VkDevice                                    device,
    const VkImportSemaphoreSciSyncInfoNV*       pImportSemaphoreSciSyncInfo)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.ImportSemaphoreSciSyncObjNV(device, pImportSemaphoreSciSyncInfo);
    safe_VkImportSemaphoreSciSyncInfoNV var_local_pImportSemaphoreSciSyncInfo;
    safe_VkImportSemaphoreSciSyncInfoNV *local_pImportSemaphoreSciSyncInfo = NULL;
    {
        if (pImportSemaphoreSciSyncInfo) {
            local_pImportSemaphoreSciSyncInfo = &var_local_pImportSemaphoreSciSyncInfo;
            local_pImportSemaphoreSciSyncInfo->initialize(pImportSemaphoreSciSyncInfo);
            if (pImportSemaphoreSciSyncInfo->semaphore) {
                local_pImportSemaphoreSciSyncInfo->semaphore = layer_data->Unwrap(pImportSemaphoreSciSyncInfo->semaphore);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.ImportSemaphoreSciSyncObjNV(device, (const VkImportSemaphoreSciSyncInfoNV*)local_pImportSemaphoreSciSyncInfo);

    return result;
}
#endif // VK_USE_PLATFORM_SCI

#ifdef VK_USE_PLATFORM_SCI

VkResult DispatchGetMemorySciBufNV(
    VkDevice                                    device,
    const VkMemoryGetSciBufInfoNV*              pGetSciBufInfo,
    NvSciBufObj*                                pHandle)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(device), layer_data_map);
    if (!wrap_handles) return layer_data->device_dispatch_table.GetMemorySciBufNV(device, pGetSciBufInfo, pHandle);
    safe_VkMemoryGetSciBufInfoNV var_local_pGetSciBufInfo;
    safe_VkMemoryGetSciBufInfoNV *local_pGetSciBufInfo = NULL;
    {
        if (pGetSciBufInfo) {
            local_pGetSciBufInfo = &var_local_pGetSciBufInfo;
            local_pGetSciBufInfo->initialize(pGetSciBufInfo);
            if (pGetSciBufInfo->memory) {
                local_pGetSciBufInfo->memory = layer_data->Unwrap(pGetSciBufInfo->memory);
            }
        }
    }
    VkResult result = layer_data->device_dispatch_table.GetMemorySciBufNV(device, (const VkMemoryGetSciBufInfoNV*)local_pGetSciBufInfo, pHandle);

    return result;
}
#endif // VK_USE_PLATFORM_SCI

#ifdef VK_USE_PLATFORM_SCI

VkResult DispatchGetPhysicalDeviceExternalMemorySciBufPropertiesNV(
    VkPhysicalDevice                            physicalDevice,
    VkExternalMemoryHandleTypeFlagBits          handleType,
    NvSciBufObj                                 handle,
    VkMemorySciBufPropertiesNV*                 pMemorySciBufProperties)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    VkResult result = layer_data->instance_dispatch_table.GetPhysicalDeviceExternalMemorySciBufPropertiesNV(physicalDevice, handleType, handle, pMemorySciBufProperties);

    return result;
}
#endif // VK_USE_PLATFORM_SCI

#ifdef VK_USE_PLATFORM_SCI

VkResult DispatchGetPhysicalDeviceSciBufAttributesNV(
    VkPhysicalDevice                            physicalDevice,
    NvSciBufAttrList                            pAttributes)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), layer_data_map);
    VkResult result = layer_data->instance_dispatch_table.GetPhysicalDeviceSciBufAttributesNV(physicalDevice, pAttributes);

    return result;
}
#endif // VK_USE_PLATFORM_SCI

void DispatchCmdSetPatchControlPointsEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    patchControlPoints)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetPatchControlPointsEXT(commandBuffer, patchControlPoints);

}

void DispatchCmdSetRasterizerDiscardEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    rasterizerDiscardEnable)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetRasterizerDiscardEnableEXT(commandBuffer, rasterizerDiscardEnable);

}

void DispatchCmdSetDepthBiasEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    depthBiasEnable)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetDepthBiasEnableEXT(commandBuffer, depthBiasEnable);

}

void DispatchCmdSetLogicOpEXT(
    VkCommandBuffer                             commandBuffer,
    VkLogicOp                                   logicOp)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetLogicOpEXT(commandBuffer, logicOp);

}

void DispatchCmdSetPrimitiveRestartEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    primitiveRestartEnable)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetPrimitiveRestartEnableEXT(commandBuffer, primitiveRestartEnable);

}

void                                    DispatchCmdSetColorWriteEnableEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    attachmentCount,
    const VkBool32*                             pColorWriteEnables)
{
    auto layer_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), layer_data_map);
    layer_data->device_dispatch_table.CmdSetColorWriteEnableEXT(commandBuffer, attachmentCount, pColorWriteEnables);

}