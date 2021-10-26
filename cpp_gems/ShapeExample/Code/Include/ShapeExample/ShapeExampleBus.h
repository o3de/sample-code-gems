/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace ShapeExample
{
    class ShapeExampleRequests
    {
    public:
        AZ_RTTI(ShapeExampleRequests, "{6c820bf6-e166-4ca6-a817-68a9ee27b837}");
        virtual ~ShapeExampleRequests() = default;
        // Put your public methods here
    };
    
    class ShapeExampleBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using ShapeExampleRequestBus = AZ::EBus<ShapeExampleRequests, ShapeExampleBusTraits>;
    using ShapeExampleInterface = AZ::Interface<ShapeExampleRequests>;

} // namespace ShapeExample
