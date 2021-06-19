
// This file is ***GENERATED***.  Do Not Edit.
// See thread_safety_generator.py for modifications.

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
#include "chassis.h"
#include "thread_safety.h"


ReadLockGuard ThreadSafety::ReadLock() {
    return ReadLockGuard(validation_object_mutex, std::defer_lock);
}

WriteLockGuard ThreadSafety::WriteLock() {
    return WriteLockGuard(validation_object_mutex, std::defer_lock);
}

void ThreadSafety::PreCallRecordAllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo *pAllocateInfo,
                                                       VkCommandBuffer *pCommandBuffers) {
    StartReadObjectParentInstance(device, "vkAllocateCommandBuffers");
    StartWriteObject(pAllocateInfo->commandPool, "vkAllocateCommandBuffers");
}

void ThreadSafety::PostCallRecordAllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo *pAllocateInfo,
                                                        VkCommandBuffer *pCommandBuffers, VkResult result) {
    FinishReadObjectParentInstance(device, "vkAllocateCommandBuffers");
    FinishWriteObject(pAllocateInfo->commandPool, "vkAllocateCommandBuffers");

    // Record mapping from command buffer to command pool
    if(pCommandBuffers) {
        auto lock = WriteLockGuard(thread_safety_lock);
        auto &pool_command_buffers = pool_command_buffers_map[pAllocateInfo->commandPool];
        for (uint32_t index = 0; index < pAllocateInfo->commandBufferCount; index++) {
            command_pool_map.insert_or_assign(pCommandBuffers[index], pAllocateInfo->commandPool);
            CreateObject(pCommandBuffers[index]);
            pool_command_buffers.insert(pCommandBuffers[index]);
        }
    }
}


void ThreadSafety::PreCallRecordCreateDescriptorSetLayout(
    VkDevice                                    device,
    const VkDescriptorSetLayoutCreateInfo*      pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDescriptorSetLayout*                      pSetLayout) {
    StartReadObjectParentInstance(device, "vkCreateDescriptorSetLayout");
}

void ThreadSafety::PostCallRecordCreateDescriptorSetLayout(
    VkDevice                                    device,
    const VkDescriptorSetLayoutCreateInfo*      pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDescriptorSetLayout*                      pSetLayout,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateDescriptorSetLayout");
    if (result == VK_SUCCESS) {
        CreateObject(*pSetLayout);

        // Check whether any binding uses UPDATE_AFTER_BIND
        bool update_after_bind = false;
        const auto *flags_create_info = LvlFindInChain<VkDescriptorSetLayoutBindingFlagsCreateInfo>(pCreateInfo->pNext);
        if (flags_create_info) {
            for (uint32_t i = 0; i < flags_create_info->bindingCount; ++i) {
                if (flags_create_info->pBindingFlags[i] & VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT) {
                    update_after_bind = true;
                    break;
                }
            }
        }
        dsl_update_after_bind_map.insert_or_assign(*pSetLayout, update_after_bind);
    }
}

void ThreadSafety::PreCallRecordAllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo *pAllocateInfo,
                                                       VkDescriptorSet *pDescriptorSets) {
    StartReadObjectParentInstance(device, "vkAllocateDescriptorSets");
    StartWriteObject(pAllocateInfo->descriptorPool, "vkAllocateDescriptorSets");
    // Host access to pAllocateInfo::descriptorPool must be externally synchronized
}

void ThreadSafety::PostCallRecordAllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo *pAllocateInfo,
                                                        VkDescriptorSet *pDescriptorSets, VkResult result) {
    FinishReadObjectParentInstance(device, "vkAllocateDescriptorSets");
    FinishWriteObject(pAllocateInfo->descriptorPool, "vkAllocateDescriptorSets");
    // Host access to pAllocateInfo::descriptorPool must be externally synchronized
    if (VK_SUCCESS == result) {
        auto lock = WriteLockGuard(thread_safety_lock);
        auto &pool_descriptor_sets = pool_descriptor_sets_map[pAllocateInfo->descriptorPool];
        for (uint32_t index0 = 0; index0 < pAllocateInfo->descriptorSetCount; index0++) {
            CreateObject(pDescriptorSets[index0]);
            pool_descriptor_sets.insert(pDescriptorSets[index0]);

            auto iter = dsl_update_after_bind_map.find(pAllocateInfo->pSetLayouts[index0]);
            if (iter != dsl_update_after_bind_map.end()) {
                ds_update_after_bind_map.insert_or_assign(pDescriptorSets[index0], iter->second);
            } else {
                assert(0 && "descriptor set layout not found");
            }
        }
    }
}

void ThreadSafety::PreCallRecordFreeDescriptorSets(
    VkDevice                                    device,
    VkDescriptorPool                            descriptorPool,
    uint32_t                                    descriptorSetCount,
    const VkDescriptorSet*                      pDescriptorSets) {
    StartReadObjectParentInstance(device, "vkFreeDescriptorSets");
    StartWriteObject(descriptorPool, "vkFreeDescriptorSets");
    if (pDescriptorSets) {
        for (uint32_t index=0; index < descriptorSetCount; index++) {
            StartWriteObject(pDescriptorSets[index], "vkFreeDescriptorSets");
        }
    }
    // Host access to descriptorPool must be externally synchronized
    // Host access to each member of pDescriptorSets must be externally synchronized
}

void ThreadSafety::PostCallRecordFreeDescriptorSets(
    VkDevice                                    device,
    VkDescriptorPool                            descriptorPool,
    uint32_t                                    descriptorSetCount,
    const VkDescriptorSet*                      pDescriptorSets,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkFreeDescriptorSets");
    FinishWriteObject(descriptorPool, "vkFreeDescriptorSets");
    if (pDescriptorSets) {
        for (uint32_t index=0; index < descriptorSetCount; index++) {
            FinishWriteObject(pDescriptorSets[index], "vkFreeDescriptorSets");
        }
    }
    // Host access to descriptorPool must be externally synchronized
    // Host access to each member of pDescriptorSets must be externally synchronized
    // Host access to pAllocateInfo::descriptorPool must be externally synchronized
    if (VK_SUCCESS == result) {
        auto lock = WriteLockGuard(thread_safety_lock);
        auto &pool_descriptor_sets = pool_descriptor_sets_map[descriptorPool];
        for (uint32_t index0 = 0; index0 < descriptorSetCount; index0++) {
            DestroyObject(pDescriptorSets[index0]);
            pool_descriptor_sets.erase(pDescriptorSets[index0]);
        }
    }
}

void ThreadSafety::PreCallRecordDestroyDescriptorPool(
    VkDevice                                    device,
    VkDescriptorPool                            descriptorPool,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroyDescriptorPool");
    StartWriteObject(descriptorPool, "vkDestroyDescriptorPool");
    // Host access to descriptorPool must be externally synchronized
    auto lock = ReadLockGuard(thread_safety_lock);
    auto iterator = pool_descriptor_sets_map.find(descriptorPool);
    // Possible to have no descriptor sets allocated from pool
    if (iterator != pool_descriptor_sets_map.end()) {
        for(auto descriptor_set : pool_descriptor_sets_map[descriptorPool]) {
            StartWriteObject(descriptor_set, "vkDestroyDescriptorPool");
        }
    }
}

void ThreadSafety::PostCallRecordDestroyDescriptorPool(
    VkDevice                                    device,
    VkDescriptorPool                            descriptorPool,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroyDescriptorPool");
    FinishWriteObject(descriptorPool, "vkDestroyDescriptorPool");
    DestroyObject(descriptorPool);
    // Host access to descriptorPool must be externally synchronized
    {
        auto lock = WriteLockGuard(thread_safety_lock);
        // remove references to implicitly freed descriptor sets
        for(auto descriptor_set : pool_descriptor_sets_map[descriptorPool]) {
            FinishWriteObject(descriptor_set, "vkDestroyDescriptorPool");
            DestroyObject(descriptor_set);
        }
        pool_descriptor_sets_map[descriptorPool].clear();
        pool_descriptor_sets_map.erase(descriptorPool);
    }
}

void ThreadSafety::PreCallRecordResetDescriptorPool(
    VkDevice                                    device,
    VkDescriptorPool                            descriptorPool,
    VkDescriptorPoolResetFlags                  flags) {
    StartReadObjectParentInstance(device, "vkResetDescriptorPool");
    StartWriteObject(descriptorPool, "vkResetDescriptorPool");
    // Host access to descriptorPool must be externally synchronized
    // any sname:VkDescriptorSet objects allocated from pname:descriptorPool must be externally synchronized between host accesses
    auto lock = ReadLockGuard(thread_safety_lock);
    auto iterator = pool_descriptor_sets_map.find(descriptorPool);
    // Possible to have no descriptor sets allocated from pool
    if (iterator != pool_descriptor_sets_map.end()) {
        for(auto descriptor_set : pool_descriptor_sets_map[descriptorPool]) {
            StartWriteObject(descriptor_set, "vkResetDescriptorPool");
        }
    }
}

void ThreadSafety::PostCallRecordResetDescriptorPool(
    VkDevice                                    device,
    VkDescriptorPool                            descriptorPool,
    VkDescriptorPoolResetFlags                  flags,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkResetDescriptorPool");
    FinishWriteObject(descriptorPool, "vkResetDescriptorPool");
    // Host access to descriptorPool must be externally synchronized
    // any sname:VkDescriptorSet objects allocated from pname:descriptorPool must be externally synchronized between host accesses
    if (VK_SUCCESS == result) {
        // remove references to implicitly freed descriptor sets
        auto lock = WriteLockGuard(thread_safety_lock);
        for(auto descriptor_set : pool_descriptor_sets_map[descriptorPool]) {
            FinishWriteObject(descriptor_set, "vkResetDescriptorPool");
            DestroyObject(descriptor_set);
        }
        pool_descriptor_sets_map[descriptorPool].clear();
    }
}

bool ThreadSafety::DsUpdateAfterBind(VkDescriptorSet set) const
{
    auto iter = ds_update_after_bind_map.find(set);
    if (iter != ds_update_after_bind_map.end()) {
        return iter->second;
    }
    return false;
}

void ThreadSafety::PreCallRecordUpdateDescriptorSets(
    VkDevice                                    device,
    uint32_t                                    descriptorWriteCount,
    const VkWriteDescriptorSet*                 pDescriptorWrites,
    uint32_t                                    descriptorCopyCount,
    const VkCopyDescriptorSet*                  pDescriptorCopies) {
    StartReadObjectParentInstance(device, "vkUpdateDescriptorSets");
    if (pDescriptorWrites) {
        for (uint32_t index=0; index < descriptorWriteCount; index++) {
            auto dstSet = pDescriptorWrites[index].dstSet;
            bool update_after_bind = DsUpdateAfterBind(dstSet);
            if (update_after_bind) {
                StartReadObject(dstSet, "vkUpdateDescriptorSets");
            } else {
                StartWriteObject(dstSet, "vkUpdateDescriptorSets");
            }
        }
    }
    if (pDescriptorCopies) {
        for (uint32_t index=0; index < descriptorCopyCount; index++) {
            auto dstSet = pDescriptorCopies[index].dstSet;
            bool update_after_bind = DsUpdateAfterBind(dstSet);
            if (update_after_bind) {
                StartReadObject(dstSet, "vkUpdateDescriptorSets");
            } else {
                StartWriteObject(dstSet, "vkUpdateDescriptorSets");
            }
            StartReadObject(pDescriptorCopies[index].srcSet, "vkUpdateDescriptorSets");
        }
    }
    // Host access to pDescriptorWrites[].dstSet must be externally synchronized
    // Host access to pDescriptorCopies[].dstSet must be externally synchronized
}

void ThreadSafety::PostCallRecordUpdateDescriptorSets(
    VkDevice                                    device,
    uint32_t                                    descriptorWriteCount,
    const VkWriteDescriptorSet*                 pDescriptorWrites,
    uint32_t                                    descriptorCopyCount,
    const VkCopyDescriptorSet*                  pDescriptorCopies) {
    FinishReadObjectParentInstance(device, "vkUpdateDescriptorSets");
    if (pDescriptorWrites) {
        for (uint32_t index=0; index < descriptorWriteCount; index++) {
            auto dstSet = pDescriptorWrites[index].dstSet;
            bool update_after_bind = DsUpdateAfterBind(dstSet);
            if (update_after_bind) {
                FinishReadObject(dstSet, "vkUpdateDescriptorSets");
            } else {
                FinishWriteObject(dstSet, "vkUpdateDescriptorSets");
            }
        }
    }
    if (pDescriptorCopies) {
        for (uint32_t index=0; index < descriptorCopyCount; index++) {
            auto dstSet = pDescriptorCopies[index].dstSet;
            bool update_after_bind = DsUpdateAfterBind(dstSet);
            if (update_after_bind) {
                FinishReadObject(dstSet, "vkUpdateDescriptorSets");
            } else {
                FinishWriteObject(dstSet, "vkUpdateDescriptorSets");
            }
            FinishReadObject(pDescriptorCopies[index].srcSet, "vkUpdateDescriptorSets");
        }
    }
    // Host access to pDescriptorWrites[].dstSet must be externally synchronized
    // Host access to pDescriptorCopies[].dstSet must be externally synchronized
}

void ThreadSafety::PreCallRecordUpdateDescriptorSetWithTemplate(
    VkDevice                                    device,
    VkDescriptorSet                             descriptorSet,
    VkDescriptorUpdateTemplate                  descriptorUpdateTemplate,
    const void*                                 pData) {
    StartReadObjectParentInstance(device, "vkUpdateDescriptorSetWithTemplate");
    StartReadObject(descriptorUpdateTemplate, "vkUpdateDescriptorSetWithTemplate");

    bool update_after_bind = DsUpdateAfterBind(descriptorSet);
    if (update_after_bind) {
        StartReadObject(descriptorSet, "vkUpdateDescriptorSetWithTemplate");
    } else {
        StartWriteObject(descriptorSet, "vkUpdateDescriptorSetWithTemplate");
    }
    // Host access to descriptorSet must be externally synchronized
}

void ThreadSafety::PostCallRecordUpdateDescriptorSetWithTemplate(
    VkDevice                                    device,
    VkDescriptorSet                             descriptorSet,
    VkDescriptorUpdateTemplate                  descriptorUpdateTemplate,
    const void*                                 pData) {
    FinishReadObjectParentInstance(device, "vkUpdateDescriptorSetWithTemplate");
    FinishReadObject(descriptorUpdateTemplate, "vkUpdateDescriptorSetWithTemplate");

    bool update_after_bind = DsUpdateAfterBind(descriptorSet);
    if (update_after_bind) {
        FinishReadObject(descriptorSet, "vkUpdateDescriptorSetWithTemplate");
    } else {
        FinishWriteObject(descriptorSet, "vkUpdateDescriptorSetWithTemplate");
    }
    // Host access to descriptorSet must be externally synchronized
}

void ThreadSafety::PreCallRecordUpdateDescriptorSetWithTemplateKHR(
    VkDevice                                    device,
    VkDescriptorSet                             descriptorSet,
    VkDescriptorUpdateTemplate                  descriptorUpdateTemplate,
    const void*                                 pData) {
    StartReadObjectParentInstance(device, "vkUpdateDescriptorSetWithTemplateKHR");
    StartReadObject(descriptorUpdateTemplate, "vkUpdateDescriptorSetWithTemplateKHR");

    bool update_after_bind = DsUpdateAfterBind(descriptorSet);
    if (update_after_bind) {
        StartReadObject(descriptorSet, "vkUpdateDescriptorSetWithTemplateKHR");
    } else {
        StartWriteObject(descriptorSet, "vkUpdateDescriptorSetWithTemplateKHR");
    }
    // Host access to descriptorSet must be externally synchronized
}

void ThreadSafety::PostCallRecordUpdateDescriptorSetWithTemplateKHR(
    VkDevice                                    device,
    VkDescriptorSet                             descriptorSet,
    VkDescriptorUpdateTemplate                  descriptorUpdateTemplate,
    const void*                                 pData) {
    FinishReadObjectParentInstance(device, "vkUpdateDescriptorSetWithTemplateKHR");
    FinishReadObject(descriptorUpdateTemplate, "vkUpdateDescriptorSetWithTemplateKHR");

    bool update_after_bind = DsUpdateAfterBind(descriptorSet);
    if (update_after_bind) {
        FinishReadObject(descriptorSet, "vkUpdateDescriptorSetWithTemplateKHR");
    } else {
        FinishWriteObject(descriptorSet, "vkUpdateDescriptorSetWithTemplateKHR");
    }
    // Host access to descriptorSet must be externally synchronized
}

void ThreadSafety::PreCallRecordFreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount,
                                                   const VkCommandBuffer *pCommandBuffers) {
    const bool lockCommandPool = false;  // pool is already directly locked
    StartReadObjectParentInstance(device, "vkFreeCommandBuffers");
    StartWriteObject(commandPool, "vkFreeCommandBuffers");
    if(pCommandBuffers) {
        // Even though we're immediately "finishing" below, we still are testing for concurrency with any call in process
        // so this isn't a no-op
        // The driver may immediately reuse command buffers in another thread.
        // These updates need to be done before calling down to the driver.
        auto lock = WriteLockGuard(thread_safety_lock);
        auto &pool_command_buffers = pool_command_buffers_map[commandPool];
        for (uint32_t index = 0; index < commandBufferCount; index++) {
            StartWriteObject(pCommandBuffers[index], "vkFreeCommandBuffers", lockCommandPool);
            FinishWriteObject(pCommandBuffers[index], "vkFreeCommandBuffers", lockCommandPool);
            DestroyObject(pCommandBuffers[index]);
            pool_command_buffers.erase(pCommandBuffers[index]);
            command_pool_map.erase(pCommandBuffers[index]);
        }
    }
}

void ThreadSafety::PostCallRecordFreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount,
                                                    const VkCommandBuffer *pCommandBuffers) {
    FinishReadObjectParentInstance(device, "vkFreeCommandBuffers");
    FinishWriteObject(commandPool, "vkFreeCommandBuffers");
}

void ThreadSafety::PreCallRecordCreateCommandPool(
    VkDevice                                    device,
    const VkCommandPoolCreateInfo*              pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkCommandPool*                              pCommandPool) {
    StartReadObjectParentInstance(device, "vkCreateCommandPool");
}

void ThreadSafety::PostCallRecordCreateCommandPool(
    VkDevice                                    device,
    const VkCommandPoolCreateInfo*              pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkCommandPool*                              pCommandPool,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateCommandPool");
    if (result == VK_SUCCESS) {
        CreateObject(*pCommandPool);
        c_VkCommandPoolContents.CreateObject(*pCommandPool);
    }
}

void ThreadSafety::PreCallRecordResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) {
    StartReadObjectParentInstance(device, "vkResetCommandPool");
    StartWriteObject(commandPool, "vkResetCommandPool");
    // Check for any uses of non-externally sync'd command buffers (for example from vkCmdExecuteCommands)
    c_VkCommandPoolContents.StartWrite(commandPool, "vkResetCommandPool");
    // Host access to commandPool must be externally synchronized
}

void ThreadSafety::PostCallRecordResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags, VkResult result) {
    FinishReadObjectParentInstance(device, "vkResetCommandPool");
    FinishWriteObject(commandPool, "vkResetCommandPool");
    c_VkCommandPoolContents.FinishWrite(commandPool, "vkResetCommandPool");
    // Host access to commandPool must be externally synchronized
}

