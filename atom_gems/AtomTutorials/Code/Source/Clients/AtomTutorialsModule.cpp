
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>
#include <Clients/AtomTutorialsSystemComponent.h>

#include "AtomTutorialsSystemComponent.h"

#include <Billboard/BillboardComponent.h>

#ifdef ATOMTUTORIALS_EDITOR
#include <Tools/AtomTutorialsEditorSystemComponent.h>
#include <Billboard/EditorBillboardComponent.h>
#endif

namespace AtomTutorials
{
    class AtomTutorialsModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(AtomTutorialsModule, "{123CAB07-FB14-4CF2-A7FD-C550136194EC}", AZ::Module);
        AZ_CLASS_ALLOCATOR(AtomTutorialsModule, AZ::SystemAllocator, 0);

        AtomTutorialsModule()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                AtomTutorialsSystemComponent::CreateDescriptor(),
                AZ::Render::BillboardComponent::CreateDescriptor(),

#ifdef ATOMTUTORIALS_EDITOR
                AtomTutorialsEditorSystemComponent::CreateDescriptor(),
                AZ::Render::EditorBillboardComponent::CreateDescriptor(),
#endif
            });
        }

        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<AtomTutorialsSystemComponent>(),
#ifdef ATOMTUTORIALS_EDITOR
                azrtti_typeid<AtomTutorialsEditorSystemComponent>(),
#endif
            };
        }

    };
}// namespace AtomTutorials

AZ_DECLARE_MODULE_CLASS(Gem_AtomTutorials, AtomTutorials::AtomTutorialsModule)
