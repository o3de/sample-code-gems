/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <ShapeExampleModuleInterface.h>
#include <ShapeExampleEditorSystemComponent.h>

void InitShapeExampleResources()
{
    // We must register our Qt resources (.qrc file) since this is being loaded from a separate module (gem)
    Q_INIT_RESOURCE(ShapeExample);
}

namespace ShapeExample
{
    class ShapeExampleEditorModule
        : public ShapeExampleModuleInterface
    {
    public:
        AZ_RTTI(ShapeExampleEditorModule, "{c967d26f-d78b-4900-b9cc-a60ac8216d1a}", ShapeExampleModuleInterface);
        AZ_CLASS_ALLOCATOR(ShapeExampleEditorModule, AZ::SystemAllocator, 0);

        ShapeExampleEditorModule()
        {
            InitShapeExampleResources();

            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                ShapeExampleEditorSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<ShapeExampleEditorSystemComponent>(),
            };
        }
    };
}// namespace ShapeExample

AZ_DECLARE_MODULE_CLASS(Gem_ShapeExample, ShapeExample::ShapeExampleEditorModule)
