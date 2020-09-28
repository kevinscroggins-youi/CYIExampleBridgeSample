// © You i Labs Inc. 2000-2020. All rights reserved.
#ifndef _TEMPLATE_PROJECT_APP_H_
#define _TEMPLATE_PROJECT_APP_H_

#include <framework/YiApp.h>

/*!
 \defgroup templateproject Template Project
 \brief An empty You.i Engine SDK project.
 \details This project loads a basic composition and is the starting point for any You.i Engine SDK application.
 */

#if defined(DOXY)
// This namespace provides a unique path to each class in the samples documentation.
namespace yi::documentation::templateproject
{
#endif

/*!
 \ingroup templateproject
*/
class TemplateProjectApp : public CYIApp
{
public:
    TemplateProjectApp();
    virtual ~TemplateProjectApp();

    /*!
     \details Application set up and initialization occurs here. This sample minimally loads and displays an exported layout.
     
     \snippet TemplateProjectApp.cpp UserInit
     */
    virtual bool UserInit() override;

    /*!
     \details Called when both engine and user initialization has completed. UserStart() will be the first thing called when the application is ready to begin. 
     */
    virtual bool UserStart() override;

    /*!
     \details Called on each frame before the update of the scene tree. This function may be used to drive other time-dependent components of an application which are not managed by You.i Engine. 
     */
    virtual void UserUpdate() override;
};

#if defined(DOXY)
}
#endif

#endif
