/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/Math/Color.h>

namespace AZ
{
    namespace Render
    {
        //! BillboardComponentRequestBus declares an interface for configuring and interacting with the billboard component
        class BillboardComponentRequests : public ComponentBus
        {
        public:
        };

        using BillboardComponentRequestBus = EBus<BillboardComponentRequests>;

        class BillboardComponentNotifications : public ComponentBus
        {
        public:
        };

        using BillboardComponentNotificationBus = EBus<BillboardComponentNotifications>;

    } // namespace Render
} // namespace AZ
