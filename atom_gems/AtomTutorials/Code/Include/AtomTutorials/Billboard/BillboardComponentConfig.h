/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/Math/Color.h>

namespace AZ
{
    namespace Render
    {
        //! Common settings for GridComponents and EditorGridComponent.
        class BillboardComponentConfig final
            : public ComponentConfig
        {
        public:
            AZ_RTTI(BillboardComponentConfig, "{864A9EB9-0ABB-47B0-AA2A-4E96840C8FD9}", ComponentConfig);
            AZ_CLASS_ALLOCATOR(BillboardComponentConfig, SystemAllocator, 0);

            static void Reflect(ReflectContext* context);

            float m_gridSize = 32.0f;
            AZ::Color m_axisColor = AZ::Color(0.0f, 0.0f, 1.0f, 1.0f);

            float m_primarySpacing = 1.0f;
            AZ::Color m_primaryColor = AZ::Color(0.25f, 0.25f, 0.25f, 1.0f);

            float m_secondarySpacing = 0.25f;
            AZ::Color m_secondaryColor = AZ::Color(0.5f, 0.5f, 0.5f, 1.0f);
        };
    } // namespace Render
} // namespace AZ
