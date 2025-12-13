// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Module_Size_None.h"
#include "Engine/TimerHandle.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeModule_Size_None() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FTimerHandle();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Size_None();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Size_None_NoRegister();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow_Module();
UPackage* Z_Construct_UPackage__Script_NX_ModularWindow();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UModule_Size_None ********************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UModule_Size_None;
UClass* UModule_Size_None::GetPrivateStaticClass()
{
	using TClass = UModule_Size_None;
	if (!Z_Registration_Info_UClass_UModule_Size_None.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("Module_Size_None"),
			Z_Registration_Info_UClass_UModule_Size_None.InnerSingleton,
			StaticRegisterNativesUModule_Size_None,
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
	return Z_Registration_Info_UClass_UModule_Size_None.InnerSingleton;
}
UClass* Z_Construct_UClass_UModule_Size_None_NoRegister()
{
	return UModule_Size_None::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UModule_Size_None_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Module_Size_None.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/Module_Size_None.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimerHandle_MetaData[] = {
		{ "ModuleRelativePath", "Public/Module_Size_None.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UModule_Size_None constinit property declarations ************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_TimerHandle;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UModule_Size_None constinit property declarations **************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UModule_Size_None>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UModule_Size_None_Statics

// ********** Begin Class UModule_Size_None Property Definitions ***********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UModule_Size_None_Statics::NewProp_TimerHandle = { "TimerHandle", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Size_None, TimerHandle), Z_Construct_UScriptStruct_FTimerHandle, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimerHandle_MetaData), NewProp_TimerHandle_MetaData) }; // 3998242883
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UModule_Size_None_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Size_None_Statics::NewProp_TimerHandle,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Size_None_Statics::PropPointers) < 2048);
// ********** End Class UModule_Size_None Property Definitions *************************************
UObject* (*const Z_Construct_UClass_UModule_Size_None_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UWindow_Module,
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Size_None_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UModule_Size_None_Statics::ClassParams = {
	&UModule_Size_None::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UModule_Size_None_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Size_None_Statics::PropPointers),
	0,
	0x009000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Size_None_Statics::Class_MetaDataParams), Z_Construct_UClass_UModule_Size_None_Statics::Class_MetaDataParams)
};
void UModule_Size_None::StaticRegisterNativesUModule_Size_None()
{
}
UClass* Z_Construct_UClass_UModule_Size_None()
{
	if (!Z_Registration_Info_UClass_UModule_Size_None.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UModule_Size_None.OuterSingleton, Z_Construct_UClass_UModule_Size_None_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UModule_Size_None.OuterSingleton;
}
UModule_Size_None::UModule_Size_None(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UModule_Size_None);
UModule_Size_None::~UModule_Size_None() {}
// ********** End Class UModule_Size_None **********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Size_None_h__Script_NX_ModularWindow_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UModule_Size_None, UModule_Size_None::StaticClass, TEXT("UModule_Size_None"), &Z_Registration_Info_UClass_UModule_Size_None, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UModule_Size_None), 1919679180U) },
	};
}; // Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Size_None_h__Script_NX_ModularWindow_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Size_None_h__Script_NX_ModularWindow_1499000617{
	TEXT("/Script/NX_ModularWindow"),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Size_None_h__Script_NX_ModularWindow_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Size_None_h__Script_NX_ModularWindow_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
