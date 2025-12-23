// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Module_Size_All_Universal.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeModule_Size_All_Universal() {}

// ********** Begin Cross Module References ********************************************************
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Size_All_Universal();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Size_All_Universal_NoRegister();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Size_AnyAxis_Universal_NoRegister();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow_NoRegister();
UMG_API UClass* Z_Construct_UClass_UButton_NoRegister();
UMG_API UClass* Z_Construct_UClass_UOverlay();
UPackage* Z_Construct_UPackage__Script_NX_ModularWindow();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UModule_Size_All_Universal Function SetEnabled ***************************
struct Z_Construct_UFunction_UModule_Size_All_Universal_SetEnabled_Statics
{
	struct Module_Size_All_Universal_eventSetEnabled_Parms
	{
		bool bEnabled;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Module_Size_All_Universal.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetEnabled constinit property declarations ****************************
	static void NewProp_bEnabled_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnabled;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetEnabled constinit property declarations ******************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetEnabled Property Definitions ***************************************
void Z_Construct_UFunction_UModule_Size_All_Universal_SetEnabled_Statics::NewProp_bEnabled_SetBit(void* Obj)
{
	((Module_Size_All_Universal_eventSetEnabled_Parms*)Obj)->bEnabled = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UModule_Size_All_Universal_SetEnabled_Statics::NewProp_bEnabled = { "bEnabled", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Module_Size_All_Universal_eventSetEnabled_Parms), &Z_Construct_UFunction_UModule_Size_All_Universal_SetEnabled_Statics::NewProp_bEnabled_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UModule_Size_All_Universal_SetEnabled_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Size_All_Universal_SetEnabled_Statics::NewProp_bEnabled,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Size_All_Universal_SetEnabled_Statics::PropPointers) < 2048);
// ********** End Function SetEnabled Property Definitions *****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UModule_Size_All_Universal_SetEnabled_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UModule_Size_All_Universal, nullptr, "SetEnabled", 	Z_Construct_UFunction_UModule_Size_All_Universal_SetEnabled_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Size_All_Universal_SetEnabled_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UModule_Size_All_Universal_SetEnabled_Statics::Module_Size_All_Universal_eventSetEnabled_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Size_All_Universal_SetEnabled_Statics::Function_MetaDataParams), Z_Construct_UFunction_UModule_Size_All_Universal_SetEnabled_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UModule_Size_All_Universal_SetEnabled_Statics::Module_Size_All_Universal_eventSetEnabled_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UModule_Size_All_Universal_SetEnabled()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UModule_Size_All_Universal_SetEnabled_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UModule_Size_All_Universal::execSetEnabled)
{
	P_GET_UBOOL(Z_Param_bEnabled);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetEnabled(Z_Param_bEnabled);
	P_NATIVE_END;
}
// ********** End Class UModule_Size_All_Universal Function SetEnabled *****************************

// ********** Begin Class UModule_Size_All_Universal Function SetWindow ****************************
struct Z_Construct_UFunction_UModule_Size_All_Universal_SetWindow_Statics
{
	struct Module_Size_All_Universal_eventSetWindow_Parms
	{
		UWindow* InWidget;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Module_Size_All_Universal.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InWidget_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetWindow constinit property declarations *****************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InWidget;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetWindow constinit property declarations *******************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetWindow Property Definitions ****************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UModule_Size_All_Universal_SetWindow_Statics::NewProp_InWidget = { "InWidget", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Size_All_Universal_eventSetWindow_Parms, InWidget), Z_Construct_UClass_UWindow_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InWidget_MetaData), NewProp_InWidget_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UModule_Size_All_Universal_SetWindow_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Size_All_Universal_SetWindow_Statics::NewProp_InWidget,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Size_All_Universal_SetWindow_Statics::PropPointers) < 2048);
