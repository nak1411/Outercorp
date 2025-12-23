// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Window_Template_0.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeWindow_Template_0() {}

// ********** Begin Cross Module References ********************************************************
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow_Template_0();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow_Template_0_NoRegister();
UMG_API UClass* Z_Construct_UClass_UVerticalBox_NoRegister();
UMG_API UClass* Z_Construct_UClass_UWidget_NoRegister();
UPackage* Z_Construct_UPackage__Script_NX_ModularWindow();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UWindow_Template_0 Function WindowSetContent *****************************
struct Z_Construct_UFunction_UWindow_Template_0_WindowSetContent_Statics
{
	struct Window_Template_0_eventWindowSetContent_Parms
	{
		UWidget* InWidget;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Window_Template_0.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InWidget_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function WindowSetContent constinit property declarations **********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InWidget;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function WindowSetContent constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function WindowSetContent Property Definitions *********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UWindow_Template_0_WindowSetContent_Statics::NewProp_InWidget = { "InWidget", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_Template_0_eventWindowSetContent_Parms, InWidget), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InWidget_MetaData), NewProp_InWidget_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_Template_0_WindowSetContent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_Template_0_WindowSetContent_Statics::NewProp_InWidget,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_Template_0_WindowSetContent_Statics::PropPointers) < 2048);
// ********** End Function WindowSetContent Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_Template_0_WindowSetContent_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow_Template_0, nullptr, "WindowSetContent", 	Z_Construct_UFunction_UWindow_Template_0_WindowSetContent_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_Template_0_WindowSetContent_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_Template_0_WindowSetContent_Statics::Window_Template_0_eventWindowSetContent_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_Template_0_WindowSetContent_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_Template_0_WindowSetContent_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_Template_0_WindowSetContent_Statics::Window_Template_0_eventWindowSetContent_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_Template_0_WindowSetContent()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_Template_0_WindowSetContent_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow_Template_0::execWindowSetContent)
{
	P_GET_OBJECT(UWidget,Z_Param_InWidget);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->WindowSetContent(Z_Param_InWidget);
	P_NATIVE_END;
}
// ********** End Class UWindow_Template_0 Function WindowSetContent *******************************

// ********** Begin Class UWindow_Template_0 *******************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UWindow_Template_0;
UClass* UWindow_Template_0::GetPrivateStaticClass()
{
	using TClass = UWindow_Template_0;
	if (!Z_Registration_Info_UClass_UWindow_Template_0.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("Window_Template_0"),
			Z_Registration_Info_UClass_UWindow_Template_0.InnerSingleton,
			StaticRegisterNativesUWindow_Template_0,
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
	return Z_Registration_Info_UClass_UWindow_Template_0.InnerSingleton;
}
UClass* Z_Construct_UClass_UWindow_Template_0_NoRegister()
{
	return UWindow_Template_0::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UWindow_Template_0_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Window_Template_0.h" },
		{ "ModuleRelativePath", "Public/Window_Template_0.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_VerticalBox_MetaData[] = {
		{ "Category", "Window" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Data//\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Window_Template_0.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Data" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class UWindow_Template_0 constinit property declarations ***********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_VerticalBox;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UWindow_Template_0 constinit property declarations *************************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("WindowSetContent"), .Pointer = &UWindow_Template_0::execWindowSetContent },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UWindow_Template_0_WindowSetContent, "WindowSetContent" }, // 591997466
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWindow_Template_0>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UWindow_Template_0_Statics

// ********** Begin Class UWindow_Template_0 Property Definitions **********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWindow_Template_0_Statics::NewProp_VerticalBox = { "VerticalBox", nullptr, (EPropertyFlags)0x001000000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWindow_Template_0, VerticalBox), Z_Construct_UClass_UVerticalBox_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_VerticalBox_MetaData), NewProp_VerticalBox_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UWindow_Template_0_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Template_0_Statics::NewProp_VerticalBox,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UWindow_Template_0_Statics::PropPointers) < 2048);
// ********** End Class UWindow_Template_0 Property Definitions ************************************
UObject* (*const Z_Construct_UClass_UWindow_Template_0_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UWindow,
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UWindow_Template_0_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UWindow_Template_0_Statics::ClassParams = {
	&UWindow_Template_0::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UWindow_Template_0_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UWindow_Template_0_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UWindow_Template_0_Statics::Class_MetaDataParams), Z_Construct_UClass_UWindow_Template_0_Statics::Class_MetaDataParams)
};
void UWindow_Template_0::StaticRegisterNativesUWindow_Template_0()
{
	UClass* Class = UWindow_Template_0::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UWindow_Template_0_Statics::Funcs));
}
UClass* Z_Construct_UClass_UWindow_Template_0()
{
	if (!Z_Registration_Info_UClass_UWindow_Template_0.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWindow_Template_0.OuterSingleton, Z_Construct_UClass_UWindow_Template_0_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UWindow_Template_0.OuterSingleton;
}
UWindow_Template_0::UWindow_Template_0(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UWindow_Template_0);
UWindow_Template_0::~UWindow_Template_0() {}
// ********** End Class UWindow_Template_0 *********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_Template_0_h__Script_NX_ModularWindow_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UWindow_Template_0, UWindow_Template_0::StaticClass, TEXT("UWindow_Template_0"), &Z_Registration_Info_UClass_UWindow_Template_0, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWindow_Template_0), 2271499239U) },
	};
}; // Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_Template_0_h__Script_NX_ModularWindow_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_Template_0_h__Script_NX_ModularWindow_1532960485{
	TEXT("/Script/NX_ModularWindow"),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_Template_0_h__Script_NX_ModularWindow_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_Template_0_h__Script_NX_ModularWindow_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
