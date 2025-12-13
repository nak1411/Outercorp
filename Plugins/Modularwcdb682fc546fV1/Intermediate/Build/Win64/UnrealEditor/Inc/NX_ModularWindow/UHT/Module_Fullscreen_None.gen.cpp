// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Module_Fullscreen_None.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeModule_Fullscreen_None() {}

// ********** Begin Cross Module References ********************************************************
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Fullscreen_None();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Fullscreen_None_NoRegister();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow_Module();
UPackage* Z_Construct_UPackage__Script_NX_ModularWindow();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UModule_Fullscreen_None **************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UModule_Fullscreen_None;
UClass* UModule_Fullscreen_None::GetPrivateStaticClass()
{
	using TClass = UModule_Fullscreen_None;
	if (!Z_Registration_Info_UClass_UModule_Fullscreen_None.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("Module_Fullscreen_None"),
			Z_Registration_Info_UClass_UModule_Fullscreen_None.InnerSingleton,
			StaticRegisterNativesUModule_Fullscreen_None,
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
	return Z_Registration_Info_UClass_UModule_Fullscreen_None.InnerSingleton;
}
UClass* Z_Construct_UClass_UModule_Fullscreen_None_NoRegister()
{
	return UModule_Fullscreen_None::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UModule_Fullscreen_None_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Module_Fullscreen_None.h" },
		{ "ModuleRelativePath", "Public/Module_Fullscreen_None.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UModule_Fullscreen_None constinit property declarations ******************
// ********** End Class UModule_Fullscreen_None constinit property declarations ********************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UModule_Fullscreen_None>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UModule_Fullscreen_None_Statics
UObject* (*const Z_Construct_UClass_UModule_Fullscreen_None_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UWindow_Module,
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Fullscreen_None_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UModule_Fullscreen_None_Statics::ClassParams = {
	&UModule_Fullscreen_None::StaticClass,
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
	0x009000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Fullscreen_None_Statics::Class_MetaDataParams), Z_Construct_UClass_UModule_Fullscreen_None_Statics::Class_MetaDataParams)
};
void UModule_Fullscreen_None::StaticRegisterNativesUModule_Fullscreen_None()
{
}
UClass* Z_Construct_UClass_UModule_Fullscreen_None()
{
	if (!Z_Registration_Info_UClass_UModule_Fullscreen_None.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UModule_Fullscreen_None.OuterSingleton, Z_Construct_UClass_UModule_Fullscreen_None_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UModule_Fullscreen_None.OuterSingleton;
}
UModule_Fullscreen_None::UModule_Fullscreen_None(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UModule_Fullscreen_None);
UModule_Fullscreen_None::~UModule_Fullscreen_None() {}
// ********** End Class UModule_Fullscreen_None ****************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Fullscreen_None_h__Script_NX_ModularWindow_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UModule_Fullscreen_None, UModule_Fullscreen_None::StaticClass, TEXT("UModule_Fullscreen_None"), &Z_Registration_Info_UClass_UModule_Fullscreen_None, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UModule_Fullscreen_None), 3173509475U) },
	};
}; // Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Fullscreen_None_h__Script_NX_ModularWindow_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Fullscreen_None_h__Script_NX_ModularWindow_3685260661{
	TEXT("/Script/NX_ModularWindow"),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Fullscreen_None_h__Script_NX_ModularWindow_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Fullscreen_None_h__Script_NX_ModularWindow_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
