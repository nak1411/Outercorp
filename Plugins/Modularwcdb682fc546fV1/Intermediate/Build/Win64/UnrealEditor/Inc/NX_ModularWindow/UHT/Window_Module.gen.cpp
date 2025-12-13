// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Window_Module.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeWindow_Module() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow_Module();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow_Module_NoRegister();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow_NoRegister();
UPackage* Z_Construct_UPackage__Script_NX_ModularWindow();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UWindow_Module Function Action *******************************************
struct Z_Construct_UFunction_UWindow_Module_Action_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Window_Module.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function Action constinit property declarations ********************************
// ********** End Function Action constinit property declarations **********************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_Module_Action_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow_Module, nullptr, "Action", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_Module_Action_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_Module_Action_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UWindow_Module_Action()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_Module_Action_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow_Module::execAction)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->Action();
	P_NATIVE_END;
}
// ********** End Class UWindow_Module Function Action *********************************************

// ********** Begin Class UWindow_Module Function Deaction *****************************************
struct Z_Construct_UFunction_UWindow_Module_Deaction_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Window_Module.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function Deaction constinit property declarations ******************************
// ********** End Function Deaction constinit property declarations ********************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_Module_Deaction_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow_Module, nullptr, "Deaction", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_Module_Deaction_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_Module_Deaction_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UWindow_Module_Deaction()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_Module_Deaction_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow_Module::execDeaction)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->Deaction();
	P_NATIVE_END;
}
// ********** End Class UWindow_Module Function Deaction *******************************************

// ********** Begin Class UWindow_Module Function GetWindow ****************************************
struct Z_Construct_UFunction_UWindow_Module_GetWindow_Statics
{
	struct Window_Module_eventGetWindow_Parms
	{
		UWindow* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Module" },
		{ "ModuleRelativePath", "Public/Window_Module.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetWindow constinit property declarations *****************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetWindow constinit property declarations *******************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetWindow Property Definitions ****************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UWindow_Module_GetWindow_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_Module_eventGetWindow_Parms, ReturnValue), Z_Construct_UClass_UWindow_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_Module_GetWindow_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_Module_GetWindow_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_Module_GetWindow_Statics::PropPointers) < 2048);
// ********** End Function GetWindow Property Definitions ******************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_Module_GetWindow_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow_Module, nullptr, "GetWindow", 	Z_Construct_UFunction_UWindow_Module_GetWindow_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_Module_GetWindow_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_Module_GetWindow_Statics::Window_Module_eventGetWindow_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_Module_GetWindow_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_Module_GetWindow_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_Module_GetWindow_Statics::Window_Module_eventGetWindow_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_Module_GetWindow()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_Module_GetWindow_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow_Module::execGetWindow)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UWindow**)Z_Param__Result=P_THIS->GetWindow();
	P_NATIVE_END;
}
// ********** End Class UWindow_Module Function GetWindow ******************************************

// ********** Begin Class UWindow_Module ***********************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UWindow_Module;
UClass* UWindow_Module::GetPrivateStaticClass()
{
	using TClass = UWindow_Module;
	if (!Z_Registration_Info_UClass_UWindow_Module.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("Window_Module"),
			Z_Registration_Info_UClass_UWindow_Module.InnerSingleton,
			StaticRegisterNativesUWindow_Module,
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
	return Z_Registration_Info_UClass_UWindow_Module.InnerSingleton;
}
UClass* Z_Construct_UClass_UWindow_Module_NoRegister()
{
	return UWindow_Module::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UWindow_Module_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Window_Module.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/Window_Module.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Window_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Window_Module.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UWindow_Module constinit property declarations ***************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Window;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UWindow_Module constinit property declarations *****************************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("Action"), .Pointer = &UWindow_Module::execAction },
		{ .NameUTF8 = UTF8TEXT("Deaction"), .Pointer = &UWindow_Module::execDeaction },
		{ .NameUTF8 = UTF8TEXT("GetWindow"), .Pointer = &UWindow_Module::execGetWindow },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UWindow_Module_Action, "Action" }, // 1804629161
		{ &Z_Construct_UFunction_UWindow_Module_Deaction, "Deaction" }, // 1221057166
		{ &Z_Construct_UFunction_UWindow_Module_GetWindow, "GetWindow" }, // 3015194937
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWindow_Module>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UWindow_Module_Statics

// ********** Begin Class UWindow_Module Property Definitions **************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWindow_Module_Statics::NewProp_Window = { "Window", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWindow_Module, Window), Z_Construct_UClass_UWindow_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Window_MetaData), NewProp_Window_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UWindow_Module_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Module_Statics::NewProp_Window,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UWindow_Module_Statics::PropPointers) < 2048);
// ********** End Class UWindow_Module Property Definitions ****************************************
UObject* (*const Z_Construct_UClass_UWindow_Module_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UWindow_Module_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UWindow_Module_Statics::ClassParams = {
	&UWindow_Module::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UWindow_Module_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UWindow_Module_Statics::PropPointers),
	0,
	0x009000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UWindow_Module_Statics::Class_MetaDataParams), Z_Construct_UClass_UWindow_Module_Statics::Class_MetaDataParams)
};
void UWindow_Module::StaticRegisterNativesUWindow_Module()
{
	UClass* Class = UWindow_Module::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UWindow_Module_Statics::Funcs));
}
UClass* Z_Construct_UClass_UWindow_Module()
{
	if (!Z_Registration_Info_UClass_UWindow_Module.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWindow_Module.OuterSingleton, Z_Construct_UClass_UWindow_Module_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UWindow_Module.OuterSingleton;
}
UWindow_Module::UWindow_Module(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UWindow_Module);
UWindow_Module::~UWindow_Module() {}
// ********** End Class UWindow_Module *************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_Module_h__Script_NX_ModularWindow_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UWindow_Module, UWindow_Module::StaticClass, TEXT("UWindow_Module"), &Z_Registration_Info_UClass_UWindow_Module, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWindow_Module), 3744494037U) },
	};
}; // Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_Module_h__Script_NX_ModularWindow_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_Module_h__Script_NX_ModularWindow_3317273451{
	TEXT("/Script/NX_ModularWindow"),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_Module_h__Script_NX_ModularWindow_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_Module_h__Script_NX_ModularWindow_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
