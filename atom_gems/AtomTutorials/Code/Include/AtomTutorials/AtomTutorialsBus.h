
#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace AtomTutorials
{
    class AtomTutorialsRequests
    {
    public:
        AZ_RTTI(AtomTutorialsRequests, "{802636CF-89EA-42DD-9E24-E3884C1EF9CA}");
        virtual ~AtomTutorialsRequests() = default;
        // Put your public methods here
    };
    
    class AtomTutorialsBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using AtomTutorialsRequestBus = AZ::EBus<AtomTutorialsRequests, AtomTutorialsBusTraits>;
    using AtomTutorialsInterface = AZ::Interface<AtomTutorialsRequests>;

} // namespace AtomTutorials
