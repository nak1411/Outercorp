// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Module_Limit_Position_Universal.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeModule_Limit_Position_Universal() {}

// ********** Begin Cross Module References ********************************************************
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Limit_Position_Line_NoRegister();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Limit_Position_None();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Limit_Position_Point_NoRegister();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Limit_Position_Universal();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Limit_Position_Universal_NoRegister();
UPackage* Z_Construct_UPackage__Script_NX_ModularWindow();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UModule_Limit_Position_Universal *****************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UModule_Limit_Position_Universal;
UClass* UModule_Limit_Position_Universal::GetPrivateStaticClass()
{
	using TClass = UModule_Limit_Position_Universal;
	if (!Z_Registration_Info_UClass_UModule_Limit_Position_Universal.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("Module_Limit_Position_Universal"),
			Z_Registration_Info_UClass_UModule_Limit_Position_Universal.InnerSingleton,
			StaticRegisterNativesUModule_Limit_Position_Universal,
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
	return Z_Registration_Info_UClass_UModule_Limit_Position_Universal.InnerSingleton;
}
UClass* Z_Construct_UClass_UModule_Limit_Position_Universal_NoRegister()
{
	return UModule_Limit_Position_Universal::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UModule_Limit_Position_Universal_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Module_Limit_Position_Universal.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/Module_Limit_Position_Universal.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LimitPoint_MetaData[] = {
		{ "ModuleRelativePath", "Public/Module_Limit_Position_Universal.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LimitLine_MetaData[] = {
		{ "ModuleRelativePath", "Public/Module_Limit_Position_Universal.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UModule_Limit_Position_Universal constinit property declarations *********
	static const UECodeGen_Private::FObjectPropertyParams NewProp_LimitPoint;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_LimitLine;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UModule_Limit_Position_Universal constinit property declarations ***********
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UModule_Limit_Position_Universal>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UModule_Limit_Position_Universal_Statics

// ********** Begin Class UModule_Limit_Position_Universal Property Definitions ********************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UModule_Limit_Position_Universal_Statics::NewProp_LimitPoint = { "LimitPoint", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Limit_Position_Universal, LimitPoint), Z_Construct_UClass_UModule_Limit_Position_Point_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LimitPoint_MetaData), NewProp_LimitPoint_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UModule_Limit_Position_Universal_Statics::NewProp_LimitLine = { "LimitLine", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Limit_Position_Universal, LimitLine), Z_Construct_UClass_UModule_Limit_Position_Line_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LimitLine_MetaData), NewProp_LimitLine_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UModule_Limit_Position_Universal_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Limit_Position_Universal_Statics::NewProp_LimitPoint,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Limit_Position_Universal_Statics::NewProp_LimitLine,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Limit_Position_Universal_Statics::PropPointers) < 2048);
// ********** End Class UModule_Limit_Position_Universal Property Definitions **********************
UObject* (*const Z_Construct_UClass_UModule_Limit_Position_Universal_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UModule_Limit_Position_None,
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Limit_Position_Universal_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UModule_Limit_Position_Universal_Statics::ClassParams = {
	&UModule_Limit_Position_Universal::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UModule_Limit_Position_Universal_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Limit_Position_Universal_Statics::PropPointers),
	0,
	0x009000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Limit_Position_Universal_Statics::Class_MetaDataParams), Z_Construct_UClass_UModule_Limit_Position_Universal_Statics::Class_MetaDataParams)
};
void UModule_Limit_Position_Universal::StaticRegisterNativesUModule_Limit_Position_Universal()
{
}
UClass* Z_Construct_UClass_UModule_Limit_Position_Universal()
{
	if (!Z_Registration_Info_UClass_UModule_Limit_Position_Universal.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UModule_Limit_Position_Universal.OuterSingleton, Z_Construct_UClass_UModule_Limit_Position_Universal_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UModule_Limit_Position_Universal.OuterSingleton;
}
UModule_Limit_Position_Universal::UModule_Limit_Position_Universal(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UModule_Limit_Position_Universal);
UModule_Limit_Position_Universal::~UModule_Limit_Position_Universal() {}
// ********** End Class UModule_Limit_Position_Universal *******************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_Position_Universal_h__Script_NX_ModularWindow_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UModule_Limit_Position_Universal, UModule_Limit_Position_Universal::StaticClass, TEXT("UModule_Limit_Position_Universal"), &Z_Registration_Info_UClass_UModule_Limit_Position_Universal, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UModule_Limit_Position_Universal), 2224767873U) },
	};
}; // Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_Position_Universal_h__Script_NX_ModularWindow_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_Position_Universal_h__Script_NX_ModularWindow_2779170738{
	TEXT("/Script/NX_ModularWindow"),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_Position_Universal_h__Script_NX_ModularWindow_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_Position_Universal_h__Script_NX_ModularWindow_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
