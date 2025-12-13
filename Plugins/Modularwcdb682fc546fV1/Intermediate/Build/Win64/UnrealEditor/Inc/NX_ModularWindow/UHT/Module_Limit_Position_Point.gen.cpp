// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Module_Limit_Position_Point.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeModule_Limit_Position_Point() {}

// ********** Begin Cross Module References ********************************************************
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Limit_Position_None();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Limit_Position_Point();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Limit_Position_Point_NoRegister();
UPackage* Z_Construct_UPackage__Script_NX_ModularWindow();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UModule_Limit_Position_Point *********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UModule_Limit_Position_Point;
UClass* UModule_Limit_Position_Point::GetPrivateStaticClass()
{
	using TClass = UModule_Limit_Position_Point;
	if (!Z_Registration_Info_UClass_UModule_Limit_Position_Point.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("Module_Limit_Position_Point"),
			Z_Registration_Info_UClass_UModule_Limit_Position_Point.InnerSingleton,
			StaticRegisterNativesUModule_Limit_Position_Point,
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
	return Z_Registration_Info_UClass_UModule_Limit_Position_Point.InnerSingleton;
}
UClass* Z_Construct_UClass_UModule_Limit_Position_Point_NoRegister()
{
	return UModule_Limit_Position_Point::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UModule_Limit_Position_Point_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Module_Limit_Position_Point.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/Module_Limit_Position_Point.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UModule_Limit_Position_Point constinit property declarations *************
// ********** End Class UModule_Limit_Position_Point constinit property declarations ***************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UModule_Limit_Position_Point>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UModule_Limit_Position_Point_Statics
UObject* (*const Z_Construct_UClass_UModule_Limit_Position_Point_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UModule_Limit_Position_None,
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Limit_Position_Point_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UModule_Limit_Position_Point_Statics::ClassParams = {
	&UModule_Limit_Position_Point::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Limit_Position_Point_Statics::Class_MetaDataParams), Z_Construct_UClass_UModule_Limit_Position_Point_Statics::Class_MetaDataParams)
};
void UModule_Limit_Position_Point::StaticRegisterNativesUModule_Limit_Position_Point()
{
}
UClass* Z_Construct_UClass_UModule_Limit_Position_Point()
{
	if (!Z_Registration_Info_UClass_UModule_Limit_Position_Point.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UModule_Limit_Position_Point.OuterSingleton, Z_Construct_UClass_UModule_Limit_Position_Point_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UModule_Limit_Position_Point.OuterSingleton;
}
UModule_Limit_Position_Point::UModule_Limit_Position_Point(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UModule_Limit_Position_Point);
UModule_Limit_Position_Point::~UModule_Limit_Position_Point() {}
// ********** End Class UModule_Limit_Position_Point ***********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_Position_Point_h__Script_NX_ModularWindow_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UModule_Limit_Position_Point, UModule_Limit_Position_Point::StaticClass, TEXT("UModule_Limit_Position_Point"), &Z_Registration_Info_UClass_UModule_Limit_Position_Point, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UModule_Limit_Position_Point), 2956147369U) },
	};
}; // Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_Position_Point_h__Script_NX_ModularWindow_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_Position_Point_h__Script_NX_ModularWindow_2837616879{
	TEXT("/Script/NX_ModularWindow"),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_Position_Point_h__Script_NX_ModularWindow_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_Position_Point_h__Script_NX_ModularWindow_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
