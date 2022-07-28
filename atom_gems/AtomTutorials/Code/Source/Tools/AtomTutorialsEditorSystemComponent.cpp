
#include <AzCore/Serialization/SerializeContext.h>

#include <AzToolsFramework/API/ViewPaneOptions.h>

#include "AtomTutorialsWidget.h"
#include "AtomTutorialsEditorSystemComponent.h"

namespace AtomTutorials
{
    void AtomTutorialsEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AtomTutorialsEditorSystemComponent, AtomTutorialsSystemComponent>()
                ->Version(0);
        }
    }

    AtomTutorialsEditorSystemComponent::AtomTutorialsEditorSystemComponent() = default;

    AtomTutorialsEditorSystemComponent::~AtomTutorialsEditorSystemComponent() = default;

    void AtomTutorialsEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("AtomTutorialsEditorService"));
    }

    void AtomTutorialsEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("AtomTutorialsEditorService"));
    }

    void AtomTutorialsEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void AtomTutorialsEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void AtomTutorialsEditorSystemComponent::Activate()
    {
        AtomTutorialsSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void AtomTutorialsEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        AtomTutorialsSystemComponent::Deactivate();
    }

    void AtomTutorialsEditorSystemComponent::NotifyRegisterViews()
    {
        AzToolsFramework::ViewPaneOptions options;
        options.paneRect = QRect(100, 100, 500, 400);
        options.showOnToolsToolbar = true;
        options.toolbarIcon = ":/AtomTutorials/toolbar_icon.svg";

        // Register our custom widget as a dockable tool with the Editor under an Examples sub-menu
        AzToolsFramework::RegisterViewPane<AtomTutorialsWidget>("AtomTutorials", "Examples", options);
    }

} // namespace AtomTutorials
