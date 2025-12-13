// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Module_Move_None.h"
#include "Engine/TimerHandle.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeModule_Move_None() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FTimerHandle();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Move_None();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Move_None_NoRegister();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow_Module();
UPackage* Z_Construct_UPackage__Script_NX_ModularWindow();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UModule_Move_None Function GetXWorking ***********************************
struct Z_Construct_UFunction_UModule_Move_None_GetXWorking_Statics
{
	struct Module_Move_None_eventGetXWorking_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Module_Move_None.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetXWorking constinit property declarations ***************************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetXWorking constinit property declarations *****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetXWorking Property Definitions **************************************
void Z_Construct_UFunction_UModule_Move_None_GetXWorking_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((Module_Move_None_eventGetXWorking_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UModule_Move_None_GetXWorking_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Module_Move_None_eventGetXWorking_Parms), &Z_Construct_UFunction_UModule_Move_None_GetXWorking_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UModule_Move_None_GetXWorking_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Move_None_GetXWorking_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Move_None_GetXWorking_Statics::PropPointers) < 2048);
// ********** End Function GetXWorking Property Definitions ****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UModule_Move_None_GetXWorking_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UModule_Move_None, nullptr, "GetXWorking", 	Z_Construct_UFunction_UModule_Move_None_GetXWorking_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Move_None_GetXWorking_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UModule_Move_None_GetXWorking_Statics::Module_Move_None_eventGetXWorking_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Move_None_GetXWorking_Statics::Function_MetaDataParams), Z_Construct_UFunction_UModule_Move_None_GetXWorking_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UModule_Move_None_GetXWorking_Statics::Module_Move_None_eventGetXWorking_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UModule_Move_None_GetXWorking()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UModule_Move_None_GetXWorking_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UModule_Move_None::execGetXWorking)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->GetXWorking();
	P_NATIVE_END;
}
// ********** End Class UModule_Move_None Function GetXWorking *************************************

// ********** Begin Class UModule_Move_None Function GetYWorking ***********************************
struct Z_Construct_UFunction_UModule_Move_None_GetYWorking_Statics
{
	struct Module_Move_None_eventGetYWorking_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Module_Move_None.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetYWorking constinit property declarations ***************************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetYWorking constinit property declarations *****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetYWorking Property Definitions **************************************
void Z_Construct_UFunction_UModule_Move_None_GetYWorking_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((Module_Move_None_eventGetYWorking_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UModule_Move_None_GetYWorking_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Module_Move_None_eventGetYWorking_Parms), &Z_Construct_UFunction_UModule_Move_None_GetYWorking_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UModule_Move_None_GetYWorking_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Move_None_GetYWorking_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Move_None_GetYWorking_Statics::PropPointers) < 2048);
// ********** End Function GetYWorking Property Definitions ****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UModule_Move_None_GetYWorking_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UModule_Move_None, nullptr, "GetYWorking", 	Z_Construct_UFunction_UModule_Move_None_GetYWorking_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Move_None_GetYWorking_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UModule_Move_None_GetYWorking_Statics::Module_Move_None_eventGetYWorking_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Move_None_GetYWorking_Statics::Function_MetaDataParams), Z_Construct_UFunction_UModule_Move_None_GetYWorking_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UModule_Move_None_GetYWorking_Statics::Module_Move_None_eventGetYWorking_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UModule_Move_None_GetYWorking()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UModule_Move_None_GetYWorking_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UModule_Move_None::execGetYWorking)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->GetYWorking();
	P_NATIVE_END;
}
// ********** End Class UModule_Move_None Function GetYWorking *************************************

