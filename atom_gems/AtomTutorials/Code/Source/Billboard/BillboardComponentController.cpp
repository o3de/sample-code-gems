/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <Atom/RPI.Public/AuxGeom/AuxGeomDraw.h>
#include <Atom/RPI.Public/AuxGeom/AuxGeomFeatureProcessorInterface.h>
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

            BillboardComponentRequestBus::Handler::BusConnect(entityId);
            AZ::TransformNotificationBus::Handler::BusConnect(entityId);

            m_meshFeatureProcessor = RPI::Scene::GetFeatureProcessorForEntity<Render::MeshFeatureProcessorInterface>(entityId);
            AZ_Assert(m_meshFeatureProcessor, "MeshFeatureProcessor not available.");

            AZ::Data::Asset<AZ::RPI::MaterialAsset> materialAsset = AZ::RPI::AssetUtils::LoadAssetByProductPath<AZ::RPI::MaterialAsset>("assets/explosion.azmaterial", AZ::RPI::AssetUtils::TraceLevel::Error);
            m_material = AZ::RPI::Material::FindOrCreate(materialAsset);
            m_modelAsset = AZ::RPI::AssetUtils::GetAssetByProductPath<AZ::RPI::ModelAsset>("materialeditor/viewportmodels/plane_1x1.azmodel", AZ::RPI::AssetUtils::TraceLevel::Assert);
            m_meshHandle = m_meshFeatureProcessor->AcquireMesh(AZ::Render::MeshHandleDescriptor{ m_modelAsset }, m_material);

            m_meshFeatureProcessor->SetTransform(m_meshHandle, AZ::Transform::CreateIdentity());
        }

        void BillboardComponentController::Deactivate()
        {
            if (m_meshFeatureProcessor && m_meshHandle.IsValid())
            {
                m_meshFeatureProcessor->ReleaseMesh(m_meshHandle);
            }

            AZ::TransformNotificationBus::Handler::BusDisconnect();
            BillboardComponentRequestBus::Handler::BusDisconnect();
            AZ::RPI::SceneNotificationBus::Handler::BusDisconnect();

            m_meshFeatureProcessor = nullptr;
            m_entityComponentIdPair = AZ::EntityComponentIdPair(AZ::EntityId(), AZ::InvalidComponentId);
            m_modelAsset.Release();
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

            BuildGrid();
        }

        void BillboardComponentController::OnTransformChanged(const Transform& local, const Transform& world)
        {
            AZ_UNUSED(local);
            AZ_UNUSED(world);
            
            BuildGrid();
        }

        void BillboardComponentController::BuildGrid()
        {
            EntityId cam;
            Camera::CameraSystemRequestBus::BroadcastResult(cam, &Camera::CameraSystemRequests::GetActiveCamera);
            AZ::Vector3 cameraWorldPosition;
            AZ::TransformBus::EventResult(cameraWorldPosition, cam, &AZ::TransformBus::Events::GetWorldTranslation);

            AZ::Vector3 myPosition;
            AZ::TransformBus::EventResult(myPosition, m_entityComponentIdPair.GetEntityId(), &AZ::TransformBus::Events::GetWorldTranslation);

            // From mesh POV, the forward axis is Z positive, even though O3DE's default is Y positive for forward axis.
            AZ::Transform tf = AZ::Transform::CreateLookAt(myPosition, cameraWorldPosition, AZ::Transform::Axis::ZPositive);
            m_meshFeatureProcessor->SetTransform(m_meshHandle, tf);
        }
    } // namespace Render
} // namespace AZ
