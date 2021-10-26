/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>
#include <ShapeExampleSystemComponent.h>

namespace ShapeExample
{
    class ShapeExampleModuleInterface
        : public AZ::Module
    {
    public:
        AZ_RTTI(ShapeExampleModuleInterface, "{dfe4e8bd-ee34-42d2-9936-4995d23644d9}", AZ::Module);
        AZ_CLASS_ALLOCATOR(ShapeExampleModuleInterface, AZ::SystemAllocator, 0);

        ShapeExampleModuleInterface()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                ShapeExampleSystemComponent::CreateDescriptor(),
                });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<ShapeExampleSystemComponent>(),
            };
        }
    };
}// namespace ShapeExample