// ********** Begin Class UModule_Move_None Function SetXWorking ***********************************
struct Z_Construct_UFunction_UModule_Move_None_SetXWorking_Statics
{
	struct Module_Move_None_eventSetXWorking_Parms
	{
		bool bValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Module_Move_None.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetXWorking constinit property declarations ***************************
	static void NewProp_bValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetXWorking constinit property declarations *****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetXWorking Property Definitions **************************************
void Z_Construct_UFunction_UModule_Move_None_SetXWorking_Statics::NewProp_bValue_SetBit(void* Obj)
{
	((Module_Move_None_eventSetXWorking_Parms*)Obj)->bValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UModule_Move_None_SetXWorking_Statics::NewProp_bValue = { "bValue", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Module_Move_None_eventSetXWorking_Parms), &Z_Construct_UFunction_UModule_Move_None_SetXWorking_Statics::NewProp_bValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UModule_Move_None_SetXWorking_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Move_None_SetXWorking_Statics::NewProp_bValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Move_None_SetXWorking_Statics::PropPointers) < 2048);
// ********** End Function SetXWorking Property Definitions ****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UModule_Move_None_SetXWorking_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UModule_Move_None, nullptr, "SetXWorking", 	Z_Construct_UFunction_UModule_Move_None_SetXWorking_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Move_None_SetXWorking_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UModule_Move_None_SetXWorking_Statics::Module_Move_None_eventSetXWorking_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Move_None_SetXWorking_Statics::Function_MetaDataParams), Z_Construct_UFunction_UModule_Move_None_SetXWorking_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UModule_Move_None_SetXWorking_Statics::Module_Move_None_eventSetXWorking_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UModule_Move_None_SetXWorking()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UModule_Move_None_SetXWorking_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UModule_Move_None::execSetXWorking)
{
	P_GET_UBOOL(Z_Param_bValue);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetXWorking(Z_Param_bValue);
	P_NATIVE_END;
}
// ********** End Class UModule_Move_None Function SetXWorking *************************************

// ********** Begin Class UModule_Move_None Function SetYWorking ***********************************
struct Z_Construct_UFunction_UModule_Move_None_SetYWorking_Statics
{
	struct Module_Move_None_eventSetYWorking_Parms
	{
		bool bValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Module_Move_None.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetYWorking constinit property declarations ***************************
	static void NewProp_bValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetYWorking constinit property declarations *****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetYWorking Property Definitions **************************************
void Z_Construct_UFunction_UModule_Move_None_SetYWorking_Statics::NewProp_bValue_SetBit(void* Obj)
{
	((Module_Move_None_eventSetYWorking_Parms*)Obj)->bValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UModule_Move_None_SetYWorking_Statics::NewProp_bValue = { "bValue", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Module_Move_None_eventSetYWorking_Parms), &Z_Construct_UFunction_UModule_Move_None_SetYWorking_Statics::NewProp_bValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UModule_Move_None_SetYWorking_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Move_None_SetYWorking_Statics::NewProp_bValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Move_None_SetYWorking_Statics::PropPointers) < 2048);
// ********** End Function SetYWorking Property Definitions ****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UModule_Move_None_SetYWorking_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UModule_Move_None, nullptr, "SetYWorking", 	Z_Construct_UFunction_UModule_Move_None_SetYWorking_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Move_None_SetYWorking_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UModule_Move_None_SetYWorking_Statics::Module_Move_None_eventSetYWorking_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Move_None_SetYWorking_Statics::Function_MetaDataParams), Z_Construct_UFunction_UModule_Move_None_SetYWorking_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UModule_Move_None_SetYWorking_Statics::Module_Move_None_eventSetYWorking_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UModule_Move_None_SetYWorking()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UModule_Move_None_SetYWorking_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UModule_Move_None::execSetYWorking)
{
	P_GET_UBOOL(Z_Param_bValue);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetYWorking(Z_Param_bValue);
	P_NATIVE_END;
}
// ********** End Class UModule_Move_None Function SetYWorking *************************************

// ********** Begin Class UModule_Move_None ********************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UModule_Move_None;
UClass* UModule_Move_None::GetPrivateStaticClass()
{
	using TClass = UModule_Move_None;
	if (!Z_Registration_Info_UClass_UModule_Move_None.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("Module_Move_None"),
			Z_Registration_Info_UClass_UModule_Move_None.InnerSingleton,
			StaticRegisterNativesUModule_Move_None,
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
	return Z_Registration_Info_UClass_UModule_Move_None.InnerSingleton;
}
UClass* Z_Construct_UClass_UModule_Move_None_NoRegister()
{
	return UModule_Move_None::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UModule_Move_None_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Module_Move_None.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/Module_Move_None.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bXWorking_MetaData[] = {
		{ "ModuleRelativePath", "Public/Module_Move_None.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bYWorking_MetaData[] = {
		{ "ModuleRelativePath", "Public/Module_Move_None.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimerHandle_MetaData[] = {
		{ "ModuleRelativePath", "Public/Module_Move_None.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UModule_Move_None constinit property declarations ************************
	static void NewProp_bXWorking_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bXWorking;
	static void NewProp_bYWorking_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bYWorking;
	static const UECodeGen_Private::FStructPropertyParams NewProp_TimerHandle;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UModule_Move_None constinit property declarations **************************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("GetXWorking"), .Pointer = &UModule_Move_None::execGetXWorking },
		{ .NameUTF8 = UTF8TEXT("GetYWorking"), .Pointer = &UModule_Move_None::execGetYWorking },
		{ .NameUTF8 = UTF8TEXT("SetXWorking"), .Pointer = &UModule_Move_None::execSetXWorking },
		{ .NameUTF8 = UTF8TEXT("SetYWorking"), .Pointer = &UModule_Move_None::execSetYWorking },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UModule_Move_None_GetXWorking, "GetXWorking" }, // 800208209
		{ &Z_Construct_UFunction_UModule_Move_None_GetYWorking, "GetYWorking" }, // 953835338
		{ &Z_Construct_UFunction_UModule_Move_None_SetXWorking, "SetXWorking" }, // 2760508344
		{ &Z_Construct_UFunction_UModule_Move_None_SetYWorking, "SetYWorking" }, // 3493128316
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UModule_Move_None>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UModule_Move_None_Statics

// ********** Begin Class UModule_Move_None Property Definitions ***********************************
void Z_Construct_UClass_UModule_Move_None_Statics::NewProp_bXWorking_SetBit(void* Obj)
{
	((UModule_Move_None*)Obj)->bXWorking = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UModule_Move_None_Statics::NewProp_bXWorking = { "bXWorking", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UModule_Move_None), &Z_Construct_UClass_UModule_Move_None_Statics::NewProp_bXWorking_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bXWorking_MetaData), NewProp_bXWorking_MetaData) };
void Z_Construct_UClass_UModule_Move_None_Statics::NewProp_bYWorking_SetBit(void* Obj)
{
	((UModule_Move_None*)Obj)->bYWorking = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UModule_Move_None_Statics::NewProp_bYWorking = { "bYWorking", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UModule_Move_None), &Z_Construct_UClass_UModule_Move_None_Statics::NewProp_bYWorking_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bYWorking_MetaData), NewProp_bYWorking_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UModule_Move_None_Statics::NewProp_TimerHandle = { "TimerHandle", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Move_None, TimerHandle), Z_Construct_UScriptStruct_FTimerHandle, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimerHandle_MetaData), NewProp_TimerHandle_MetaData) }; // 3998242883
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UModule_Move_None_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Move_None_Statics::NewProp_bXWorking,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Move_None_Statics::NewProp_bYWorking,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Move_None_Statics::NewProp_TimerHandle,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Move_None_Statics::PropPointers) < 2048);
// ********** End Class UModule_Move_None Property Definitions *************************************
UObject* (*const Z_Construct_UClass_UModule_Move_None_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UWindow_Module,
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Move_None_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UModule_Move_None_Statics::ClassParams = {
	&UModule_Move_None::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UModule_Move_None_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Move_None_Statics::PropPointers),
	0,
	0x009000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Move_None_Statics::Class_MetaDataParams), Z_Construct_UClass_UModule_Move_None_Statics::Class_MetaDataParams)
};
void UModule_Move_None::StaticRegisterNativesUModule_Move_None()
{
	UClass* Class = UModule_Move_None::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UModule_Move_None_Statics::Funcs));
}
UClass* Z_Construct_UClass_UModule_Move_None()
{
	if (!Z_Registration_Info_UClass_UModule_Move_None.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UModule_Move_None.OuterSingleton, Z_Construct_UClass_UModule_Move_None_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UModule_Move_None.OuterSingleton;
}
UModule_Move_None::UModule_Move_None(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UModule_Move_None);
UModule_Move_None::~UModule_Move_None() {}
// ********** End Class UModule_Move_None **********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Move_None_h__Script_NX_ModularWindow_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UModule_Move_None, UModule_Move_None::StaticClass, TEXT("UModule_Move_None"), &Z_Registration_Info_UClass_UModule_Move_None, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UModule_Move_None), 1639898638U) },
	};
}; // Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Move_None_h__Script_NX_ModularWindow_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Move_None_h__Script_NX_ModularWindow_906546829{
	TEXT("/Script/NX_ModularWindow"),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Move_None_h__Script_NX_ModularWindow_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Move_None_h__Script_NX_ModularWindow_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
