
#include <AzCore/Serialization/SerializeContext.h>
#include <PyShapeExampleEditorSystemComponent.h>

namespace PyShapeExample
{
    void PyShapeExampleEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<PyShapeExampleEditorSystemComponent, AZ::Component>();
        }
    }

    PyShapeExampleEditorSystemComponent::PyShapeExampleEditorSystemComponent()
    {
        if (PyShapeExampleInterface::Get() == nullptr)
        {
            PyShapeExampleInterface::Register(this);
        }
    }

    PyShapeExampleEditorSystemComponent::~PyShapeExampleEditorSystemComponent()
    {
        if (PyShapeExampleInterface::Get() == this)
        {
            PyShapeExampleInterface::Unregister(this);
        }
    }

    void PyShapeExampleEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("PyShapeExampleEditorService"));
    }

    void PyShapeExampleEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("PyShapeExampleEditorService"));
    }

    void PyShapeExampleEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void PyShapeExampleEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    void PyShapeExampleEditorSystemComponent::Activate()
    {
        PyShapeExampleRequestBus::Handler::BusConnect();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void PyShapeExampleEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        PyShapeExampleRequestBus::Handler::BusDisconnect();
    }

} // namespace PyShapeExample