void ThreadSafety::PreCallRecordDestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks *pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroyCommandPool");
    StartWriteObject(commandPool, "vkDestroyCommandPool");
    // Check for any uses of non-externally sync'd command buffers (for example from vkCmdExecuteCommands)
    c_VkCommandPoolContents.StartWrite(commandPool, "vkDestroyCommandPool");
    // Host access to commandPool must be externally synchronized

    auto lock = WriteLockGuard(thread_safety_lock);
    // The driver may immediately reuse command buffers in another thread.
    // These updates need to be done before calling down to the driver.
    // remove references to implicitly freed command pools
    for(auto command_buffer : pool_command_buffers_map[commandPool]) {
        DestroyObject(command_buffer);
    }
    pool_command_buffers_map[commandPool].clear();
    pool_command_buffers_map.erase(commandPool);
}

void ThreadSafety::PostCallRecordDestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks *pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroyCommandPool");
    FinishWriteObject(commandPool, "vkDestroyCommandPool");
    DestroyObject(commandPool);
    c_VkCommandPoolContents.FinishWrite(commandPool, "vkDestroyCommandPool");
    c_VkCommandPoolContents.DestroyObject(commandPool);
}

// GetSwapchainImages can return a non-zero count with a NULL pSwapchainImages pointer.  Let's avoid crashes by ignoring
// pSwapchainImages.
void ThreadSafety::PreCallRecordGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t *pSwapchainImageCount,
                                                      VkImage *pSwapchainImages) {
    StartReadObjectParentInstance(device, "vkGetSwapchainImagesKHR");
    StartReadObjectParentInstance(swapchain, "vkGetSwapchainImagesKHR");
}

void ThreadSafety::PostCallRecordGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t *pSwapchainImageCount,
                                                       VkImage *pSwapchainImages, VkResult result) {
    FinishReadObjectParentInstance(device, "vkGetSwapchainImagesKHR");
    FinishReadObjectParentInstance(swapchain, "vkGetSwapchainImagesKHR");
    if (pSwapchainImages != NULL) {
        auto lock = WriteLockGuard(thread_safety_lock);
        auto &wrapped_swapchain_image_handles = swapchain_wrapped_image_handle_map[swapchain];
        for (uint32_t i = static_cast<uint32_t>(wrapped_swapchain_image_handles.size()); i < *pSwapchainImageCount; i++) {
            CreateObject(pSwapchainImages[i]);
            wrapped_swapchain_image_handles.emplace_back(pSwapchainImages[i]);
        }
    }
}

void ThreadSafety::PreCallRecordDestroySwapchainKHR(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroySwapchainKHR");
    StartWriteObjectParentInstance(swapchain, "vkDestroySwapchainKHR");
    // Host access to swapchain must be externally synchronized
    auto lock = ReadLockGuard(thread_safety_lock);
    for (auto &image_handle : swapchain_wrapped_image_handle_map[swapchain]) {
        StartWriteObject(image_handle, "vkDestroySwapchainKHR");
    }
}

void ThreadSafety::PostCallRecordDestroySwapchainKHR(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroySwapchainKHR");
    FinishWriteObjectParentInstance(swapchain, "vkDestroySwapchainKHR");
    DestroyObjectParentInstance(swapchain);
    // Host access to swapchain must be externally synchronized
    auto lock = WriteLockGuard(thread_safety_lock);
    for (auto &image_handle : swapchain_wrapped_image_handle_map[swapchain]) {
        FinishWriteObject(image_handle, "vkDestroySwapchainKHR");
        DestroyObject(image_handle);
    }
    swapchain_wrapped_image_handle_map.erase(swapchain);
}

void ThreadSafety::PreCallRecordDestroyDevice(
    VkDevice                                    device,
    const VkAllocationCallbacks*                pAllocator) {
    StartWriteObjectParentInstance(device, "vkDestroyDevice");
    // Host access to device must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroyDevice(
    VkDevice                                    device,
    const VkAllocationCallbacks*                pAllocator) {
    FinishWriteObjectParentInstance(device, "vkDestroyDevice");
    DestroyObjectParentInstance(device);
    // Host access to device must be externally synchronized
    auto lock = WriteLockGuard(thread_safety_lock);
    for (auto &queue : device_queues_map[device]) {
        DestroyObject(queue);
    }
    device_queues_map[device].clear();
}

void ThreadSafety::PreCallRecordGetDeviceQueue(
    VkDevice                                    device,
    uint32_t                                    queueFamilyIndex,
    uint32_t                                    queueIndex,
    VkQueue*                                    pQueue) {
    StartReadObjectParentInstance(device, "vkGetDeviceQueue");
}

void ThreadSafety::PostCallRecordGetDeviceQueue(
    VkDevice                                    device,
    uint32_t                                    queueFamilyIndex,
    uint32_t                                    queueIndex,
    VkQueue*                                    pQueue) {
    FinishReadObjectParentInstance(device, "vkGetDeviceQueue");
    CreateObject(*pQueue);
    auto lock = WriteLockGuard(thread_safety_lock);
    device_queues_map[device].insert(*pQueue);
}

void ThreadSafety::PreCallRecordGetDeviceQueue2(
    VkDevice                                    device,
    const VkDeviceQueueInfo2*                   pQueueInfo,
    VkQueue*                                    pQueue) {
    StartReadObjectParentInstance(device, "vkGetDeviceQueue2");
}

void ThreadSafety::PostCallRecordGetDeviceQueue2(
    VkDevice                                    device,
    const VkDeviceQueueInfo2*                   pQueueInfo,
    VkQueue*                                    pQueue) {
    FinishReadObjectParentInstance(device, "vkGetDeviceQueue2");
    CreateObject(*pQueue);
    auto lock = WriteLockGuard(thread_safety_lock);
    device_queues_map[device].insert(*pQueue);
}

void ThreadSafety::PostCallRecordGetPhysicalDeviceDisplayPropertiesKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pPropertyCount,
    VkDisplayPropertiesKHR*                     pProperties,
    VkResult                                    result) {
    if ((result != VK_SUCCESS) && (result != VK_INCOMPLETE)) return;
    if (pProperties) {
        for (uint32_t i = 0; i < *pPropertyCount; ++i) {
            CreateObject(pProperties[i].display);
        }
    }
}

void ThreadSafety::PostCallRecordGetPhysicalDeviceDisplayProperties2KHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pPropertyCount,
    VkDisplayProperties2KHR*                    pProperties,
    VkResult                                    result) {
    if ((result != VK_SUCCESS) && (result != VK_INCOMPLETE)) return;
    if (pProperties) {
        for (uint32_t i = 0; i < *pPropertyCount; ++i) {
            CreateObject(pProperties[i].displayProperties.display);
        }
    }
}

void ThreadSafety::PostCallRecordGetPhysicalDeviceDisplayPlanePropertiesKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pPropertyCount,
    VkDisplayPlanePropertiesKHR*                pProperties,
    VkResult                                    result) {
    if ((result != VK_SUCCESS) && (result != VK_INCOMPLETE)) return;
    if (pProperties) {
        for (uint32_t i = 0; i < *pPropertyCount; ++i) {
            CreateObject(pProperties[i].currentDisplay);
        }
    }
}

void ThreadSafety::PostCallRecordGetPhysicalDeviceDisplayPlaneProperties2KHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pPropertyCount,
    VkDisplayPlaneProperties2KHR*               pProperties,
    VkResult                                    result) {
    if ((result != VK_SUCCESS) && (result != VK_INCOMPLETE)) return;
    if (pProperties) {
        for (uint32_t i = 0; i < *pPropertyCount; ++i) {
            CreateObject(pProperties[i].displayPlaneProperties.currentDisplay);
        }
    }
}

void ThreadSafety::PreCallRecordGetDisplayPlaneSupportedDisplaysKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    planeIndex,
    uint32_t*                                   pDisplayCount,
    VkDisplayKHR*                               pDisplays) {
    // Nothing to do for this pre-call function
}

void ThreadSafety::PostCallRecordGetDisplayPlaneSupportedDisplaysKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    planeIndex,
    uint32_t*                                   pDisplayCount,
    VkDisplayKHR*                               pDisplays,
    VkResult                                    result) {
    if ((result != VK_SUCCESS) && (result != VK_INCOMPLETE)) return;
    if (pDisplays) {
        for (uint32_t index = 0; index < *pDisplayCount; index++) {
            CreateObject(pDisplays[index]);
        }
    }
}

void ThreadSafety::PreCallRecordGetDisplayModePropertiesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayKHR                                display,
    uint32_t*                                   pPropertyCount,
    VkDisplayModePropertiesKHR*                 pProperties) {
    StartReadObject(display, "vkGetDisplayModePropertiesKHR");
}

void ThreadSafety::PostCallRecordGetDisplayModePropertiesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayKHR                                display,
    uint32_t*                                   pPropertyCount,
    VkDisplayModePropertiesKHR*                 pProperties,
    VkResult                                    result) {
    FinishReadObject(display, "vkGetDisplayModePropertiesKHR");
    if ((result != VK_SUCCESS) && (result != VK_INCOMPLETE)) return;
    if (pProperties != nullptr) {
        for (uint32_t index = 0; index < *pPropertyCount; index++) {
            CreateObject(pProperties[index].displayMode);
        }
    }
}

void ThreadSafety::PreCallRecordGetDisplayModeProperties2KHR(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayKHR                                display,
    uint32_t*                                   pPropertyCount,
    VkDisplayModeProperties2KHR*                pProperties) {
    StartReadObject(display, "vkGetDisplayModeProperties2KHR");
}

void ThreadSafety::PostCallRecordGetDisplayModeProperties2KHR(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayKHR                                display,
    uint32_t*                                   pPropertyCount,
    VkDisplayModeProperties2KHR*                pProperties,
    VkResult                                    result) {
    FinishReadObject(display, "vkGetDisplayModeProperties2KHR");
    if ((result != VK_SUCCESS) && (result != VK_INCOMPLETE)) return;
    if (pProperties != nullptr) {
        for (uint32_t index = 0; index < *pPropertyCount; index++) {
            CreateObject(pProperties[index].displayModeProperties.displayMode);
        }
    }
}

void ThreadSafety::PreCallRecordGetDisplayPlaneCapabilities2KHR(
    VkPhysicalDevice                            physicalDevice,
    const VkDisplayPlaneInfo2KHR*               pDisplayPlaneInfo,
    VkDisplayPlaneCapabilities2KHR*             pCapabilities) {
    StartWriteObject(pDisplayPlaneInfo->mode, "vkGetDisplayPlaneCapabilities2KHR");
}

void ThreadSafety::PostCallRecordGetDisplayPlaneCapabilities2KHR(
    VkPhysicalDevice                            physicalDevice,
    const VkDisplayPlaneInfo2KHR*               pDisplayPlaneInfo,
    VkDisplayPlaneCapabilities2KHR*             pCapabilities,
    VkResult                                    result) {
    FinishWriteObject(pDisplayPlaneInfo->mode, "vkGetDisplayPlaneCapabilities2KHR");
}

#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT

void ThreadSafety::PostCallRecordGetRandROutputDisplayEXT(
    VkPhysicalDevice                            physicalDevice,
    Display*                                    dpy,
    RROutput                                    rrOutput,
    VkDisplayKHR*                               pDisplay,
    VkResult                                    result) {
    if ((result != VK_SUCCESS) || (pDisplay == nullptr)) return;
    CreateObject(*pDisplay);
}

#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT

void ThreadSafety::PreCallRecordDeviceWaitIdle(
    VkDevice                                    device) {
    StartReadObjectParentInstance(device, "vkDeviceWaitIdle");
    auto lock = ReadLockGuard(thread_safety_lock);
    const auto &queue_set = device_queues_map[device];
    for (const auto &queue : queue_set) {
        StartWriteObject(queue, "vkDeviceWaitIdle");
    }
}

void ThreadSafety::PostCallRecordDeviceWaitIdle(
    VkDevice                                    device,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkDeviceWaitIdle");
    auto lock = ReadLockGuard(thread_safety_lock);
    const auto &queue_set = device_queues_map[device];
    for (const auto &queue : queue_set) {
        FinishWriteObject(queue, "vkDeviceWaitIdle");
    }
}


void ThreadSafety::PreCallRecordCreateInstance(
    const VkInstanceCreateInfo*                 pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkInstance*                                 pInstance) {
    
}

void ThreadSafety::PostCallRecordCreateInstance(
    const VkInstanceCreateInfo*                 pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkInstance*                                 pInstance,
    VkResult                                    result) {
    if (result == VK_SUCCESS) {
        CreateObjectParentInstance(*pInstance);
    }
}

void ThreadSafety::PreCallRecordDestroyInstance(
    VkInstance                                  instance,
    const VkAllocationCallbacks*                pAllocator) {
    StartWriteObjectParentInstance(instance, "vkDestroyInstance");
    // Host access to instance must be externally synchronized
    // all sname:VkPhysicalDevice objects enumerated from pname:instance must be externally synchronized between host accesses
}

void ThreadSafety::PostCallRecordDestroyInstance(
    VkInstance                                  instance,
    const VkAllocationCallbacks*                pAllocator) {
    FinishWriteObjectParentInstance(instance, "vkDestroyInstance");
    DestroyObjectParentInstance(instance);
    // Host access to instance must be externally synchronized
    // all sname:VkPhysicalDevice objects enumerated from pname:instance must be externally synchronized between host accesses
}

void ThreadSafety::PreCallRecordEnumeratePhysicalDevices(
    VkInstance                                  instance,
    uint32_t*                                   pPhysicalDeviceCount,
    VkPhysicalDevice*                           pPhysicalDevices) {
    StartReadObjectParentInstance(instance, "vkEnumeratePhysicalDevices");
}

void ThreadSafety::PostCallRecordEnumeratePhysicalDevices(
    VkInstance                                  instance,
    uint32_t*                                   pPhysicalDeviceCount,
    VkPhysicalDevice*                           pPhysicalDevices,
    VkResult                                    result) {
    FinishReadObjectParentInstance(instance, "vkEnumeratePhysicalDevices");
}

void ThreadSafety::PreCallRecordGetInstanceProcAddr(
    VkInstance                                  instance,
    const char*                                 pName) {
    StartReadObjectParentInstance(instance, "vkGetInstanceProcAddr");
}

void ThreadSafety::PostCallRecordGetInstanceProcAddr(
    VkInstance                                  instance,
    const char*                                 pName) {
    FinishReadObjectParentInstance(instance, "vkGetInstanceProcAddr");
}

void ThreadSafety::PreCallRecordGetDeviceProcAddr(
    VkDevice                                    device,
    const char*                                 pName) {
    StartReadObjectParentInstance(device, "vkGetDeviceProcAddr");
}

void ThreadSafety::PostCallRecordGetDeviceProcAddr(
    VkDevice                                    device,
    const char*                                 pName) {
    FinishReadObjectParentInstance(device, "vkGetDeviceProcAddr");
}

void ThreadSafety::PreCallRecordCreateDevice(
    VkPhysicalDevice                            physicalDevice,
    const VkDeviceCreateInfo*                   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDevice*                                   pDevice) {
    
}

void ThreadSafety::PostCallRecordCreateDevice(
    VkPhysicalDevice                            physicalDevice,
    const VkDeviceCreateInfo*                   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDevice*                                   pDevice,
    VkResult                                    result) {
    if (result == VK_SUCCESS) {
        CreateObjectParentInstance(*pDevice);
    }
}

void ThreadSafety::PreCallRecordQueueSubmit(
    VkQueue                                     queue,
    uint32_t                                    submitCount,
    const VkSubmitInfo*                         pSubmits,
    VkFence                                     fence) {
    StartWriteObject(queue, "vkQueueSubmit");
    StartWriteObject(fence, "vkQueueSubmit");
    // Host access to queue must be externally synchronized
    // Host access to fence must be externally synchronized
}

void ThreadSafety::PostCallRecordQueueSubmit(
    VkQueue                                     queue,
    uint32_t                                    submitCount,
    const VkSubmitInfo*                         pSubmits,
    VkFence                                     fence,
    VkResult                                    result) {
    FinishWriteObject(queue, "vkQueueSubmit");
    FinishWriteObject(fence, "vkQueueSubmit");
    // Host access to queue must be externally synchronized
    // Host access to fence must be externally synchronized
}

void ThreadSafety::PreCallRecordQueueWaitIdle(
    VkQueue                                     queue) {
    StartWriteObject(queue, "vkQueueWaitIdle");
    // Host access to queue must be externally synchronized
}

void ThreadSafety::PostCallRecordQueueWaitIdle(
    VkQueue                                     queue,
    VkResult                                    result) {
    FinishWriteObject(queue, "vkQueueWaitIdle");
    // Host access to queue must be externally synchronized
}

void ThreadSafety::PreCallRecordAllocateMemory(
    VkDevice                                    device,
    const VkMemoryAllocateInfo*                 pAllocateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDeviceMemory*                             pMemory) {
    StartReadObjectParentInstance(device, "vkAllocateMemory");
}

void ThreadSafety::PostCallRecordAllocateMemory(
    VkDevice                                    device,
    const VkMemoryAllocateInfo*                 pAllocateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDeviceMemory*                             pMemory,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkAllocateMemory");
    if (result == VK_SUCCESS) {
        CreateObject(*pMemory);
    }
}

void ThreadSafety::PreCallRecordMapMemory(
    VkDevice                                    device,
    VkDeviceMemory                              memory,
    VkDeviceSize                                offset,
    VkDeviceSize                                size,
    VkMemoryMapFlags                            flags,
    void**                                      ppData) {
    StartReadObjectParentInstance(device, "vkMapMemory");
    StartWriteObject(memory, "vkMapMemory");
    // Host access to memory must be externally synchronized
}

void ThreadSafety::PostCallRecordMapMemory(
    VkDevice                                    device,
    VkDeviceMemory                              memory,
    VkDeviceSize                                offset,
    VkDeviceSize                                size,
    VkMemoryMapFlags                            flags,
    void**                                      ppData,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkMapMemory");
    FinishWriteObject(memory, "vkMapMemory");
    // Host access to memory must be externally synchronized
}

void ThreadSafety::PreCallRecordUnmapMemory(
    VkDevice                                    device,
    VkDeviceMemory                              memory) {
    StartReadObjectParentInstance(device, "vkUnmapMemory");
    StartWriteObject(memory, "vkUnmapMemory");
    // Host access to memory must be externally synchronized
}

void ThreadSafety::PostCallRecordUnmapMemory(
    VkDevice                                    device,
    VkDeviceMemory                              memory) {
    FinishReadObjectParentInstance(device, "vkUnmapMemory");
    FinishWriteObject(memory, "vkUnmapMemory");
    // Host access to memory must be externally synchronized
}

void ThreadSafety::PreCallRecordFlushMappedMemoryRanges(
    VkDevice                                    device,
    uint32_t                                    memoryRangeCount,
    const VkMappedMemoryRange*                  pMemoryRanges) {
    StartReadObjectParentInstance(device, "vkFlushMappedMemoryRanges");
}

void ThreadSafety::PostCallRecordFlushMappedMemoryRanges(
    VkDevice                                    device,
    uint32_t                                    memoryRangeCount,
    const VkMappedMemoryRange*                  pMemoryRanges,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkFlushMappedMemoryRanges");
}

void ThreadSafety::PreCallRecordInvalidateMappedMemoryRanges(
    VkDevice                                    device,
    uint32_t                                    memoryRangeCount,
    const VkMappedMemoryRange*                  pMemoryRanges) {
    StartReadObjectParentInstance(device, "vkInvalidateMappedMemoryRanges");
}

