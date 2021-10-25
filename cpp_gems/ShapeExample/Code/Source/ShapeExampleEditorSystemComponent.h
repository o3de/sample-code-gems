/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <ShapeExampleSystemComponent.h>

#include <AzToolsFramework/Entity/EditorEntityContextBus.h>

namespace ShapeExample
{
    /// System component for ShapeExample editor
    class ShapeExampleEditorSystemComponent
        : public ShapeExampleSystemComponent
        , private AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = ShapeExampleSystemComponent;
    public:
        AZ_COMPONENT(ShapeExampleEditorSystemComponent, "{cb622a07-50c0-4efb-9008-ada4ef38386c}", BaseSystemComponent);
        static void Reflect(AZ::ReflectContext* context);

        ShapeExampleEditorSystemComponent();
        ~ShapeExampleEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;

        // AzToolsFramework::EditorEventsBus overrides ...
        void NotifyRegisterViews() override;
    };
} // namespace ShapeExample
