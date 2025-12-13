// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Module_Limit_Size_None.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeModule_Limit_Size_None() {}

// ********** Begin Cross Module References ********************************************************
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Limit_None();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Limit_Size_None();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UModule_Limit_Size_None_NoRegister();
UMG_API UClass* Z_Construct_UClass_UWidget_NoRegister();
UPackage* Z_Construct_UPackage__Script_NX_ModularWindow();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UModule_Limit_Size_None Function ModifyDown ******************************
struct Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown_Statics
{
	struct Module_Limit_Size_None_eventModifyDown_Parms
	{
		UWidget* InWindow;
		float InValue;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Module_Limit_Size_None.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InWindow_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function ModifyDown constinit property declarations ****************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InWindow;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InValue;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ModifyDown constinit property declarations ******************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ModifyDown Property Definitions ***************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown_Statics::NewProp_InWindow = { "InWindow", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Limit_Size_None_eventModifyDown_Parms, InWindow), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InWindow_MetaData), NewProp_InWindow_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown_Statics::NewProp_InValue = { "InValue", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Limit_Size_None_eventModifyDown_Parms, InValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Limit_Size_None_eventModifyDown_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown_Statics::NewProp_InWindow,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown_Statics::NewProp_InValue,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown_Statics::PropPointers) < 2048);
// ********** End Function ModifyDown Property Definitions *****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UModule_Limit_Size_None, nullptr, "ModifyDown", 	Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown_Statics::Module_Limit_Size_None_eventModifyDown_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown_Statics::Function_MetaDataParams), Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown_Statics::Module_Limit_Size_None_eventModifyDown_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UModule_Limit_Size_None::execModifyDown)
{
	P_GET_OBJECT(UWidget,Z_Param_InWindow);
	P_GET_PROPERTY(FFloatProperty,Z_Param_InValue);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=P_THIS->ModifyDown(Z_Param_InWindow,Z_Param_InValue);
	P_NATIVE_END;
}
// ********** End Class UModule_Limit_Size_None Function ModifyDown ********************************

// ********** Begin Class UModule_Limit_Size_None Function ModifyLeft ******************************
struct Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft_Statics
{
	struct Module_Limit_Size_None_eventModifyLeft_Parms
	{
		UWidget* InWindow;
		float InValue;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Module_Limit_Size_None.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InWindow_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function ModifyLeft constinit property declarations ****************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InWindow;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InValue;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ModifyLeft constinit property declarations ******************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ModifyLeft Property Definitions ***************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft_Statics::NewProp_InWindow = { "InWindow", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Limit_Size_None_eventModifyLeft_Parms, InWindow), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InWindow_MetaData), NewProp_InWindow_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft_Statics::NewProp_InValue = { "InValue", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Limit_Size_None_eventModifyLeft_Parms, InValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Limit_Size_None_eventModifyLeft_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft_Statics::NewProp_InWindow,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft_Statics::NewProp_InValue,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft_Statics::PropPointers) < 2048);
// ********** End Function ModifyLeft Property Definitions *****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UModule_Limit_Size_None, nullptr, "ModifyLeft", 	Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft_Statics::Module_Limit_Size_None_eventModifyLeft_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft_Statics::Function_MetaDataParams), Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft_Statics::Module_Limit_Size_None_eventModifyLeft_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UModule_Limit_Size_None::execModifyLeft)
{
	P_GET_OBJECT(UWidget,Z_Param_InWindow);
	P_GET_PROPERTY(FFloatProperty,Z_Param_InValue);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=P_THIS->ModifyLeft(Z_Param_InWindow,Z_Param_InValue);
	P_NATIVE_END;
}
// ********** End Class UModule_Limit_Size_None Function ModifyLeft ********************************

// ********** Begin Class UModule_Limit_Size_None Function ModifyRight *****************************
struct Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight_Statics
{
	struct Module_Limit_Size_None_eventModifyRight_Parms
	{
		UWidget* InWindow;
		float InValue;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Module_Limit_Size_None.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InWindow_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function ModifyRight constinit property declarations ***************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InWindow;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InValue;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ModifyRight constinit property declarations *****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ModifyRight Property Definitions **************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight_Statics::NewProp_InWindow = { "InWindow", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Limit_Size_None_eventModifyRight_Parms, InWindow), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InWindow_MetaData), NewProp_InWindow_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight_Statics::NewProp_InValue = { "InValue", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Limit_Size_None_eventModifyRight_Parms, InValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Limit_Size_None_eventModifyRight_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight_Statics::NewProp_InWindow,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight_Statics::NewProp_InValue,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight_Statics::PropPointers) < 2048);
// ********** End Function ModifyRight Property Definitions ****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UModule_Limit_Size_None, nullptr, "ModifyRight", 	Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight_Statics::Module_Limit_Size_None_eventModifyRight_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight_Statics::Function_MetaDataParams), Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight_Statics::Module_Limit_Size_None_eventModifyRight_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UModule_Limit_Size_None::execModifyRight)
{
	P_GET_OBJECT(UWidget,Z_Param_InWindow);
	P_GET_PROPERTY(FFloatProperty,Z_Param_InValue);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=P_THIS->ModifyRight(Z_Param_InWindow,Z_Param_InValue);
	P_NATIVE_END;
}
// ********** End Class UModule_Limit_Size_None Function ModifyRight *******************************

