// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "WindowContentInterface.h"

#ifdef NX_MODULARWINDOW_WindowContentInterface_generated_h
#error "WindowContentInterface.generated.h already included, missing '#pragma once' in WindowContentInterface.h"
#endif
#define NX_MODULARWINDOW_WindowContentInterface_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FWindowCapabilities;

// ********** Begin ScriptStruct FWindowCapabilities ***********************************************
struct Z_Construct_UScriptStruct_FWindowCapabilities_Statics;
#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h_12_GENERATED_BODY \
	friend struct ::Z_Construct_UScriptStruct_FWindowCapabilities_Statics; \
	NX_MODULARWINDOW_API static class UScriptStruct* StaticStruct();


struct FWindowCapabilities;
// ********** End ScriptStruct FWindowCapabilities *************************************************

// ********** Begin Interface UWindowContentInterface **********************************************
#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h_39_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetWindowCapabilities);


#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h_39_CALLBACK_WRAPPERS
struct Z_Construct_UClass_UWindowContentInterface_Statics;
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindowContentInterface_NoRegister();

#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h_39_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NX_MODULARWINDOW_API UWindowContentInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UWindowContentInterface(UWindowContentInterface&&) = delete; \
	UWindowContentInterface(const UWindowContentInterface&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NX_MODULARWINDOW_API, UWindowContentInterface); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UWindowContentInterface); \
	DEFINE_ABSTRACT_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UWindowContentInterface) \
	virtual ~UWindowContentInterface() = default;


#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h_39_GENERATED_UINTERFACE_BODY() \
private: \
	static void StaticRegisterNativesUWindowContentInterface(); \
	friend struct ::Z_Construct_UClass_UWindowContentInterface_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend NX_MODULARWINDOW_API UClass* ::Z_Construct_UClass_UWindowContentInterface_NoRegister(); \
public: \
	DECLARE_CLASS2(UWindowContentInterface, UInterface, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Interface), CASTCLASS_None, TEXT("/Script/NX_ModularWindow"), Z_Construct_UClass_UWindowContentInterface_NoRegister) \
	DECLARE_SERIALIZER(UWindowContentInterface)


#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h_39_GENERATED_BODY \
	PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h_39_GENERATED_UINTERFACE_BODY() \
	FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h_39_ENHANCED_CONSTRUCTORS \
private: \
	PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h_39_INCLASS_IINTERFACE_NO_PURE_DECLS \
protected: \
	virtual ~IWindowContentInterface() {} \
public: \
	typedef UWindowContentInterface UClassType; \
	typedef IWindowContentInterface ThisClass; \
	static FWindowCapabilities Execute_GetWindowCapabilities(const UObject* O); \
	virtual UObject* _getUObject() const { return nullptr; }


#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h_36_PROLOG
#define FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h_44_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h_39_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h_39_CALLBACK_WRAPPERS \
	FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h_39_INCLASS_IINTERFACE_NO_PURE_DECLS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UWindowContentInterface;

// ********** End Interface UWindowContentInterface ************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
