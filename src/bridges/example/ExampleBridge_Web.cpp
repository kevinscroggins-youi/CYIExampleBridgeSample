#include "ExampleBridge_Web.h"

#include <logging/YiLogger.h>
#include <platform/YiWebBridgeLocator.h>
#include <platform/YiWebFileLoader.h>
#include <utility/YiError.h>

#include <chrono>

#define LOG_TAG "ExampleBridgeWeb"

static constexpr const char *EXAMPLE_BRIDGE_CLASS_NAME = "ExampleBridge";
static constexpr const char *EXAMPLE_BRIDGE_INSTANCE_ACCESSOR_NAME = "getInstance";
static constexpr std::chrono::milliseconds GET_IP_ADDRESS_RESPONSE_TIMEOUT_MS(5000);

CYIWebMessagingBridge *ExampleBridgeWeb::GetWebMessagingBridge() const
{
    CYIWebMessagingBridge *pWebMessagingBridge = CYIWebBridgeLocator::GetWebMessagingBridge();

    if (!pWebMessagingBridge)
    {
        if (!m_pWebViewController)
        {
            YI_LOGI(LOG_TAG, "No required web view controller provided, creating instance.");

            m_pWebViewController = std::move(CYIWebViewController::CreateIfSupported());

            YI_ASSERT(m_pWebViewController, LOG_TAG, "Failed to create required web view controller.");
        }

        pWebMessagingBridge = CYIWebBridgeLocator::GetWebMessagingBridge(m_pWebViewController.get());
    }

    YI_ASSERT(pWebMessagingBridge, LOG_TAG, "Web messaging bridge is not available on this platform, or web view controller state has changed.");

    return pWebMessagingBridge;
}

CYIWebFileLoader *ExampleBridgeWeb::GetFileLoader() const
{
    CYIWebFileLoader *pWebFileLoader = GetWebMessagingBridge()->GetFileLoader();

    YI_ASSERT(pWebFileLoader, LOG_TAG, "Web file loader is not available on this platform, or web view controller state has changed.");

    return pWebFileLoader;
}

CYIWebMessagingBridge::FutureResponse ExampleBridgeWeb::CallStaticFunction(yi::rapidjson::Document &&message, const CYIString &functionName, yi::rapidjson::Value &&functionArgumentsValue, bool *pMessageSent)
{
    return GetWebMessagingBridge()->CallStaticFunctionWithArgs(std::move(message), EXAMPLE_BRIDGE_CLASS_NAME, functionName, std::move(functionArgumentsValue), pMessageSent);
}

CYIWebMessagingBridge::FutureResponse ExampleBridgeWeb::CallInstanceFunction(yi::rapidjson::Document &&message, const CYIString &functionName, yi::rapidjson::Value &&functionArgumentsValue, bool *pMessageSent)
{
    return GetWebMessagingBridge()->CallInstanceFunctionWithArgs(std::move(message), EXAMPLE_BRIDGE_CLASS_NAME, EXAMPLE_BRIDGE_INSTANCE_ACCESSOR_NAME, functionName, std::move(functionArgumentsValue), yi::rapidjson::Value(yi::rapidjson::kArrayType), pMessageSent);
}

uint64_t ExampleBridgeWeb::RegisterEventHandler(const CYIString &eventName, CYIWebMessagingBridge::EventCallback &&eventCallback)
{
    return GetWebMessagingBridge()->RegisterEventHandler(EXAMPLE_BRIDGE_CLASS_NAME, eventName, std::move(eventCallback));
}

void ExampleBridgeWeb::UnregisterEventHandler(uint64_t &eventHandlerId)
{
    GetWebMessagingBridge()->UnregisterEventHandler(eventHandlerId);
    eventHandlerId = 0;
}

bool ExampleBridgeWeb::LoadScript()
{
    static constexpr const char *SCRIPT_FILE_NAME = "ExampleBridge.js";

    CYIWebFileLoader *pFileLoader = GetWebMessagingBridge()->GetFileLoader();

    bool messageSent = false;
    CYIWebMessagingBridge::FutureResponse futureResponse = pFileLoader->LoadFile(SCRIPT_FILE_NAME, &messageSent);

    if (!messageSent)
    {
        YI_LOGE(LOG_TAG, "Failed to invoke web file loader load file function when attempting to load example bridge script.");
        return false;
    }

    bool valueAssigned = false;
    CYIWebMessagingBridge::Response response = futureResponse.Take(CYIWebMessagingBridge::DEFAULT_RESPONSE_TIMEOUT_MS, &valueAssigned);

    if (!valueAssigned)
    {
        YI_LOGE(LOG_TAG, "Did not receive a response from the web messaging bridge when attempting to load example bridge script!");
        return false;
    }

    if (response.HasError())
    {
        YI_LOGE(LOG_TAG, "%s", response.GetError()->GetStacktraceOrMessage().GetData());
        return false;
    }

    return true;
}

