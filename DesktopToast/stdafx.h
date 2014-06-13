

#pragma once

#include <sdkddkver.h>

#include <Windows.h>  //this is the base which includes all other windows header files.
#include <sal.h>   //sal annotation
#include <Psapi.h>   //process status api
#include <strsafe.h>  // safe string functions to override the existing unsafe C funtions.
#include <objbase.h>  //Component object model (COM) defintions.
#include <ShObjIdl.h>  //shell object library
#include <propvarutil.h>  //Variant and PropVariant helpers
#include <functiondiscoverykeys.h>
#include <intsafe.h> //helper to prevent integer overflow bugs
#include <guiddef.h>  // guid definition

#include <roapi.h>  //winrt api for com 
#include <wrl\client.h>   //windows runtime library helpers for client eg. comptr, ActivateInstance
#include <wrl\implements.h>  //wrl helper for RuntimeClass , Make and composable types
#include <windows.ui.notifications.h>  //notifications api eg. toast


#include "DesktopToastsSample.h"
#include "StringReferenceWrapper.h"
#include "ToastEventHandler.h"