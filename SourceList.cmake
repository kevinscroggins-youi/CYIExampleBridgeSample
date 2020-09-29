# =============================================================================
# © You i Labs Inc. 2000-2020. All rights reserved.
set(SOURCE_TIZEN-NACL
    src/bridges/example/ExampleBridge_TizenNaCl.cpp
)

set(HEADERS_TIZEN-NACL
    src/bridges/example/ExampleBridge_TizenNaCl.h
)

set(YI_PROJECT_SOURCE
    src/ExampleBridgeSampleApp.cpp
    src/ExampleBridgeSampleAppFactory.cpp
    src/bridges/example/ExampleBridge.cpp
    src/bridges/example/ExampleBridgeLocator.cpp
    ${SOURCE_${YI_PLATFORM_UPPER}}
)

set(YI_PROJECT_HEADERS
    src/ExampleBridgeSampleApp.h
    src/bridges/example/ExampleBridge.h
    src/bridges/example/ExampleBridgeLocator.h
    ${HEADERS_${YI_PLATFORM_UPPER}}
)
