cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

if(__configure_web_assets_included)
    return()
endif()

set(__configure_web_assets_included 1)

function(configure_web_assets)
    list(APPEND TIZEN_JS_FILES "ExampleBridge.js")

    set(YI_USER_TIZEN_JS_FILES ${TIZEN_JS_FILES} PARENT_SCOPE)
endfunction()