// ********** Begin Class UModule_Limit_Size_None Function ModifyUp ********************************
struct Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp_Statics
{
	struct Module_Limit_Size_None_eventModifyUp_Parms
	{
		UWidget* InWindow;
		float InValue;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Module_Limit_Size_None.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InWindow_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function ModifyUp constinit property declarations ******************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InWindow;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InValue;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ModifyUp constinit property declarations ********************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ModifyUp Property Definitions *****************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp_Statics::NewProp_InWindow = { "InWindow", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Limit_Size_None_eventModifyUp_Parms, InWindow), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InWindow_MetaData), NewProp_InWindow_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp_Statics::NewProp_InValue = { "InValue", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Limit_Size_None_eventModifyUp_Parms, InValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Module_Limit_Size_None_eventModifyUp_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp_Statics::NewProp_InWindow,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp_Statics::NewProp_InValue,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp_Statics::PropPointers) < 2048);
// ********** End Function ModifyUp Property Definitions *******************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UModule_Limit_Size_None, nullptr, "ModifyUp", 	Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp_Statics::Module_Limit_Size_None_eventModifyUp_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp_Statics::Function_MetaDataParams), Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp_Statics::Module_Limit_Size_None_eventModifyUp_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UModule_Limit_Size_None::execModifyUp)
{
	P_GET_OBJECT(UWidget,Z_Param_InWindow);
	P_GET_PROPERTY(FFloatProperty,Z_Param_InValue);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=P_THIS->ModifyUp(Z_Param_InWindow,Z_Param_InValue);
	P_NATIVE_END;
}
// ********** End Class UModule_Limit_Size_None Function ModifyUp **********************************

// ********** Begin Class UModule_Limit_Size_None **************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UModule_Limit_Size_None;
UClass* UModule_Limit_Size_None::GetPrivateStaticClass()
{
	using TClass = UModule_Limit_Size_None;
	if (!Z_Registration_Info_UClass_UModule_Limit_Size_None.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("Module_Limit_Size_None"),
			Z_Registration_Info_UClass_UModule_Limit_Size_None.InnerSingleton,
			StaticRegisterNativesUModule_Limit_Size_None,
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
	return Z_Registration_Info_UClass_UModule_Limit_Size_None.InnerSingleton;
}
UClass* Z_Construct_UClass_UModule_Limit_Size_None_NoRegister()
{
	return UModule_Limit_Size_None::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UModule_Limit_Size_None_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Module_Limit_Size_None.h" },
		{ "ModuleRelativePath", "Public/Module_Limit_Size_None.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UModule_Limit_Size_None constinit property declarations ******************
// ********** End Class UModule_Limit_Size_None constinit property declarations ********************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("ModifyDown"), .Pointer = &UModule_Limit_Size_None::execModifyDown },
		{ .NameUTF8 = UTF8TEXT("ModifyLeft"), .Pointer = &UModule_Limit_Size_None::execModifyLeft },
		{ .NameUTF8 = UTF8TEXT("ModifyRight"), .Pointer = &UModule_Limit_Size_None::execModifyRight },
		{ .NameUTF8 = UTF8TEXT("ModifyUp"), .Pointer = &UModule_Limit_Size_None::execModifyUp },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UModule_Limit_Size_None_ModifyDown, "ModifyDown" }, // 3514699399
		{ &Z_Construct_UFunction_UModule_Limit_Size_None_ModifyLeft, "ModifyLeft" }, // 1778468288
		{ &Z_Construct_UFunction_UModule_Limit_Size_None_ModifyRight, "ModifyRight" }, // 2707686034
		{ &Z_Construct_UFunction_UModule_Limit_Size_None_ModifyUp, "ModifyUp" }, // 780068112
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UModule_Limit_Size_None>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UModule_Limit_Size_None_Statics
UObject* (*const Z_Construct_UClass_UModule_Limit_Size_None_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UModule_Limit_None,
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Limit_Size_None_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UModule_Limit_Size_None_Statics::ClassParams = {
	&UModule_Limit_Size_None::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UModule_Limit_Size_None_Statics::Class_MetaDataParams), Z_Construct_UClass_UModule_Limit_Size_None_Statics::Class_MetaDataParams)
};
void UModule_Limit_Size_None::StaticRegisterNativesUModule_Limit_Size_None()
{
	UClass* Class = UModule_Limit_Size_None::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UModule_Limit_Size_None_Statics::Funcs));
}
UClass* Z_Construct_UClass_UModule_Limit_Size_None()
{
	if (!Z_Registration_Info_UClass_UModule_Limit_Size_None.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UModule_Limit_Size_None.OuterSingleton, Z_Construct_UClass_UModule_Limit_Size_None_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UModule_Limit_Size_None.OuterSingleton;
}
UModule_Limit_Size_None::UModule_Limit_Size_None(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UModule_Limit_Size_None);
UModule_Limit_Size_None::~UModule_Limit_Size_None() {}
// ********** End Class UModule_Limit_Size_None ****************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_Size_None_h__Script_NX_ModularWindow_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UModule_Limit_Size_None, UModule_Limit_Size_None::StaticClass, TEXT("UModule_Limit_Size_None"), &Z_Registration_Info_UClass_UModule_Limit_Size_None, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UModule_Limit_Size_None), 304004880U) },
	};
}; // Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_Size_None_h__Script_NX_ModularWindow_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_Size_None_h__Script_NX_ModularWindow_3564487881{
	TEXT("/Script/NX_ModularWindow"),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_Size_None_h__Script_NX_ModularWindow_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Module_Limit_Size_None_h__Script_NX_ModularWindow_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
