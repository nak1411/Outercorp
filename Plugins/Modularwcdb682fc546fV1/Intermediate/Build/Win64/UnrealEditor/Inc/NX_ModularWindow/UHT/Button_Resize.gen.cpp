// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Button_Resize.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeButton_Resize() {}

// ********** Begin Cross Module References ********************************************************
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UButton_Resize();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UButton_Resize_NoRegister();
UMG_API UClass* Z_Construct_UClass_UButton();
UPackage* Z_Construct_UPackage__Script_NX_ModularWindow();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UButton_Resize ***********************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UButton_Resize;
UClass* UButton_Resize::GetPrivateStaticClass()
{
	using TClass = UButton_Resize;
	if (!Z_Registration_Info_UClass_UButton_Resize.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("Button_Resize"),
			Z_Registration_Info_UClass_UButton_Resize.InnerSingleton,
			StaticRegisterNativesUButton_Resize,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_UButton_Resize.InnerSingleton;
}
UClass* Z_Construct_UClass_UButton_Resize_NoRegister()
{
	return UButton_Resize::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UButton_Resize_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Button_Resize.h" },
		{ "ModuleRelativePath", "Public/Button_Resize.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
#endif // WITH_METADATA

// ********** Begin Class UButton_Resize constinit property declarations ***************************
// ********** End Class UButton_Resize constinit property declarations *****************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UButton_Resize>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UButton_Resize_Statics
UObject* (*const Z_Construct_UClass_UButton_Resize_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UButton,
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UButton_Resize_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UButton_Resize_Statics::ClassParams = {
	&UButton_Resize::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x00B000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UButton_Resize_Statics::Class_MetaDataParams), Z_Construct_UClass_UButton_Resize_Statics::Class_MetaDataParams)
};
void UButton_Resize::StaticRegisterNativesUButton_Resize()
{
}
UClass* Z_Construct_UClass_UButton_Resize()
{
	if (!Z_Registration_Info_UClass_UButton_Resize.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UButton_Resize.OuterSingleton, Z_Construct_UClass_UButton_Resize_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UButton_Resize.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UButton_Resize);
UButton_Resize::~UButton_Resize() {}
// ********** End Class UButton_Resize *************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Button_Resize_h__Script_NX_ModularWindow_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UButton_Resize, UButton_Resize::StaticClass, TEXT("UButton_Resize"), &Z_Registration_Info_UClass_UButton_Resize, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UButton_Resize), 2070519058U) },
	};
}; // Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Button_Resize_h__Script_NX_ModularWindow_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Button_Resize_h__Script_NX_ModularWindow_2393767990{
	TEXT("/Script/NX_ModularWindow"),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Button_Resize_h__Script_NX_ModularWindow_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Button_Resize_h__Script_NX_ModularWindow_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