void ThreadSafety::PostCallRecordInvalidateMappedMemoryRanges(
    VkDevice                                    device,
    uint32_t                                    memoryRangeCount,
    const VkMappedMemoryRange*                  pMemoryRanges,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkInvalidateMappedMemoryRanges");
}

void ThreadSafety::PreCallRecordGetDeviceMemoryCommitment(
    VkDevice                                    device,
    VkDeviceMemory                              memory,
    VkDeviceSize*                               pCommittedMemoryInBytes) {
    StartReadObjectParentInstance(device, "vkGetDeviceMemoryCommitment");
    StartReadObject(memory, "vkGetDeviceMemoryCommitment");
}

void ThreadSafety::PostCallRecordGetDeviceMemoryCommitment(
    VkDevice                                    device,
    VkDeviceMemory                              memory,
    VkDeviceSize*                               pCommittedMemoryInBytes) {
    FinishReadObjectParentInstance(device, "vkGetDeviceMemoryCommitment");
    FinishReadObject(memory, "vkGetDeviceMemoryCommitment");
}

void ThreadSafety::PreCallRecordBindBufferMemory(
    VkDevice                                    device,
    VkBuffer                                    buffer,
    VkDeviceMemory                              memory,
    VkDeviceSize                                memoryOffset) {
    StartReadObjectParentInstance(device, "vkBindBufferMemory");
    StartWriteObject(buffer, "vkBindBufferMemory");
    StartReadObject(memory, "vkBindBufferMemory");
    // Host access to buffer must be externally synchronized
}

void ThreadSafety::PostCallRecordBindBufferMemory(
    VkDevice                                    device,
    VkBuffer                                    buffer,
    VkDeviceMemory                              memory,
    VkDeviceSize                                memoryOffset,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkBindBufferMemory");
    FinishWriteObject(buffer, "vkBindBufferMemory");
    FinishReadObject(memory, "vkBindBufferMemory");
    // Host access to buffer must be externally synchronized
}

void ThreadSafety::PreCallRecordBindImageMemory(
    VkDevice                                    device,
    VkImage                                     image,
    VkDeviceMemory                              memory,
    VkDeviceSize                                memoryOffset) {
    StartReadObjectParentInstance(device, "vkBindImageMemory");
    StartWriteObject(image, "vkBindImageMemory");
    StartReadObject(memory, "vkBindImageMemory");
    // Host access to image must be externally synchronized
}

void ThreadSafety::PostCallRecordBindImageMemory(
    VkDevice                                    device,
    VkImage                                     image,
    VkDeviceMemory                              memory,
    VkDeviceSize                                memoryOffset,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkBindImageMemory");
    FinishWriteObject(image, "vkBindImageMemory");
    FinishReadObject(memory, "vkBindImageMemory");
    // Host access to image must be externally synchronized
}

void ThreadSafety::PreCallRecordGetBufferMemoryRequirements(
    VkDevice                                    device,
    VkBuffer                                    buffer,
    VkMemoryRequirements*                       pMemoryRequirements) {
    StartReadObjectParentInstance(device, "vkGetBufferMemoryRequirements");
    StartReadObject(buffer, "vkGetBufferMemoryRequirements");
}

void ThreadSafety::PostCallRecordGetBufferMemoryRequirements(
    VkDevice                                    device,
    VkBuffer                                    buffer,
    VkMemoryRequirements*                       pMemoryRequirements) {
    FinishReadObjectParentInstance(device, "vkGetBufferMemoryRequirements");
    FinishReadObject(buffer, "vkGetBufferMemoryRequirements");
}

void ThreadSafety::PreCallRecordGetImageMemoryRequirements(
    VkDevice                                    device,
    VkImage                                     image,
    VkMemoryRequirements*                       pMemoryRequirements) {
    StartReadObjectParentInstance(device, "vkGetImageMemoryRequirements");
    StartReadObject(image, "vkGetImageMemoryRequirements");
}

void ThreadSafety::PostCallRecordGetImageMemoryRequirements(
    VkDevice                                    device,
    VkImage                                     image,
    VkMemoryRequirements*                       pMemoryRequirements) {
    FinishReadObjectParentInstance(device, "vkGetImageMemoryRequirements");
    FinishReadObject(image, "vkGetImageMemoryRequirements");
}

void ThreadSafety::PreCallRecordCreateFence(
    VkDevice                                    device,
    const VkFenceCreateInfo*                    pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFence*                                    pFence) {
    StartReadObjectParentInstance(device, "vkCreateFence");
}

void ThreadSafety::PostCallRecordCreateFence(
    VkDevice                                    device,
    const VkFenceCreateInfo*                    pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFence*                                    pFence,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateFence");
    if (result == VK_SUCCESS) {
        CreateObject(*pFence);
    }
}

void ThreadSafety::PreCallRecordDestroyFence(
    VkDevice                                    device,
    VkFence                                     fence,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroyFence");
    StartWriteObject(fence, "vkDestroyFence");
    // Host access to fence must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroyFence(
    VkDevice                                    device,
    VkFence                                     fence,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroyFence");
    FinishWriteObject(fence, "vkDestroyFence");
    DestroyObject(fence);
    // Host access to fence must be externally synchronized
}

void ThreadSafety::PreCallRecordResetFences(
    VkDevice                                    device,
    uint32_t                                    fenceCount,
    const VkFence*                              pFences) {
    StartReadObjectParentInstance(device, "vkResetFences");
    if (pFences) {
        for (uint32_t index=0; index < fenceCount; index++) {
            StartWriteObject(pFences[index], "vkResetFences");
        }
    }
    // Host access to each member of pFences must be externally synchronized
}

void ThreadSafety::PostCallRecordResetFences(
    VkDevice                                    device,
    uint32_t                                    fenceCount,
    const VkFence*                              pFences,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkResetFences");
    if (pFences) {
        for (uint32_t index=0; index < fenceCount; index++) {
            FinishWriteObject(pFences[index], "vkResetFences");
        }
    }
    // Host access to each member of pFences must be externally synchronized
}

void ThreadSafety::PreCallRecordGetFenceStatus(
    VkDevice                                    device,
    VkFence                                     fence) {
    StartReadObjectParentInstance(device, "vkGetFenceStatus");
    StartReadObject(fence, "vkGetFenceStatus");
}

void ThreadSafety::PostCallRecordGetFenceStatus(
    VkDevice                                    device,
    VkFence                                     fence,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkGetFenceStatus");
    FinishReadObject(fence, "vkGetFenceStatus");
}

void ThreadSafety::PreCallRecordWaitForFences(
    VkDevice                                    device,
    uint32_t                                    fenceCount,
    const VkFence*                              pFences,
    VkBool32                                    waitAll,
    uint64_t                                    timeout) {
    StartReadObjectParentInstance(device, "vkWaitForFences");
    if (pFences) {
        for (uint32_t index = 0; index < fenceCount; index++) {
            StartReadObject(pFences[index], "vkWaitForFences");
        }
    }
}

void ThreadSafety::PostCallRecordWaitForFences(
    VkDevice                                    device,
    uint32_t                                    fenceCount,
    const VkFence*                              pFences,
    VkBool32                                    waitAll,
    uint64_t                                    timeout,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkWaitForFences");
    if (pFences) {
        for (uint32_t index = 0; index < fenceCount; index++) {
            FinishReadObject(pFences[index], "vkWaitForFences");
        }
    }
}

void ThreadSafety::PreCallRecordCreateSemaphore(
    VkDevice                                    device,
    const VkSemaphoreCreateInfo*                pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSemaphore*                                pSemaphore) {
    StartReadObjectParentInstance(device, "vkCreateSemaphore");
}

void ThreadSafety::PostCallRecordCreateSemaphore(
    VkDevice                                    device,
    const VkSemaphoreCreateInfo*                pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSemaphore*                                pSemaphore,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateSemaphore");
    if (result == VK_SUCCESS) {
        CreateObject(*pSemaphore);
    }
}

void ThreadSafety::PreCallRecordDestroySemaphore(
    VkDevice                                    device,
    VkSemaphore                                 semaphore,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroySemaphore");
    StartWriteObject(semaphore, "vkDestroySemaphore");
    // Host access to semaphore must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroySemaphore(
    VkDevice                                    device,
    VkSemaphore                                 semaphore,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroySemaphore");
    FinishWriteObject(semaphore, "vkDestroySemaphore");
    DestroyObject(semaphore);
    // Host access to semaphore must be externally synchronized
}

void ThreadSafety::PreCallRecordCreateEvent(
    VkDevice                                    device,
    const VkEventCreateInfo*                    pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkEvent*                                    pEvent) {
    StartReadObjectParentInstance(device, "vkCreateEvent");
}

void ThreadSafety::PostCallRecordCreateEvent(
    VkDevice                                    device,
    const VkEventCreateInfo*                    pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkEvent*                                    pEvent,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateEvent");
    if (result == VK_SUCCESS) {
        CreateObject(*pEvent);
    }
}

void ThreadSafety::PreCallRecordDestroyEvent(
    VkDevice                                    device,
    VkEvent                                     event,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroyEvent");
    StartWriteObject(event, "vkDestroyEvent");
    // Host access to event must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroyEvent(
    VkDevice                                    device,
    VkEvent                                     event,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroyEvent");
    FinishWriteObject(event, "vkDestroyEvent");
    DestroyObject(event);
    // Host access to event must be externally synchronized
}

void ThreadSafety::PreCallRecordGetEventStatus(
    VkDevice                                    device,
    VkEvent                                     event) {
    StartReadObjectParentInstance(device, "vkGetEventStatus");
    StartReadObject(event, "vkGetEventStatus");
}

void ThreadSafety::PostCallRecordGetEventStatus(
    VkDevice                                    device,
    VkEvent                                     event,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkGetEventStatus");
    FinishReadObject(event, "vkGetEventStatus");
}

void ThreadSafety::PreCallRecordSetEvent(
    VkDevice                                    device,
    VkEvent                                     event) {
    StartReadObjectParentInstance(device, "vkSetEvent");
    StartWriteObject(event, "vkSetEvent");
    // Host access to event must be externally synchronized
}

void ThreadSafety::PostCallRecordSetEvent(
    VkDevice                                    device,
    VkEvent                                     event,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkSetEvent");
    FinishWriteObject(event, "vkSetEvent");
    // Host access to event must be externally synchronized
}

void ThreadSafety::PreCallRecordResetEvent(
    VkDevice                                    device,
    VkEvent                                     event) {
    StartReadObjectParentInstance(device, "vkResetEvent");
    StartWriteObject(event, "vkResetEvent");
    // Host access to event must be externally synchronized
}

void ThreadSafety::PostCallRecordResetEvent(
    VkDevice                                    device,
    VkEvent                                     event,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkResetEvent");
    FinishWriteObject(event, "vkResetEvent");
    // Host access to event must be externally synchronized
}

void ThreadSafety::PreCallRecordCreateQueryPool(
    VkDevice                                    device,
    const VkQueryPoolCreateInfo*                pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkQueryPool*                                pQueryPool) {
    StartReadObjectParentInstance(device, "vkCreateQueryPool");
}

void ThreadSafety::PostCallRecordCreateQueryPool(
    VkDevice                                    device,
    const VkQueryPoolCreateInfo*                pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkQueryPool*                                pQueryPool,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateQueryPool");
    if (result == VK_SUCCESS) {
        CreateObject(*pQueryPool);
    }
}

void ThreadSafety::PreCallRecordGetQueryPoolResults(
    VkDevice                                    device,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount,
    size_t                                      dataSize,
    void*                                       pData,
    VkDeviceSize                                stride,
    VkQueryResultFlags                          flags) {
    StartReadObjectParentInstance(device, "vkGetQueryPoolResults");
    StartReadObject(queryPool, "vkGetQueryPoolResults");
}

void ThreadSafety::PostCallRecordGetQueryPoolResults(
    VkDevice                                    device,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount,
    size_t                                      dataSize,
    void*                                       pData,
    VkDeviceSize                                stride,
    VkQueryResultFlags                          flags,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkGetQueryPoolResults");
    FinishReadObject(queryPool, "vkGetQueryPoolResults");
}

void ThreadSafety::PreCallRecordCreateBuffer(
    VkDevice                                    device,
    const VkBufferCreateInfo*                   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkBuffer*                                   pBuffer) {
    StartReadObjectParentInstance(device, "vkCreateBuffer");
}

void ThreadSafety::PostCallRecordCreateBuffer(
    VkDevice                                    device,
    const VkBufferCreateInfo*                   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkBuffer*                                   pBuffer,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateBuffer");
    if (result == VK_SUCCESS) {
        CreateObject(*pBuffer);
    }
}

void ThreadSafety::PreCallRecordDestroyBuffer(
    VkDevice                                    device,
    VkBuffer                                    buffer,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroyBuffer");
    StartWriteObject(buffer, "vkDestroyBuffer");
    // Host access to buffer must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroyBuffer(
    VkDevice                                    device,
    VkBuffer                                    buffer,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroyBuffer");
    FinishWriteObject(buffer, "vkDestroyBuffer");
    DestroyObject(buffer);
    // Host access to buffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCreateBufferView(
    VkDevice                                    device,
    const VkBufferViewCreateInfo*               pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkBufferView*                               pView) {
    StartReadObjectParentInstance(device, "vkCreateBufferView");
}

void ThreadSafety::PostCallRecordCreateBufferView(
    VkDevice                                    device,
    const VkBufferViewCreateInfo*               pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkBufferView*                               pView,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateBufferView");
    if (result == VK_SUCCESS) {
        CreateObject(*pView);
    }
}

void ThreadSafety::PreCallRecordDestroyBufferView(
    VkDevice                                    device,
    VkBufferView                                bufferView,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroyBufferView");
    StartWriteObject(bufferView, "vkDestroyBufferView");
    // Host access to bufferView must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroyBufferView(
    VkDevice                                    device,
    VkBufferView                                bufferView,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroyBufferView");
    FinishWriteObject(bufferView, "vkDestroyBufferView");
    DestroyObject(bufferView);
    // Host access to bufferView must be externally synchronized
}

void ThreadSafety::PreCallRecordCreateImage(
    VkDevice                                    device,
    const VkImageCreateInfo*                    pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkImage*                                    pImage) {
    StartReadObjectParentInstance(device, "vkCreateImage");
}

void ThreadSafety::PostCallRecordCreateImage(
    VkDevice                                    device,
    const VkImageCreateInfo*                    pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkImage*                                    pImage,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateImage");
    if (result == VK_SUCCESS) {
        CreateObject(*pImage);
    }
}

void ThreadSafety::PreCallRecordDestroyImage(
    VkDevice                                    device,
    VkImage                                     image,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroyImage");
    StartWriteObject(image, "vkDestroyImage");
    // Host access to image must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroyImage(
    VkDevice                                    device,
    VkImage                                     image,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroyImage");
    FinishWriteObject(image, "vkDestroyImage");
    DestroyObject(image);
    // Host access to image must be externally synchronized
}

void ThreadSafety::PreCallRecordGetImageSubresourceLayout(
    VkDevice                                    device,
    VkImage                                     image,
    const VkImageSubresource*                   pSubresource,
    VkSubresourceLayout*                        pLayout) {
    StartReadObjectParentInstance(device, "vkGetImageSubresourceLayout");
    StartReadObject(image, "vkGetImageSubresourceLayout");
}

void ThreadSafety::PostCallRecordGetImageSubresourceLayout(
    VkDevice                                    device,
    VkImage                                     image,
    const VkImageSubresource*                   pSubresource,
    VkSubresourceLayout*                        pLayout) {
    FinishReadObjectParentInstance(device, "vkGetImageSubresourceLayout");
    FinishReadObject(image, "vkGetImageSubresourceLayout");
}

void ThreadSafety::PreCallRecordCreateImageView(
    VkDevice                                    device,
    const VkImageViewCreateInfo*                pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkImageView*                                pView) {
    StartReadObjectParentInstance(device, "vkCreateImageView");
}

void ThreadSafety::PostCallRecordCreateImageView(
    VkDevice                                    device,
    const VkImageViewCreateInfo*                pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkImageView*                                pView,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateImageView");
    if (result == VK_SUCCESS) {
        CreateObject(*pView);
    }
}

void ThreadSafety::PreCallRecordDestroyImageView(
    VkDevice                                    device,
    VkImageView                                 imageView,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroyImageView");
    StartWriteObject(imageView, "vkDestroyImageView");
    // Host access to imageView must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroyImageView(
    VkDevice                                    device,
    VkImageView                                 imageView,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroyImageView");
    FinishWriteObject(imageView, "vkDestroyImageView");
    DestroyObject(imageView);
    // Host access to imageView must be externally synchronized
}

void ThreadSafety::PreCallRecordCreatePipelineCache(
    VkDevice                                    device,
    const VkPipelineCacheCreateInfo*            pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkPipelineCache*                            pPipelineCache) {
    StartReadObjectParentInstance(device, "vkCreatePipelineCache");
}

void ThreadSafety::PostCallRecordCreatePipelineCache(
    VkDevice                                    device,
    const VkPipelineCacheCreateInfo*            pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkPipelineCache*                            pPipelineCache,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreatePipelineCache");
    if (result == VK_SUCCESS) {
        CreateObject(*pPipelineCache);
    }
}

void ThreadSafety::PreCallRecordDestroyPipelineCache(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroyPipelineCache");
    StartWriteObject(pipelineCache, "vkDestroyPipelineCache");
    // Host access to pipelineCache must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroyPipelineCache(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroyPipelineCache");
    FinishWriteObject(pipelineCache, "vkDestroyPipelineCache");
    DestroyObject(pipelineCache);
    // Host access to pipelineCache must be externally synchronized
}

void ThreadSafety::PreCallRecordCreateGraphicsPipelines(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    uint32_t                                    createInfoCount,
    const VkGraphicsPipelineCreateInfo*         pCreateInfos,
    const VkAllocationCallbacks*                pAllocator,
    VkPipeline*                                 pPipelines) {
    StartReadObjectParentInstance(device, "vkCreateGraphicsPipelines");
    StartReadObject(pipelineCache, "vkCreateGraphicsPipelines");
}

void ThreadSafety::PostCallRecordCreateGraphicsPipelines(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    uint32_t                                    createInfoCount,
    const VkGraphicsPipelineCreateInfo*         pCreateInfos,
    const VkAllocationCallbacks*                pAllocator,
    VkPipeline*                                 pPipelines,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateGraphicsPipelines");
    FinishReadObject(pipelineCache, "vkCreateGraphicsPipelines");
    if (pPipelines) {
        for (uint32_t index = 0; index < createInfoCount; index++) {
            if (!pPipelines[index]) continue;
            CreateObject(pPipelines[index]);
        }
    }
}

void ThreadSafety::PreCallRecordCreateComputePipelines(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    uint32_t                                    createInfoCount,
    const VkComputePipelineCreateInfo*          pCreateInfos,
    const VkAllocationCallbacks*                pAllocator,
    VkPipeline*                                 pPipelines) {
    StartReadObjectParentInstance(device, "vkCreateComputePipelines");
    StartReadObject(pipelineCache, "vkCreateComputePipelines");
}

void ThreadSafety::PostCallRecordCreateComputePipelines(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    uint32_t                                    createInfoCount,
    const VkComputePipelineCreateInfo*          pCreateInfos,
    const VkAllocationCallbacks*                pAllocator,
    VkPipeline*                                 pPipelines,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateComputePipelines");
    FinishReadObject(pipelineCache, "vkCreateComputePipelines");
    if (pPipelines) {
        for (uint32_t index = 0; index < createInfoCount; index++) {
            if (!pPipelines[index]) continue;
            CreateObject(pPipelines[index]);
        }
    }
}

