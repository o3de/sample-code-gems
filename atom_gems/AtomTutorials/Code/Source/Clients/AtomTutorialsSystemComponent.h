
#pragma once

#include <AzCore/Component/Component.h>

namespace AtomTutorials
{
    class AtomTutorialsSystemComponent
        : public AZ::Component
    {
    public:
        AZ_COMPONENT(AtomTutorialsSystemComponent, "{301F9A4A-0D92-4E7C-9D8B-41DF80624A18}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

    protected:

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
    };

} // namespace AtomTutorials
