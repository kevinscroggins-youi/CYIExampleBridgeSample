#ifndef _EXAMPLE_BRIDGE_LOCATOR_H_
#define _EXAMPLE_BRIDGE_LOCATOR_H_

#include "ExampleBridge.h"

class ExampleBridgeLocator
{
public:
    static std::shared_ptr<ExampleBridge> GetExampleBridge();
};

#endif // _EXAMPLE_BRIDGE_LOCATOR_H_
