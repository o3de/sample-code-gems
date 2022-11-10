/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <Billboard/BillboardComponent.h>
#include <AtomTutorials/Billboard/BillboardComponentConstants.h>
#include <AtomTutorials/Billboard/BillboardComponentBus.h>
#include <Atom/Feature/Utils/EditorRenderComponentAdapter.h>

namespace AZ
{
    namespace Render
    {
        //! In-editor grid component
        class EditorBillboardComponent final
            : public EditorRenderComponentAdapter<BillboardComponentController, BillboardComponent, BillboardComponentConfig>
        {
        public:
            using BaseClass = EditorRenderComponentAdapter<BillboardComponentController, BillboardComponent, BillboardComponentConfig>;
            AZ_EDITOR_COMPONENT(AZ::Render::EditorBillboardComponent, EditorBillboardComponentTypeId, BaseClass);

            static void Reflect(AZ::ReflectContext* context);

            EditorBillboardComponent() = default;
            EditorBillboardComponent(const BillboardComponentConfig& config);
        };
    } // namespace Render
} // namespace AZ
