// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "DraggableWindow.h"

#ifdef DRAGGABLEWINDOWUMG_DraggableWindow_generated_h
#error "DraggableWindow.generated.h already included, missing '#pragma once' in DraggableWindow.h"
#endif
#define DRAGGABLEWINDOWUMG_DraggableWindow_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UClass;
class UDraggableWindow;
class UObject;
class UUserWidget;
enum class EOperatingSystemWindowStartupState : uint8;
struct FEventReply;
struct FGeometry;
struct FPointerEvent;

// ********** Begin Class UDraggableWindow *********************************************************
#define FID_build_U5M_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetChildWidget); \
	DECLARE_FUNCTION(execUpdateWindowSize); \
	DECLARE_FUNCTION(execCenterWindowToScreen); \
	DECLARE_FUNCTION(execToggleMaximize); \
	DECLARE_FUNCTION(execAddContentWidget); \
	DECLARE_FUNCTION(execAddContentWidgetClass); \
	DECLARE_FUNCTION(execCloseWindow); \
	DECLARE_FUNCTION(execInternal_OnMouseButtonDown_ResizeArea); \
	DECLARE_FUNCTION(execInternal_OnMouseButtonDown_WindowTitleBorder); \
	DECLARE_FUNCTION(execInternal_OnMouseButtonUp_WindowTitleBorder); \
	DECLARE_FUNCTION(execCreateDraggableWindowFromWidget); \
	DECLARE_FUNCTION(execCreateDraggableWindow);


#define FID_build_U5M_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h_18_CALLBACK_WRAPPERS
struct Z_Construct_UClass_UDraggableWindow_Statics;
DRAGGABLEWINDOWUMG_API UClass* Z_Construct_UClass_UDraggableWindow_NoRegister();

#define FID_build_U5M_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h_18_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUDraggableWindow(); \
	friend struct ::Z_Construct_UClass_UDraggableWindow_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend DRAGGABLEWINDOWUMG_API UClass* ::Z_Construct_UClass_UDraggableWindow_NoRegister(); \
public: \
	DECLARE_CLASS2(UDraggableWindow, UUserWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/DraggableWindowUMG"), Z_Construct_UClass_UDraggableWindow_NoRegister) \
	DECLARE_SERIALIZER(UDraggableWindow)


#define FID_build_U5M_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h_18_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	UDraggableWindow(UDraggableWindow&&) = delete; \
	UDraggableWindow(const UDraggableWindow&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UDraggableWindow); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UDraggableWindow); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UDraggableWindow) \
	NO_API virtual ~UDraggableWindow();


#define FID_build_U5M_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h_15_PROLOG
#define FID_build_U5M_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h_18_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_build_U5M_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_build_U5M_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h_18_CALLBACK_WRAPPERS \
	FID_build_U5M_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h_18_INCLASS_NO_PURE_DECLS \
	FID_build_U5M_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h_18_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UDraggableWindow;

// ********** End Class UDraggableWindow ***********************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_build_U5M_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h

// ********** Begin Enum EOperatingSystemWindowStartupState ****************************************
#define FOREACH_ENUM_EOPERATINGSYSTEMWINDOWSTARTUPSTATE(op) \
	op(EOperatingSystemWindowStartupState::Default) \
	op(EOperatingSystemWindowStartupState::StartMaximized) 

enum class EOperatingSystemWindowStartupState : uint8;
template<> struct TIsUEnumClass<EOperatingSystemWindowStartupState> { enum { Value = true }; };
template<> DRAGGABLEWINDOWUMG_NON_ATTRIBUTED_API UEnum* StaticEnum<EOperatingSystemWindowStartupState>();
// ********** End Enum EOperatingSystemWindowStartupState ******************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
