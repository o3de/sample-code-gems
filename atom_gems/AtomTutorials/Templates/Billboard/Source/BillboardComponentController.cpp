/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <Atom/RPI.Public/Scene.h>
#include <Atom/Utils/Utils.h>
#include <AtomTutorials/Billboard/BillboardComponentConstants.h>
#include <AzCore/Component/TransformBus.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <Billboard/BillboardComponentController.h>

namespace AZ
{
    namespace Render
    {
        void BillboardComponentController::Reflect(ReflectContext* context)
        {
            BillboardComponentConfig::Reflect(context);

            if (SerializeContext* serializeContext = azrtti_cast<SerializeContext*>(context))
            {
                serializeContext->Class<BillboardComponentController>()
                    ->Version(0)
                    ->Field("Configuration", &BillboardComponentController::m_configuration)
                    ;
            }

            if (AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
            {
                behaviorContext->EBus<BillboardComponentRequestBus>("BillboardComponentRequestBus")
                    ;
            }
        }

        void BillboardComponentController::GetProvidedServices(ComponentDescriptor::DependencyArrayType& provided)
        {
            provided.push_back(AZ_CRC_CE("BillboardService"));
        }

        void BillboardComponentController::GetIncompatibleServices(ComponentDescriptor::DependencyArrayType& incompatible)
        {
            incompatible.push_back(AZ_CRC_CE("BillboardService"));
        }

        BillboardComponentController::BillboardComponentController(const BillboardComponentConfig& config)
            : m_configuration(config)
        {
        }

        void BillboardComponentController::Activate(const AZ::EntityComponentIdPair& entityComponentIdPair)
        {
            const AZ::EntityId entityId = entityComponentIdPair.GetEntityId();
            m_entityComponentIdPair = entityComponentIdPair;

            RPI::Scene* scene = RPI::Scene::GetSceneForEntityId(entityId);
            if (scene)
            {
                AZ::RPI::SceneNotificationBus::Handler::BusConnect(scene->GetId());
            }
        }

        void BillboardComponentController::Deactivate()
        {
            AZ::RPI::SceneNotificationBus::Handler::BusDisconnect();

            m_entityComponentIdPair = AZ::EntityComponentIdPair(AZ::EntityId(), AZ::InvalidComponentId);
        }

        void BillboardComponentController::SetConfiguration(const BillboardComponentConfig& config)
        {
            m_configuration = config;
        }

        const BillboardComponentConfig& BillboardComponentController::GetConfiguration() const
        {
            return m_configuration;
        }


        void BillboardComponentController::OnBeginPrepareRender()
        {
            m_meshFeatureProcessor = RPI::Scene::GetFeatureProcessorForEntity<Render::MeshFeatureProcessorInterface>(m_entityComponentIdPair.GetEntityId());
            if (!m_meshFeatureProcessor) {
                return;
            }

            SetBillboardTransform();
        }

        void BillboardComponentController::OnTransformChanged(const Transform& local, const Transform& world)
        {
            AZ_UNUSED(local);
            AZ_UNUSED(world);
            
            SetBillboardTransform();
        }

        void BillboardComponentController::SetBillboardTransform()
        {
            
        }
    } // namespace Render
} // namespace AZ
