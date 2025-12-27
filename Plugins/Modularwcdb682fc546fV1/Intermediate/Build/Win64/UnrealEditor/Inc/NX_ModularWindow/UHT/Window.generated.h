// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Window.h"

#ifdef NX_MODULARWINDOW_Window_generated_h
#error "Window.generated.h already included, missing '#pragma once' in Window.h"
#endif
#define NX_MODULARWINDOW_Window_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UCanvasPanelSlot;
class UUserWidget;
class UWindow;
class UWindow_Module;
struct FWindowCapabilities;

// ********** Begin Delegate FED_WindowClicked *****************************************************
#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_227_DELEGATE \
static void FED_WindowClicked_DelegateWrapper(const FMulticastScriptDelegate& ED_WindowClicked, UWindow* ClickedWindow);


// ********** End Delegate FED_WindowClicked *******************************************************

// ********** Begin Delegate FED_InteractStart *****************************************************
#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_232_DELEGATE \
static void FED_InteractStart_DelegateWrapper(const FMulticastScriptDelegate& ED_InteractStart);


// ********** End Delegate FED_InteractStart *******************************************************

// ********** Begin Delegate FED_InteractEnd *******************************************************
#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_236_DELEGATE \
static void FED_InteractEnd_DelegateWrapper(const FMulticastScriptDelegate& ED_InteractEnd);


// ********** End Delegate FED_InteractEnd *********************************************************

// ********** Begin Delegate FED_PositionStart *****************************************************
#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_243_DELEGATE \
static void FED_PositionStart_DelegateWrapper(const FMulticastScriptDelegate& ED_PositionStart);


// ********** End Delegate FED_PositionStart *******************************************************

// ********** Begin Delegate FED_PositionChanged ***************************************************
#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_247_DELEGATE \
static void FED_PositionChanged_DelegateWrapper(const FMulticastScriptDelegate& ED_PositionChanged);


// ********** End Delegate FED_PositionChanged *****************************************************

// ********** Begin Delegate FED_PositionEnd *******************************************************
#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_251_DELEGATE \
static void FED_PositionEnd_DelegateWrapper(const FMulticastScriptDelegate& ED_PositionEnd);


// ********** End Delegate FED_PositionEnd *********************************************************

// ********** Begin Delegate FED_SizeStart *********************************************************
#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_258_DELEGATE \
static void FED_SizeStart_DelegateWrapper(const FMulticastScriptDelegate& ED_SizeStart);


// ********** End Delegate FED_SizeStart ***********************************************************

// ********** Begin Delegate FED_SizeChanged *******************************************************
#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_262_DELEGATE \
static void FED_SizeChanged_DelegateWrapper(const FMulticastScriptDelegate& ED_SizeChanged);


// ********** End Delegate FED_SizeChanged *********************************************************

// ********** Begin Delegate FED_SizeEnd ***********************************************************
#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_266_DELEGATE \
static void FED_SizeEnd_DelegateWrapper(const FMulticastScriptDelegate& ED_SizeEnd);


// ********** End Delegate FED_SizeEnd *************************************************************

// ********** Begin Class UWindow ******************************************************************
#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_22_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execUpdateUIForCapabilities); \
	DECLARE_FUNCTION(execApplyCapabilitiesFromStruct); \
	DECLARE_FUNCTION(execApplyCapabilitiesFromContent); \
	DECLARE_FUNCTION(execSetWindowCapabilities); \
	DECLARE_FUNCTION(execSetCanFullscreen); \
	DECLARE_FUNCTION(execSetCanResize); \
	DECLARE_FUNCTION(execSetCanMove); \
	DECLARE_FUNCTION(execCanFullscreen); \
	DECLARE_FUNCTION(execCanResize); \
	DECLARE_FUNCTION(execCanMove); \
	DECLARE_FUNCTION(execIsAnchorPoint); \
	DECLARE_FUNCTION(execRemoveModule); \
	DECLARE_FUNCTION(execAddModule); \
	DECLARE_FUNCTION(execSetResizing); \
	DECLARE_FUNCTION(execSetDragging); \
	DECLARE_FUNCTION(execSetPositionDirect); \
	DECLARE_FUNCTION(execSetByParameterSize); \
	DECLARE_FUNCTION(execSetByParameterPosition); \
	DECLARE_FUNCTION(execSetByParameter); \
	DECLARE_FUNCTION(execSetSizeY); \
	DECLARE_FUNCTION(execSetSizeX); \
	DECLARE_FUNCTION(execSetSize); \
	DECLARE_FUNCTION(execGetSize); \
	DECLARE_FUNCTION(execSetPositionY); \
	DECLARE_FUNCTION(execSetPositionX); \
	DECLARE_FUNCTION(execSetPosition); \
	DECLARE_FUNCTION(execGetPosition); \
	DECLARE_FUNCTION(execSetZOrder); \
	DECLARE_FUNCTION(execGetZOrder); \
	DECLARE_FUNCTION(execBringToFront); \
	DECLARE_FUNCTION(execGetCanvasSlot); \
	DECLARE_FUNCTION(execUninit); \
	DECLARE_FUNCTION(execInit);


#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_22_CALLBACK_WRAPPERS
struct Z_Construct_UClass_UWindow_Statics;
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow_NoRegister();

#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_22_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUWindow(); \
	friend struct ::Z_Construct_UClass_UWindow_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend NX_MODULARWINDOW_API UClass* ::Z_Construct_UClass_UWindow_NoRegister(); \
public: \
	DECLARE_CLASS2(UWindow, UUserWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/NX_ModularWindow"), Z_Construct_UClass_UWindow_NoRegister) \
	DECLARE_SERIALIZER(UWindow)


#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_22_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UWindow(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UWindow(UWindow&&) = delete; \
	UWindow(const UWindow&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UWindow); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UWindow); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UWindow) \
	NO_API virtual ~UWindow();


#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_19_PROLOG
#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_22_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_22_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_22_CALLBACK_WRAPPERS \
	FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_22_INCLASS_NO_PURE_DECLS \
	FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h_22_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UWindow;

// ********** End Class UWindow ********************************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