ExampleBridgeWeb::ExampleBridgeWeb()
    : m_sequentialNumberEventHandlerId(0)
{
    static constexpr const char *SEQUENTIAL_NUMBER_EVENT_NAME = "sequentialNumber";

    m_sequentialNumberEventHandlerId = RegisterEventHandler(SEQUENTIAL_NUMBER_EVENT_NAME, [this](yi::rapidjson::Document &&event) {
        if (event.HasMember(CYIWebMessagingBridge::EVENT_DATA_ATTRIBUTE_NAME) && event[CYIWebMessagingBridge::EVENT_DATA_ATTRIBUTE_NAME].IsInt())
        {
            SequentialNumber.Emit(event[CYIWebMessagingBridge::EVENT_DATA_ATTRIBUTE_NAME].GetInt());
        }
        else
        {
            YI_LOGE(LOG_TAG, "Invalid '%s' event data. JSON string for event: '%s'.", SEQUENTIAL_NUMBER_EVENT_NAME, CYIRapidJSONUtility::CreateStringFromValue(event).GetData());
        }
    });

    if (!LoadScript())
    {
        YI_ASSERT(false, LOG_TAG, "Failed to load example bridge script!");
    }
}

ExampleBridgeWeb::~ExampleBridgeWeb()
{
}

void ExampleBridgeWeb::Init()
{
    static constexpr const char *FUNCTION_NAME = "init";

    bool messageSent = false;
    CYIWebMessagingBridge::FutureResponse futureResponse = CallInstanceFunction(yi::rapidjson::Document(), FUNCTION_NAME, yi::rapidjson::Value(yi::rapidjson::kArrayType), &messageSent);

    if (!messageSent)
    {
        YI_LOGE(LOG_TAG, "Failed to invoke %s function.", FUNCTION_NAME);
    }
    else
    {
        bool valueAssigned = false;
        CYIWebMessagingBridge::Response response = futureResponse.Take(CYIWebMessagingBridge::DEFAULT_RESPONSE_TIMEOUT_MS, &valueAssigned);

        if (!valueAssigned)
        {
            YI_LOGE(LOG_TAG, "%s did not receive a response from the web messaging bridge!", FUNCTION_NAME);
        }
        else if (response.HasError())
        {
            YI_LOGE(LOG_TAG, "%s", response.GetError()->GetMessage().GetData());
        }
    }
}

CYIString ExampleBridgeWeb::GetNickname()
{
    static constexpr const char *FUNCTION_NAME = "getNickname";

    bool messageSent = false;
    CYIWebMessagingBridge::FutureResponse futureResponse = CallInstanceFunction(yi::rapidjson::Document(), FUNCTION_NAME, yi::rapidjson::Value(yi::rapidjson::kArrayType), &messageSent);

    if (!messageSent)
    {
        YI_LOGE(LOG_TAG, "Failed to invoke %s function.", FUNCTION_NAME);
    }
    else
    {
        bool valueAssigned = false;
        CYIWebMessagingBridge::Response response = futureResponse.Take(CYIWebMessagingBridge::DEFAULT_RESPONSE_TIMEOUT_MS, &valueAssigned);

        if (!valueAssigned)
        {
            YI_LOGE(LOG_TAG, "%s did not receive a response from the web messaging bridge!", FUNCTION_NAME);
        }
        else if (response.HasError())
        {
            YI_LOGE(LOG_TAG, "%s", response.GetError()->GetMessage().GetData());
        }
        else
        {
            const yi::rapidjson::Value *pData = response.GetResult();

            if (!pData->IsString())
            {
                YI_LOGE(LOG_TAG, "%s expected a string type for result, received %s. JSON string for result: %s", FUNCTION_NAME, CYIRapidJSONUtility::TypeToString(pData->GetType()).GetData(), CYIRapidJSONUtility::CreateStringFromValue(*pData).GetData());
            }
            else
            {
                return pData->GetString();
            }
        }
    }

    return CYIString();
}

