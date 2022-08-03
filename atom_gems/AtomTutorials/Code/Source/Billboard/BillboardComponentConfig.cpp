/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AtomTutorials/Billboard/BillboardComponentConfig.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace AZ
{
    namespace Render
    {
        void BillboardComponentConfig::Reflect(ReflectContext* context)
        {
            if (AZ::SerializeContext* serializeContext = azrtti_cast<SerializeContext*>(context))
            {
                serializeContext->Class<BillboardComponentConfig, ComponentConfig>()
                    ->Version(0)
                    ->Field("gridSize", &BillboardComponentConfig::m_gridSize)
                    ;
            }
        }
    } // namespace Render
} // namespace AZ
