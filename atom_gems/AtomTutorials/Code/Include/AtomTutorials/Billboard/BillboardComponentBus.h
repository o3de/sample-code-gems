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
        //! GridComponentRequestBus declares an interface for configuring and interacting with the grid component
        class BillboardComponentRequests : public ComponentBus
        {
        public:
            //! Sets the dimensions of the grid in meters
            virtual void SetSize(float size) = 0;

            //! Returns the dimensions of the grid in meters
            virtual float GetSize() const = 0;
        };

        using BillboardComponentRequestBus = EBus<BillboardComponentRequests>;

        //! BillboardComponentNotificationBus notifications are triggered whenever the grid changes
        class BillboardComponentNotifications : public ComponentBus
        {
        public:
            //! Notify any handlers that the grid has been modified
            virtual void OnGridChanged(){};
        };

        using BillboardComponentNotificationBus = EBus<BillboardComponentNotifications>;

    } // namespace Render
} // namespace AZ