bool ExampleBridgeWeb::SetNickname(const CYIString &nickname)
{
    static constexpr const char *FUNCTION_NAME = "setNickname";

    bool messageSent = false;

    yi::rapidjson::Document command(yi::rapidjson::kObjectType);
    yi::rapidjson::Value arguments(yi::rapidjson::kArrayType);
    yi::rapidjson::MemoryPoolAllocator<yi::rapidjson::CrtAllocator> &allocator = command.GetAllocator();

    yi::rapidjson::Value nicknameValue(nickname.GetData(), allocator);
    arguments.PushBack(nicknameValue, allocator);

    CYIWebMessagingBridge::FutureResponse futureResponse = CallInstanceFunction(std::move(command), FUNCTION_NAME, std::move(arguments), &messageSent);

    if (!messageSent)
    {
        YI_LOGE(LOG_TAG, "Failed to invoke %s function.", FUNCTION_NAME);
    }
    else
    {
        bool valueAssigned = false;
        CYIWebMessagingBridge::Response response = futureResponse.Take(CYIWebMessagingBridge::DEFAULT_RESPONSE_TIMEOUT_MS, &valueAssigned);

        if (!valueAssigned)
        {
            YI_LOGE(LOG_TAG, "%s did not receive a response from the web messaging bridge!", FUNCTION_NAME);
        }
        else if (response.HasError())
        {
            YI_LOGE(LOG_TAG, "%s", response.GetError()->GetMessage().GetData());
        }
        else
        {
            return true;
        }
    }

    return false;
}

CYIString ExampleBridgeWeb::GetIPAddress()
{
    static constexpr const char *FUNCTION_NAME = "getIPAddress";

    bool messageSent = false;
    CYIWebMessagingBridge::FutureResponse futureResponse = CallInstanceFunction(yi::rapidjson::Document(), FUNCTION_NAME, yi::rapidjson::Value(yi::rapidjson::kArrayType), &messageSent);

    if (!messageSent)
    {
        YI_LOGE(LOG_TAG, "Failed to invoke %s function.", FUNCTION_NAME);
    }
    else
    {
        bool valueAssigned = false;
        CYIWebMessagingBridge::Response response = futureResponse.Take(GET_IP_ADDRESS_RESPONSE_TIMEOUT_MS.count(), &valueAssigned);

        if (!valueAssigned)
        {
            YI_LOGE(LOG_TAG, "%s did not receive a response from the web messaging bridge!", FUNCTION_NAME);
        }
        else if (response.HasError())
        {
            YI_LOGE(LOG_TAG, "%s", response.GetError()->GetMessage().GetData());
        }
        else
        {
            const yi::rapidjson::Value *pData = response.GetResult();

            if (!pData->IsString())
            {
                YI_LOGE(LOG_TAG, "%s expected a string type for result, received %s. JSON string for result: %s", FUNCTION_NAME, CYIRapidJSONUtility::TypeToString(pData->GetType()).GetData(), CYIRapidJSONUtility::CreateStringFromValue(*pData).GetData());
            }
            else
            {
                return pData->GetString();
            }
        }
    }

    return CYIString();
}

std::vector<CYIString> ExampleBridgeWeb::GetLoadedScripts()
{
    static constexpr const char *FUNCTION_NAME = "getLoadedScripts";

    std::vector<CYIString> loadedScripts;

    bool messageSent = false;
    CYIWebMessagingBridge::FutureResponse futureResponse = CallStaticFunction(yi::rapidjson::Document(), FUNCTION_NAME, yi::rapidjson::Value(yi::rapidjson::kArrayType), &messageSent);

    if (!messageSent)
    {
        YI_LOGE(LOG_TAG, "Failed to invoke %s function.", FUNCTION_NAME);
    }
    else
    {
        bool valueAssigned = false;
        CYIWebMessagingBridge::Response response = futureResponse.Take(CYIWebMessagingBridge::DEFAULT_RESPONSE_TIMEOUT_MS, &valueAssigned);

        if (!valueAssigned)
        {
            YI_LOGE(LOG_TAG, "GetLoadedScripts did not receive a response from the web messaging bridge!");
        }
        else if (response.HasError())
        {
            YI_LOGE(LOG_TAG, "%s", response.GetError()->GetMessage().GetData());
        }
        else
        {
            const yi::rapidjson::Value *pData = response.GetResult();

            if (!pData->IsArray())
            {
                YI_LOGE(LOG_TAG, "GetLoadedScripts expected an array type for result, received %s. JSON string for result: %s", CYIRapidJSONUtility::TypeToString(pData->GetType()).GetData(), CYIRapidJSONUtility::CreateStringFromValue(*pData).GetData());
            }
            else
            {
                for (uint32_t i = 0; i < pData->Size(); i++)
                {
                    if ((*pData)[i].IsString())
                    {
                        loadedScripts.emplace_back((*pData)[i].GetString());
                    }
                    else
                    {
                        YI_LOGW(LOG_TAG, "GetLoadedScripts encountered an invalid script name at index %u, expected string but received %s. JSON string for element: %s", i, CYIRapidJSONUtility::TypeToString((*pData)[i].GetType()).GetData(), CYIRapidJSONUtility::CreateStringFromValue((*pData)[i]).GetData());
                    }
                }
            }
        }
    }

    return loadedScripts;
}
