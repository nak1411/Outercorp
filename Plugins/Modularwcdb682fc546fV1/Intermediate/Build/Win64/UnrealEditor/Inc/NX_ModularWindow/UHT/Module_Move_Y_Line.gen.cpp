// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Module_Move_Y_Line.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeModule_Move_Y_Line() {}

// ********** Begin Cross Module References ********************************************************
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Move_None();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Move_Y_Line();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Move_Y_Line_NoRegister();
UPackage* Z_Construct_UPackage__Script_NX_ModularWindow();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UModule_Move_Y_Line ******************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UModule_Move_Y_Line;
UClass* UModule_Move_Y_Line::GetPrivateStaticClass()
{
	using TClass = UModule_Move_Y_Line;
	if (!Z_Registration_Info_UClass_UModule_Move_Y_Line.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("Module_Move_Y_Line"),
			Z_Registration_Info_UClass_UModule_Move_Y_Line.InnerSingleton,
			StaticRegisterNativesUModule_Move_Y_Line,
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
	return Z_Registration_Info_UClass_UModule_Move_Y_Line.InnerSingleton;
}
UClass* Z_Construct_UClass_UModule_Move_Y_Line_NoRegister()
{
	return UModule_Move_Y_Line::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UModule_Move_Y_Line_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Module_Move_Y_Line.h" },
		{ "ModuleRelativePath", "Public/Module_Move_Y_Line.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UModule_Move_Y_Line constinit property declarations **********************
// ********** End Class UModule_Move_Y_Line constinit property declarations ************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UModule_Move_Y_Line>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UModule_Move_Y_Line_Statics
UObject* (*const Z_Construct_UClass_UModule_Move_Y_Line_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UModule_Move_None,
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Move_Y_Line_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UModule_Move_Y_Line_Statics::ClassParams = {
	&UModule_Move_Y_Line::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Move_Y_Line_Statics::Class_MetaDataParams), Z_Construct_UClass_UModule_Move_Y_Line_Statics::Class_MetaDataParams)
};
void UModule_Move_Y_Line::StaticRegisterNativesUModule_Move_Y_Line()
{
}
UClass* Z_Construct_UClass_UModule_Move_Y_Line()
{
	if (!Z_Registration_Info_UClass_UModule_Move_Y_Line.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UModule_Move_Y_Line.OuterSingleton, Z_Construct_UClass_UModule_Move_Y_Line_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UModule_Move_Y_Line.OuterSingleton;
}
UModule_Move_Y_Line::UModule_Move_Y_Line(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UModule_Move_Y_Line);
UModule_Move_Y_Line::~UModule_Move_Y_Line() {}
// ********** End Class UModule_Move_Y_Line ********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Move_Y_Line_h__Script_NX_ModularWindow_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UModule_Move_Y_Line, UModule_Move_Y_Line::StaticClass, TEXT("UModule_Move_Y_Line"), &Z_Registration_Info_UClass_UModule_Move_Y_Line, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UModule_Move_Y_Line), 477064918U) },
	};
}; // Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Move_Y_Line_h__Script_NX_ModularWindow_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Move_Y_Line_h__Script_NX_ModularWindow_2262899208{
	TEXT("/Script/NX_ModularWindow"),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Move_Y_Line_h__Script_NX_ModularWindow_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Move_Y_Line_h__Script_NX_ModularWindow_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
