// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "WindowContentInterface.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeWindowContentInterface() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UInterface();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindowContentInterface();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindowContentInterface_NoRegister();
NX_MODULARWINDOW_API UScriptStruct* Z_Construct_UScriptStruct_FWindowCapabilities();
UPackage* Z_Construct_UPackage__Script_NX_ModularWindow();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FWindowCapabilities ***********************************************
struct Z_Construct_UScriptStruct_FWindowCapabilities_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FWindowCapabilities); }
	static inline consteval int16 GetStructAlignment() { return alignof(FWindowCapabilities); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/WindowContentInterface.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanMove_MetaData[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/WindowContentInterface.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanResize_MetaData[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/WindowContentInterface.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanFullscreen_MetaData[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/WindowContentInterface.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FWindowCapabilities constinit property declarations ***************
	static void NewProp_bCanMove_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanMove;
	static void NewProp_bCanResize_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanResize;
	static void NewProp_bCanFullscreen_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanFullscreen;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FWindowCapabilities constinit property declarations *****************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FWindowCapabilities>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FWindowCapabilities_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FWindowCapabilities;
class UScriptStruct* FWindowCapabilities::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FWindowCapabilities.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FWindowCapabilities.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FWindowCapabilities, (UObject*)Z_Construct_UPackage__Script_NX_ModularWindow(), TEXT("WindowCapabilities"));
	}
	return Z_Registration_Info_UScriptStruct_FWindowCapabilities.OuterSingleton;
	}

// ********** Begin ScriptStruct FWindowCapabilities Property Definitions **************************
void Z_Construct_UScriptStruct_FWindowCapabilities_Statics::NewProp_bCanMove_SetBit(void* Obj)
{
	((FWindowCapabilities*)Obj)->bCanMove = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FWindowCapabilities_Statics::NewProp_bCanMove = { "bCanMove", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FWindowCapabilities), &Z_Construct_UScriptStruct_FWindowCapabilities_Statics::NewProp_bCanMove_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanMove_MetaData), NewProp_bCanMove_MetaData) };
void Z_Construct_UScriptStruct_FWindowCapabilities_Statics::NewProp_bCanResize_SetBit(void* Obj)
{
	((FWindowCapabilities*)Obj)->bCanResize = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FWindowCapabilities_Statics::NewProp_bCanResize = { "bCanResize", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FWindowCapabilities), &Z_Construct_UScriptStruct_FWindowCapabilities_Statics::NewProp_bCanResize_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanResize_MetaData), NewProp_bCanResize_MetaData) };
void Z_Construct_UScriptStruct_FWindowCapabilities_Statics::NewProp_bCanFullscreen_SetBit(void* Obj)
{
	((FWindowCapabilities*)Obj)->bCanFullscreen = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FWindowCapabilities_Statics::NewProp_bCanFullscreen = { "bCanFullscreen", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FWindowCapabilities), &Z_Construct_UScriptStruct_FWindowCapabilities_Statics::NewProp_bCanFullscreen_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanFullscreen_MetaData), NewProp_bCanFullscreen_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FWindowCapabilities_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FWindowCapabilities_Statics::NewProp_bCanMove,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FWindowCapabilities_Statics::NewProp_bCanResize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FWindowCapabilities_Statics::NewProp_bCanFullscreen,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FWindowCapabilities_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FWindowCapabilities Property Definitions ****************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FWindowCapabilities_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
	nullptr,
	&NewStructOps,
	"WindowCapabilities",
	Z_Construct_UScriptStruct_FWindowCapabilities_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FWindowCapabilities_Statics::PropPointers),
	sizeof(FWindowCapabilities),
	alignof(FWindowCapabilities),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FWindowCapabilities_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FWindowCapabilities_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FWindowCapabilities()
{
	if (!Z_Registration_Info_UScriptStruct_FWindowCapabilities.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FWindowCapabilities.InnerSingleton, Z_Construct_UScriptStruct_FWindowCapabilities_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FWindowCapabilities.InnerSingleton);
}
// ********** End ScriptStruct FWindowCapabilities *************************************************