// ********** End Function SetWindow Property Definitions ******************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UModule_Size_All_Universal_SetWindow_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UModule_Size_All_Universal, nullptr, "SetWindow", 	Z_Construct_UFunction_UModule_Size_All_Universal_SetWindow_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Size_All_Universal_SetWindow_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UModule_Size_All_Universal_SetWindow_Statics::Module_Size_All_Universal_eventSetWindow_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Size_All_Universal_SetWindow_Statics::Function_MetaDataParams), Z_Construct_UFunction_UModule_Size_All_Universal_SetWindow_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UModule_Size_All_Universal_SetWindow_Statics::Module_Size_All_Universal_eventSetWindow_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UModule_Size_All_Universal_SetWindow()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UModule_Size_All_Universal_SetWindow_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UModule_Size_All_Universal::execSetWindow)
{
	P_GET_OBJECT(UWindow,Z_Param_InWidget);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetWindow(Z_Param_InWidget);
	P_NATIVE_END;
}
// ********** End Class UModule_Size_All_Universal Function SetWindow ******************************

// ********** Begin Class UModule_Size_All_Universal ***********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UModule_Size_All_Universal;
UClass* UModule_Size_All_Universal::GetPrivateStaticClass()
{
	using TClass = UModule_Size_All_Universal;
	if (!Z_Registration_Info_UClass_UModule_Size_All_Universal.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("Module_Size_All_Universal"),
			Z_Registration_Info_UClass_UModule_Size_All_Universal.InnerSingleton,
			StaticRegisterNativesUModule_Size_All_Universal,
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
	return Z_Registration_Info_UClass_UModule_Size_All_Universal.InnerSingleton;
}
UClass* Z_Construct_UClass_UModule_Size_All_Universal_NoRegister()
{
	return UModule_Size_All_Universal::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UModule_Size_All_Universal_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Module_Size_All_Universal.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/Module_Size_All_Universal.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Window_MetaData[] = {
		{ "Category", "Window" },
		{ "EditInline", "true" },
		{ "ExposeOnSpawn", "true" },
		{ "ModuleRelativePath", "Public/Module_Size_All_Universal.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Module_Left_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Modules//\n" },
#endif
		{ "ModuleRelativePath", "Public/Module_Size_All_Universal.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Modules" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Module_Up_MetaData[] = {
		{ "ModuleRelativePath", "Public/Module_Size_All_Universal.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Module_Right_MetaData[] = {
		{ "ModuleRelativePath", "Public/Module_Size_All_Universal.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Module_Down_MetaData[] = {
		{ "ModuleRelativePath", "Public/Module_Size_All_Universal.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Left_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Buttons//\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Module_Size_All_Universal.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Buttons" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Up_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Module_Size_All_Universal.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Right_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Module_Size_All_Universal.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Down_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Module_Size_All_Universal.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LeftUp_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Module_Size_All_Universal.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UpRight_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Module_Size_All_Universal.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RightDown_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Module_Size_All_Universal.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DownLeft_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Module_Size_All_Universal.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UModule_Size_All_Universal constinit property declarations ***************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Window;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Module_Left;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Module_Up;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Module_Right;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Module_Down;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Left;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Up;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Right;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Down;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_LeftUp;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_UpRight;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_RightDown;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_DownLeft;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UModule_Size_All_Universal constinit property declarations *****************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("SetEnabled"), .Pointer = &UModule_Size_All_Universal::execSetEnabled },
		{ .NameUTF8 = UTF8TEXT("SetWindow"), .Pointer = &UModule_Size_All_Universal::execSetWindow },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UModule_Size_All_Universal_SetEnabled, "SetEnabled" }, // 3193938455
		{ &Z_Construct_UFunction_UModule_Size_All_Universal_SetWindow, "SetWindow" }, // 4203103007
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UModule_Size_All_Universal>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UModule_Size_All_Universal_Statics

// ********** Begin Class UModule_Size_All_Universal Property Definitions **************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Window = { "Window", nullptr, (EPropertyFlags)0x001300000008000d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Size_All_Universal, Window), Z_Construct_UClass_UWindow_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Window_MetaData), NewProp_Window_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Module_Left = { "Module_Left", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Size_All_Universal, Module_Left), Z_Construct_UClass_UModule_Size_AnyAxis_Universal_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Module_Left_MetaData), NewProp_Module_Left_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Module_Up = { "Module_Up", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Size_All_Universal, Module_Up), Z_Construct_UClass_UModule_Size_AnyAxis_Universal_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Module_Up_MetaData), NewProp_Module_Up_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Module_Right = { "Module_Right", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Size_All_Universal, Module_Right), Z_Construct_UClass_UModule_Size_AnyAxis_Universal_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Module_Right_MetaData), NewProp_Module_Right_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Module_Down = { "Module_Down", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Size_All_Universal, Module_Down), Z_Construct_UClass_UModule_Size_AnyAxis_Universal_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Module_Down_MetaData), NewProp_Module_Down_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Left = { "Left", nullptr, (EPropertyFlags)0x0010000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Size_All_Universal, Left), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Left_MetaData), NewProp_Left_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Up = { "Up", nullptr, (EPropertyFlags)0x0010000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Size_All_Universal, Up), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Up_MetaData), NewProp_Up_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Right = { "Right", nullptr, (EPropertyFlags)0x0010000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Size_All_Universal, Right), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Right_MetaData), NewProp_Right_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Down = { "Down", nullptr, (EPropertyFlags)0x0010000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Size_All_Universal, Down), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Down_MetaData), NewProp_Down_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_LeftUp = { "LeftUp", nullptr, (EPropertyFlags)0x0010000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Size_All_Universal, LeftUp), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LeftUp_MetaData), NewProp_LeftUp_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_UpRight = { "UpRight", nullptr, (EPropertyFlags)0x0010000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Size_All_Universal, UpRight), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UpRight_MetaData), NewProp_UpRight_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_RightDown = { "RightDown", nullptr, (EPropertyFlags)0x0010000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Size_All_Universal, RightDown), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RightDown_MetaData), NewProp_RightDown_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_DownLeft = { "DownLeft", nullptr, (EPropertyFlags)0x0010000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UModule_Size_All_Universal, DownLeft), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DownLeft_MetaData), NewProp_DownLeft_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UModule_Size_All_Universal_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Window,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Module_Left,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Module_Up,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Module_Right,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Module_Down,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Left,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Up,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Right,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_Down,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_LeftUp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_UpRight,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_RightDown,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UModule_Size_All_Universal_Statics::NewProp_DownLeft,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Size_All_Universal_Statics::PropPointers) < 2048);
// ********** End Class UModule_Size_All_Universal Property Definitions ****************************
UObject* (*const Z_Construct_UClass_UModule_Size_All_Universal_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UOverlay,
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Size_All_Universal_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UModule_Size_All_Universal_Statics::ClassParams = {
	&UModule_Size_All_Universal::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UModule_Size_All_Universal_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Size_All_Universal_Statics::PropPointers),
	0,
	0x00B000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Size_All_Universal_Statics::Class_MetaDataParams), Z_Construct_UClass_UModule_Size_All_Universal_Statics::Class_MetaDataParams)
};
void UModule_Size_All_Universal::StaticRegisterNativesUModule_Size_All_Universal()
{
	UClass* Class = UModule_Size_All_Universal::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UModule_Size_All_Universal_Statics::Funcs));
}
UClass* Z_Construct_UClass_UModule_Size_All_Universal()
{
	if (!Z_Registration_Info_UClass_UModule_Size_All_Universal.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UModule_Size_All_Universal.OuterSingleton, Z_Construct_UClass_UModule_Size_All_Universal_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UModule_Size_All_Universal.OuterSingleton;
}
UModule_Size_All_Universal::UModule_Size_All_Universal(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UModule_Size_All_Universal);
UModule_Size_All_Universal::~UModule_Size_All_Universal() {}
// ********** End Class UModule_Size_All_Universal *************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Size_All_Universal_h__Script_NX_ModularWindow_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UModule_Size_All_Universal, UModule_Size_All_Universal::StaticClass, TEXT("UModule_Size_All_Universal"), &Z_Registration_Info_UClass_UModule_Size_All_Universal, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UModule_Size_All_Universal), 1430509110U) },
	};
}; // Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Size_All_Universal_h__Script_NX_ModularWindow_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Size_All_Universal_h__Script_NX_ModularWindow_2254130710{
	TEXT("/Script/NX_ModularWindow"),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Size_All_Universal_h__Script_NX_ModularWindow_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Size_All_Universal_h__Script_NX_ModularWindow_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
