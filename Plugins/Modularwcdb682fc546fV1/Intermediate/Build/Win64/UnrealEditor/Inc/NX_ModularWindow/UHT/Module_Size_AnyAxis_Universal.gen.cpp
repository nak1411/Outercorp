// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Module_Size_AnyAxis_Universal.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeModule_Size_AnyAxis_Universal() {}

// ********** Begin Cross Module References ********************************************************
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Size_AnyAxis_Universal();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Size_AnyAxis_Universal_NoRegister();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Size_None();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Size_None_NoRegister();
UPackage* Z_Construct_UPackage__Script_NX_ModularWindow();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UModule_Size_AnyAxis_Universal Function SetSide **************************
struct Z_Construct_UFunction_UModule_Size_AnyAxis_Universal_SetSide_Statics
{
	struct Module_Size_AnyAxis_Universal_eventSetSide_Parms
	{
		uint8 InData;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//0 Up, 1 Right, 2 Down, 3 Left\n" },
#endif
		{ "ModuleRelativePath", "Public/Module_Size_AnyAxis_Universal.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "0 Up, 1 Right, 2 Down, 3 Left" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Function SetSide constinit property declarations *******************************
	static const UECodeGen_Private::FBytePropertyParams NewProp_InData;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetSide constinit property declarations *********************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetSide Property Definitions ******************************************
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UModule_Size_AnyAxis_Universal_SetSide_Statics::NewProp_InData = { "InData", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Size_AnyAxis_Universal_eventSetSide_Parms, InData), nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UModule_Size_AnyAxis_Universal_SetSide_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Size_AnyAxis_Universal_SetSide_Statics::NewProp_InData,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Size_AnyAxis_Universal_SetSide_Statics::PropPointers) < 2048);
// ********** End Function SetSide Property Definitions ********************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UModule_Size_AnyAxis_Universal_SetSide_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UModule_Size_AnyAxis_Universal, nullptr, "SetSide", 	Z_Construct_UFunction_UModule_Size_AnyAxis_Universal_SetSide_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Size_AnyAxis_Universal_SetSide_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UModule_Size_AnyAxis_Universal_SetSide_Statics::Module_Size_AnyAxis_Universal_eventSetSide_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Size_AnyAxis_Universal_SetSide_Statics::Function_MetaDataParams), Z_Construct_UFunction_UModule_Size_AnyAxis_Universal_SetSide_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UModule_Size_AnyAxis_Universal_SetSide_Statics::Module_Size_AnyAxis_Universal_eventSetSide_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UModule_Size_AnyAxis_Universal_SetSide()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UModule_Size_AnyAxis_Universal_SetSide_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UModule_Size_AnyAxis_Universal::execSetSide)
{
	P_GET_PROPERTY(FByteProperty,Z_Param_InData);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetSide(Z_Param_InData);
	P_NATIVE_END;
}
// ********** End Class UModule_Size_AnyAxis_Universal Function SetSide ****************************

// ********** Begin Class UModule_Size_AnyAxis_Universal *******************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UModule_Size_AnyAxis_Universal;
UClass* UModule_Size_AnyAxis_Universal::GetPrivateStaticClass()
{
	using TClass = UModule_Size_AnyAxis_Universal;
	if (!Z_Registration_Info_UClass_UModule_Size_AnyAxis_Universal.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("Module_Size_AnyAxis_Universal"),
			Z_Registration_Info_UClass_UModule_Size_AnyAxis_Universal.InnerSingleton,
			StaticRegisterNativesUModule_Size_AnyAxis_Universal,
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
	return Z_Registration_Info_UClass_UModule_Size_AnyAxis_Universal.InnerSingleton;
}
UClass* Z_Construct_UClass_UModule_Size_AnyAxis_Universal_NoRegister()
{
	return UModule_Size_AnyAxis_Universal::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Module_Size_AnyAxis_Universal.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/Module_Size_AnyAxis_Universal.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Side_MetaData[] = {
		{ "Category", "Window" },
		{ "ExposeOnSpawn", "TRUE" },
		{ "ModuleRelativePath", "Public/Module_Size_AnyAxis_Universal.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "0 Up, 1 Right, 2 Down, 3 Left" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Point_MetaData[] = {
		{ "ModuleRelativePath", "Public/Module_Size_AnyAxis_Universal.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Line_MetaData[] = {
		{ "ModuleRelativePath", "Public/Module_Size_AnyAxis_Universal.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UModule_Size_AnyAxis_Universal constinit property declarations ***********
	static const UECodeGen_Private::FBytePropertyParams NewProp_Side;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Point;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Line;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UModule_Size_AnyAxis_Universal constinit property declarations *************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("SetSide"), .Pointer = &UModule_Size_AnyAxis_Universal::execSetSide },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UModule_Size_AnyAxis_Universal_SetSide, "SetSide" }, // 1514845748
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UModule_Size_AnyAxis_Universal>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics

// ********** Begin Class UModule_Size_AnyAxis_Universal Property Definitions **********************
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics::NewProp_Side = { "Side", nullptr, (EPropertyFlags)0x0011000000000005, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Size_AnyAxis_Universal, Side), nullptr, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Side_MetaData), NewProp_Side_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics::NewProp_Point = { "Point", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Size_AnyAxis_Universal, Point), Z_Construct_UClass_UModule_Size_None_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Point_MetaData), NewProp_Point_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics::NewProp_Line = { "Line", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Size_AnyAxis_Universal, Line), Z_Construct_UClass_UModule_Size_None_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Line_MetaData), NewProp_Line_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics::NewProp_Side,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics::NewProp_Point,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics::NewProp_Line,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics::PropPointers) < 2048);
// ********** End Class UModule_Size_AnyAxis_Universal Property Definitions ************************
UObject* (*const Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UModule_Size_None,
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics::ClassParams = {
	&UModule_Size_AnyAxis_Universal::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics::PropPointers),
	0,
	0x009000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics::Class_MetaDataParams), Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics::Class_MetaDataParams)
};
void UModule_Size_AnyAxis_Universal::StaticRegisterNativesUModule_Size_AnyAxis_Universal()
{
	UClass* Class = UModule_Size_AnyAxis_Universal::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics::Funcs));
}
UClass* Z_Construct_UClass_UModule_Size_AnyAxis_Universal()
{
	if (!Z_Registration_Info_UClass_UModule_Size_AnyAxis_Universal.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UModule_Size_AnyAxis_Universal.OuterSingleton, Z_Construct_UClass_UModule_Size_AnyAxis_Universal_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UModule_Size_AnyAxis_Universal.OuterSingleton;
}
UModule_Size_AnyAxis_Universal::UModule_Size_AnyAxis_Universal(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UModule_Size_AnyAxis_Universal);
UModule_Size_AnyAxis_Universal::~UModule_Size_AnyAxis_Universal() {}
// ********** End Class UModule_Size_AnyAxis_Universal *********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Size_AnyAxis_Universal_h__Script_NX_ModularWindow_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UModule_Size_AnyAxis_Universal, UModule_Size_AnyAxis_Universal::StaticClass, TEXT("UModule_Size_AnyAxis_Universal"), &Z_Registration_Info_UClass_UModule_Size_AnyAxis_Universal, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UModule_Size_AnyAxis_Universal), 4241302973U) },
	};
}; // Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Size_AnyAxis_Universal_h__Script_NX_ModularWindow_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Size_AnyAxis_Universal_h__Script_NX_ModularWindow_1048562699{
	TEXT("/Script/NX_ModularWindow"),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Size_AnyAxis_Universal_h__Script_NX_ModularWindow_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Size_AnyAxis_Universal_h__Script_NX_ModularWindow_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