void ThreadSafety::PreCallRecordDestroyPipeline(
    VkDevice                                    device,
    VkPipeline                                  pipeline,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroyPipeline");
    StartWriteObject(pipeline, "vkDestroyPipeline");
    // Host access to pipeline must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroyPipeline(
    VkDevice                                    device,
    VkPipeline                                  pipeline,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroyPipeline");
    FinishWriteObject(pipeline, "vkDestroyPipeline");
    DestroyObject(pipeline);
    // Host access to pipeline must be externally synchronized
}

void ThreadSafety::PreCallRecordCreatePipelineLayout(
    VkDevice                                    device,
    const VkPipelineLayoutCreateInfo*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkPipelineLayout*                           pPipelineLayout) {
    StartReadObjectParentInstance(device, "vkCreatePipelineLayout");
}

void ThreadSafety::PostCallRecordCreatePipelineLayout(
    VkDevice                                    device,
    const VkPipelineLayoutCreateInfo*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkPipelineLayout*                           pPipelineLayout,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreatePipelineLayout");
    if (result == VK_SUCCESS) {
        CreateObject(*pPipelineLayout);
    }
}

void ThreadSafety::PreCallRecordDestroyPipelineLayout(
    VkDevice                                    device,
    VkPipelineLayout                            pipelineLayout,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroyPipelineLayout");
    StartWriteObject(pipelineLayout, "vkDestroyPipelineLayout");
    // Host access to pipelineLayout must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroyPipelineLayout(
    VkDevice                                    device,
    VkPipelineLayout                            pipelineLayout,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroyPipelineLayout");
    FinishWriteObject(pipelineLayout, "vkDestroyPipelineLayout");
    DestroyObject(pipelineLayout);
    // Host access to pipelineLayout must be externally synchronized
}

void ThreadSafety::PreCallRecordCreateSampler(
    VkDevice                                    device,
    const VkSamplerCreateInfo*                  pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSampler*                                  pSampler) {
    StartReadObjectParentInstance(device, "vkCreateSampler");
}

void ThreadSafety::PostCallRecordCreateSampler(
    VkDevice                                    device,
    const VkSamplerCreateInfo*                  pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSampler*                                  pSampler,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateSampler");
    if (result == VK_SUCCESS) {
        CreateObject(*pSampler);
    }
}

void ThreadSafety::PreCallRecordDestroySampler(
    VkDevice                                    device,
    VkSampler                                   sampler,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroySampler");
    StartWriteObject(sampler, "vkDestroySampler");
    // Host access to sampler must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroySampler(
    VkDevice                                    device,
    VkSampler                                   sampler,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroySampler");
    FinishWriteObject(sampler, "vkDestroySampler");
    DestroyObject(sampler);
    // Host access to sampler must be externally synchronized
}

void ThreadSafety::PreCallRecordDestroyDescriptorSetLayout(
    VkDevice                                    device,
    VkDescriptorSetLayout                       descriptorSetLayout,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroyDescriptorSetLayout");
    StartWriteObject(descriptorSetLayout, "vkDestroyDescriptorSetLayout");
    // Host access to descriptorSetLayout must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroyDescriptorSetLayout(
    VkDevice                                    device,
    VkDescriptorSetLayout                       descriptorSetLayout,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroyDescriptorSetLayout");
    FinishWriteObject(descriptorSetLayout, "vkDestroyDescriptorSetLayout");
    DestroyObject(descriptorSetLayout);
    // Host access to descriptorSetLayout must be externally synchronized
}

void ThreadSafety::PreCallRecordCreateDescriptorPool(
    VkDevice                                    device,
    const VkDescriptorPoolCreateInfo*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDescriptorPool*                           pDescriptorPool) {
    StartReadObjectParentInstance(device, "vkCreateDescriptorPool");
}

void ThreadSafety::PostCallRecordCreateDescriptorPool(
    VkDevice                                    device,
    const VkDescriptorPoolCreateInfo*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDescriptorPool*                           pDescriptorPool,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateDescriptorPool");
    if (result == VK_SUCCESS) {
        CreateObject(*pDescriptorPool);
    }
}

void ThreadSafety::PreCallRecordCreateFramebuffer(
    VkDevice                                    device,
    const VkFramebufferCreateInfo*              pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFramebuffer*                              pFramebuffer) {
    StartReadObjectParentInstance(device, "vkCreateFramebuffer");
}

void ThreadSafety::PostCallRecordCreateFramebuffer(
    VkDevice                                    device,
    const VkFramebufferCreateInfo*              pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFramebuffer*                              pFramebuffer,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateFramebuffer");
    if (result == VK_SUCCESS) {
        CreateObject(*pFramebuffer);
    }
}

void ThreadSafety::PreCallRecordDestroyFramebuffer(
    VkDevice                                    device,
    VkFramebuffer                               framebuffer,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroyFramebuffer");
    StartWriteObject(framebuffer, "vkDestroyFramebuffer");
    // Host access to framebuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroyFramebuffer(
    VkDevice                                    device,
    VkFramebuffer                               framebuffer,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroyFramebuffer");
    FinishWriteObject(framebuffer, "vkDestroyFramebuffer");
    DestroyObject(framebuffer);
    // Host access to framebuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCreateRenderPass(
    VkDevice                                    device,
    const VkRenderPassCreateInfo*               pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkRenderPass*                               pRenderPass) {
    StartReadObjectParentInstance(device, "vkCreateRenderPass");
}

void ThreadSafety::PostCallRecordCreateRenderPass(
    VkDevice                                    device,
    const VkRenderPassCreateInfo*               pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkRenderPass*                               pRenderPass,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateRenderPass");
    if (result == VK_SUCCESS) {
        CreateObject(*pRenderPass);
    }
}

void ThreadSafety::PreCallRecordDestroyRenderPass(
    VkDevice                                    device,
    VkRenderPass                                renderPass,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroyRenderPass");
    StartWriteObject(renderPass, "vkDestroyRenderPass");
    // Host access to renderPass must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroyRenderPass(
    VkDevice                                    device,
    VkRenderPass                                renderPass,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroyRenderPass");
    FinishWriteObject(renderPass, "vkDestroyRenderPass");
    DestroyObject(renderPass);
    // Host access to renderPass must be externally synchronized
}

void ThreadSafety::PreCallRecordGetRenderAreaGranularity(
    VkDevice                                    device,
    VkRenderPass                                renderPass,
    VkExtent2D*                                 pGranularity) {
    StartReadObjectParentInstance(device, "vkGetRenderAreaGranularity");
    StartReadObject(renderPass, "vkGetRenderAreaGranularity");
}

void ThreadSafety::PostCallRecordGetRenderAreaGranularity(
    VkDevice                                    device,
    VkRenderPass                                renderPass,
    VkExtent2D*                                 pGranularity) {
    FinishReadObjectParentInstance(device, "vkGetRenderAreaGranularity");
    FinishReadObject(renderPass, "vkGetRenderAreaGranularity");
}

void ThreadSafety::PreCallRecordBeginCommandBuffer(
    VkCommandBuffer                             commandBuffer,
    const VkCommandBufferBeginInfo*             pBeginInfo) {
    StartWriteObject(commandBuffer, "vkBeginCommandBuffer");
    // Host access to commandBuffer must be externally synchronized
    // the sname:VkCommandPool that pname:commandBuffer was allocated from must be externally synchronized between host accesses
}

void ThreadSafety::PostCallRecordBeginCommandBuffer(
    VkCommandBuffer                             commandBuffer,
    const VkCommandBufferBeginInfo*             pBeginInfo,
    VkResult                                    result) {
    FinishWriteObject(commandBuffer, "vkBeginCommandBuffer");
    // Host access to commandBuffer must be externally synchronized
    // the sname:VkCommandPool that pname:commandBuffer was allocated from must be externally synchronized between host accesses
}

void ThreadSafety::PreCallRecordEndCommandBuffer(
    VkCommandBuffer                             commandBuffer) {
    StartWriteObject(commandBuffer, "vkEndCommandBuffer");
    // Host access to commandBuffer must be externally synchronized
    // the sname:VkCommandPool that pname:commandBuffer was allocated from must be externally synchronized between host accesses
}

void ThreadSafety::PostCallRecordEndCommandBuffer(
    VkCommandBuffer                             commandBuffer,
    VkResult                                    result) {
    FinishWriteObject(commandBuffer, "vkEndCommandBuffer");
    // Host access to commandBuffer must be externally synchronized
    // the sname:VkCommandPool that pname:commandBuffer was allocated from must be externally synchronized between host accesses
}

void ThreadSafety::PreCallRecordResetCommandBuffer(
    VkCommandBuffer                             commandBuffer,
    VkCommandBufferResetFlags                   flags) {
    StartWriteObject(commandBuffer, "vkResetCommandBuffer");
    // Host access to commandBuffer must be externally synchronized
    // the sname:VkCommandPool that pname:commandBuffer was allocated from must be externally synchronized between host accesses
}

void ThreadSafety::PostCallRecordResetCommandBuffer(
    VkCommandBuffer                             commandBuffer,
    VkCommandBufferResetFlags                   flags,
    VkResult                                    result) {
    FinishWriteObject(commandBuffer, "vkResetCommandBuffer");
    // Host access to commandBuffer must be externally synchronized
    // the sname:VkCommandPool that pname:commandBuffer was allocated from must be externally synchronized between host accesses
}

