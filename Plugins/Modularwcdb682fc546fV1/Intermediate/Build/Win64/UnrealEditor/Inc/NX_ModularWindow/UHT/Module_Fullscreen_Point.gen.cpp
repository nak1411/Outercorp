// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Module_Fullscreen_Point.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeModule_Fullscreen_Point() {}

// ********** Begin Cross Module References ********************************************************
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Fullscreen_None();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Fullscreen_Point();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Fullscreen_Point_NoRegister();
UPackage* Z_Construct_UPackage__Script_NX_ModularWindow();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UModule_Fullscreen_Point *************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UModule_Fullscreen_Point;
UClass* UModule_Fullscreen_Point::GetPrivateStaticClass()
{
	using TClass = UModule_Fullscreen_Point;
	if (!Z_Registration_Info_UClass_UModule_Fullscreen_Point.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("Module_Fullscreen_Point"),
			Z_Registration_Info_UClass_UModule_Fullscreen_Point.InnerSingleton,
			StaticRegisterNativesUModule_Fullscreen_Point,
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
	return Z_Registration_Info_UClass_UModule_Fullscreen_Point.InnerSingleton;
}
UClass* Z_Construct_UClass_UModule_Fullscreen_Point_NoRegister()
{
	return UModule_Fullscreen_Point::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UModule_Fullscreen_Point_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Module_Fullscreen_Point.h" },
		{ "ModuleRelativePath", "Public/Module_Fullscreen_Point.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UModule_Fullscreen_Point constinit property declarations *****************
// ********** End Class UModule_Fullscreen_Point constinit property declarations *******************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UModule_Fullscreen_Point>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UModule_Fullscreen_Point_Statics
UObject* (*const Z_Construct_UClass_UModule_Fullscreen_Point_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UModule_Fullscreen_None,
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Fullscreen_Point_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UModule_Fullscreen_Point_Statics::ClassParams = {
	&UModule_Fullscreen_Point::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Fullscreen_Point_Statics::Class_MetaDataParams), Z_Construct_UClass_UModule_Fullscreen_Point_Statics::Class_MetaDataParams)
};
void UModule_Fullscreen_Point::StaticRegisterNativesUModule_Fullscreen_Point()
{
}
UClass* Z_Construct_UClass_UModule_Fullscreen_Point()
{
	if (!Z_Registration_Info_UClass_UModule_Fullscreen_Point.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UModule_Fullscreen_Point.OuterSingleton, Z_Construct_UClass_UModule_Fullscreen_Point_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UModule_Fullscreen_Point.OuterSingleton;
}
UModule_Fullscreen_Point::UModule_Fullscreen_Point(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UModule_Fullscreen_Point);
UModule_Fullscreen_Point::~UModule_Fullscreen_Point() {}
// ********** End Class UModule_Fullscreen_Point ***************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Fullscreen_Point_h__Script_NX_ModularWindow_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UModule_Fullscreen_Point, UModule_Fullscreen_Point::StaticClass, TEXT("UModule_Fullscreen_Point"), &Z_Registration_Info_UClass_UModule_Fullscreen_Point, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UModule_Fullscreen_Point), 229960850U) },
	};
}; // Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Fullscreen_Point_h__Script_NX_ModularWindow_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Fullscreen_Point_h__Script_NX_ModularWindow_1509125283{
	TEXT("/Script/NX_ModularWindow"),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Fullscreen_Point_h__Script_NX_ModularWindow_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Fullscreen_Point_h__Script_NX_ModularWindow_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
