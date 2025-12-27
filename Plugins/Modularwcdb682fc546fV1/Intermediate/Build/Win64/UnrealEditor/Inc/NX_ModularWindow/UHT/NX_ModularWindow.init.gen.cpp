// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNX_ModularWindow_init() {}
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");	NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_InteractEnd__DelegateSignature();
	NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_InteractStart__DelegateSignature();
	NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_PositionChanged__DelegateSignature();
	NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_PositionEnd__DelegateSignature();
	NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_PositionStart__DelegateSignature();
	NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_SizeChanged__DelegateSignature();
	NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_SizeEnd__DelegateSignature();
	NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_SizeStart__DelegateSignature();
	NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature();
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_NX_ModularWindow;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_NX_ModularWindow()
	{
		if (!Z_Registration_Info_UPackage__Script_NX_ModularWindow.OuterSingleton)
		{
		static UObject* (*const SingletonFuncArray[])() = {
			(UObject* (*)())Z_Construct_UDelegateFunction_UWindow_ED_InteractEnd__DelegateSignature,
			(UObject* (*)())Z_Construct_UDelegateFunction_UWindow_ED_InteractStart__DelegateSignature,
			(UObject* (*)())Z_Construct_UDelegateFunction_UWindow_ED_PositionChanged__DelegateSignature,
			(UObject* (*)())Z_Construct_UDelegateFunction_UWindow_ED_PositionEnd__DelegateSignature,
			(UObject* (*)())Z_Construct_UDelegateFunction_UWindow_ED_PositionStart__DelegateSignature,
			(UObject* (*)())Z_Construct_UDelegateFunction_UWindow_ED_SizeChanged__DelegateSignature,
			(UObject* (*)())Z_Construct_UDelegateFunction_UWindow_ED_SizeEnd__DelegateSignature,
			(UObject* (*)())Z_Construct_UDelegateFunction_UWindow_ED_SizeStart__DelegateSignature,
			(UObject* (*)())Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature,
		};
		static const UECodeGen_Private::FPackageParams PackageParams = {
			"/Script/NX_ModularWindow",
			SingletonFuncArray,
			UE_ARRAY_COUNT(SingletonFuncArray),
			PKG_CompiledIn | 0x00000000,
			0xF240109F,
			0xB6E53087,
			METADATA_PARAMS(0, nullptr)
		};
		UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_NX_ModularWindow.OuterSingleton, PackageParams);
	}
	return Z_Registration_Info_UPackage__Script_NX_ModularWindow.OuterSingleton;
}
static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_NX_ModularWindow(Z_Construct_UPackage__Script_NX_ModularWindow, TEXT("/Script/NX_ModularWindow"), Z_Registration_Info_UPackage__Script_NX_ModularWindow, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0xF240109F, 0xB6E53087));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