void ThreadSafety::PreCallRecordCmdBindPipeline(
    VkCommandBuffer                             commandBuffer,
    VkPipelineBindPoint                         pipelineBindPoint,
    VkPipeline                                  pipeline) {
    StartWriteObject(commandBuffer, "vkCmdBindPipeline");
    StartReadObject(pipeline, "vkCmdBindPipeline");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdBindPipeline(
    VkCommandBuffer                             commandBuffer,
    VkPipelineBindPoint                         pipelineBindPoint,
    VkPipeline                                  pipeline) {
    FinishWriteObject(commandBuffer, "vkCmdBindPipeline");
    FinishReadObject(pipeline, "vkCmdBindPipeline");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetViewport(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstViewport,
    uint32_t                                    viewportCount,
    const VkViewport*                           pViewports) {
    StartWriteObject(commandBuffer, "vkCmdSetViewport");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetViewport(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstViewport,
    uint32_t                                    viewportCount,
    const VkViewport*                           pViewports) {
    FinishWriteObject(commandBuffer, "vkCmdSetViewport");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetScissor(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstScissor,
    uint32_t                                    scissorCount,
    const VkRect2D*                             pScissors) {
    StartWriteObject(commandBuffer, "vkCmdSetScissor");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetScissor(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstScissor,
    uint32_t                                    scissorCount,
    const VkRect2D*                             pScissors) {
    FinishWriteObject(commandBuffer, "vkCmdSetScissor");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetLineWidth(
    VkCommandBuffer                             commandBuffer,
    float                                       lineWidth) {
    StartWriteObject(commandBuffer, "vkCmdSetLineWidth");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetLineWidth(
    VkCommandBuffer                             commandBuffer,
    float                                       lineWidth) {
    FinishWriteObject(commandBuffer, "vkCmdSetLineWidth");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetDepthBias(
    VkCommandBuffer                             commandBuffer,
    float                                       depthBiasConstantFactor,
    float                                       depthBiasClamp,
    float                                       depthBiasSlopeFactor) {
    StartWriteObject(commandBuffer, "vkCmdSetDepthBias");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetDepthBias(
    VkCommandBuffer                             commandBuffer,
    float                                       depthBiasConstantFactor,
    float                                       depthBiasClamp,
    float                                       depthBiasSlopeFactor) {
    FinishWriteObject(commandBuffer, "vkCmdSetDepthBias");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetBlendConstants(
    VkCommandBuffer                             commandBuffer,
    const float                                 blendConstants[4]) {
    StartWriteObject(commandBuffer, "vkCmdSetBlendConstants");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetBlendConstants(
    VkCommandBuffer                             commandBuffer,
    const float                                 blendConstants[4]) {
    FinishWriteObject(commandBuffer, "vkCmdSetBlendConstants");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetDepthBounds(
    VkCommandBuffer                             commandBuffer,
    float                                       minDepthBounds,
    float                                       maxDepthBounds) {
    StartWriteObject(commandBuffer, "vkCmdSetDepthBounds");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetDepthBounds(
    VkCommandBuffer                             commandBuffer,
    float                                       minDepthBounds,
    float                                       maxDepthBounds) {
    FinishWriteObject(commandBuffer, "vkCmdSetDepthBounds");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetStencilCompareMask(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    compareMask) {
    StartWriteObject(commandBuffer, "vkCmdSetStencilCompareMask");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetStencilCompareMask(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    compareMask) {
    FinishWriteObject(commandBuffer, "vkCmdSetStencilCompareMask");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetStencilWriteMask(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    writeMask) {
    StartWriteObject(commandBuffer, "vkCmdSetStencilWriteMask");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetStencilWriteMask(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    writeMask) {
    FinishWriteObject(commandBuffer, "vkCmdSetStencilWriteMask");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetStencilReference(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    reference) {
    StartWriteObject(commandBuffer, "vkCmdSetStencilReference");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetStencilReference(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    reference) {
    FinishWriteObject(commandBuffer, "vkCmdSetStencilReference");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdBindDescriptorSets(
    VkCommandBuffer                             commandBuffer,
    VkPipelineBindPoint                         pipelineBindPoint,
    VkPipelineLayout                            layout,
    uint32_t                                    firstSet,
    uint32_t                                    descriptorSetCount,
    const VkDescriptorSet*                      pDescriptorSets,
    uint32_t                                    dynamicOffsetCount,
    const uint32_t*                             pDynamicOffsets) {
    StartWriteObject(commandBuffer, "vkCmdBindDescriptorSets");
    StartReadObject(layout, "vkCmdBindDescriptorSets");
    if (pDescriptorSets) {
        for (uint32_t index = 0; index < descriptorSetCount; index++) {
            StartReadObject(pDescriptorSets[index], "vkCmdBindDescriptorSets");
        }
    }
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdBindDescriptorSets(
    VkCommandBuffer                             commandBuffer,
    VkPipelineBindPoint                         pipelineBindPoint,
    VkPipelineLayout                            layout,
    uint32_t                                    firstSet,
    uint32_t                                    descriptorSetCount,
    const VkDescriptorSet*                      pDescriptorSets,
    uint32_t                                    dynamicOffsetCount,
    const uint32_t*                             pDynamicOffsets) {
    FinishWriteObject(commandBuffer, "vkCmdBindDescriptorSets");
    FinishReadObject(layout, "vkCmdBindDescriptorSets");
    if (pDescriptorSets) {
        for (uint32_t index = 0; index < descriptorSetCount; index++) {
            FinishReadObject(pDescriptorSets[index], "vkCmdBindDescriptorSets");
        }
    }
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdBindIndexBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkIndexType                                 indexType) {
    StartWriteObject(commandBuffer, "vkCmdBindIndexBuffer");
    StartReadObject(buffer, "vkCmdBindIndexBuffer");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdBindIndexBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkIndexType                                 indexType) {
    FinishWriteObject(commandBuffer, "vkCmdBindIndexBuffer");
    FinishReadObject(buffer, "vkCmdBindIndexBuffer");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdBindVertexBuffers(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstBinding,
    uint32_t                                    bindingCount,
    const VkBuffer*                             pBuffers,
    const VkDeviceSize*                         pOffsets) {
    StartWriteObject(commandBuffer, "vkCmdBindVertexBuffers");
    if (pBuffers) {
        for (uint32_t index = 0; index < bindingCount; index++) {
            StartReadObject(pBuffers[index], "vkCmdBindVertexBuffers");
        }
    }
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdBindVertexBuffers(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstBinding,
    uint32_t                                    bindingCount,
    const VkBuffer*                             pBuffers,
    const VkDeviceSize*                         pOffsets) {
    FinishWriteObject(commandBuffer, "vkCmdBindVertexBuffers");
    if (pBuffers) {
        for (uint32_t index = 0; index < bindingCount; index++) {
            FinishReadObject(pBuffers[index], "vkCmdBindVertexBuffers");
        }
    }
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdDraw(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    vertexCount,
    uint32_t                                    instanceCount,
    uint32_t                                    firstVertex,
    uint32_t                                    firstInstance) {
    StartWriteObject(commandBuffer, "vkCmdDraw");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdDraw(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    vertexCount,
    uint32_t                                    instanceCount,
    uint32_t                                    firstVertex,
    uint32_t                                    firstInstance) {
    FinishWriteObject(commandBuffer, "vkCmdDraw");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdDrawIndexed(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    indexCount,
    uint32_t                                    instanceCount,
    uint32_t                                    firstIndex,
    int32_t                                     vertexOffset,
    uint32_t                                    firstInstance) {
    StartWriteObject(commandBuffer, "vkCmdDrawIndexed");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdDrawIndexed(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    indexCount,
    uint32_t                                    instanceCount,
    uint32_t                                    firstIndex,
    int32_t                                     vertexOffset,
    uint32_t                                    firstInstance) {
    FinishWriteObject(commandBuffer, "vkCmdDrawIndexed");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdDrawIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    uint32_t                                    drawCount,
    uint32_t                                    stride) {
    StartWriteObject(commandBuffer, "vkCmdDrawIndirect");
    StartReadObject(buffer, "vkCmdDrawIndirect");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdDrawIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    uint32_t                                    drawCount,
    uint32_t                                    stride) {
    FinishWriteObject(commandBuffer, "vkCmdDrawIndirect");
    FinishReadObject(buffer, "vkCmdDrawIndirect");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdDrawIndexedIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    uint32_t                                    drawCount,
    uint32_t                                    stride) {
    StartWriteObject(commandBuffer, "vkCmdDrawIndexedIndirect");
    StartReadObject(buffer, "vkCmdDrawIndexedIndirect");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdDrawIndexedIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    uint32_t                                    drawCount,
    uint32_t                                    stride) {
    FinishWriteObject(commandBuffer, "vkCmdDrawIndexedIndirect");
    FinishReadObject(buffer, "vkCmdDrawIndexedIndirect");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdDispatch(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    groupCountX,
    uint32_t                                    groupCountY,
    uint32_t                                    groupCountZ) {
    StartWriteObject(commandBuffer, "vkCmdDispatch");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdDispatch(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    groupCountX,
    uint32_t                                    groupCountY,
    uint32_t                                    groupCountZ) {
    FinishWriteObject(commandBuffer, "vkCmdDispatch");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdDispatchIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset) {
    StartWriteObject(commandBuffer, "vkCmdDispatchIndirect");
    StartReadObject(buffer, "vkCmdDispatchIndirect");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdDispatchIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset) {
    FinishWriteObject(commandBuffer, "vkCmdDispatchIndirect");
    FinishReadObject(buffer, "vkCmdDispatchIndirect");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdCopyBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    srcBuffer,
    VkBuffer                                    dstBuffer,
    uint32_t                                    regionCount,
    const VkBufferCopy*                         pRegions) {
    StartWriteObject(commandBuffer, "vkCmdCopyBuffer");
    StartReadObject(srcBuffer, "vkCmdCopyBuffer");
    StartReadObject(dstBuffer, "vkCmdCopyBuffer");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdCopyBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    srcBuffer,
    VkBuffer                                    dstBuffer,
    uint32_t                                    regionCount,
    const VkBufferCopy*                         pRegions) {
    FinishWriteObject(commandBuffer, "vkCmdCopyBuffer");
    FinishReadObject(srcBuffer, "vkCmdCopyBuffer");
    FinishReadObject(dstBuffer, "vkCmdCopyBuffer");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdCopyImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkImageCopy*                          pRegions) {
    StartWriteObject(commandBuffer, "vkCmdCopyImage");
    StartReadObject(srcImage, "vkCmdCopyImage");
    StartReadObject(dstImage, "vkCmdCopyImage");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdCopyImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkImageCopy*                          pRegions) {
    FinishWriteObject(commandBuffer, "vkCmdCopyImage");
    FinishReadObject(srcImage, "vkCmdCopyImage");
    FinishReadObject(dstImage, "vkCmdCopyImage");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdBlitImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkImageBlit*                          pRegions,
    VkFilter                                    filter) {
    StartWriteObject(commandBuffer, "vkCmdBlitImage");
    StartReadObject(srcImage, "vkCmdBlitImage");
    StartReadObject(dstImage, "vkCmdBlitImage");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdBlitImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkImageBlit*                          pRegions,
    VkFilter                                    filter) {
    FinishWriteObject(commandBuffer, "vkCmdBlitImage");
    FinishReadObject(srcImage, "vkCmdBlitImage");
    FinishReadObject(dstImage, "vkCmdBlitImage");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdCopyBufferToImage(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    srcBuffer,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkBufferImageCopy*                    pRegions) {
    StartWriteObject(commandBuffer, "vkCmdCopyBufferToImage");
    StartReadObject(srcBuffer, "vkCmdCopyBufferToImage");
    StartReadObject(dstImage, "vkCmdCopyBufferToImage");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdCopyBufferToImage(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    srcBuffer,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkBufferImageCopy*                    pRegions) {
    FinishWriteObject(commandBuffer, "vkCmdCopyBufferToImage");
    FinishReadObject(srcBuffer, "vkCmdCopyBufferToImage");
    FinishReadObject(dstImage, "vkCmdCopyBufferToImage");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdCopyImageToBuffer(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkBuffer                                    dstBuffer,
    uint32_t                                    regionCount,
    const VkBufferImageCopy*                    pRegions) {
    StartWriteObject(commandBuffer, "vkCmdCopyImageToBuffer");
    StartReadObject(srcImage, "vkCmdCopyImageToBuffer");
    StartReadObject(dstBuffer, "vkCmdCopyImageToBuffer");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdCopyImageToBuffer(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkBuffer                                    dstBuffer,
    uint32_t                                    regionCount,
    const VkBufferImageCopy*                    pRegions) {
    FinishWriteObject(commandBuffer, "vkCmdCopyImageToBuffer");
    FinishReadObject(srcImage, "vkCmdCopyImageToBuffer");
    FinishReadObject(dstBuffer, "vkCmdCopyImageToBuffer");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdUpdateBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    VkDeviceSize                                dataSize,
    const void*                                 pData) {
    StartWriteObject(commandBuffer, "vkCmdUpdateBuffer");
    StartReadObject(dstBuffer, "vkCmdUpdateBuffer");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdUpdateBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    VkDeviceSize                                dataSize,
    const void*                                 pData) {
    FinishWriteObject(commandBuffer, "vkCmdUpdateBuffer");
    FinishReadObject(dstBuffer, "vkCmdUpdateBuffer");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdFillBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    VkDeviceSize                                size,
    uint32_t                                    data) {
    StartWriteObject(commandBuffer, "vkCmdFillBuffer");
    StartReadObject(dstBuffer, "vkCmdFillBuffer");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdFillBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    VkDeviceSize                                size,
    uint32_t                                    data) {
    FinishWriteObject(commandBuffer, "vkCmdFillBuffer");
    FinishReadObject(dstBuffer, "vkCmdFillBuffer");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdClearColorImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     image,
    VkImageLayout                               imageLayout,
    const VkClearColorValue*                    pColor,
    uint32_t                                    rangeCount,
    const VkImageSubresourceRange*              pRanges) {
    StartWriteObject(commandBuffer, "vkCmdClearColorImage");
    StartReadObject(image, "vkCmdClearColorImage");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdClearColorImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     image,
    VkImageLayout                               imageLayout,
    const VkClearColorValue*                    pColor,
    uint32_t                                    rangeCount,
    const VkImageSubresourceRange*              pRanges) {
    FinishWriteObject(commandBuffer, "vkCmdClearColorImage");
    FinishReadObject(image, "vkCmdClearColorImage");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdClearDepthStencilImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     image,
    VkImageLayout                               imageLayout,
    const VkClearDepthStencilValue*             pDepthStencil,
    uint32_t                                    rangeCount,
    const VkImageSubresourceRange*              pRanges) {
    StartWriteObject(commandBuffer, "vkCmdClearDepthStencilImage");
    StartReadObject(image, "vkCmdClearDepthStencilImage");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdClearDepthStencilImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     image,
    VkImageLayout                               imageLayout,
    const VkClearDepthStencilValue*             pDepthStencil,
    uint32_t                                    rangeCount,
    const VkImageSubresourceRange*              pRanges) {
    FinishWriteObject(commandBuffer, "vkCmdClearDepthStencilImage");
    FinishReadObject(image, "vkCmdClearDepthStencilImage");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdClearAttachments(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    attachmentCount,
    const VkClearAttachment*                    pAttachments,
    uint32_t                                    rectCount,
    const VkClearRect*                          pRects) {
    StartWriteObject(commandBuffer, "vkCmdClearAttachments");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdClearAttachments(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    attachmentCount,
    const VkClearAttachment*                    pAttachments,
    uint32_t                                    rectCount,
    const VkClearRect*                          pRects) {
    FinishWriteObject(commandBuffer, "vkCmdClearAttachments");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdResolveImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkImageResolve*                       pRegions) {
    StartWriteObject(commandBuffer, "vkCmdResolveImage");
    StartReadObject(srcImage, "vkCmdResolveImage");
    StartReadObject(dstImage, "vkCmdResolveImage");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdResolveImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkImageResolve*                       pRegions) {
    FinishWriteObject(commandBuffer, "vkCmdResolveImage");
    FinishReadObject(srcImage, "vkCmdResolveImage");
    FinishReadObject(dstImage, "vkCmdResolveImage");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetEvent(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    VkPipelineStageFlags                        stageMask) {
    StartWriteObject(commandBuffer, "vkCmdSetEvent");
    StartReadObject(event, "vkCmdSetEvent");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetEvent(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    VkPipelineStageFlags                        stageMask) {
    FinishWriteObject(commandBuffer, "vkCmdSetEvent");
    FinishReadObject(event, "vkCmdSetEvent");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdResetEvent(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    VkPipelineStageFlags                        stageMask) {
    StartWriteObject(commandBuffer, "vkCmdResetEvent");
    StartReadObject(event, "vkCmdResetEvent");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdResetEvent(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    VkPipelineStageFlags                        stageMask) {
    FinishWriteObject(commandBuffer, "vkCmdResetEvent");
    FinishReadObject(event, "vkCmdResetEvent");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdWaitEvents(
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
    const VkImageMemoryBarrier*                 pImageMemoryBarriers) {
    StartWriteObject(commandBuffer, "vkCmdWaitEvents");
    if (pEvents) {
        for (uint32_t index = 0; index < eventCount; index++) {
            StartReadObject(pEvents[index], "vkCmdWaitEvents");
        }
    }
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdWaitEvents(
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
    const VkImageMemoryBarrier*                 pImageMemoryBarriers) {
    FinishWriteObject(commandBuffer, "vkCmdWaitEvents");
    if (pEvents) {
        for (uint32_t index = 0; index < eventCount; index++) {
            FinishReadObject(pEvents[index], "vkCmdWaitEvents");
        }
    }
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdPipelineBarrier(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlags                        srcStageMask,
    VkPipelineStageFlags                        dstStageMask,
    VkDependencyFlags                           dependencyFlags,
    uint32_t                                    memoryBarrierCount,
    const VkMemoryBarrier*                      pMemoryBarriers,
    uint32_t                                    bufferMemoryBarrierCount,
    const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
    uint32_t                                    imageMemoryBarrierCount,
    const VkImageMemoryBarrier*                 pImageMemoryBarriers) {
    StartWriteObject(commandBuffer, "vkCmdPipelineBarrier");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdPipelineBarrier(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlags                        srcStageMask,
    VkPipelineStageFlags                        dstStageMask,
    VkDependencyFlags                           dependencyFlags,
    uint32_t                                    memoryBarrierCount,
    const VkMemoryBarrier*                      pMemoryBarriers,
    uint32_t                                    bufferMemoryBarrierCount,
    const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
    uint32_t                                    imageMemoryBarrierCount,
    const VkImageMemoryBarrier*                 pImageMemoryBarriers) {
    FinishWriteObject(commandBuffer, "vkCmdPipelineBarrier");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdBeginQuery(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    query,
    VkQueryControlFlags                         flags) {
    StartWriteObject(commandBuffer, "vkCmdBeginQuery");
    StartReadObject(queryPool, "vkCmdBeginQuery");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdBeginQuery(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    query,
    VkQueryControlFlags                         flags) {
    FinishWriteObject(commandBuffer, "vkCmdBeginQuery");
    FinishReadObject(queryPool, "vkCmdBeginQuery");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdEndQuery(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    query) {
    StartWriteObject(commandBuffer, "vkCmdEndQuery");
    StartReadObject(queryPool, "vkCmdEndQuery");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdEndQuery(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    query) {
    FinishWriteObject(commandBuffer, "vkCmdEndQuery");
    FinishReadObject(queryPool, "vkCmdEndQuery");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdResetQueryPool(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount) {
    StartWriteObject(commandBuffer, "vkCmdResetQueryPool");
    StartReadObject(queryPool, "vkCmdResetQueryPool");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdResetQueryPool(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount) {
    FinishWriteObject(commandBuffer, "vkCmdResetQueryPool");
    FinishReadObject(queryPool, "vkCmdResetQueryPool");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdWriteTimestamp(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlagBits                     pipelineStage,
    VkQueryPool                                 queryPool,
    uint32_t                                    query) {
    StartWriteObject(commandBuffer, "vkCmdWriteTimestamp");
    StartReadObject(queryPool, "vkCmdWriteTimestamp");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdWriteTimestamp(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlagBits                     pipelineStage,
    VkQueryPool                                 queryPool,
    uint32_t                                    query) {
    FinishWriteObject(commandBuffer, "vkCmdWriteTimestamp");
    FinishReadObject(queryPool, "vkCmdWriteTimestamp");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdCopyQueryPoolResults(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    VkDeviceSize                                stride,
    VkQueryResultFlags                          flags) {
    StartWriteObject(commandBuffer, "vkCmdCopyQueryPoolResults");
    StartReadObject(queryPool, "vkCmdCopyQueryPoolResults");
    StartReadObject(dstBuffer, "vkCmdCopyQueryPoolResults");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdCopyQueryPoolResults(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    VkDeviceSize                                stride,
    VkQueryResultFlags                          flags) {
    FinishWriteObject(commandBuffer, "vkCmdCopyQueryPoolResults");
    FinishReadObject(queryPool, "vkCmdCopyQueryPoolResults");
    FinishReadObject(dstBuffer, "vkCmdCopyQueryPoolResults");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdPushConstants(
    VkCommandBuffer                             commandBuffer,
    VkPipelineLayout                            layout,
    VkShaderStageFlags                          stageFlags,
    uint32_t                                    offset,
    uint32_t                                    size,
    const void*                                 pValues) {
    StartWriteObject(commandBuffer, "vkCmdPushConstants");
    StartReadObject(layout, "vkCmdPushConstants");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdPushConstants(
    VkCommandBuffer                             commandBuffer,
    VkPipelineLayout                            layout,
    VkShaderStageFlags                          stageFlags,
    uint32_t                                    offset,
    uint32_t                                    size,
    const void*                                 pValues) {
    FinishWriteObject(commandBuffer, "vkCmdPushConstants");
    FinishReadObject(layout, "vkCmdPushConstants");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdBeginRenderPass(
    VkCommandBuffer                             commandBuffer,
    const VkRenderPassBeginInfo*                pRenderPassBegin,
    VkSubpassContents                           contents) {
    StartWriteObject(commandBuffer, "vkCmdBeginRenderPass");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdBeginRenderPass(
    VkCommandBuffer                             commandBuffer,
    const VkRenderPassBeginInfo*                pRenderPassBegin,
    VkSubpassContents                           contents) {
    FinishWriteObject(commandBuffer, "vkCmdBeginRenderPass");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdNextSubpass(
    VkCommandBuffer                             commandBuffer,
    VkSubpassContents                           contents) {
    StartWriteObject(commandBuffer, "vkCmdNextSubpass");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdNextSubpass(
    VkCommandBuffer                             commandBuffer,
    VkSubpassContents                           contents) {
    FinishWriteObject(commandBuffer, "vkCmdNextSubpass");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdEndRenderPass(
    VkCommandBuffer                             commandBuffer) {
    StartWriteObject(commandBuffer, "vkCmdEndRenderPass");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdEndRenderPass(
    VkCommandBuffer                             commandBuffer) {
    FinishWriteObject(commandBuffer, "vkCmdEndRenderPass");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdExecuteCommands(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    commandBufferCount,
    const VkCommandBuffer*                      pCommandBuffers) {
    StartWriteObject(commandBuffer, "vkCmdExecuteCommands");
    if (pCommandBuffers) {
        for (uint32_t index = 0; index < commandBufferCount; index++) {
            StartReadObject(pCommandBuffers[index], "vkCmdExecuteCommands");
        }
    }
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdExecuteCommands(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    commandBufferCount,
    const VkCommandBuffer*                      pCommandBuffers) {
    FinishWriteObject(commandBuffer, "vkCmdExecuteCommands");
    if (pCommandBuffers) {
        for (uint32_t index = 0; index < commandBufferCount; index++) {
            FinishReadObject(pCommandBuffers[index], "vkCmdExecuteCommands");
        }
    }
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordBindBufferMemory2(
    VkDevice                                    device,
    uint32_t                                    bindInfoCount,
    const VkBindBufferMemoryInfo*               pBindInfos) {
    StartReadObjectParentInstance(device, "vkBindBufferMemory2");
}

void ThreadSafety::PostCallRecordBindBufferMemory2(
    VkDevice                                    device,
    uint32_t                                    bindInfoCount,
    const VkBindBufferMemoryInfo*               pBindInfos,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkBindBufferMemory2");
}

void ThreadSafety::PreCallRecordBindImageMemory2(
    VkDevice                                    device,
    uint32_t                                    bindInfoCount,
    const VkBindImageMemoryInfo*                pBindInfos) {
    StartReadObjectParentInstance(device, "vkBindImageMemory2");
}

void ThreadSafety::PostCallRecordBindImageMemory2(
    VkDevice                                    device,
    uint32_t                                    bindInfoCount,
    const VkBindImageMemoryInfo*                pBindInfos,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkBindImageMemory2");
}

void ThreadSafety::PreCallRecordGetDeviceGroupPeerMemoryFeatures(
    VkDevice                                    device,
    uint32_t                                    heapIndex,
    uint32_t                                    localDeviceIndex,
    uint32_t                                    remoteDeviceIndex,
    VkPeerMemoryFeatureFlags*                   pPeerMemoryFeatures) {
    StartReadObjectParentInstance(device, "vkGetDeviceGroupPeerMemoryFeatures");
}

void ThreadSafety::PostCallRecordGetDeviceGroupPeerMemoryFeatures(
    VkDevice                                    device,
    uint32_t                                    heapIndex,
    uint32_t                                    localDeviceIndex,
    uint32_t                                    remoteDeviceIndex,
    VkPeerMemoryFeatureFlags*                   pPeerMemoryFeatures) {
    FinishReadObjectParentInstance(device, "vkGetDeviceGroupPeerMemoryFeatures");
}

void ThreadSafety::PreCallRecordCmdSetDeviceMask(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    deviceMask) {
    StartWriteObject(commandBuffer, "vkCmdSetDeviceMask");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetDeviceMask(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    deviceMask) {
    FinishWriteObject(commandBuffer, "vkCmdSetDeviceMask");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdDispatchBase(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    baseGroupX,
    uint32_t                                    baseGroupY,
    uint32_t                                    baseGroupZ,
    uint32_t                                    groupCountX,
    uint32_t                                    groupCountY,
    uint32_t                                    groupCountZ) {
    StartWriteObject(commandBuffer, "vkCmdDispatchBase");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdDispatchBase(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    baseGroupX,
    uint32_t                                    baseGroupY,
    uint32_t                                    baseGroupZ,
    uint32_t                                    groupCountX,
    uint32_t                                    groupCountY,
    uint32_t                                    groupCountZ) {
    FinishWriteObject(commandBuffer, "vkCmdDispatchBase");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordEnumeratePhysicalDeviceGroups(
    VkInstance                                  instance,
    uint32_t*                                   pPhysicalDeviceGroupCount,
    VkPhysicalDeviceGroupProperties*            pPhysicalDeviceGroupProperties) {
    StartReadObjectParentInstance(instance, "vkEnumeratePhysicalDeviceGroups");
}

void ThreadSafety::PostCallRecordEnumeratePhysicalDeviceGroups(
    VkInstance                                  instance,
    uint32_t*                                   pPhysicalDeviceGroupCount,
    VkPhysicalDeviceGroupProperties*            pPhysicalDeviceGroupProperties,
    VkResult                                    result) {
    FinishReadObjectParentInstance(instance, "vkEnumeratePhysicalDeviceGroups");
}

void ThreadSafety::PreCallRecordGetImageMemoryRequirements2(
    VkDevice                                    device,
    const VkImageMemoryRequirementsInfo2*       pInfo,
    VkMemoryRequirements2*                      pMemoryRequirements) {
    StartReadObjectParentInstance(device, "vkGetImageMemoryRequirements2");
}

void ThreadSafety::PostCallRecordGetImageMemoryRequirements2(
    VkDevice                                    device,
    const VkImageMemoryRequirementsInfo2*       pInfo,
    VkMemoryRequirements2*                      pMemoryRequirements) {
    FinishReadObjectParentInstance(device, "vkGetImageMemoryRequirements2");
}

void ThreadSafety::PreCallRecordGetBufferMemoryRequirements2(
    VkDevice                                    device,
    const VkBufferMemoryRequirementsInfo2*      pInfo,
    VkMemoryRequirements2*                      pMemoryRequirements) {
    StartReadObjectParentInstance(device, "vkGetBufferMemoryRequirements2");
}

void ThreadSafety::PostCallRecordGetBufferMemoryRequirements2(
    VkDevice                                    device,
    const VkBufferMemoryRequirementsInfo2*      pInfo,
    VkMemoryRequirements2*                      pMemoryRequirements) {
    FinishReadObjectParentInstance(device, "vkGetBufferMemoryRequirements2");
}

void ThreadSafety::PreCallRecordCreateSamplerYcbcrConversion(
    VkDevice                                    device,
    const VkSamplerYcbcrConversionCreateInfo*   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSamplerYcbcrConversion*                   pYcbcrConversion) {
    StartReadObjectParentInstance(device, "vkCreateSamplerYcbcrConversion");
}

void ThreadSafety::PostCallRecordCreateSamplerYcbcrConversion(
    VkDevice                                    device,
    const VkSamplerYcbcrConversionCreateInfo*   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSamplerYcbcrConversion*                   pYcbcrConversion,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateSamplerYcbcrConversion");
    if (result == VK_SUCCESS) {
        CreateObject(*pYcbcrConversion);
    }
}

void ThreadSafety::PreCallRecordDestroySamplerYcbcrConversion(
    VkDevice                                    device,
    VkSamplerYcbcrConversion                    ycbcrConversion,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(device, "vkDestroySamplerYcbcrConversion");
    StartWriteObject(ycbcrConversion, "vkDestroySamplerYcbcrConversion");
    // Host access to ycbcrConversion must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroySamplerYcbcrConversion(
    VkDevice                                    device,
    VkSamplerYcbcrConversion                    ycbcrConversion,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(device, "vkDestroySamplerYcbcrConversion");
    FinishWriteObject(ycbcrConversion, "vkDestroySamplerYcbcrConversion");
    DestroyObject(ycbcrConversion);
    // Host access to ycbcrConversion must be externally synchronized
}

void ThreadSafety::PreCallRecordGetDescriptorSetLayoutSupport(
    VkDevice                                    device,
    const VkDescriptorSetLayoutCreateInfo*      pCreateInfo,
    VkDescriptorSetLayoutSupport*               pSupport) {
    StartReadObjectParentInstance(device, "vkGetDescriptorSetLayoutSupport");
}

void ThreadSafety::PostCallRecordGetDescriptorSetLayoutSupport(
    VkDevice                                    device,
    const VkDescriptorSetLayoutCreateInfo*      pCreateInfo,
    VkDescriptorSetLayoutSupport*               pSupport) {
    FinishReadObjectParentInstance(device, "vkGetDescriptorSetLayoutSupport");
}

void ThreadSafety::PreCallRecordCmdDrawIndirectCount(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkBuffer                                    countBuffer,
    VkDeviceSize                                countBufferOffset,
    uint32_t                                    maxDrawCount,
    uint32_t                                    stride) {
    StartWriteObject(commandBuffer, "vkCmdDrawIndirectCount");
    StartReadObject(buffer, "vkCmdDrawIndirectCount");
    StartReadObject(countBuffer, "vkCmdDrawIndirectCount");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdDrawIndirectCount(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkBuffer                                    countBuffer,
    VkDeviceSize                                countBufferOffset,
    uint32_t                                    maxDrawCount,
    uint32_t                                    stride) {
    FinishWriteObject(commandBuffer, "vkCmdDrawIndirectCount");
    FinishReadObject(buffer, "vkCmdDrawIndirectCount");
    FinishReadObject(countBuffer, "vkCmdDrawIndirectCount");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdDrawIndexedIndirectCount(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkBuffer                                    countBuffer,
    VkDeviceSize                                countBufferOffset,
    uint32_t                                    maxDrawCount,
    uint32_t                                    stride) {
    StartWriteObject(commandBuffer, "vkCmdDrawIndexedIndirectCount");
    StartReadObject(buffer, "vkCmdDrawIndexedIndirectCount");
    StartReadObject(countBuffer, "vkCmdDrawIndexedIndirectCount");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdDrawIndexedIndirectCount(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkBuffer                                    countBuffer,
    VkDeviceSize                                countBufferOffset,
    uint32_t                                    maxDrawCount,
    uint32_t                                    stride) {
    FinishWriteObject(commandBuffer, "vkCmdDrawIndexedIndirectCount");
    FinishReadObject(buffer, "vkCmdDrawIndexedIndirectCount");
    FinishReadObject(countBuffer, "vkCmdDrawIndexedIndirectCount");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCreateRenderPass2(
    VkDevice                                    device,
    const VkRenderPassCreateInfo2*              pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkRenderPass*                               pRenderPass) {
    StartReadObjectParentInstance(device, "vkCreateRenderPass2");
}

void ThreadSafety::PostCallRecordCreateRenderPass2(
    VkDevice                                    device,
    const VkRenderPassCreateInfo2*              pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkRenderPass*                               pRenderPass,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateRenderPass2");
    if (result == VK_SUCCESS) {
        CreateObject(*pRenderPass);
    }
}

void ThreadSafety::PreCallRecordCmdBeginRenderPass2(
    VkCommandBuffer                             commandBuffer,
    const VkRenderPassBeginInfo*                pRenderPassBegin,
    const VkSubpassBeginInfo*                   pSubpassBeginInfo) {
    StartWriteObject(commandBuffer, "vkCmdBeginRenderPass2");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdBeginRenderPass2(
    VkCommandBuffer                             commandBuffer,
    const VkRenderPassBeginInfo*                pRenderPassBegin,
    const VkSubpassBeginInfo*                   pSubpassBeginInfo) {
    FinishWriteObject(commandBuffer, "vkCmdBeginRenderPass2");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdNextSubpass2(
    VkCommandBuffer                             commandBuffer,
    const VkSubpassBeginInfo*                   pSubpassBeginInfo,
    const VkSubpassEndInfo*                     pSubpassEndInfo) {
    StartWriteObject(commandBuffer, "vkCmdNextSubpass2");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdNextSubpass2(
    VkCommandBuffer                             commandBuffer,
    const VkSubpassBeginInfo*                   pSubpassBeginInfo,
    const VkSubpassEndInfo*                     pSubpassEndInfo) {
    FinishWriteObject(commandBuffer, "vkCmdNextSubpass2");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdEndRenderPass2(
    VkCommandBuffer                             commandBuffer,
    const VkSubpassEndInfo*                     pSubpassEndInfo) {
    StartWriteObject(commandBuffer, "vkCmdEndRenderPass2");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdEndRenderPass2(
    VkCommandBuffer                             commandBuffer,
    const VkSubpassEndInfo*                     pSubpassEndInfo) {
    FinishWriteObject(commandBuffer, "vkCmdEndRenderPass2");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordResetQueryPool(
    VkDevice                                    device,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount) {
    StartReadObjectParentInstance(device, "vkResetQueryPool");
    StartReadObject(queryPool, "vkResetQueryPool");
}

void ThreadSafety::PostCallRecordResetQueryPool(
    VkDevice                                    device,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount) {
    FinishReadObjectParentInstance(device, "vkResetQueryPool");
    FinishReadObject(queryPool, "vkResetQueryPool");
}

void ThreadSafety::PreCallRecordGetSemaphoreCounterValue(
    VkDevice                                    device,
    VkSemaphore                                 semaphore,
    uint64_t*                                   pValue) {
    StartReadObjectParentInstance(device, "vkGetSemaphoreCounterValue");
    StartReadObject(semaphore, "vkGetSemaphoreCounterValue");
}

void ThreadSafety::PostCallRecordGetSemaphoreCounterValue(
    VkDevice                                    device,
    VkSemaphore                                 semaphore,
    uint64_t*                                   pValue,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkGetSemaphoreCounterValue");
    FinishReadObject(semaphore, "vkGetSemaphoreCounterValue");
}

void ThreadSafety::PreCallRecordWaitSemaphores(
    VkDevice                                    device,
    const VkSemaphoreWaitInfo*                  pWaitInfo,
    uint64_t                                    timeout) {
    StartReadObjectParentInstance(device, "vkWaitSemaphores");
}

void ThreadSafety::PostCallRecordWaitSemaphores(
    VkDevice                                    device,
    const VkSemaphoreWaitInfo*                  pWaitInfo,
    uint64_t                                    timeout,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkWaitSemaphores");
}

void ThreadSafety::PreCallRecordSignalSemaphore(
    VkDevice                                    device,
    const VkSemaphoreSignalInfo*                pSignalInfo) {
    StartReadObjectParentInstance(device, "vkSignalSemaphore");
}

void ThreadSafety::PostCallRecordSignalSemaphore(
    VkDevice                                    device,
    const VkSemaphoreSignalInfo*                pSignalInfo,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkSignalSemaphore");
}

void ThreadSafety::PreCallRecordGetBufferDeviceAddress(
    VkDevice                                    device,
    const VkBufferDeviceAddressInfo*            pInfo) {
    StartReadObjectParentInstance(device, "vkGetBufferDeviceAddress");
}

void ThreadSafety::PostCallRecordGetBufferDeviceAddress(
    VkDevice                                    device,
    const VkBufferDeviceAddressInfo*            pInfo,
    VkDeviceAddress                             result) {
    FinishReadObjectParentInstance(device, "vkGetBufferDeviceAddress");
}

void ThreadSafety::PreCallRecordGetBufferOpaqueCaptureAddress(
    VkDevice                                    device,
    const VkBufferDeviceAddressInfo*            pInfo) {
    StartReadObjectParentInstance(device, "vkGetBufferOpaqueCaptureAddress");
}

void ThreadSafety::PostCallRecordGetBufferOpaqueCaptureAddress(
    VkDevice                                    device,
    const VkBufferDeviceAddressInfo*            pInfo) {
    FinishReadObjectParentInstance(device, "vkGetBufferOpaqueCaptureAddress");
}

void ThreadSafety::PreCallRecordGetDeviceMemoryOpaqueCaptureAddress(
    VkDevice                                    device,
    const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) {
    StartReadObjectParentInstance(device, "vkGetDeviceMemoryOpaqueCaptureAddress");
}

void ThreadSafety::PostCallRecordGetDeviceMemoryOpaqueCaptureAddress(
    VkDevice                                    device,
    const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) {
    FinishReadObjectParentInstance(device, "vkGetDeviceMemoryOpaqueCaptureAddress");
}

void ThreadSafety::PreCallRecordGetCommandPoolMemoryConsumption(
    VkDevice                                    device,
    VkCommandPool                               commandPool,
    VkCommandBuffer                             commandBuffer,
    VkCommandPoolMemoryConsumption*             pConsumption) {
    StartReadObjectParentInstance(device, "vkGetCommandPoolMemoryConsumption");
    StartWriteObject(commandPool, "vkGetCommandPoolMemoryConsumption");
    StartWriteObject(commandBuffer, "vkGetCommandPoolMemoryConsumption");
    // Host access to commandPool must be externally synchronized
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordGetCommandPoolMemoryConsumption(
    VkDevice                                    device,
    VkCommandPool                               commandPool,
    VkCommandBuffer                             commandBuffer,
    VkCommandPoolMemoryConsumption*             pConsumption) {
    FinishReadObjectParentInstance(device, "vkGetCommandPoolMemoryConsumption");
    FinishWriteObject(commandPool, "vkGetCommandPoolMemoryConsumption");
    FinishWriteObject(commandBuffer, "vkGetCommandPoolMemoryConsumption");
    // Host access to commandPool must be externally synchronized
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordGetFaultData(
    VkDevice                                    device,
    VkFaultQueryBehavior                        faultQueryBehavior,
    VkBool32*                                   pUnrecordedFaults,
    uint32_t*                                   pFaultCount,
    VkFaultData*                                pFaults) {
    StartReadObjectParentInstance(device, "vkGetFaultData");
}

void ThreadSafety::PostCallRecordGetFaultData(
    VkDevice                                    device,
    VkFaultQueryBehavior                        faultQueryBehavior,
    VkBool32*                                   pUnrecordedFaults,
    uint32_t*                                   pFaultCount,
    VkFaultData*                                pFaults,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkGetFaultData");
}

void ThreadSafety::PreCallRecordDestroySurfaceKHR(
    VkInstance                                  instance,
    VkSurfaceKHR                                surface,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(instance, "vkDestroySurfaceKHR");
    StartWriteObjectParentInstance(surface, "vkDestroySurfaceKHR");
    // Host access to surface must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroySurfaceKHR(
    VkInstance                                  instance,
    VkSurfaceKHR                                surface,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(instance, "vkDestroySurfaceKHR");
    FinishWriteObjectParentInstance(surface, "vkDestroySurfaceKHR");
    DestroyObjectParentInstance(surface);
    // Host access to surface must be externally synchronized
}

void ThreadSafety::PreCallRecordGetPhysicalDeviceSurfaceSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex,
    VkSurfaceKHR                                surface,
    VkBool32*                                   pSupported) {
    StartReadObjectParentInstance(surface, "vkGetPhysicalDeviceSurfaceSupportKHR");
}

void ThreadSafety::PostCallRecordGetPhysicalDeviceSurfaceSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex,
    VkSurfaceKHR                                surface,
    VkBool32*                                   pSupported,
    VkResult                                    result) {
    FinishReadObjectParentInstance(surface, "vkGetPhysicalDeviceSurfaceSupportKHR");
}

void ThreadSafety::PreCallRecordGetPhysicalDeviceSurfaceCapabilitiesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    VkSurfaceCapabilitiesKHR*                   pSurfaceCapabilities) {
    StartReadObjectParentInstance(surface, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
}

void ThreadSafety::PostCallRecordGetPhysicalDeviceSurfaceCapabilitiesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    VkSurfaceCapabilitiesKHR*                   pSurfaceCapabilities,
    VkResult                                    result) {
    FinishReadObjectParentInstance(surface, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
}

void ThreadSafety::PreCallRecordGetPhysicalDeviceSurfaceFormatsKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    uint32_t*                                   pSurfaceFormatCount,
    VkSurfaceFormatKHR*                         pSurfaceFormats) {
    StartReadObjectParentInstance(surface, "vkGetPhysicalDeviceSurfaceFormatsKHR");
}

void ThreadSafety::PostCallRecordGetPhysicalDeviceSurfaceFormatsKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    uint32_t*                                   pSurfaceFormatCount,
    VkSurfaceFormatKHR*                         pSurfaceFormats,
    VkResult                                    result) {
    FinishReadObjectParentInstance(surface, "vkGetPhysicalDeviceSurfaceFormatsKHR");
}

void ThreadSafety::PreCallRecordGetPhysicalDeviceSurfacePresentModesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    uint32_t*                                   pPresentModeCount,
    VkPresentModeKHR*                           pPresentModes) {
    StartReadObjectParentInstance(surface, "vkGetPhysicalDeviceSurfacePresentModesKHR");
}

void ThreadSafety::PostCallRecordGetPhysicalDeviceSurfacePresentModesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    uint32_t*                                   pPresentModeCount,
    VkPresentModeKHR*                           pPresentModes,
    VkResult                                    result) {
    FinishReadObjectParentInstance(surface, "vkGetPhysicalDeviceSurfacePresentModesKHR");
}

void ThreadSafety::PreCallRecordCreateSwapchainKHR(
    VkDevice                                    device,
    const VkSwapchainCreateInfoKHR*             pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSwapchainKHR*                             pSwapchain) {
    StartReadObjectParentInstance(device, "vkCreateSwapchainKHR");
        StartWriteObjectParentInstance(pCreateInfo->surface, "vkCreateSwapchainKHR");
    // Host access to pCreateInfo->surface must be externally synchronized
}

void ThreadSafety::PostCallRecordCreateSwapchainKHR(
    VkDevice                                    device,
    const VkSwapchainCreateInfoKHR*             pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSwapchainKHR*                             pSwapchain,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateSwapchainKHR");
        FinishWriteObjectParentInstance(pCreateInfo->surface, "vkCreateSwapchainKHR");
    if (result == VK_SUCCESS) {
        CreateObjectParentInstance(*pSwapchain);
    }
    // Host access to pCreateInfo->surface must be externally synchronized
}

void ThreadSafety::PreCallRecordAcquireNextImageKHR(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    uint64_t                                    timeout,
    VkSemaphore                                 semaphore,
    VkFence                                     fence,
    uint32_t*                                   pImageIndex) {
    StartReadObjectParentInstance(device, "vkAcquireNextImageKHR");
    StartWriteObjectParentInstance(swapchain, "vkAcquireNextImageKHR");
    StartWriteObject(semaphore, "vkAcquireNextImageKHR");
    StartWriteObject(fence, "vkAcquireNextImageKHR");
    // Host access to swapchain must be externally synchronized
    // Host access to semaphore must be externally synchronized
    // Host access to fence must be externally synchronized
}

void ThreadSafety::PostCallRecordAcquireNextImageKHR(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    uint64_t                                    timeout,
    VkSemaphore                                 semaphore,
    VkFence                                     fence,
    uint32_t*                                   pImageIndex,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkAcquireNextImageKHR");
    FinishWriteObjectParentInstance(swapchain, "vkAcquireNextImageKHR");
    FinishWriteObject(semaphore, "vkAcquireNextImageKHR");
    FinishWriteObject(fence, "vkAcquireNextImageKHR");
    // Host access to swapchain must be externally synchronized
    // Host access to semaphore must be externally synchronized
    // Host access to fence must be externally synchronized
}

void ThreadSafety::PreCallRecordGetDeviceGroupPresentCapabilitiesKHR(
    VkDevice                                    device,
    VkDeviceGroupPresentCapabilitiesKHR*        pDeviceGroupPresentCapabilities) {
    StartReadObjectParentInstance(device, "vkGetDeviceGroupPresentCapabilitiesKHR");
}

void ThreadSafety::PostCallRecordGetDeviceGroupPresentCapabilitiesKHR(
    VkDevice                                    device,
    VkDeviceGroupPresentCapabilitiesKHR*        pDeviceGroupPresentCapabilities,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkGetDeviceGroupPresentCapabilitiesKHR");
}

void ThreadSafety::PreCallRecordGetDeviceGroupSurfacePresentModesKHR(
    VkDevice                                    device,
    VkSurfaceKHR                                surface,
    VkDeviceGroupPresentModeFlagsKHR*           pModes) {
    StartReadObjectParentInstance(device, "vkGetDeviceGroupSurfacePresentModesKHR");
    StartWriteObjectParentInstance(surface, "vkGetDeviceGroupSurfacePresentModesKHR");
    // Host access to surface must be externally synchronized
}

void ThreadSafety::PostCallRecordGetDeviceGroupSurfacePresentModesKHR(
    VkDevice                                    device,
    VkSurfaceKHR                                surface,
    VkDeviceGroupPresentModeFlagsKHR*           pModes,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkGetDeviceGroupSurfacePresentModesKHR");
    FinishWriteObjectParentInstance(surface, "vkGetDeviceGroupSurfacePresentModesKHR");
    // Host access to surface must be externally synchronized
}

void ThreadSafety::PreCallRecordGetPhysicalDevicePresentRectanglesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    uint32_t*                                   pRectCount,
    VkRect2D*                                   pRects) {
    StartWriteObjectParentInstance(surface, "vkGetPhysicalDevicePresentRectanglesKHR");
    // Host access to surface must be externally synchronized
}

void ThreadSafety::PostCallRecordGetPhysicalDevicePresentRectanglesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    uint32_t*                                   pRectCount,
    VkRect2D*                                   pRects,
    VkResult                                    result) {
    FinishWriteObjectParentInstance(surface, "vkGetPhysicalDevicePresentRectanglesKHR");
    // Host access to surface must be externally synchronized
}

void ThreadSafety::PreCallRecordAcquireNextImage2KHR(
    VkDevice                                    device,
    const VkAcquireNextImageInfoKHR*            pAcquireInfo,
    uint32_t*                                   pImageIndex) {
    StartReadObjectParentInstance(device, "vkAcquireNextImage2KHR");
}

void ThreadSafety::PostCallRecordAcquireNextImage2KHR(
    VkDevice                                    device,
    const VkAcquireNextImageInfoKHR*            pAcquireInfo,
    uint32_t*                                   pImageIndex,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkAcquireNextImage2KHR");
}

void ThreadSafety::PreCallRecordCreateDisplayModeKHR(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayKHR                                display,
    const VkDisplayModeCreateInfoKHR*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDisplayModeKHR*                           pMode) {
    StartWriteObject(display, "vkCreateDisplayModeKHR");
    // Host access to display must be externally synchronized
}

void ThreadSafety::PostCallRecordCreateDisplayModeKHR(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayKHR                                display,
    const VkDisplayModeCreateInfoKHR*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDisplayModeKHR*                           pMode,
    VkResult                                    result) {
    FinishWriteObject(display, "vkCreateDisplayModeKHR");
    if (result == VK_SUCCESS) {
        CreateObject(*pMode);
    }
    // Host access to display must be externally synchronized
}

void ThreadSafety::PreCallRecordGetDisplayPlaneCapabilitiesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayModeKHR                            mode,
    uint32_t                                    planeIndex,
    VkDisplayPlaneCapabilitiesKHR*              pCapabilities) {
    StartWriteObject(mode, "vkGetDisplayPlaneCapabilitiesKHR");
    // Host access to mode must be externally synchronized
}

void ThreadSafety::PostCallRecordGetDisplayPlaneCapabilitiesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayModeKHR                            mode,
    uint32_t                                    planeIndex,
    VkDisplayPlaneCapabilitiesKHR*              pCapabilities,
    VkResult                                    result) {
    FinishWriteObject(mode, "vkGetDisplayPlaneCapabilitiesKHR");
    // Host access to mode must be externally synchronized
}

void ThreadSafety::PreCallRecordCreateDisplayPlaneSurfaceKHR(
    VkInstance                                  instance,
    const VkDisplaySurfaceCreateInfoKHR*        pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface) {
    StartReadObjectParentInstance(instance, "vkCreateDisplayPlaneSurfaceKHR");
}

void ThreadSafety::PostCallRecordCreateDisplayPlaneSurfaceKHR(
    VkInstance                                  instance,
    const VkDisplaySurfaceCreateInfoKHR*        pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface,
    VkResult                                    result) {
    FinishReadObjectParentInstance(instance, "vkCreateDisplayPlaneSurfaceKHR");
    if (result == VK_SUCCESS) {
        CreateObjectParentInstance(*pSurface);
    }
}

void ThreadSafety::PreCallRecordCreateSharedSwapchainsKHR(
    VkDevice                                    device,
    uint32_t                                    swapchainCount,
    const VkSwapchainCreateInfoKHR*             pCreateInfos,
    const VkAllocationCallbacks*                pAllocator,
    VkSwapchainKHR*                             pSwapchains) {
    StartReadObjectParentInstance(device, "vkCreateSharedSwapchainsKHR");
    if (pCreateInfos) {
        for (uint32_t index=0; index < swapchainCount; index++) {
            StartWriteObjectParentInstance(pCreateInfos[index].surface, "vkCreateSharedSwapchainsKHR");
        }
    }
    if (pSwapchains) {
        for (uint32_t index = 0; index < swapchainCount; index++) {
            StartReadObjectParentInstance(pSwapchains[index], "vkCreateSharedSwapchainsKHR");
        }
    }
    // Host access to pCreateInfos[].surface must be externally synchronized
}

void ThreadSafety::PostCallRecordCreateSharedSwapchainsKHR(
    VkDevice                                    device,
    uint32_t                                    swapchainCount,
    const VkSwapchainCreateInfoKHR*             pCreateInfos,
    const VkAllocationCallbacks*                pAllocator,
    VkSwapchainKHR*                             pSwapchains,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkCreateSharedSwapchainsKHR");
    if (pCreateInfos) {
        for (uint32_t index=0; index < swapchainCount; index++) {
            FinishWriteObjectParentInstance(pCreateInfos[index].surface, "vkCreateSharedSwapchainsKHR");
        }
    }
    if (result == VK_SUCCESS) {
        if (pSwapchains) {
            for (uint32_t index = 0; index < swapchainCount; index++) {
                CreateObjectParentInstance(pSwapchains[index]);
            }
        }
    }
    // Host access to pCreateInfos[].surface must be externally synchronized
}

void ThreadSafety::PreCallRecordGetMemoryFdKHR(
    VkDevice                                    device,
    const VkMemoryGetFdInfoKHR*                 pGetFdInfo,
    int*                                        pFd) {
    StartReadObjectParentInstance(device, "vkGetMemoryFdKHR");
}

void ThreadSafety::PostCallRecordGetMemoryFdKHR(
    VkDevice                                    device,
    const VkMemoryGetFdInfoKHR*                 pGetFdInfo,
    int*                                        pFd,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkGetMemoryFdKHR");
}

void ThreadSafety::PreCallRecordGetMemoryFdPropertiesKHR(
    VkDevice                                    device,
    VkExternalMemoryHandleTypeFlagBits          handleType,
    int                                         fd,
    VkMemoryFdPropertiesKHR*                    pMemoryFdProperties) {
    StartReadObjectParentInstance(device, "vkGetMemoryFdPropertiesKHR");
}

void ThreadSafety::PostCallRecordGetMemoryFdPropertiesKHR(
    VkDevice                                    device,
    VkExternalMemoryHandleTypeFlagBits          handleType,
    int                                         fd,
    VkMemoryFdPropertiesKHR*                    pMemoryFdProperties,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkGetMemoryFdPropertiesKHR");
}

void ThreadSafety::PreCallRecordImportSemaphoreFdKHR(
    VkDevice                                    device,
    const VkImportSemaphoreFdInfoKHR*           pImportSemaphoreFdInfo) {
    StartReadObjectParentInstance(device, "vkImportSemaphoreFdKHR");
}

void ThreadSafety::PostCallRecordImportSemaphoreFdKHR(
    VkDevice                                    device,
    const VkImportSemaphoreFdInfoKHR*           pImportSemaphoreFdInfo,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkImportSemaphoreFdKHR");
}

void ThreadSafety::PreCallRecordGetSemaphoreFdKHR(
    VkDevice                                    device,
    const VkSemaphoreGetFdInfoKHR*              pGetFdInfo,
    int*                                        pFd) {
    StartReadObjectParentInstance(device, "vkGetSemaphoreFdKHR");
}

void ThreadSafety::PostCallRecordGetSemaphoreFdKHR(
    VkDevice                                    device,
    const VkSemaphoreGetFdInfoKHR*              pGetFdInfo,
    int*                                        pFd,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkGetSemaphoreFdKHR");
}

void ThreadSafety::PreCallRecordGetSwapchainStatusKHR(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain) {
    StartReadObjectParentInstance(device, "vkGetSwapchainStatusKHR");
    StartWriteObjectParentInstance(swapchain, "vkGetSwapchainStatusKHR");
    // Host access to swapchain must be externally synchronized
}

void ThreadSafety::PostCallRecordGetSwapchainStatusKHR(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkGetSwapchainStatusKHR");
    FinishWriteObjectParentInstance(swapchain, "vkGetSwapchainStatusKHR");
    // Host access to swapchain must be externally synchronized
}

void ThreadSafety::PreCallRecordImportFenceFdKHR(
    VkDevice                                    device,
    const VkImportFenceFdInfoKHR*               pImportFenceFdInfo) {
    StartReadObjectParentInstance(device, "vkImportFenceFdKHR");
}

void ThreadSafety::PostCallRecordImportFenceFdKHR(
    VkDevice                                    device,
    const VkImportFenceFdInfoKHR*               pImportFenceFdInfo,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkImportFenceFdKHR");
}

void ThreadSafety::PreCallRecordGetFenceFdKHR(
    VkDevice                                    device,
    const VkFenceGetFdInfoKHR*                  pGetFdInfo,
    int*                                        pFd) {
    StartReadObjectParentInstance(device, "vkGetFenceFdKHR");
}

void ThreadSafety::PostCallRecordGetFenceFdKHR(
    VkDevice                                    device,
    const VkFenceGetFdInfoKHR*                  pGetFdInfo,
    int*                                        pFd,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkGetFenceFdKHR");
}

void ThreadSafety::PreCallRecordAcquireProfilingLockKHR(
    VkDevice                                    device,
    const VkAcquireProfilingLockInfoKHR*        pInfo) {
    StartReadObjectParentInstance(device, "vkAcquireProfilingLockKHR");
}

void ThreadSafety::PostCallRecordAcquireProfilingLockKHR(
    VkDevice                                    device,
    const VkAcquireProfilingLockInfoKHR*        pInfo,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkAcquireProfilingLockKHR");
}

void ThreadSafety::PreCallRecordReleaseProfilingLockKHR(
    VkDevice                                    device) {
    StartReadObjectParentInstance(device, "vkReleaseProfilingLockKHR");
}

void ThreadSafety::PostCallRecordReleaseProfilingLockKHR(
    VkDevice                                    device) {
    FinishReadObjectParentInstance(device, "vkReleaseProfilingLockKHR");
}

void ThreadSafety::PreCallRecordCmdSetFragmentShadingRateKHR(
    VkCommandBuffer                             commandBuffer,
    const VkExtent2D*                           pFragmentSize,
    const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) {
    StartWriteObject(commandBuffer, "vkCmdSetFragmentShadingRateKHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetFragmentShadingRateKHR(
    VkCommandBuffer                             commandBuffer,
    const VkExtent2D*                           pFragmentSize,
    const VkFragmentShadingRateCombinerOpKHR    combinerOps[2]) {
    FinishWriteObject(commandBuffer, "vkCmdSetFragmentShadingRateKHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdRefreshObjectsKHR(
    VkCommandBuffer                             commandBuffer,
    const VkRefreshObjectListKHR*               pRefreshObjects) {
    StartWriteObject(commandBuffer, "vkCmdRefreshObjectsKHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdRefreshObjectsKHR(
    VkCommandBuffer                             commandBuffer,
    const VkRefreshObjectListKHR*               pRefreshObjects) {
    FinishWriteObject(commandBuffer, "vkCmdRefreshObjectsKHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetEvent2KHR(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    const VkDependencyInfoKHR*                  pDependencyInfo) {
    StartWriteObject(commandBuffer, "vkCmdSetEvent2KHR");
    StartReadObject(event, "vkCmdSetEvent2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetEvent2KHR(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    const VkDependencyInfoKHR*                  pDependencyInfo) {
    FinishWriteObject(commandBuffer, "vkCmdSetEvent2KHR");
    FinishReadObject(event, "vkCmdSetEvent2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdResetEvent2KHR(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    VkPipelineStageFlags2KHR                    stageMask) {
    StartWriteObject(commandBuffer, "vkCmdResetEvent2KHR");
    StartReadObject(event, "vkCmdResetEvent2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdResetEvent2KHR(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    VkPipelineStageFlags2KHR                    stageMask) {
    FinishWriteObject(commandBuffer, "vkCmdResetEvent2KHR");
    FinishReadObject(event, "vkCmdResetEvent2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdWaitEvents2KHR(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    eventCount,
    const VkEvent*                              pEvents,
    const VkDependencyInfoKHR*                  pDependencyInfos) {
    StartWriteObject(commandBuffer, "vkCmdWaitEvents2KHR");
    if (pEvents) {
        for (uint32_t index = 0; index < eventCount; index++) {
            StartReadObject(pEvents[index], "vkCmdWaitEvents2KHR");
        }
    }
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdWaitEvents2KHR(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    eventCount,
    const VkEvent*                              pEvents,
    const VkDependencyInfoKHR*                  pDependencyInfos) {
    FinishWriteObject(commandBuffer, "vkCmdWaitEvents2KHR");
    if (pEvents) {
        for (uint32_t index = 0; index < eventCount; index++) {
            FinishReadObject(pEvents[index], "vkCmdWaitEvents2KHR");
        }
    }
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdPipelineBarrier2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkDependencyInfoKHR*                  pDependencyInfo) {
    StartWriteObject(commandBuffer, "vkCmdPipelineBarrier2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdPipelineBarrier2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkDependencyInfoKHR*                  pDependencyInfo) {
    FinishWriteObject(commandBuffer, "vkCmdPipelineBarrier2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdWriteTimestamp2KHR(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlags2KHR                    stage,
    VkQueryPool                                 queryPool,
    uint32_t                                    query) {
    StartWriteObject(commandBuffer, "vkCmdWriteTimestamp2KHR");
    StartReadObject(queryPool, "vkCmdWriteTimestamp2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdWriteTimestamp2KHR(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlags2KHR                    stage,
    VkQueryPool                                 queryPool,
    uint32_t                                    query) {
    FinishWriteObject(commandBuffer, "vkCmdWriteTimestamp2KHR");
    FinishReadObject(queryPool, "vkCmdWriteTimestamp2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordQueueSubmit2KHR(
    VkQueue                                     queue,
    uint32_t                                    submitCount,
    const VkSubmitInfo2KHR*                     pSubmits,
    VkFence                                     fence) {
    StartWriteObject(queue, "vkQueueSubmit2KHR");
    StartWriteObject(fence, "vkQueueSubmit2KHR");
    // Host access to queue must be externally synchronized
    // Host access to fence must be externally synchronized
}

void ThreadSafety::PostCallRecordQueueSubmit2KHR(
    VkQueue                                     queue,
    uint32_t                                    submitCount,
    const VkSubmitInfo2KHR*                     pSubmits,
    VkFence                                     fence,
    VkResult                                    result) {
    FinishWriteObject(queue, "vkQueueSubmit2KHR");
    FinishWriteObject(fence, "vkQueueSubmit2KHR");
    // Host access to queue must be externally synchronized
    // Host access to fence must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdWriteBufferMarker2AMD(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlags2KHR                    stage,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    uint32_t                                    marker) {
    StartWriteObject(commandBuffer, "vkCmdWriteBufferMarker2AMD");
    StartReadObject(dstBuffer, "vkCmdWriteBufferMarker2AMD");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdWriteBufferMarker2AMD(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlags2KHR                    stage,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    uint32_t                                    marker) {
    FinishWriteObject(commandBuffer, "vkCmdWriteBufferMarker2AMD");
    FinishReadObject(dstBuffer, "vkCmdWriteBufferMarker2AMD");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordGetQueueCheckpointData2NV(
    VkQueue                                     queue,
    uint32_t*                                   pCheckpointDataCount,
    VkCheckpointData2NV*                        pCheckpointData) {
    StartReadObject(queue, "vkGetQueueCheckpointData2NV");
}

void ThreadSafety::PostCallRecordGetQueueCheckpointData2NV(
    VkQueue                                     queue,
    uint32_t*                                   pCheckpointDataCount,
    VkCheckpointData2NV*                        pCheckpointData) {
    FinishReadObject(queue, "vkGetQueueCheckpointData2NV");
}

void ThreadSafety::PreCallRecordCmdCopyBuffer2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkCopyBufferInfo2KHR*                 pCopyBufferInfo) {
    StartWriteObject(commandBuffer, "vkCmdCopyBuffer2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdCopyBuffer2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkCopyBufferInfo2KHR*                 pCopyBufferInfo) {
    FinishWriteObject(commandBuffer, "vkCmdCopyBuffer2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdCopyImage2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkCopyImageInfo2KHR*                  pCopyImageInfo) {
    StartWriteObject(commandBuffer, "vkCmdCopyImage2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdCopyImage2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkCopyImageInfo2KHR*                  pCopyImageInfo) {
    FinishWriteObject(commandBuffer, "vkCmdCopyImage2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdCopyBufferToImage2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkCopyBufferToImageInfo2KHR*          pCopyBufferToImageInfo) {
    StartWriteObject(commandBuffer, "vkCmdCopyBufferToImage2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdCopyBufferToImage2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkCopyBufferToImageInfo2KHR*          pCopyBufferToImageInfo) {
    FinishWriteObject(commandBuffer, "vkCmdCopyBufferToImage2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdCopyImageToBuffer2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkCopyImageToBufferInfo2KHR*          pCopyImageToBufferInfo) {
    StartWriteObject(commandBuffer, "vkCmdCopyImageToBuffer2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdCopyImageToBuffer2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkCopyImageToBufferInfo2KHR*          pCopyImageToBufferInfo) {
    FinishWriteObject(commandBuffer, "vkCmdCopyImageToBuffer2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdBlitImage2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkBlitImageInfo2KHR*                  pBlitImageInfo) {
    StartWriteObject(commandBuffer, "vkCmdBlitImage2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdBlitImage2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkBlitImageInfo2KHR*                  pBlitImageInfo) {
    FinishWriteObject(commandBuffer, "vkCmdBlitImage2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdResolveImage2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkResolveImageInfo2KHR*               pResolveImageInfo) {
    StartWriteObject(commandBuffer, "vkCmdResolveImage2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdResolveImage2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkResolveImageInfo2KHR*               pResolveImageInfo) {
    FinishWriteObject(commandBuffer, "vkCmdResolveImage2KHR");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordReleaseDisplayEXT(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayKHR                                display) {
    StartReadObject(display, "vkReleaseDisplayEXT");
}

void ThreadSafety::PostCallRecordReleaseDisplayEXT(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayKHR                                display,
    VkResult                                    result) {
    FinishReadObject(display, "vkReleaseDisplayEXT");
}

void ThreadSafety::PreCallRecordGetPhysicalDeviceSurfaceCapabilities2EXT(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    VkSurfaceCapabilities2EXT*                  pSurfaceCapabilities) {
    StartReadObjectParentInstance(surface, "vkGetPhysicalDeviceSurfaceCapabilities2EXT");
}

void ThreadSafety::PostCallRecordGetPhysicalDeviceSurfaceCapabilities2EXT(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    VkSurfaceCapabilities2EXT*                  pSurfaceCapabilities,
    VkResult                                    result) {
    FinishReadObjectParentInstance(surface, "vkGetPhysicalDeviceSurfaceCapabilities2EXT");
}

void ThreadSafety::PreCallRecordDisplayPowerControlEXT(
    VkDevice                                    device,
    VkDisplayKHR                                display,
    const VkDisplayPowerInfoEXT*                pDisplayPowerInfo) {
    StartReadObjectParentInstance(device, "vkDisplayPowerControlEXT");
    StartReadObject(display, "vkDisplayPowerControlEXT");
}

void ThreadSafety::PostCallRecordDisplayPowerControlEXT(
    VkDevice                                    device,
    VkDisplayKHR                                display,
    const VkDisplayPowerInfoEXT*                pDisplayPowerInfo,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkDisplayPowerControlEXT");
    FinishReadObject(display, "vkDisplayPowerControlEXT");
}

void ThreadSafety::PreCallRecordRegisterDeviceEventEXT(
    VkDevice                                    device,
    const VkDeviceEventInfoEXT*                 pDeviceEventInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFence*                                    pFence) {
    StartReadObjectParentInstance(device, "vkRegisterDeviceEventEXT");
}

void ThreadSafety::PostCallRecordRegisterDeviceEventEXT(
    VkDevice                                    device,
    const VkDeviceEventInfoEXT*                 pDeviceEventInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFence*                                    pFence,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkRegisterDeviceEventEXT");
}

void ThreadSafety::PreCallRecordRegisterDisplayEventEXT(
    VkDevice                                    device,
    VkDisplayKHR                                display,
    const VkDisplayEventInfoEXT*                pDisplayEventInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFence*                                    pFence) {
    StartReadObjectParentInstance(device, "vkRegisterDisplayEventEXT");
    StartReadObject(display, "vkRegisterDisplayEventEXT");
}

void ThreadSafety::PostCallRecordRegisterDisplayEventEXT(
    VkDevice                                    device,
    VkDisplayKHR                                display,
    const VkDisplayEventInfoEXT*                pDisplayEventInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFence*                                    pFence,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkRegisterDisplayEventEXT");
    FinishReadObject(display, "vkRegisterDisplayEventEXT");
}

void ThreadSafety::PreCallRecordGetSwapchainCounterEXT(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    VkSurfaceCounterFlagBitsEXT                 counter,
    uint64_t*                                   pCounterValue) {
    StartReadObjectParentInstance(device, "vkGetSwapchainCounterEXT");
    StartReadObjectParentInstance(swapchain, "vkGetSwapchainCounterEXT");
}

void ThreadSafety::PostCallRecordGetSwapchainCounterEXT(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    VkSurfaceCounterFlagBitsEXT                 counter,
    uint64_t*                                   pCounterValue,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkGetSwapchainCounterEXT");
    FinishReadObjectParentInstance(swapchain, "vkGetSwapchainCounterEXT");
}

void ThreadSafety::PreCallRecordCmdSetDiscardRectangleEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstDiscardRectangle,
    uint32_t                                    discardRectangleCount,
    const VkRect2D*                             pDiscardRectangles) {
    StartWriteObject(commandBuffer, "vkCmdSetDiscardRectangleEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetDiscardRectangleEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstDiscardRectangle,
    uint32_t                                    discardRectangleCount,
    const VkRect2D*                             pDiscardRectangles) {
    FinishWriteObject(commandBuffer, "vkCmdSetDiscardRectangleEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordSetHdrMetadataEXT(
    VkDevice                                    device,
    uint32_t                                    swapchainCount,
    const VkSwapchainKHR*                       pSwapchains,
    const VkHdrMetadataEXT*                     pMetadata) {
    StartReadObjectParentInstance(device, "vkSetHdrMetadataEXT");
    if (pSwapchains) {
        for (uint32_t index = 0; index < swapchainCount; index++) {
            StartReadObjectParentInstance(pSwapchains[index], "vkSetHdrMetadataEXT");
        }
    }
}

void ThreadSafety::PostCallRecordSetHdrMetadataEXT(
    VkDevice                                    device,
    uint32_t                                    swapchainCount,
    const VkSwapchainKHR*                       pSwapchains,
    const VkHdrMetadataEXT*                     pMetadata) {
    FinishReadObjectParentInstance(device, "vkSetHdrMetadataEXT");
    if (pSwapchains) {
        for (uint32_t index = 0; index < swapchainCount; index++) {
            FinishReadObjectParentInstance(pSwapchains[index], "vkSetHdrMetadataEXT");
        }
    }
}
// TODO - not wrapping EXT function vkSetDebugUtilsObjectNameEXT
// TODO - not wrapping EXT function vkSetDebugUtilsObjectTagEXT

void ThreadSafety::PreCallRecordQueueBeginDebugUtilsLabelEXT(
    VkQueue                                     queue,
    const VkDebugUtilsLabelEXT*                 pLabelInfo) {
    StartReadObject(queue, "vkQueueBeginDebugUtilsLabelEXT");
}

void ThreadSafety::PostCallRecordQueueBeginDebugUtilsLabelEXT(
    VkQueue                                     queue,
    const VkDebugUtilsLabelEXT*                 pLabelInfo) {
    FinishReadObject(queue, "vkQueueBeginDebugUtilsLabelEXT");
}

void ThreadSafety::PreCallRecordQueueEndDebugUtilsLabelEXT(
    VkQueue                                     queue) {
    StartReadObject(queue, "vkQueueEndDebugUtilsLabelEXT");
}

void ThreadSafety::PostCallRecordQueueEndDebugUtilsLabelEXT(
    VkQueue                                     queue) {
    FinishReadObject(queue, "vkQueueEndDebugUtilsLabelEXT");
}

void ThreadSafety::PreCallRecordQueueInsertDebugUtilsLabelEXT(
    VkQueue                                     queue,
    const VkDebugUtilsLabelEXT*                 pLabelInfo) {
    StartReadObject(queue, "vkQueueInsertDebugUtilsLabelEXT");
}

void ThreadSafety::PostCallRecordQueueInsertDebugUtilsLabelEXT(
    VkQueue                                     queue,
    const VkDebugUtilsLabelEXT*                 pLabelInfo) {
    FinishReadObject(queue, "vkQueueInsertDebugUtilsLabelEXT");
}

void ThreadSafety::PreCallRecordCmdBeginDebugUtilsLabelEXT(
    VkCommandBuffer                             commandBuffer,
    const VkDebugUtilsLabelEXT*                 pLabelInfo) {
    StartWriteObject(commandBuffer, "vkCmdBeginDebugUtilsLabelEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdBeginDebugUtilsLabelEXT(
    VkCommandBuffer                             commandBuffer,
    const VkDebugUtilsLabelEXT*                 pLabelInfo) {
    FinishWriteObject(commandBuffer, "vkCmdBeginDebugUtilsLabelEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdEndDebugUtilsLabelEXT(
    VkCommandBuffer                             commandBuffer) {
    StartWriteObject(commandBuffer, "vkCmdEndDebugUtilsLabelEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdEndDebugUtilsLabelEXT(
    VkCommandBuffer                             commandBuffer) {
    FinishWriteObject(commandBuffer, "vkCmdEndDebugUtilsLabelEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdInsertDebugUtilsLabelEXT(
    VkCommandBuffer                             commandBuffer,
    const VkDebugUtilsLabelEXT*                 pLabelInfo) {
    StartWriteObject(commandBuffer, "vkCmdInsertDebugUtilsLabelEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdInsertDebugUtilsLabelEXT(
    VkCommandBuffer                             commandBuffer,
    const VkDebugUtilsLabelEXT*                 pLabelInfo) {
    FinishWriteObject(commandBuffer, "vkCmdInsertDebugUtilsLabelEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCreateDebugUtilsMessengerEXT(
    VkInstance                                  instance,
    const VkDebugUtilsMessengerCreateInfoEXT*   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDebugUtilsMessengerEXT*                   pMessenger) {
    StartReadObjectParentInstance(instance, "vkCreateDebugUtilsMessengerEXT");
}

void ThreadSafety::PostCallRecordCreateDebugUtilsMessengerEXT(
    VkInstance                                  instance,
    const VkDebugUtilsMessengerCreateInfoEXT*   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDebugUtilsMessengerEXT*                   pMessenger,
    VkResult                                    result) {
    FinishReadObjectParentInstance(instance, "vkCreateDebugUtilsMessengerEXT");
    if (result == VK_SUCCESS) {
        CreateObjectParentInstance(*pMessenger);
    }
}

void ThreadSafety::PreCallRecordDestroyDebugUtilsMessengerEXT(
    VkInstance                                  instance,
    VkDebugUtilsMessengerEXT                    messenger,
    const VkAllocationCallbacks*                pAllocator) {
    StartReadObjectParentInstance(instance, "vkDestroyDebugUtilsMessengerEXT");
    StartWriteObjectParentInstance(messenger, "vkDestroyDebugUtilsMessengerEXT");
    // Host access to messenger must be externally synchronized
}

void ThreadSafety::PostCallRecordDestroyDebugUtilsMessengerEXT(
    VkInstance                                  instance,
    VkDebugUtilsMessengerEXT                    messenger,
    const VkAllocationCallbacks*                pAllocator) {
    FinishReadObjectParentInstance(instance, "vkDestroyDebugUtilsMessengerEXT");
    FinishWriteObjectParentInstance(messenger, "vkDestroyDebugUtilsMessengerEXT");
    DestroyObjectParentInstance(messenger);
    // Host access to messenger must be externally synchronized
}

void ThreadSafety::PreCallRecordSubmitDebugUtilsMessageEXT(
    VkInstance                                  instance,
    VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) {
    StartReadObjectParentInstance(instance, "vkSubmitDebugUtilsMessageEXT");
}

void ThreadSafety::PostCallRecordSubmitDebugUtilsMessageEXT(
    VkInstance                                  instance,
    VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) {
    FinishReadObjectParentInstance(instance, "vkSubmitDebugUtilsMessageEXT");
}

void ThreadSafety::PreCallRecordCmdSetSampleLocationsEXT(
    VkCommandBuffer                             commandBuffer,
    const VkSampleLocationsInfoEXT*             pSampleLocationsInfo) {
    StartWriteObject(commandBuffer, "vkCmdSetSampleLocationsEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetSampleLocationsEXT(
    VkCommandBuffer                             commandBuffer,
    const VkSampleLocationsInfoEXT*             pSampleLocationsInfo) {
    FinishWriteObject(commandBuffer, "vkCmdSetSampleLocationsEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordGetImageDrmFormatModifierPropertiesEXT(
    VkDevice                                    device,
    VkImage                                     image,
    VkImageDrmFormatModifierPropertiesEXT*      pProperties) {
    StartReadObjectParentInstance(device, "vkGetImageDrmFormatModifierPropertiesEXT");
    StartReadObject(image, "vkGetImageDrmFormatModifierPropertiesEXT");
}

void ThreadSafety::PostCallRecordGetImageDrmFormatModifierPropertiesEXT(
    VkDevice                                    device,
    VkImage                                     image,
    VkImageDrmFormatModifierPropertiesEXT*      pProperties,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkGetImageDrmFormatModifierPropertiesEXT");
    FinishReadObject(image, "vkGetImageDrmFormatModifierPropertiesEXT");
}

void ThreadSafety::PreCallRecordGetMemoryHostPointerPropertiesEXT(
    VkDevice                                    device,
    VkExternalMemoryHandleTypeFlagBits          handleType,
    const void*                                 pHostPointer,
    VkMemoryHostPointerPropertiesEXT*           pMemoryHostPointerProperties) {
    StartReadObjectParentInstance(device, "vkGetMemoryHostPointerPropertiesEXT");
}

void ThreadSafety::PostCallRecordGetMemoryHostPointerPropertiesEXT(
    VkDevice                                    device,
    VkExternalMemoryHandleTypeFlagBits          handleType,
    const void*                                 pHostPointer,
    VkMemoryHostPointerPropertiesEXT*           pMemoryHostPointerProperties,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkGetMemoryHostPointerPropertiesEXT");
}

void ThreadSafety::PreCallRecordGetCalibratedTimestampsEXT(
    VkDevice                                    device,
    uint32_t                                    timestampCount,
    const VkCalibratedTimestampInfoEXT*         pTimestampInfos,
    uint64_t*                                   pTimestamps,
    uint64_t*                                   pMaxDeviation) {
    StartReadObjectParentInstance(device, "vkGetCalibratedTimestampsEXT");
}

void ThreadSafety::PostCallRecordGetCalibratedTimestampsEXT(
    VkDevice                                    device,
    uint32_t                                    timestampCount,
    const VkCalibratedTimestampInfoEXT*         pTimestampInfos,
    uint64_t*                                   pTimestamps,
    uint64_t*                                   pMaxDeviation,
    VkResult                                    result) {
    FinishReadObjectParentInstance(device, "vkGetCalibratedTimestampsEXT");
}

void ThreadSafety::PreCallRecordCreateHeadlessSurfaceEXT(
    VkInstance                                  instance,
    const VkHeadlessSurfaceCreateInfoEXT*       pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface) {
    StartReadObjectParentInstance(instance, "vkCreateHeadlessSurfaceEXT");
}

void ThreadSafety::PostCallRecordCreateHeadlessSurfaceEXT(
    VkInstance                                  instance,
    const VkHeadlessSurfaceCreateInfoEXT*       pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface,
    VkResult                                    result) {
    FinishReadObjectParentInstance(instance, "vkCreateHeadlessSurfaceEXT");
    if (result == VK_SUCCESS) {
        CreateObjectParentInstance(*pSurface);
    }
}

void ThreadSafety::PreCallRecordCmdSetLineStippleEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    lineStippleFactor,
    uint16_t                                    lineStipplePattern) {
    StartWriteObject(commandBuffer, "vkCmdSetLineStippleEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetLineStippleEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    lineStippleFactor,
    uint16_t                                    lineStipplePattern) {
    FinishWriteObject(commandBuffer, "vkCmdSetLineStippleEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetCullModeEXT(
    VkCommandBuffer                             commandBuffer,
    VkCullModeFlags                             cullMode) {
    StartWriteObject(commandBuffer, "vkCmdSetCullModeEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetCullModeEXT(
    VkCommandBuffer                             commandBuffer,
    VkCullModeFlags                             cullMode) {
    FinishWriteObject(commandBuffer, "vkCmdSetCullModeEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetFrontFaceEXT(
    VkCommandBuffer                             commandBuffer,
    VkFrontFace                                 frontFace) {
    StartWriteObject(commandBuffer, "vkCmdSetFrontFaceEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetFrontFaceEXT(
    VkCommandBuffer                             commandBuffer,
    VkFrontFace                                 frontFace) {
    FinishWriteObject(commandBuffer, "vkCmdSetFrontFaceEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetPrimitiveTopologyEXT(
    VkCommandBuffer                             commandBuffer,
    VkPrimitiveTopology                         primitiveTopology) {
    StartWriteObject(commandBuffer, "vkCmdSetPrimitiveTopologyEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetPrimitiveTopologyEXT(
    VkCommandBuffer                             commandBuffer,
    VkPrimitiveTopology                         primitiveTopology) {
    FinishWriteObject(commandBuffer, "vkCmdSetPrimitiveTopologyEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetViewportWithCountEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    viewportCount,
    const VkViewport*                           pViewports) {
    StartWriteObject(commandBuffer, "vkCmdSetViewportWithCountEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetViewportWithCountEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    viewportCount,
    const VkViewport*                           pViewports) {
    FinishWriteObject(commandBuffer, "vkCmdSetViewportWithCountEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetScissorWithCountEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    scissorCount,
    const VkRect2D*                             pScissors) {
    StartWriteObject(commandBuffer, "vkCmdSetScissorWithCountEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetScissorWithCountEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    scissorCount,
    const VkRect2D*                             pScissors) {
    FinishWriteObject(commandBuffer, "vkCmdSetScissorWithCountEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdBindVertexBuffers2EXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstBinding,
    uint32_t                                    bindingCount,
    const VkBuffer*                             pBuffers,
    const VkDeviceSize*                         pOffsets,
    const VkDeviceSize*                         pSizes,
    const VkDeviceSize*                         pStrides) {
    StartWriteObject(commandBuffer, "vkCmdBindVertexBuffers2EXT");
    if (pBuffers) {
        for (uint32_t index = 0; index < bindingCount; index++) {
            StartReadObject(pBuffers[index], "vkCmdBindVertexBuffers2EXT");
        }
    }
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdBindVertexBuffers2EXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstBinding,
    uint32_t                                    bindingCount,
    const VkBuffer*                             pBuffers,
    const VkDeviceSize*                         pOffsets,
    const VkDeviceSize*                         pSizes,
    const VkDeviceSize*                         pStrides) {
    FinishWriteObject(commandBuffer, "vkCmdBindVertexBuffers2EXT");
    if (pBuffers) {
        for (uint32_t index = 0; index < bindingCount; index++) {
            FinishReadObject(pBuffers[index], "vkCmdBindVertexBuffers2EXT");
        }
    }
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetDepthTestEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    depthTestEnable) {
    StartWriteObject(commandBuffer, "vkCmdSetDepthTestEnableEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetDepthTestEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    depthTestEnable) {
    FinishWriteObject(commandBuffer, "vkCmdSetDepthTestEnableEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetDepthWriteEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    depthWriteEnable) {
    StartWriteObject(commandBuffer, "vkCmdSetDepthWriteEnableEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetDepthWriteEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    depthWriteEnable) {
    FinishWriteObject(commandBuffer, "vkCmdSetDepthWriteEnableEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetDepthCompareOpEXT(
    VkCommandBuffer                             commandBuffer,
    VkCompareOp                                 depthCompareOp) {
    StartWriteObject(commandBuffer, "vkCmdSetDepthCompareOpEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetDepthCompareOpEXT(
    VkCommandBuffer                             commandBuffer,
    VkCompareOp                                 depthCompareOp) {
    FinishWriteObject(commandBuffer, "vkCmdSetDepthCompareOpEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetDepthBoundsTestEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    depthBoundsTestEnable) {
    StartWriteObject(commandBuffer, "vkCmdSetDepthBoundsTestEnableEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetDepthBoundsTestEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    depthBoundsTestEnable) {
    FinishWriteObject(commandBuffer, "vkCmdSetDepthBoundsTestEnableEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetStencilTestEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    stencilTestEnable) {
    StartWriteObject(commandBuffer, "vkCmdSetStencilTestEnableEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetStencilTestEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    stencilTestEnable) {
    FinishWriteObject(commandBuffer, "vkCmdSetStencilTestEnableEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetStencilOpEXT(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    VkStencilOp                                 failOp,
    VkStencilOp                                 passOp,
    VkStencilOp                                 depthFailOp,
    VkCompareOp                                 compareOp) {
    StartWriteObject(commandBuffer, "vkCmdSetStencilOpEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetStencilOpEXT(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    VkStencilOp                                 failOp,
    VkStencilOp                                 passOp,
    VkStencilOp                                 depthFailOp,
    VkCompareOp                                 compareOp) {
    FinishWriteObject(commandBuffer, "vkCmdSetStencilOpEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetVertexInputEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    vertexBindingDescriptionCount,
    const VkVertexInputBindingDescription2EXT*  pVertexBindingDescriptions,
    uint32_t                                    vertexAttributeDescriptionCount,
    const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) {
    StartWriteObject(commandBuffer, "vkCmdSetVertexInputEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetVertexInputEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    vertexBindingDescriptionCount,
    const VkVertexInputBindingDescription2EXT*  pVertexBindingDescriptions,
    uint32_t                                    vertexAttributeDescriptionCount,
    const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) {
    FinishWriteObject(commandBuffer, "vkCmdSetVertexInputEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetPatchControlPointsEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    patchControlPoints) {
    StartWriteObject(commandBuffer, "vkCmdSetPatchControlPointsEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetPatchControlPointsEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    patchControlPoints) {
    FinishWriteObject(commandBuffer, "vkCmdSetPatchControlPointsEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetRasterizerDiscardEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    rasterizerDiscardEnable) {
    StartWriteObject(commandBuffer, "vkCmdSetRasterizerDiscardEnableEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetRasterizerDiscardEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    rasterizerDiscardEnable) {
    FinishWriteObject(commandBuffer, "vkCmdSetRasterizerDiscardEnableEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetDepthBiasEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    depthBiasEnable) {
    StartWriteObject(commandBuffer, "vkCmdSetDepthBiasEnableEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetDepthBiasEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    depthBiasEnable) {
    FinishWriteObject(commandBuffer, "vkCmdSetDepthBiasEnableEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetLogicOpEXT(
    VkCommandBuffer                             commandBuffer,
    VkLogicOp                                   logicOp) {
    StartWriteObject(commandBuffer, "vkCmdSetLogicOpEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetLogicOpEXT(
    VkCommandBuffer                             commandBuffer,
    VkLogicOp                                   logicOp) {
    FinishWriteObject(commandBuffer, "vkCmdSetLogicOpEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetPrimitiveRestartEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    primitiveRestartEnable) {
    StartWriteObject(commandBuffer, "vkCmdSetPrimitiveRestartEnableEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetPrimitiveRestartEnableEXT(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    primitiveRestartEnable) {
    FinishWriteObject(commandBuffer, "vkCmdSetPrimitiveRestartEnableEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PreCallRecordCmdSetColorWriteEnableEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    attachmentCount,
    const VkBool32*                             pColorWriteEnables) {
    StartWriteObject(commandBuffer, "vkCmdSetColorWriteEnableEXT");
    // Host access to commandBuffer must be externally synchronized
}

void ThreadSafety::PostCallRecordCmdSetColorWriteEnableEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    attachmentCount,
    const VkBool32*                             pColorWriteEnables) {
    FinishWriteObject(commandBuffer, "vkCmdSetColorWriteEnableEXT");
    // Host access to commandBuffer must be externally synchronized
}
