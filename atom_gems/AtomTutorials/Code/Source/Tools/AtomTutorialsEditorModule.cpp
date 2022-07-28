
#include <AtomTutorialsModuleInterface.h>
#include "AtomTutorialsEditorSystemComponent.h"

void InitAtomTutorialsResources()
{
    // We must register our Qt resources (.qrc file) since this is being loaded from a separate module (gem)
    Q_INIT_RESOURCE(AtomTutorials);
}

namespace AtomTutorials
{
    class AtomTutorialsEditorModule
        : public AtomTutorialsModuleInterface
    {
    public:
        AZ_RTTI(AtomTutorialsEditorModule, "{C96BD29A-15A9-4C5F-8076-4C4DB113A5FE}", AtomTutorialsModuleInterface);
        AZ_CLASS_ALLOCATOR(AtomTutorialsEditorModule, AZ::SystemAllocator, 0);

        AtomTutorialsEditorModule()
        {
            InitAtomTutorialsResources();

            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                AtomTutorialsEditorSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<AtomTutorialsEditorSystemComponent>(),
            };
        }
    };
}// namespace AtomTutorials

AZ_DECLARE_MODULE_CLASS(Gem_AtomTutorials, AtomTutorials::AtomTutorialsEditorModule)
