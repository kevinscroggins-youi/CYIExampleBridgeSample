#include "ExampleBridgeLocator.h"

#include "ExampleBridge.h"

#include <framework/YiPredef.h>

#define LOG_TAG "ExampleBridgeLocator"

#if defined(YI_TIZEN_NACL) || defined(YI_UWP)
#    include "ExampleBridge_Web.h"
#endif

static std::unique_ptr<ExampleBridge> s_pCachedExampleBridge = nullptr;

ExampleBridge *ExampleBridgeLocator::GetExampleBridge()
{
    if (!s_pCachedExampleBridge)
    {
#if defined(YI_TIZEN_NACL) || defined(YI_UWP)
        s_pCachedExampleBridge = std::make_unique<ExampleBridgeWeb>();
#endif

        if (s_pCachedExampleBridge)
        {
            s_pCachedExampleBridge->Init();
        }
    }

    return s_pCachedExampleBridge.get();
}
