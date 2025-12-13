// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Window_Timer_Event.h"
#include "Engine/TimerHandle.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeWindow_Timer_Event() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FTimerHandle();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow_NoRegister();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow_Timer_Event();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow_Timer_Event_NoRegister();
UPackage* Z_Construct_UPackage__Script_NX_ModularWindow();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UWindow_Timer_Event ******************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UWindow_Timer_Event;
UClass* UWindow_Timer_Event::GetPrivateStaticClass()
{
	using TClass = UWindow_Timer_Event;
	if (!Z_Registration_Info_UClass_UWindow_Timer_Event.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("Window_Timer_Event"),
			Z_Registration_Info_UClass_UWindow_Timer_Event.InnerSingleton,
			StaticRegisterNativesUWindow_Timer_Event,
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
	return Z_Registration_Info_UClass_UWindow_Timer_Event.InnerSingleton;
}
UClass* Z_Construct_UClass_UWindow_Timer_Event_NoRegister()
{
	return UWindow_Timer_Event::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UWindow_Timer_Event_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Window_Timer_Event.h" },
		{ "ModuleRelativePath", "Public/Window_Timer_Event.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Window_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Window_Timer_Event.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimerPosition_MetaData[] = {
		{ "ModuleRelativePath", "Public/Window_Timer_Event.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimerSize_MetaData[] = {
		{ "ModuleRelativePath", "Public/Window_Timer_Event.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UWindow_Timer_Event constinit property declarations **********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Window;
	static const UECodeGen_Private::FStructPropertyParams NewProp_TimerPosition;
	static const UECodeGen_Private::FStructPropertyParams NewProp_TimerSize;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UWindow_Timer_Event constinit property declarations ************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWindow_Timer_Event>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UWindow_Timer_Event_Statics

// ********** Begin Class UWindow_Timer_Event Property Definitions *********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWindow_Timer_Event_Statics::NewProp_Window = { "Window", nullptr, (EPropertyFlags)0x0010000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWindow_Timer_Event, Window), Z_Construct_UClass_UWindow_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Window_MetaData), NewProp_Window_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UWindow_Timer_Event_Statics::NewProp_TimerPosition = { "TimerPosition", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWindow_Timer_Event, TimerPosition), Z_Construct_UScriptStruct_FTimerHandle, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimerPosition_MetaData), NewProp_TimerPosition_MetaData) }; // 3998242883
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UWindow_Timer_Event_Statics::NewProp_TimerSize = { "TimerSize", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWindow_Timer_Event, TimerSize), Z_Construct_UScriptStruct_FTimerHandle, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimerSize_MetaData), NewProp_TimerSize_MetaData) }; // 3998242883
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UWindow_Timer_Event_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Timer_Event_Statics::NewProp_Window,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Timer_Event_Statics::NewProp_TimerPosition,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Timer_Event_Statics::NewProp_TimerSize,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UWindow_Timer_Event_Statics::PropPointers) < 2048);
// ********** End Class UWindow_Timer_Event Property Definitions ***********************************
UObject* (*const Z_Construct_UClass_UWindow_Timer_Event_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UWindow_Timer_Event_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UWindow_Timer_Event_Statics::ClassParams = {
	&UWindow_Timer_Event::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UWindow_Timer_Event_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UWindow_Timer_Event_Statics::PropPointers),
	0,
	0x009000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UWindow_Timer_Event_Statics::Class_MetaDataParams), Z_Construct_UClass_UWindow_Timer_Event_Statics::Class_MetaDataParams)
};
void UWindow_Timer_Event::StaticRegisterNativesUWindow_Timer_Event()
{
}
UClass* Z_Construct_UClass_UWindow_Timer_Event()
{
	if (!Z_Registration_Info_UClass_UWindow_Timer_Event.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWindow_Timer_Event.OuterSingleton, Z_Construct_UClass_UWindow_Timer_Event_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UWindow_Timer_Event.OuterSingleton;
}
UWindow_Timer_Event::UWindow_Timer_Event(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UWindow_Timer_Event);
UWindow_Timer_Event::~UWindow_Timer_Event() {}
// ********** End Class UWindow_Timer_Event ********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_Timer_Event_h__Script_NX_ModularWindow_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UWindow_Timer_Event, UWindow_Timer_Event::StaticClass, TEXT("UWindow_Timer_Event"), &Z_Registration_Info_UClass_UWindow_Timer_Event, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWindow_Timer_Event), 1028416879U) },
	};
}; // Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_Timer_Event_h__Script_NX_ModularWindow_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_Timer_Event_h__Script_NX_ModularWindow_1568502188{
	TEXT("/Script/NX_ModularWindow"),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_Timer_Event_h__Script_NX_ModularWindow_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_Timer_Event_h__Script_NX_ModularWindow_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
