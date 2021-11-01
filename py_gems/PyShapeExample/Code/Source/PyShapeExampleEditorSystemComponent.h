
#pragma once
#include <AzCore/Component/Component.h>
#include <PyShapeExample/PyShapeExampleBus.h>

#include <AzToolsFramework/Entity/EditorEntityContextBus.h>

namespace PyShapeExample
{
    /// System component for PyShapeExample editor
    class PyShapeExampleEditorSystemComponent
        : public PyShapeExampleRequestBus::Handler
        , private AzToolsFramework::EditorEvents::Bus::Handler
        , public AZ::Component
    {
    public:
        AZ_COMPONENT(PyShapeExampleEditorSystemComponent, "{a4475498-5700-4502-885b-3406dc685a4c}");
        static void Reflect(AZ::ReflectContext* context);

        PyShapeExampleEditorSystemComponent();
        ~PyShapeExampleEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate();
        void Deactivate();
    };
} // namespace PyShapeExample
