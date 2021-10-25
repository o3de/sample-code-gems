/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <ShapeExampleSystemComponent.h>

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>

namespace ShapeExample
{
    void ShapeExampleSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<ShapeExampleSystemComponent, AZ::Component>()
                ->Version(0)
                ;

            if (AZ::EditContext* ec = serialize->GetEditContext())
            {
                ec->Class<ShapeExampleSystemComponent>("ShapeExample", "[Description of functionality provided by this System Component]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ;
            }
        }
    }

    void ShapeExampleSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("ShapeExampleService"));
    }

    void ShapeExampleSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("ShapeExampleService"));
    }

    void ShapeExampleSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void ShapeExampleSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    ShapeExampleSystemComponent::ShapeExampleSystemComponent()
    {
        if (ShapeExampleInterface::Get() == nullptr)
        {
            ShapeExampleInterface::Register(this);
        }
    }

    ShapeExampleSystemComponent::~ShapeExampleSystemComponent()
    {
        if (ShapeExampleInterface::Get() == this)
        {
            ShapeExampleInterface::Unregister(this);
        }
    }

    void ShapeExampleSystemComponent::Init()
    {
    }

    void ShapeExampleSystemComponent::Activate()
    {
        ShapeExampleRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
    }

    void ShapeExampleSystemComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        ShapeExampleRequestBus::Handler::BusDisconnect();
    }

    void ShapeExampleSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }

} // namespace ShapeExample
