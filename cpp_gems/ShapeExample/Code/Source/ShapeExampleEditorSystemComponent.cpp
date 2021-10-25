/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/Serialization/SerializeContext.h>

#include <AzToolsFramework/API/ViewPaneOptions.h>

#include <ShapeExampleWidget.h>
#include <ShapeExampleEditorSystemComponent.h>

namespace ShapeExample
{
    void ShapeExampleEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ShapeExampleEditorSystemComponent, ShapeExampleSystemComponent>()
                ->Version(0);
        }
    }

    ShapeExampleEditorSystemComponent::ShapeExampleEditorSystemComponent() = default;

    ShapeExampleEditorSystemComponent::~ShapeExampleEditorSystemComponent() = default;

    void ShapeExampleEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("ShapeExampleEditorService"));
    }

    void ShapeExampleEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("ShapeExampleEditorService"));
    }

    void ShapeExampleEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void ShapeExampleEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void ShapeExampleEditorSystemComponent::Activate()
    {
        ShapeExampleSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void ShapeExampleEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        ShapeExampleSystemComponent::Deactivate();
    }

    void ShapeExampleEditorSystemComponent::NotifyRegisterViews()
    {
        AzToolsFramework::ViewPaneOptions options;
        options.paneRect = QRect(100, 100, 500, 350);
        options.showOnToolsToolbar = true;
        options.toolbarIcon = ":/ShapeExample/toolbar_icon.svg";

        // Register our custom widget as a dockable tool with the Editor
        AzToolsFramework::RegisterViewPane<ShapeExampleWidget>("ShapeExample", "Tools", options);
    }

} // namespace ShapeExample
