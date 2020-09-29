#include "ExampleBridge_TizenNaCl.h"

#include <logging/YiLogger.h>
#include <platform/YiWebBridgeLocator.h>

#define LOG_TAG "ExampleBridge"

static constexpr const char *JAVASCRIPT_FILE_LOADER_CLASS_NAME("ExampleBridge");
static constexpr const char *JAVASCRIPT_FILE_LOADER_INSTANCE_ACCESSOR_NAME("getInstance");
static constexpr uint64_t GET_IP_ADDRESS_RESPONSE_TIMEOUT_MS = 5000;

static CYIWebMessagingBridge::FutureResponse CallStaticFunction(yi::rapidjson::Document &&message, const CYIString &functionName, yi::rapidjson::Value &&functionArgumentsValue, bool *pMessageSent)
{
    return CYIWebBridgeLocator::GetWebMessagingBridge()->CallStaticFunctionWithArgs(std::move(message), JAVASCRIPT_FILE_LOADER_CLASS_NAME, functionName, std::move(functionArgumentsValue), pMessageSent);
}

static CYIWebMessagingBridge::FutureResponse CallInstanceFunction(yi::rapidjson::Document &&message, const CYIString &functionName, yi::rapidjson::Value &&functionArgumentsValue, bool *pMessageSent)
{
    return CYIWebBridgeLocator::GetWebMessagingBridge()->CallInstanceFunctionWithArgs(std::move(message), JAVASCRIPT_FILE_LOADER_CLASS_NAME, JAVASCRIPT_FILE_LOADER_INSTANCE_ACCESSOR_NAME, functionName, std::move(functionArgumentsValue), yi::rapidjson::Value(yi::rapidjson::kArrayType), pMessageSent);
}

static uint64_t RegisterEventHandler(const CYIString &eventName, CYIWebMessagingBridge::EventCallback &&eventCallback)
{
    return CYIWebBridgeLocator::GetWebMessagingBridge()->RegisterEventHandler(JAVASCRIPT_FILE_LOADER_CLASS_NAME, eventName, std::move(eventCallback));
}

static void UnregisterEventHandler(uint64_t &eventHandlerId)
{
    CYIWebBridgeLocator::GetWebMessagingBridge()->UnregisterEventHandler(eventHandlerId);
    eventHandlerId = 0;
}

ExampleBridge_TizenNaCl::ExampleBridge_TizenNaCl()
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
}

ExampleBridge_TizenNaCl::~ExampleBridge_TizenNaCl() = default;

void ExampleBridge_TizenNaCl::Init()
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

CYIString ExampleBridge_TizenNaCl::GetNickname()
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
        else {
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

bool ExampleBridge_TizenNaCl::SetNickname(const CYIString &nickname)
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
        else {
            return true;
        }
    }

    return false;
}

CYIString ExampleBridge_TizenNaCl::GetIPAddress()
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
        CYIWebMessagingBridge::Response response = futureResponse.Take(GET_IP_ADDRESS_RESPONSE_TIMEOUT_MS, &valueAssigned);

        if (!valueAssigned)
        {
            YI_LOGE(LOG_TAG, "%s did not receive a response from the web messaging bridge!", FUNCTION_NAME);
        }
        else if (response.HasError())
        {
            YI_LOGE(LOG_TAG, "%s", response.GetError()->GetMessage().GetData());
        }
        else {
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

std::vector<CYIString> ExampleBridge_TizenNaCl::GetLoadedScripts()
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
                for(uint32_t i = 0; i < pData->Size(); i++)
                {
                    if((*pData)[i].IsString())
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
