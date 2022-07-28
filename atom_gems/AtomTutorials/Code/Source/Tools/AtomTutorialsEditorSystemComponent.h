
#pragma once

#include <Clients/AtomTutorialsSystemComponent.h>

#include <AzToolsFramework/Entity/EditorEntityContextBus.h>

namespace AtomTutorials
{
    /// System component for AtomTutorials editor
    class AtomTutorialsEditorSystemComponent
        : public AtomTutorialsSystemComponent
        , private AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = AtomTutorialsSystemComponent;
    public:
        AZ_COMPONENT(AtomTutorialsEditorSystemComponent, "{DEB4D116-5835-4C7D-8130-35A69F29EB39}", BaseSystemComponent);
        static void Reflect(AZ::ReflectContext* context);

        AtomTutorialsEditorSystemComponent();
        ~AtomTutorialsEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;

        // AzToolsFramework::EditorEventsBus overrides ...
        void NotifyRegisterViews() override;
    };
} // namespace AtomTutorials
