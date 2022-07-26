
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>
#include <Clients/AtomTutorialsSystemComponent.h>

namespace AtomTutorials
{
    class AtomTutorialsModuleInterface
        : public AZ::Module
    {
    public:
        AZ_RTTI(AtomTutorialsModuleInterface, "{553B66F8-E05B-450B-81D2-3D5139B60B8A}", AZ::Module);
        AZ_CLASS_ALLOCATOR(AtomTutorialsModuleInterface, AZ::SystemAllocator, 0);

        AtomTutorialsModuleInterface()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                AtomTutorialsSystemComponent::CreateDescriptor(),
                });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<AtomTutorialsSystemComponent>(),
            };
        }
    };
}// namespace AtomTutorials