// ********** Begin Interface UWindowContentInterface Function GetWindowCapabilities ***************
struct WindowContentInterface_eventGetWindowCapabilities_Parms
{
	FWindowCapabilities ReturnValue;
};
FWindowCapabilities IWindowContentInterface::GetWindowCapabilities() const
{
	check(0 && "Do not directly call Event functions in Interfaces. Call Execute_GetWindowCapabilities instead.");
	WindowContentInterface_eventGetWindowCapabilities_Parms Parms;
	return Parms.ReturnValue;
}
static FName NAME_UWindowContentInterface_GetWindowCapabilities = FName(TEXT("GetWindowCapabilities"));
FWindowCapabilities IWindowContentInterface::Execute_GetWindowCapabilities(const UObject* O)
{
	check(O != NULL);
	check(O->GetClass()->ImplementsInterface(UWindowContentInterface::StaticClass()));
	WindowContentInterface_eventGetWindowCapabilities_Parms Parms;
	UFunction* const Func = O->FindFunction(NAME_UWindowContentInterface_GetWindowCapabilities);
	if (Func)
	{
		const_cast<UObject*>(O)->ProcessEvent(Func, &Parms);
	}
	else if (auto I = (const IWindowContentInterface*)(O->GetNativeInterfaceAddress(UWindowContentInterface::StaticClass())))
	{
		Parms.ReturnValue = I->GetWindowCapabilities_Implementation();
	}
	return Parms.ReturnValue;
}
struct Z_Construct_UFunction_UWindowContentInterface_GetWindowCapabilities_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/WindowContentInterface.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetWindowCapabilities constinit property declarations *****************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetWindowCapabilities constinit property declarations *******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetWindowCapabilities Property Definitions ****************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UWindowContentInterface_GetWindowCapabilities_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(WindowContentInterface_eventGetWindowCapabilities_Parms, ReturnValue), Z_Construct_UScriptStruct_FWindowCapabilities, METADATA_PARAMS(0, nullptr) }; // 235622890
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindowContentInterface_GetWindowCapabilities_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindowContentInterface_GetWindowCapabilities_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindowContentInterface_GetWindowCapabilities_Statics::PropPointers) < 2048);
// ********** End Function GetWindowCapabilities Property Definitions ******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindowContentInterface_GetWindowCapabilities_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindowContentInterface, nullptr, "GetWindowCapabilities", 	Z_Construct_UFunction_UWindowContentInterface_GetWindowCapabilities_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindowContentInterface_GetWindowCapabilities_Statics::PropPointers), 
sizeof(WindowContentInterface_eventGetWindowCapabilities_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x5C020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindowContentInterface_GetWindowCapabilities_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindowContentInterface_GetWindowCapabilities_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(WindowContentInterface_eventGetWindowCapabilities_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindowContentInterface_GetWindowCapabilities()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindowContentInterface_GetWindowCapabilities_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(IWindowContentInterface::execGetWindowCapabilities)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FWindowCapabilities*)Z_Param__Result=P_THIS->GetWindowCapabilities_Implementation();
	P_NATIVE_END;
}
// ********** End Interface UWindowContentInterface Function GetWindowCapabilities *****************

// ********** Begin Interface UWindowContentInterface **********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UWindowContentInterface;
UClass* UWindowContentInterface::GetPrivateStaticClass()
{
	using TClass = UWindowContentInterface;
	if (!Z_Registration_Info_UClass_UWindowContentInterface.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("WindowContentInterface"),
			Z_Registration_Info_UClass_UWindowContentInterface.InnerSingleton,
			StaticRegisterNativesUWindowContentInterface,
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
	return Z_Registration_Info_UClass_UWindowContentInterface.InnerSingleton;
}
UClass* Z_Construct_UClass_UWindowContentInterface_NoRegister()
{
	return UWindowContentInterface::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UWindowContentInterface_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/WindowContentInterface.h" },
	};
#endif // WITH_METADATA

// ********** Begin Interface UWindowContentInterface constinit property declarations **************
// ********** End Interface UWindowContentInterface constinit property declarations ****************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("GetWindowCapabilities"), .Pointer = &IWindowContentInterface::execGetWindowCapabilities },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UWindowContentInterface_GetWindowCapabilities, "GetWindowCapabilities" }, // 4076815698
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<IWindowContentInterface>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UWindowContentInterface_Statics
UObject* (*const Z_Construct_UClass_UWindowContentInterface_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInterface,
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UWindowContentInterface_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UWindowContentInterface_Statics::ClassParams = {
	&UWindowContentInterface::StaticClass,
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
	0x000840A1u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UWindowContentInterface_Statics::Class_MetaDataParams), Z_Construct_UClass_UWindowContentInterface_Statics::Class_MetaDataParams)
};
void UWindowContentInterface::StaticRegisterNativesUWindowContentInterface()
{
	UClass* Class = UWindowContentInterface::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UWindowContentInterface_Statics::Funcs));
}
UClass* Z_Construct_UClass_UWindowContentInterface()
{
	if (!Z_Registration_Info_UClass_UWindowContentInterface.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWindowContentInterface.OuterSingleton, Z_Construct_UClass_UWindowContentInterface_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UWindowContentInterface.OuterSingleton;
}
UWindowContentInterface::UWindowContentInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UWindowContentInterface);
// ********** End Interface UWindowContentInterface ************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h__Script_NX_ModularWindow_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FWindowCapabilities::StaticStruct, Z_Construct_UScriptStruct_FWindowCapabilities_Statics::NewStructOps, TEXT("WindowCapabilities"),&Z_Registration_Info_UScriptStruct_FWindowCapabilities, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FWindowCapabilities), 235622890U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UWindowContentInterface, UWindowContentInterface::StaticClass, TEXT("UWindowContentInterface"), &Z_Registration_Info_UClass_UWindowContentInterface, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWindowContentInterface), 2881561737U) },
	};
}; // Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h__Script_NX_ModularWindow_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h__Script_NX_ModularWindow_2684890801{
	TEXT("/Script/NX_ModularWindow"),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h__Script_NX_ModularWindow_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h__Script_NX_ModularWindow_Statics::ClassInfo),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h__Script_NX_ModularWindow_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_WindowContentInterface_h__Script_NX_ModularWindow_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
