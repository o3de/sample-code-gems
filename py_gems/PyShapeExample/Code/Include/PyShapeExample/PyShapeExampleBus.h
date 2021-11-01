
#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace PyShapeExample
{
    class PyShapeExampleRequests
    {
    public:
        AZ_RTTI(PyShapeExampleRequests, "{fc0caa1d-9f70-48be-b78a-fcdc853c87e2}");
        virtual ~PyShapeExampleRequests() = default;
        // Put your public methods here
    };
    
    class PyShapeExampleBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using PyShapeExampleRequestBus = AZ::EBus<PyShapeExampleRequests, PyShapeExampleBusTraits>;
    using PyShapeExampleInterface = AZ::Interface<PyShapeExampleRequests>;

} // namespace PyShapeExample
