// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Module_Limit_None.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeModule_Limit_None() {}

// ********** Begin Cross Module References ********************************************************
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Limit_None();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Limit_None_NoRegister();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow_Module();
UMG_API UClass* Z_Construct_UClass_UWidget_NoRegister();
UPackage* Z_Construct_UPackage__Script_NX_ModularWindow();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UModule_Limit_None Function ModifyByParameter ****************************
struct Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics
{
	struct Module_Limit_None_eventModifyByParameter_Parms
	{
		UWidget* InWindow;
		TArray<FName> InNames;
		TArray<float> InValues;
		TArray<float> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Module_Limit_None.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InWindow_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function ModifyByParameter constinit property declarations *********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InWindow;
	static const UECodeGen_Private::FNamePropertyParams NewProp_InNames_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_InNames;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InValues_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_InValues;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ModifyByParameter constinit property declarations ***********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ModifyByParameter Property Definitions ********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::NewProp_InWindow = { "InWindow", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Limit_None_eventModifyByParameter_Parms, InWindow), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InWindow_MetaData), NewProp_InWindow_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::NewProp_InNames_Inner = { "InNames", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::NewProp_InNames = { "InNames", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Limit_None_eventModifyByParameter_Parms, InNames), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::NewProp_InValues_Inner = { "InValues", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::NewProp_InValues = { "InValues", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Limit_None_eventModifyByParameter_Parms, InValues), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Limit_None_eventModifyByParameter_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::NewProp_InWindow,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::NewProp_InNames_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::NewProp_InNames,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::NewProp_InValues_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::NewProp_InValues,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::PropPointers) < 2048);
// ********** End Function ModifyByParameter Property Definitions **********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UModule_Limit_None, nullptr, "ModifyByParameter", 	Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::Module_Limit_None_eventModifyByParameter_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::Function_MetaDataParams), Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::Module_Limit_None_eventModifyByParameter_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UModule_Limit_None::execModifyByParameter)
{
	P_GET_OBJECT(UWidget,Z_Param_InWindow);
	P_GET_TARRAY(FName,Z_Param_InNames);
	P_GET_TARRAY(float,Z_Param_InValues);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<float>*)Z_Param__Result=P_THIS->ModifyByParameter(Z_Param_InWindow,Z_Param_InNames,Z_Param_InValues);
	P_NATIVE_END;
}
// ********** End Class UModule_Limit_None Function ModifyByParameter ******************************

// ********** Begin Class UModule_Limit_None Function Prepare **************************************
struct Z_Construct_UFunction_UModule_Limit_None_Prepare_Statics
{
	struct Module_Limit_None_eventPrepare_Parms
	{
		UWidget* InWindow;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Module_Limit_None.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InWindow_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function Prepare constinit property declarations *******************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InWindow;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function Prepare constinit property declarations *********************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function Prepare Property Definitions ******************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UModule_Limit_None_Prepare_Statics::NewProp_InWindow = { "InWindow", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Limit_None_eventPrepare_Parms, InWindow), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InWindow_MetaData), NewProp_InWindow_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UModule_Limit_None_Prepare_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_None_Prepare_Statics::NewProp_InWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_None_Prepare_Statics::PropPointers) < 2048);
// ********** End Function Prepare Property Definitions ********************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UModule_Limit_None_Prepare_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UModule_Limit_None, nullptr, "Prepare", 	Z_Construct_UFunction_UModule_Limit_None_Prepare_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_None_Prepare_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UModule_Limit_None_Prepare_Statics::Module_Limit_None_eventPrepare_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_None_Prepare_Statics::Function_MetaDataParams), Z_Construct_UFunction_UModule_Limit_None_Prepare_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UModule_Limit_None_Prepare_Statics::Module_Limit_None_eventPrepare_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UModule_Limit_None_Prepare()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UModule_Limit_None_Prepare_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UModule_Limit_None::execPrepare)
{
	P_GET_OBJECT(UWidget,Z_Param_InWindow);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->Prepare(Z_Param_InWindow);
	P_NATIVE_END;
}
// ********** End Class UModule_Limit_None Function Prepare ****************************************

// ********** Begin Class UModule_Limit_None *******************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UModule_Limit_None;
UClass* UModule_Limit_None::GetPrivateStaticClass()
{
	using TClass = UModule_Limit_None;
	if (!Z_Registration_Info_UClass_UModule_Limit_None.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("Module_Limit_None"),
			Z_Registration_Info_UClass_UModule_Limit_None.InnerSingleton,
			StaticRegisterNativesUModule_Limit_None,
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
	return Z_Registration_Info_UClass_UModule_Limit_None.InnerSingleton;
}
UClass* Z_Construct_UClass_UModule_Limit_None_NoRegister()
{
	return UModule_Limit_None::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UModule_Limit_None_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Module_Limit_None.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/Module_Limit_None.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UModule_Limit_None constinit property declarations ***********************
// ********** End Class UModule_Limit_None constinit property declarations *************************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("ModifyByParameter"), .Pointer = &UModule_Limit_None::execModifyByParameter },
		{ .NameUTF8 = UTF8TEXT("Prepare"), .Pointer = &UModule_Limit_None::execPrepare },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UModule_Limit_None_ModifyByParameter, "ModifyByParameter" }, // 2068147718
		{ &Z_Construct_UFunction_UModule_Limit_None_Prepare, "Prepare" }, // 231259228
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UModule_Limit_None>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UModule_Limit_None_Statics
UObject* (*const Z_Construct_UClass_UModule_Limit_None_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UWindow_Module,
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Limit_None_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UModule_Limit_None_Statics::ClassParams = {
	&UModule_Limit_None::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	0,
	0,
	0x009000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Limit_None_Statics::Class_MetaDataParams), Z_Construct_UClass_UModule_Limit_None_Statics::Class_MetaDataParams)
};
void UModule_Limit_None::StaticRegisterNativesUModule_Limit_None()
{
	UClass* Class = UModule_Limit_None::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UModule_Limit_None_Statics::Funcs));
}
UClass* Z_Construct_UClass_UModule_Limit_None()
{
	if (!Z_Registration_Info_UClass_UModule_Limit_None.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UModule_Limit_None.OuterSingleton, Z_Construct_UClass_UModule_Limit_None_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UModule_Limit_None.OuterSingleton;
}
UModule_Limit_None::UModule_Limit_None(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UModule_Limit_None);
UModule_Limit_None::~UModule_Limit_None() {}
// ********** End Class UModule_Limit_None *********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_None_h__Script_NX_ModularWindow_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UModule_Limit_None, UModule_Limit_None::StaticClass, TEXT("UModule_Limit_None"), &Z_Registration_Info_UClass_UModule_Limit_None, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UModule_Limit_None), 3340296456U) },
	};
}; // Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_None_h__Script_NX_ModularWindow_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_None_h__Script_NX_ModularWindow_2695335405{
	TEXT("/Script/NX_ModularWindow"),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_None_h__Script_NX_ModularWindow_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_None_h__Script_NX_ModularWindow_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
