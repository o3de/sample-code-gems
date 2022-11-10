/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <Billboard/BillboardComponentController.h>
#include <AtomTutorials/Billboard/BillboardComponentConstants.h>
#include <AzFramework/Components/ComponentAdapter.h>

namespace AZ
{
    namespace Render
    {
        class BillboardComponent final
            : public AzFramework::Components::ComponentAdapter<BillboardComponentController, BillboardComponentConfig>
        {
        public:

            using BaseClass = AzFramework::Components::ComponentAdapter<BillboardComponentController, BillboardComponentConfig>;
            AZ_COMPONENT(AZ::Render::BillboardComponent, BillboardComponentTypeId, BaseClass);

            BillboardComponent() = default;
            BillboardComponent(const BillboardComponentConfig& config);

            static void Reflect(AZ::ReflectContext* context);
        };
    } // namespace Render
} // namespace AZ
