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

            BillboardComponentRequestBus::Handler::BusConnect(entityId);

            RPI::Scene* scene = RPI::Scene::GetSceneForEntityId(entityId);
            if (scene)
            {
                AZ::RPI::SceneNotificationBus::Handler::BusConnect(scene->GetId());
            }

            AZ::TransformNotificationBus::Handler::BusConnect(entityId);

            m_meshFeatureProcessor = RPI::Scene::GetFeatureProcessorForEntity<Render::MeshFeatureProcessorInterface>(entityId);
            AZ_Assert(m_meshFeatureProcessor, "MeshFeatureProcessor not available.");

            AZ::Data::Asset<AZ::RPI::MaterialAsset> materialAsset = AZ::RPI::AssetUtils::LoadAssetByProductPath<AZ::RPI::MaterialAsset>("materials/presets/pbr/metal_gold.azmaterial", AZ::RPI::AssetUtils::TraceLevel::Error);
            m_material = AZ::RPI::Material::FindOrCreate(materialAsset);
            m_modelAsset = AZ::RPI::AssetUtils::GetAssetByProductPath<AZ::RPI::ModelAsset>("materialeditor/viewportmodels/plane_1x1.azmodel", AZ::RPI::AssetUtils::TraceLevel::Assert);
            m_meshHandle = m_meshFeatureProcessor->AcquireMesh(AZ::Render::MeshHandleDescriptor{ m_modelAsset }, m_material);

            AZ::Transform entityTransform;
            AZ::TransformBus::EventResult(entityTransform, m_entityComponentIdPair.GetEntityId(), &AZ::TransformBus::Events::GetWorldTM);

            m_meshFeatureProcessor->SetTransform(m_meshHandle, entityTransform);
        }

        void BillboardComponentController::Deactivate()
        {
            AZ::TransformNotificationBus::Handler::BusDisconnect();
            AZ::RPI::SceneNotificationBus::Handler::BusDisconnect();
            BillboardComponentRequestBus::Handler::BusDisconnect();

            if (m_meshFeatureProcessor && m_meshHandle.IsValid())
            {
                m_meshFeatureProcessor->ReleaseMesh(m_meshHandle);
            }

            m_meshFeatureProcessor = nullptr;
            m_modelAsset.Release();

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
            m_meshFeatureProcessor = RPI::Scene::GetFeatureProcessorForEntity<Render::MeshFeatureProcessorInterface>(m_entityComponentIdPair.GetEntityId());
            if (!m_meshFeatureProcessor) {
                return;
            }

            AZ::Vector3 entityWorldPosition;
            AZ::TransformBus::EventResult(entityWorldPosition, m_entityComponentIdPair.GetEntityId(), &AZ::TransformBus::Events::GetWorldTranslation);

            EntityId cameraId;
            Camera::CameraSystemRequestBus::BroadcastResult(cameraId, &Camera::CameraSystemRequests::GetActiveCamera);
            AZ::Vector3 cameraWorldPosition;
            AZ::TransformBus::EventResult(cameraWorldPosition, cameraId, &AZ::TransformBus::Events::GetWorldTranslation);

            // From mesh POV, the forward axis is Z positive, even though O3DE's default is Y positive for forward axis.
            AZ::Transform transform = AZ::Transform::CreateLookAt(entityWorldPosition, cameraWorldPosition, AZ::Transform::Axis::ZPositive);
            m_meshFeatureProcessor->SetTransform(m_meshHandle, transform);
        }
    } // namespace Render
} // namespace AZ
