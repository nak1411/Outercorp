// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "DraggableWindowUMG/DraggableWindow.h"
#include "Blueprint/UserWidget.h"
#include "Components/SlateWrapperTypes.h"
#include "Input/Events.h"
#include "InputCoreTypes.h"
#include "Layout/Geometry.h"
#include "UObject/Class.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeDraggableWindow() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UClass_NoRegister();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
DRAGGABLEWINDOWUMG_API UClass* Z_Construct_UClass_UDraggableWindow();
DRAGGABLEWINDOWUMG_API UClass* Z_Construct_UClass_UDraggableWindow_NoRegister();
DRAGGABLEWINDOWUMG_API UEnum* Z_Construct_UEnum_DraggableWindowUMG_EOperatingSystemWindowStartupState();
INPUTCORE_API UScriptStruct* Z_Construct_UScriptStruct_FKey();
SLATECORE_API UScriptStruct* Z_Construct_UScriptStruct_FGeometry();
SLATECORE_API UScriptStruct* Z_Construct_UScriptStruct_FPointerEvent();
UMG_API UClass* Z_Construct_UClass_UBorder_NoRegister();
UMG_API UClass* Z_Construct_UClass_UCanvasPanel_NoRegister();
UMG_API UClass* Z_Construct_UClass_UCanvasPanelSlot_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UMG_API UClass* Z_Construct_UClass_UUserWidget_NoRegister();
UMG_API UScriptStruct* Z_Construct_UScriptStruct_FEventReply();
UPackage* Z_Construct_UPackage__Script_DraggableWindowUMG();
// ********** End Cross Module References **********************************************************

// ********** Begin Enum EOperatingSystemWindowStartupState ****************************************
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EOperatingSystemWindowStartupState;
static UEnum* EOperatingSystemWindowStartupState_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EOperatingSystemWindowStartupState.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EOperatingSystemWindowStartupState.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_DraggableWindowUMG_EOperatingSystemWindowStartupState, (UObject*)Z_Construct_UPackage__Script_DraggableWindowUMG(), TEXT("EOperatingSystemWindowStartupState"));
	}
	return Z_Registration_Info_UEnum_EOperatingSystemWindowStartupState.OuterSingleton;
}
template<> DRAGGABLEWINDOWUMG_NON_ATTRIBUTED_API UEnum* StaticEnum<EOperatingSystemWindowStartupState>()
{
	return EOperatingSystemWindowStartupState_StaticEnum();
}
struct Z_Construct_UEnum_DraggableWindowUMG_EOperatingSystemWindowStartupState_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "Default.Name", "EOperatingSystemWindowStartupState::Default" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "StartMaximized.Name", "EOperatingSystemWindowStartupState::StartMaximized" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EOperatingSystemWindowStartupState::Default", (int64)EOperatingSystemWindowStartupState::Default },
		{ "EOperatingSystemWindowStartupState::StartMaximized", (int64)EOperatingSystemWindowStartupState::StartMaximized },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
}; // struct Z_Construct_UEnum_DraggableWindowUMG_EOperatingSystemWindowStartupState_Statics 
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_DraggableWindowUMG_EOperatingSystemWindowStartupState_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_DraggableWindowUMG,
	nullptr,
	"EOperatingSystemWindowStartupState",
	"EOperatingSystemWindowStartupState",
	Z_Construct_UEnum_DraggableWindowUMG_EOperatingSystemWindowStartupState_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_DraggableWindowUMG_EOperatingSystemWindowStartupState_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_DraggableWindowUMG_EOperatingSystemWindowStartupState_Statics::Enum_MetaDataParams), Z_Construct_UEnum_DraggableWindowUMG_EOperatingSystemWindowStartupState_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_DraggableWindowUMG_EOperatingSystemWindowStartupState()
{
	if (!Z_Registration_Info_UEnum_EOperatingSystemWindowStartupState.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EOperatingSystemWindowStartupState.InnerSingleton, Z_Construct_UEnum_DraggableWindowUMG_EOperatingSystemWindowStartupState_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EOperatingSystemWindowStartupState.InnerSingleton;
}
// ********** End Enum EOperatingSystemWindowStartupState ******************************************

// ********** Begin Class UDraggableWindow Function AddContentWidget *******************************
struct Z_Construct_UFunction_UDraggableWindow_AddContentWidget_Statics
{
	struct DraggableWindow_eventAddContentWidget_Parms
	{
		UUserWidget* InWidget;
		bool bClearPrevious;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Draggable Window" },
		{ "Comment", "/**\n\x09* protected UDraggableWindow::AddContentWidget\n\x09* Adds a content to this window.\n\x09* @See AddContentWidgetClass().\n\x09* @param InWidget [class UUserWidget*] Reference to an existing content.\n\x09* @param bClearPrevious [const bool] Should we clear all existing children from Child Widget Canvas? Recommended to keep this on.\n\x09**/" },
		{ "CPP_Default_bClearPrevious", "true" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "protected UDraggableWindow::AddContentWidget\nAdds a content to this window.\n@See AddContentWidgetClass().\n@param InWidget [class UUserWidget*] Reference to an existing content.\n@param bClearPrevious [const bool] Should we clear all existing children from Child Widget Canvas? Recommended to keep this on." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InWidget_MetaData[] = {
		{ "EditInline", "true" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bClearPrevious_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function AddContentWidget constinit property declarations **********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InWidget;
	static void NewProp_bClearPrevious_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bClearPrevious;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function AddContentWidget constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function AddContentWidget Property Definitions *********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UDraggableWindow_AddContentWidget_Statics::NewProp_InWidget = { "InWidget", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventAddContentWidget_Parms, InWidget), Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InWidget_MetaData), NewProp_InWidget_MetaData) };
void Z_Construct_UFunction_UDraggableWindow_AddContentWidget_Statics::NewProp_bClearPrevious_SetBit(void* Obj)
{
	((DraggableWindow_eventAddContentWidget_Parms*)Obj)->bClearPrevious = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UDraggableWindow_AddContentWidget_Statics::NewProp_bClearPrevious = { "bClearPrevious", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(DraggableWindow_eventAddContentWidget_Parms), &Z_Construct_UFunction_UDraggableWindow_AddContentWidget_Statics::NewProp_bClearPrevious_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bClearPrevious_MetaData), NewProp_bClearPrevious_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UDraggableWindow_AddContentWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_AddContentWidget_Statics::NewProp_InWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_AddContentWidget_Statics::NewProp_bClearPrevious,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_AddContentWidget_Statics::PropPointers) < 2048);
// ********** End Function AddContentWidget Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_AddContentWidget_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "AddContentWidget", 	Z_Construct_UFunction_UDraggableWindow_AddContentWidget_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_AddContentWidget_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UDraggableWindow_AddContentWidget_Statics::DraggableWindow_eventAddContentWidget_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04080409, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_AddContentWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_AddContentWidget_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UDraggableWindow_AddContentWidget_Statics::DraggableWindow_eventAddContentWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UDraggableWindow_AddContentWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_AddContentWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UDraggableWindow::execAddContentWidget)
{
	P_GET_OBJECT(UUserWidget,Z_Param_InWidget);
	P_GET_UBOOL(Z_Param_bClearPrevious);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->AddContentWidget(Z_Param_InWidget,Z_Param_bClearPrevious);
	P_NATIVE_END;
}
// ********** End Class UDraggableWindow Function AddContentWidget *********************************

// ********** Begin Class UDraggableWindow Function AddContentWidgetClass **************************
struct Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass_Statics
{
	struct DraggableWindow_eventAddContentWidgetClass_Parms
	{
		TSubclassOf<UUserWidget> InWidgetClass;
		bool bClearPrevious;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Draggable Window" },
		{ "Comment", "/**\n\x09* protected UDraggableWindow::AddContentWidgetClass\n\x09* Adds a content to this window. A new widget is created from the given class.\n\x09* @See AddContentWidget().\n\x09* @param InWidgetClass [TSubclassOf<class UUserWidget>] Widget class to use as content for this window.\n\x09* @param bClearPrevious [const bool] Should we clear all existing children from Child Widget Canvas? Recommended to keep this on.\n\x09**/" },
		{ "CPP_Default_bClearPrevious", "true" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "protected UDraggableWindow::AddContentWidgetClass\nAdds a content to this window. A new widget is created from the given class.\n@See AddContentWidget().\n@param InWidgetClass [TSubclassOf<class UUserWidget>] Widget class to use as content for this window.\n@param bClearPrevious [const bool] Should we clear all existing children from Child Widget Canvas? Recommended to keep this on." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bClearPrevious_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function AddContentWidgetClass constinit property declarations *****************
	static const UECodeGen_Private::FClassPropertyParams NewProp_InWidgetClass;
	static void NewProp_bClearPrevious_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bClearPrevious;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function AddContentWidgetClass constinit property declarations *******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function AddContentWidgetClass Property Definitions ****************************
const UECodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass_Statics::NewProp_InWidgetClass = { "InWidgetClass", nullptr, (EPropertyFlags)0x0014000000000080, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventAddContentWidgetClass_Parms, InWidgetClass), Z_Construct_UClass_UClass_NoRegister, Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass_Statics::NewProp_bClearPrevious_SetBit(void* Obj)
{
	((DraggableWindow_eventAddContentWidgetClass_Parms*)Obj)->bClearPrevious = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass_Statics::NewProp_bClearPrevious = { "bClearPrevious", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(DraggableWindow_eventAddContentWidgetClass_Parms), &Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass_Statics::NewProp_bClearPrevious_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bClearPrevious_MetaData), NewProp_bClearPrevious_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass_Statics::NewProp_InWidgetClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass_Statics::NewProp_bClearPrevious,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass_Statics::PropPointers) < 2048);
// ********** End Function AddContentWidgetClass Property Definitions ******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "AddContentWidgetClass", 	Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass_Statics::DraggableWindow_eventAddContentWidgetClass_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04080409, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass_Statics::DraggableWindow_eventAddContentWidgetClass_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UDraggableWindow::execAddContentWidgetClass)
{
	P_GET_OBJECT(UClass,Z_Param_InWidgetClass);
	P_GET_UBOOL(Z_Param_bClearPrevious);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->AddContentWidgetClass(Z_Param_InWidgetClass,Z_Param_bClearPrevious);
	P_NATIVE_END;
}
// ********** End Class UDraggableWindow Function AddContentWidgetClass ****************************

// ********** Begin Class UDraggableWindow Function CenterWindowToScreen ***************************
struct Z_Construct_UFunction_UDraggableWindow_CenterWindowToScreen_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Draggable Window" },
		{ "Comment", "/**\n\x09* protected UDraggableWindow::CenterWindowToScreen\n\x09* Center this window to screen. Will not work if window is maximized.\n\x09**/" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "protected UDraggableWindow::CenterWindowToScreen\nCenter this window to screen. Will not work if window is maximized." },
	};
#endif // WITH_METADATA

// ********** Begin Function CenterWindowToScreen constinit property declarations ******************
// ********** End Function CenterWindowToScreen constinit property declarations ********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_CenterWindowToScreen_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "CenterWindowToScreen", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04080409, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_CenterWindowToScreen_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_CenterWindowToScreen_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UDraggableWindow_CenterWindowToScreen()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_CenterWindowToScreen_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UDraggableWindow::execCenterWindowToScreen)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->CenterWindowToScreen();
	P_NATIVE_END;
}
// ********** End Class UDraggableWindow Function CenterWindowToScreen *****************************

// ********** Begin Class UDraggableWindow Function CloseWindow ************************************
struct Z_Construct_UFunction_UDraggableWindow_CloseWindow_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Draggable Window" },
		{ "Comment", "/**\n\x09 * public UDraggableWindow::CloseWindow\n\x09 * Closes this window. This will first call the OnCloseWindow native event and then calls OnCloseWindow on Blueprint.\n\x09 * Lastly, it will clear the child canvas and removes itself from whatever it was added to.\n\x09 */" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "public UDraggableWindow::CloseWindow\nCloses this window. This will first call the OnCloseWindow native event and then calls OnCloseWindow on Blueprint.\nLastly, it will clear the child canvas and removes itself from whatever it was added to." },
	};
#endif // WITH_METADATA

// ********** Begin Function CloseWindow constinit property declarations ***************************
// ********** End Function CloseWindow constinit property declarations *****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_CloseWindow_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "CloseWindow", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020409, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_CloseWindow_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_CloseWindow_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UDraggableWindow_CloseWindow()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_CloseWindow_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UDraggableWindow::execCloseWindow)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->CloseWindow();
	P_NATIVE_END;
}
// ********** End Class UDraggableWindow Function CloseWindow **************************************

// ********** Begin Class UDraggableWindow Function CreateDraggableWindow **************************
struct Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics
{
	struct DraggableWindow_eventCreateDraggableWindow_Parms
	{
		const UObject* WorldContextObject;
		TSubclassOf<UDraggableWindow> WindowClass;
		TSubclassOf<UUserWidget> ChildWidgetClass;
		FVector2D StartSize;
		FVector2D ResizeWidth;
		FVector2D ResizeHeight;
		bool bCenterToScreen;
		bool bCanDrag;
		bool bCanResize;
		EOperatingSystemWindowStartupState WindowStartupState;
		UDraggableWindow* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Draggable Window" },
		{ "Comment", "/**\n\x09* public static UDraggableWindow::CreateDraggableWindow\n\x09* Creates a draggable window widget instance from given widget class. You can optionally add a child widget also which will be the content for this window.\n\x09* @param WorldContextObject [const UObject*] World context object.\n\x09* @param WindowClass [TSubclassOf<UDraggableWindow>] Window widget class to create.\n\x09* @param ChildWidgetClass [TSubclassOf<UUserWidget>] [OPTIONAL] Child content class to add to the newly created window widget.\n\x09* @param StartSize [const FVector2D] What size should we start the window.\n\x09* @param ResizeWidth [const FVector2D] Min (X) and Max (Y) width of this window.\n\x09* @param ResizeHeight [const FVector2D] Min (X) and Max (Y) height of this window.\n\x09* @param bCenterToScreen [const bool] Should we center the window to the player screen.\n\x09* @param bCanDrag [const bool] Determines if the window can be dragged.\n\x09* @param bCanResize [const bool] Determines if the window can be resized (You must have a Border widget with Is Variable set to true and variable name set to ResizeAreaWidget).\n\x09* @param WindowStartupState [const EOperatingSystemWindowStartupState] Window state to apply.\n\x09* @return [UDraggableWindow*] Newly created window widget.\n\x09**/" },
		{ "CPP_Default_bCanDrag", "true" },
		{ "CPP_Default_bCanResize", "true" },
		{ "CPP_Default_bCenterToScreen", "true" },
		{ "CPP_Default_ResizeHeight", "(X=0.000,Y=0.000)" },
		{ "CPP_Default_ResizeWidth", "(X=640.000,Y=480.000)" },
		{ "CPP_Default_StartSize", "(X=640.000,Y=480.000)" },
		{ "CPP_Default_WindowStartupState", "Default" },
		{ "DeterminesOutputType", "ChildWidgetClass" },
		{ "DisplayName", "Create Draggable Window (Child Widget Class)" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "public static UDraggableWindow::CreateDraggableWindow\nCreates a draggable window widget instance from given widget class. You can optionally add a child widget also which will be the content for this window.\n@param WorldContextObject [const UObject*] World context object.\n@param WindowClass [TSubclassOf<UDraggableWindow>] Window widget class to create.\n@param ChildWidgetClass [TSubclassOf<UUserWidget>] [OPTIONAL] Child content class to add to the newly created window widget.\n@param StartSize [const FVector2D] What size should we start the window.\n@param ResizeWidth [const FVector2D] Min (X) and Max (Y) width of this window.\n@param ResizeHeight [const FVector2D] Min (X) and Max (Y) height of this window.\n@param bCenterToScreen [const bool] Should we center the window to the player screen.\n@param bCanDrag [const bool] Determines if the window can be dragged.\n@param bCanResize [const bool] Determines if the window can be resized (You must have a Border widget with Is Variable set to true and variable name set to ResizeAreaWidget).\n@param WindowStartupState [const EOperatingSystemWindowStartupState] Window state to apply.\n@return [UDraggableWindow*] Newly created window widget." },
		{ "WorldContext", "WorldContextObject" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WorldContextObject_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StartSize_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ResizeWidth_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ResizeHeight_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCenterToScreen_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanDrag_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanResize_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WindowStartupState_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function CreateDraggableWindow constinit property declarations *****************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_WorldContextObject;
	static const UECodeGen_Private::FClassPropertyParams NewProp_WindowClass;
	static const UECodeGen_Private::FClassPropertyParams NewProp_ChildWidgetClass;
	static const UECodeGen_Private::FStructPropertyParams NewProp_StartSize;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ResizeWidth;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ResizeHeight;
	static void NewProp_bCenterToScreen_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCenterToScreen;
	static void NewProp_bCanDrag_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanDrag;
	static void NewProp_bCanResize_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanResize;
	static const UECodeGen_Private::FBytePropertyParams NewProp_WindowStartupState_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_WindowStartupState;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function CreateDraggableWindow constinit property declarations *******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function CreateDraggableWindow Property Definitions ****************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_WorldContextObject = { "WorldContextObject", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventCreateDraggableWindow_Parms, WorldContextObject), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WorldContextObject_MetaData), NewProp_WorldContextObject_MetaData) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_WindowClass = { "WindowClass", nullptr, (EPropertyFlags)0x0014000000000080, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventCreateDraggableWindow_Parms, WindowClass), Z_Construct_UClass_UClass_NoRegister, Z_Construct_UClass_UDraggableWindow_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_ChildWidgetClass = { "ChildWidgetClass", nullptr, (EPropertyFlags)0x0014000000000080, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventCreateDraggableWindow_Parms, ChildWidgetClass), Z_Construct_UClass_UClass_NoRegister, Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_StartSize = { "StartSize", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventCreateDraggableWindow_Parms, StartSize), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StartSize_MetaData), NewProp_StartSize_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_ResizeWidth = { "ResizeWidth", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventCreateDraggableWindow_Parms, ResizeWidth), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ResizeWidth_MetaData), NewProp_ResizeWidth_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_ResizeHeight = { "ResizeHeight", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventCreateDraggableWindow_Parms, ResizeHeight), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ResizeHeight_MetaData), NewProp_ResizeHeight_MetaData) };
void Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_bCenterToScreen_SetBit(void* Obj)
{
	((DraggableWindow_eventCreateDraggableWindow_Parms*)Obj)->bCenterToScreen = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_bCenterToScreen = { "bCenterToScreen", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(DraggableWindow_eventCreateDraggableWindow_Parms), &Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_bCenterToScreen_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCenterToScreen_MetaData), NewProp_bCenterToScreen_MetaData) };
void Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_bCanDrag_SetBit(void* Obj)
{
	((DraggableWindow_eventCreateDraggableWindow_Parms*)Obj)->bCanDrag = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_bCanDrag = { "bCanDrag", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(DraggableWindow_eventCreateDraggableWindow_Parms), &Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_bCanDrag_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanDrag_MetaData), NewProp_bCanDrag_MetaData) };
void Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_bCanResize_SetBit(void* Obj)
{
	((DraggableWindow_eventCreateDraggableWindow_Parms*)Obj)->bCanResize = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_bCanResize = { "bCanResize", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(DraggableWindow_eventCreateDraggableWindow_Parms), &Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_bCanResize_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanResize_MetaData), NewProp_bCanResize_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_WindowStartupState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_WindowStartupState = { "WindowStartupState", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventCreateDraggableWindow_Parms, WindowStartupState), Z_Construct_UEnum_DraggableWindowUMG_EOperatingSystemWindowStartupState, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WindowStartupState_MetaData), NewProp_WindowStartupState_MetaData) }; // 2372957549
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventCreateDraggableWindow_Parms, ReturnValue), Z_Construct_UClass_UDraggableWindow_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_WorldContextObject,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_WindowClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_ChildWidgetClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_StartSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_ResizeWidth,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_ResizeHeight,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_bCenterToScreen,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_bCanDrag,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_bCanResize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_WindowStartupState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_WindowStartupState,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::PropPointers) < 2048);
// ********** End Function CreateDraggableWindow Property Definitions ******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "CreateDraggableWindow", 	Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::DraggableWindow_eventCreateDraggableWindow_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822409, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::DraggableWindow_eventCreateDraggableWindow_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UDraggableWindow::execCreateDraggableWindow)
{
	P_GET_OBJECT(UObject,Z_Param_WorldContextObject);
	P_GET_OBJECT(UClass,Z_Param_WindowClass);
	P_GET_OBJECT(UClass,Z_Param_ChildWidgetClass);
	P_GET_STRUCT(FVector2D,Z_Param_StartSize);
	P_GET_STRUCT(FVector2D,Z_Param_ResizeWidth);
	P_GET_STRUCT(FVector2D,Z_Param_ResizeHeight);
	P_GET_UBOOL(Z_Param_bCenterToScreen);
	P_GET_UBOOL(Z_Param_bCanDrag);
	P_GET_UBOOL(Z_Param_bCanResize);
	P_GET_ENUM(EOperatingSystemWindowStartupState,Z_Param_WindowStartupState);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UDraggableWindow**)Z_Param__Result=UDraggableWindow::CreateDraggableWindow(Z_Param_WorldContextObject,Z_Param_WindowClass,Z_Param_ChildWidgetClass,Z_Param_StartSize,Z_Param_ResizeWidth,Z_Param_ResizeHeight,Z_Param_bCenterToScreen,Z_Param_bCanDrag,Z_Param_bCanResize,EOperatingSystemWindowStartupState(Z_Param_WindowStartupState));
	P_NATIVE_END;
}
// ********** End Class UDraggableWindow Function CreateDraggableWindow ****************************

// ********** Begin Class UDraggableWindow Function CreateDraggableWindowFromWidget ****************
struct Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics
{
	struct DraggableWindow_eventCreateDraggableWindowFromWidget_Parms
	{
		const UObject* WorldContextObject;
		TSubclassOf<UDraggableWindow> WindowClass;
		UUserWidget* NewChildWidget;
		FVector2D StartSize;
		FVector2D ResizeWidth;
		FVector2D ResizeHeight;
		bool bCenterToScreen;
		bool bCanDrag;
		bool bCanResize;
		EOperatingSystemWindowStartupState WindowStartupState;
		UDraggableWindow* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Draggable Window" },
		{ "Comment", "/**\n\x09* public static UDraggableWindow::CreateDraggableWindowFromWidget\n\x09* Creates a draggable window widget instance from given widget class. You can optionally add a child widget also which will be the content for this window.\n\x09* @param WorldContextObject [const UObject*] World context object.\n\x09* @param WindowClass [TSubclassOf<UDraggableWindow>] Window widget class to create.\n\x09* @param NewChildWidget [UUserWidget*] [OPTIONAL] Child content to add to the newly created window widget.\n\x09* @param StartSize [const FVector2D] What size should we start the window.\n\x09* @param ResizeWidth [const FVector2D] Min (X) and Max (Y) width of this window.\n\x09* @param ResizeHeight [const FVector2D] Min (X) and Max (Y) height of this window.\n\x09* @param bCenterToScreen [const bool] Should we center the window to the player screen.\n\x09* @param bCanDrag [const bool] Determines if the window can be dragged.\n\x09* @param bCanResize [const bool] Determines if the window can be resized (You must have a Border widget with Is Variable set to true and variable name set to ResizeAreaWidget).\n\x09* @param WindowStartupState [const EOperatingSystemWindowStartupState] Window state to apply.\n\x09* @return [UDraggableWindow*] Newly created window widget.\n\x09**/" },
		{ "CPP_Default_bCanDrag", "true" },
		{ "CPP_Default_bCanResize", "true" },
		{ "CPP_Default_bCenterToScreen", "true" },
		{ "CPP_Default_ResizeHeight", "(X=0.000,Y=0.000)" },
		{ "CPP_Default_ResizeWidth", "(X=640.000,Y=480.000)" },
		{ "CPP_Default_StartSize", "(X=640.000,Y=480.000)" },
		{ "CPP_Default_WindowStartupState", "Default" },
		{ "DisplayName", "Create Draggable Window (Child Widget Reference)" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "public static UDraggableWindow::CreateDraggableWindowFromWidget\nCreates a draggable window widget instance from given widget class. You can optionally add a child widget also which will be the content for this window.\n@param WorldContextObject [const UObject*] World context object.\n@param WindowClass [TSubclassOf<UDraggableWindow>] Window widget class to create.\n@param NewChildWidget [UUserWidget*] [OPTIONAL] Child content to add to the newly created window widget.\n@param StartSize [const FVector2D] What size should we start the window.\n@param ResizeWidth [const FVector2D] Min (X) and Max (Y) width of this window.\n@param ResizeHeight [const FVector2D] Min (X) and Max (Y) height of this window.\n@param bCenterToScreen [const bool] Should we center the window to the player screen.\n@param bCanDrag [const bool] Determines if the window can be dragged.\n@param bCanResize [const bool] Determines if the window can be resized (You must have a Border widget with Is Variable set to true and variable name set to ResizeAreaWidget).\n@param WindowStartupState [const EOperatingSystemWindowStartupState] Window state to apply.\n@return [UDraggableWindow*] Newly created window widget." },
		{ "WorldContext", "WorldContextObject" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WorldContextObject_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NewChildWidget_MetaData[] = {
		{ "EditInline", "true" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StartSize_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ResizeWidth_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ResizeHeight_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCenterToScreen_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanDrag_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanResize_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WindowStartupState_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function CreateDraggableWindowFromWidget constinit property declarations *******
	static const UECodeGen_Private::FObjectPropertyParams NewProp_WorldContextObject;
	static const UECodeGen_Private::FClassPropertyParams NewProp_WindowClass;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NewChildWidget;
	static const UECodeGen_Private::FStructPropertyParams NewProp_StartSize;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ResizeWidth;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ResizeHeight;
	static void NewProp_bCenterToScreen_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCenterToScreen;
	static void NewProp_bCanDrag_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanDrag;
	static void NewProp_bCanResize_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanResize;
	static const UECodeGen_Private::FBytePropertyParams NewProp_WindowStartupState_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_WindowStartupState;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function CreateDraggableWindowFromWidget constinit property declarations *********
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function CreateDraggableWindowFromWidget Property Definitions ******************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_WorldContextObject = { "WorldContextObject", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventCreateDraggableWindowFromWidget_Parms, WorldContextObject), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WorldContextObject_MetaData), NewProp_WorldContextObject_MetaData) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_WindowClass = { "WindowClass", nullptr, (EPropertyFlags)0x0014000000000080, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventCreateDraggableWindowFromWidget_Parms, WindowClass), Z_Construct_UClass_UClass_NoRegister, Z_Construct_UClass_UDraggableWindow_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_NewChildWidget = { "NewChildWidget", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventCreateDraggableWindowFromWidget_Parms, NewChildWidget), Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NewChildWidget_MetaData), NewProp_NewChildWidget_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_StartSize = { "StartSize", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventCreateDraggableWindowFromWidget_Parms, StartSize), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StartSize_MetaData), NewProp_StartSize_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_ResizeWidth = { "ResizeWidth", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventCreateDraggableWindowFromWidget_Parms, ResizeWidth), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ResizeWidth_MetaData), NewProp_ResizeWidth_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_ResizeHeight = { "ResizeHeight", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventCreateDraggableWindowFromWidget_Parms, ResizeHeight), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ResizeHeight_MetaData), NewProp_ResizeHeight_MetaData) };
void Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_bCenterToScreen_SetBit(void* Obj)
{
	((DraggableWindow_eventCreateDraggableWindowFromWidget_Parms*)Obj)->bCenterToScreen = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_bCenterToScreen = { "bCenterToScreen", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(DraggableWindow_eventCreateDraggableWindowFromWidget_Parms), &Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_bCenterToScreen_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCenterToScreen_MetaData), NewProp_bCenterToScreen_MetaData) };
void Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_bCanDrag_SetBit(void* Obj)
{
	((DraggableWindow_eventCreateDraggableWindowFromWidget_Parms*)Obj)->bCanDrag = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_bCanDrag = { "bCanDrag", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(DraggableWindow_eventCreateDraggableWindowFromWidget_Parms), &Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_bCanDrag_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanDrag_MetaData), NewProp_bCanDrag_MetaData) };
void Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_bCanResize_SetBit(void* Obj)
{
	((DraggableWindow_eventCreateDraggableWindowFromWidget_Parms*)Obj)->bCanResize = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_bCanResize = { "bCanResize", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(DraggableWindow_eventCreateDraggableWindowFromWidget_Parms), &Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_bCanResize_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanResize_MetaData), NewProp_bCanResize_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_WindowStartupState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_WindowStartupState = { "WindowStartupState", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventCreateDraggableWindowFromWidget_Parms, WindowStartupState), Z_Construct_UEnum_DraggableWindowUMG_EOperatingSystemWindowStartupState, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WindowStartupState_MetaData), NewProp_WindowStartupState_MetaData) }; // 2372957549
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventCreateDraggableWindowFromWidget_Parms, ReturnValue), Z_Construct_UClass_UDraggableWindow_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_WorldContextObject,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_WindowClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_NewChildWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_StartSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_ResizeWidth,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_ResizeHeight,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_bCenterToScreen,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_bCanDrag,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_bCanResize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_WindowStartupState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_WindowStartupState,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::PropPointers) < 2048);
// ********** End Function CreateDraggableWindowFromWidget Property Definitions ********************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "CreateDraggableWindowFromWidget", 	Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::DraggableWindow_eventCreateDraggableWindowFromWidget_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822409, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::DraggableWindow_eventCreateDraggableWindowFromWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UDraggableWindow::execCreateDraggableWindowFromWidget)
{
	P_GET_OBJECT(UObject,Z_Param_WorldContextObject);
	P_GET_OBJECT(UClass,Z_Param_WindowClass);
	P_GET_OBJECT(UUserWidget,Z_Param_NewChildWidget);
	P_GET_STRUCT(FVector2D,Z_Param_StartSize);
	P_GET_STRUCT(FVector2D,Z_Param_ResizeWidth);
	P_GET_STRUCT(FVector2D,Z_Param_ResizeHeight);
	P_GET_UBOOL(Z_Param_bCenterToScreen);
	P_GET_UBOOL(Z_Param_bCanDrag);
	P_GET_UBOOL(Z_Param_bCanResize);
	P_GET_ENUM(EOperatingSystemWindowStartupState,Z_Param_WindowStartupState);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UDraggableWindow**)Z_Param__Result=UDraggableWindow::CreateDraggableWindowFromWidget(Z_Param_WorldContextObject,Z_Param_WindowClass,Z_Param_NewChildWidget,Z_Param_StartSize,Z_Param_ResizeWidth,Z_Param_ResizeHeight,Z_Param_bCenterToScreen,Z_Param_bCanDrag,Z_Param_bCanResize,EOperatingSystemWindowStartupState(Z_Param_WindowStartupState));
	P_NATIVE_END;
}
// ********** End Class UDraggableWindow Function CreateDraggableWindowFromWidget ******************

// ********** Begin Class UDraggableWindow Function GetChildWidget *********************************
struct Z_Construct_UFunction_UDraggableWindow_GetChildWidget_Statics
{
	struct DraggableWindow_eventGetChildWidget_Parms
	{
		UUserWidget* OutChildWidget;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Draggable Window" },
		{ "Comment", "/**\n\x09* protected UDraggableWindow::GetChildWidget const\n\x09* Returns the child widget (if any) of this draggable window. If the Return Value is true, then OutChildWidget will be valid.\n\x09* @param OutChildWidget [UUserWidget*&] Reference to the child widget created by this window.\n\x09* @return [bool] True if a valid child widget is available.\n\x09**/" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "protected UDraggableWindow::GetChildWidget const\nReturns the child widget (if any) of this draggable window. If the Return Value is true, then OutChildWidget will be valid.\n@param OutChildWidget [UUserWidget*&] Reference to the child widget created by this window.\n@return [bool] True if a valid child widget is available." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OutChildWidget_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetChildWidget constinit property declarations ************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_OutChildWidget;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetChildWidget constinit property declarations **************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetChildWidget Property Definitions ***********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UDraggableWindow_GetChildWidget_Statics::NewProp_OutChildWidget = { "OutChildWidget", nullptr, (EPropertyFlags)0x0010000000080180, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventGetChildWidget_Parms, OutChildWidget), Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OutChildWidget_MetaData), NewProp_OutChildWidget_MetaData) };
void Z_Construct_UFunction_UDraggableWindow_GetChildWidget_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((DraggableWindow_eventGetChildWidget_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UDraggableWindow_GetChildWidget_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(DraggableWindow_eventGetChildWidget_Parms), &Z_Construct_UFunction_UDraggableWindow_GetChildWidget_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UDraggableWindow_GetChildWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_GetChildWidget_Statics::NewProp_OutChildWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_GetChildWidget_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_GetChildWidget_Statics::PropPointers) < 2048);
// ********** End Function GetChildWidget Property Definitions *************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_GetChildWidget_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "GetChildWidget", 	Z_Construct_UFunction_UDraggableWindow_GetChildWidget_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_GetChildWidget_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UDraggableWindow_GetChildWidget_Statics::DraggableWindow_eventGetChildWidget_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54480409, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_GetChildWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_GetChildWidget_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UDraggableWindow_GetChildWidget_Statics::DraggableWindow_eventGetChildWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UDraggableWindow_GetChildWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_GetChildWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UDraggableWindow::execGetChildWidget)
{
	P_GET_OBJECT_REF(UUserWidget,Z_Param_Out_OutChildWidget);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->GetChildWidget(P_ARG_GC_BARRIER(Z_Param_Out_OutChildWidget));
	P_NATIVE_END;
}
// ********** End Class UDraggableWindow Function GetChildWidget ***********************************

// ********** Begin Class UDraggableWindow Function Internal_OnMouseButtonDown_ResizeArea **********
struct Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea_Statics
{
	struct DraggableWindow_eventInternal_OnMouseButtonDown_ResizeArea_Parms
	{
		FGeometry InGeometry;
		FPointerEvent InMouseEvent;
		FEventReply ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "DraggableWindow.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InMouseEvent_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function Internal_OnMouseButtonDown_ResizeArea constinit property declarations *
	static const UECodeGen_Private::FStructPropertyParams NewProp_InGeometry;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InMouseEvent;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function Internal_OnMouseButtonDown_ResizeArea constinit property declarations ***
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function Internal_OnMouseButtonDown_ResizeArea Property Definitions ************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea_Statics::NewProp_InGeometry = { "InGeometry", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventInternal_OnMouseButtonDown_ResizeArea_Parms, InGeometry), Z_Construct_UScriptStruct_FGeometry, METADATA_PARAMS(0, nullptr) }; // 3467472672
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea_Statics::NewProp_InMouseEvent = { "InMouseEvent", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventInternal_OnMouseButtonDown_ResizeArea_Parms, InMouseEvent), Z_Construct_UScriptStruct_FPointerEvent, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InMouseEvent_MetaData), NewProp_InMouseEvent_MetaData) }; // 1036461280
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventInternal_OnMouseButtonDown_ResizeArea_Parms, ReturnValue), Z_Construct_UScriptStruct_FEventReply, METADATA_PARAMS(0, nullptr) }; // 3581695936
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea_Statics::NewProp_InGeometry,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea_Statics::NewProp_InMouseEvent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea_Statics::PropPointers) < 2048);
// ********** End Function Internal_OnMouseButtonDown_ResizeArea Property Definitions **************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "Internal_OnMouseButtonDown_ResizeArea", 	Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea_Statics::DraggableWindow_eventInternal_OnMouseButtonDown_ResizeArea_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00440401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea_Statics::DraggableWindow_eventInternal_OnMouseButtonDown_ResizeArea_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UDraggableWindow::execInternal_OnMouseButtonDown_ResizeArea)
{
	P_GET_STRUCT(FGeometry,Z_Param_InGeometry);
	P_GET_STRUCT_REF(FPointerEvent,Z_Param_Out_InMouseEvent);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FEventReply*)Z_Param__Result=P_THIS->Internal_OnMouseButtonDown_ResizeArea(Z_Param_InGeometry,Z_Param_Out_InMouseEvent);
	P_NATIVE_END;
}
// ********** End Class UDraggableWindow Function Internal_OnMouseButtonDown_ResizeArea ************

// ********** Begin Class UDraggableWindow Function Internal_OnMouseButtonDown_WindowTitleBorder ***
struct Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder_Statics
{
	struct DraggableWindow_eventInternal_OnMouseButtonDown_WindowTitleBorder_Parms
	{
		FGeometry InGeometry;
		FPointerEvent InMouseEvent;
		FEventReply ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "DraggableWindow.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InMouseEvent_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function Internal_OnMouseButtonDown_WindowTitleBorder constinit property declarations 
	static const UECodeGen_Private::FStructPropertyParams NewProp_InGeometry;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InMouseEvent;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function Internal_OnMouseButtonDown_WindowTitleBorder constinit property declarations 
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function Internal_OnMouseButtonDown_WindowTitleBorder Property Definitions *****
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder_Statics::NewProp_InGeometry = { "InGeometry", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventInternal_OnMouseButtonDown_WindowTitleBorder_Parms, InGeometry), Z_Construct_UScriptStruct_FGeometry, METADATA_PARAMS(0, nullptr) }; // 3467472672
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder_Statics::NewProp_InMouseEvent = { "InMouseEvent", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventInternal_OnMouseButtonDown_WindowTitleBorder_Parms, InMouseEvent), Z_Construct_UScriptStruct_FPointerEvent, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InMouseEvent_MetaData), NewProp_InMouseEvent_MetaData) }; // 1036461280
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventInternal_OnMouseButtonDown_WindowTitleBorder_Parms, ReturnValue), Z_Construct_UScriptStruct_FEventReply, METADATA_PARAMS(0, nullptr) }; // 3581695936
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder_Statics::NewProp_InGeometry,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder_Statics::NewProp_InMouseEvent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder_Statics::PropPointers) < 2048);
// ********** End Function Internal_OnMouseButtonDown_WindowTitleBorder Property Definitions *******
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "Internal_OnMouseButtonDown_WindowTitleBorder", 	Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder_Statics::DraggableWindow_eventInternal_OnMouseButtonDown_WindowTitleBorder_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00440401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder_Statics::DraggableWindow_eventInternal_OnMouseButtonDown_WindowTitleBorder_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UDraggableWindow::execInternal_OnMouseButtonDown_WindowTitleBorder)
{
	P_GET_STRUCT(FGeometry,Z_Param_InGeometry);
	P_GET_STRUCT_REF(FPointerEvent,Z_Param_Out_InMouseEvent);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FEventReply*)Z_Param__Result=P_THIS->Internal_OnMouseButtonDown_WindowTitleBorder(Z_Param_InGeometry,Z_Param_Out_InMouseEvent);
	P_NATIVE_END;
}
// ********** End Class UDraggableWindow Function Internal_OnMouseButtonDown_WindowTitleBorder *****

// ********** Begin Class UDraggableWindow Function Internal_OnMouseButtonUp_WindowTitleBorder *****
struct Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder_Statics
{
	struct DraggableWindow_eventInternal_OnMouseButtonUp_WindowTitleBorder_Parms
	{
		FGeometry InGeometry;
		FPointerEvent InMouseEvent;
		FEventReply ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "DraggableWindow.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InMouseEvent_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function Internal_OnMouseButtonUp_WindowTitleBorder constinit property declarations 
	static const UECodeGen_Private::FStructPropertyParams NewProp_InGeometry;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InMouseEvent;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function Internal_OnMouseButtonUp_WindowTitleBorder constinit property declarations 
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function Internal_OnMouseButtonUp_WindowTitleBorder Property Definitions *******
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder_Statics::NewProp_InGeometry = { "InGeometry", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventInternal_OnMouseButtonUp_WindowTitleBorder_Parms, InGeometry), Z_Construct_UScriptStruct_FGeometry, METADATA_PARAMS(0, nullptr) }; // 3467472672
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder_Statics::NewProp_InMouseEvent = { "InMouseEvent", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventInternal_OnMouseButtonUp_WindowTitleBorder_Parms, InMouseEvent), Z_Construct_UScriptStruct_FPointerEvent, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InMouseEvent_MetaData), NewProp_InMouseEvent_MetaData) }; // 1036461280
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventInternal_OnMouseButtonUp_WindowTitleBorder_Parms, ReturnValue), Z_Construct_UScriptStruct_FEventReply, METADATA_PARAMS(0, nullptr) }; // 3581695936
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder_Statics::NewProp_InGeometry,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder_Statics::NewProp_InMouseEvent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder_Statics::PropPointers) < 2048);
// ********** End Function Internal_OnMouseButtonUp_WindowTitleBorder Property Definitions *********
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "Internal_OnMouseButtonUp_WindowTitleBorder", 	Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder_Statics::DraggableWindow_eventInternal_OnMouseButtonUp_WindowTitleBorder_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00440401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder_Statics::DraggableWindow_eventInternal_OnMouseButtonUp_WindowTitleBorder_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UDraggableWindow::execInternal_OnMouseButtonUp_WindowTitleBorder)
{
	P_GET_STRUCT(FGeometry,Z_Param_InGeometry);
	P_GET_STRUCT_REF(FPointerEvent,Z_Param_Out_InMouseEvent);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FEventReply*)Z_Param__Result=P_THIS->Internal_OnMouseButtonUp_WindowTitleBorder(Z_Param_InGeometry,Z_Param_Out_InMouseEvent);
	P_NATIVE_END;
}
// ********** End Class UDraggableWindow Function Internal_OnMouseButtonUp_WindowTitleBorder *******

// ********** Begin Class UDraggableWindow Function K2_OnCloseWindow *******************************
static FName NAME_UDraggableWindow_K2_OnCloseWindow = FName(TEXT("K2_OnCloseWindow"));
void UDraggableWindow::K2_OnCloseWindow()
{
	UFunction* Func = FindFunctionChecked(NAME_UDraggableWindow_K2_OnCloseWindow);
	ProcessEvent(Func,NULL);
}
struct Z_Construct_UFunction_UDraggableWindow_K2_OnCloseWindow_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Draggable Window" },
		{ "Comment", "/**\n\x09* protected UDraggableWindow::K2_OnCloseWindow\n\x09* Event called when this window is closed. C++ implementation calls RemoveFromParent automatically.\n\x09**/" },
		{ "DisplayName", "On Close Window" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "protected UDraggableWindow::K2_OnCloseWindow\nEvent called when this window is closed. C++ implementation calls RemoveFromParent automatically." },
	};
#endif // WITH_METADATA

// ********** Begin Function K2_OnCloseWindow constinit property declarations **********************
// ********** End Function K2_OnCloseWindow constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_K2_OnCloseWindow_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "K2_OnCloseWindow", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08080808, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_K2_OnCloseWindow_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_K2_OnCloseWindow_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UDraggableWindow_K2_OnCloseWindow()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_K2_OnCloseWindow_Statics::FuncParams);
	}
	return ReturnFunction;
}
// ********** End Class UDraggableWindow Function K2_OnCloseWindow *********************************

// ********** Begin Class UDraggableWindow Function K2_OnContentWidgetAdded ************************
struct DraggableWindow_eventK2_OnContentWidgetAdded_Parms
{
	const UUserWidget* AddedUserWidget;
};
static FName NAME_UDraggableWindow_K2_OnContentWidgetAdded = FName(TEXT("K2_OnContentWidgetAdded"));
void UDraggableWindow::K2_OnContentWidgetAdded(const UUserWidget* AddedUserWidget)
{
	DraggableWindow_eventK2_OnContentWidgetAdded_Parms Parms;
	Parms.AddedUserWidget=AddedUserWidget;
	UFunction* Func = FindFunctionChecked(NAME_UDraggableWindow_K2_OnContentWidgetAdded);
	ProcessEvent(Func,&Parms);
}
struct Z_Construct_UFunction_UDraggableWindow_K2_OnContentWidgetAdded_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Draggable Window" },
		{ "Comment", "/**\n\x09* protected UDraggableWindow::K2_OnContentWidgetAdded\n\x09* Event called when a new content is added to this window.\n\x09* @param AddedUserWidget [const UUserWidget*] Reference to the content that is being added.\n\x09**/" },
		{ "DisplayName", "On Content Widget Added" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "protected UDraggableWindow::K2_OnContentWidgetAdded\nEvent called when a new content is added to this window.\n@param AddedUserWidget [const UUserWidget*] Reference to the content that is being added." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AddedUserWidget_MetaData[] = {
		{ "EditInline", "true" },
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function K2_OnContentWidgetAdded constinit property declarations ***************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_AddedUserWidget;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function K2_OnContentWidgetAdded constinit property declarations *****************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function K2_OnContentWidgetAdded Property Definitions **************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UDraggableWindow_K2_OnContentWidgetAdded_Statics::NewProp_AddedUserWidget = { "AddedUserWidget", nullptr, (EPropertyFlags)0x0010000000080082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventK2_OnContentWidgetAdded_Parms, AddedUserWidget), Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AddedUserWidget_MetaData), NewProp_AddedUserWidget_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UDraggableWindow_K2_OnContentWidgetAdded_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_K2_OnContentWidgetAdded_Statics::NewProp_AddedUserWidget,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_K2_OnContentWidgetAdded_Statics::PropPointers) < 2048);
// ********** End Function K2_OnContentWidgetAdded Property Definitions ****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_K2_OnContentWidgetAdded_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "K2_OnContentWidgetAdded", 	Z_Construct_UFunction_UDraggableWindow_K2_OnContentWidgetAdded_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_K2_OnContentWidgetAdded_Statics::PropPointers), 
sizeof(DraggableWindow_eventK2_OnContentWidgetAdded_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08080808, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_K2_OnContentWidgetAdded_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_K2_OnContentWidgetAdded_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(DraggableWindow_eventK2_OnContentWidgetAdded_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UDraggableWindow_K2_OnContentWidgetAdded()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_K2_OnContentWidgetAdded_Statics::FuncParams);
	}
	return ReturnFunction;
}
// ********** End Class UDraggableWindow Function K2_OnContentWidgetAdded **************************

// ********** Begin Class UDraggableWindow Function K2_OnDragStart *********************************
struct DraggableWindow_eventK2_OnDragStart_Parms
{
	FPointerEvent InMouseEvent;
};
static FName NAME_UDraggableWindow_K2_OnDragStart = FName(TEXT("K2_OnDragStart"));
void UDraggableWindow::K2_OnDragStart(FPointerEvent const& InMouseEvent)
{
	DraggableWindow_eventK2_OnDragStart_Parms Parms;
	Parms.InMouseEvent=InMouseEvent;
	UFunction* Func = FindFunctionChecked(NAME_UDraggableWindow_K2_OnDragStart);
	ProcessEvent(Func,&Parms);
}
struct Z_Construct_UFunction_UDraggableWindow_K2_OnDragStart_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Draggable Window" },
		{ "Comment", "/**\n\x09* protected UDraggableWindow::K2_OnDragStart\n\x09* Event called when resizing is started. Only called if Enable Drag is true.\n\x09* @param InMouseEvent [const FPointerEvent&] Mouse or touch action.\n\x09**/" },
		{ "DisplayName", "On Drag Start" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "protected UDraggableWindow::K2_OnDragStart\nEvent called when resizing is started. Only called if Enable Drag is true.\n@param InMouseEvent [const FPointerEvent&] Mouse or touch action." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InMouseEvent_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function K2_OnDragStart constinit property declarations ************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_InMouseEvent;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function K2_OnDragStart constinit property declarations **************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function K2_OnDragStart Property Definitions ***********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_K2_OnDragStart_Statics::NewProp_InMouseEvent = { "InMouseEvent", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventK2_OnDragStart_Parms, InMouseEvent), Z_Construct_UScriptStruct_FPointerEvent, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InMouseEvent_MetaData), NewProp_InMouseEvent_MetaData) }; // 1036461280
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UDraggableWindow_K2_OnDragStart_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_K2_OnDragStart_Statics::NewProp_InMouseEvent,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_K2_OnDragStart_Statics::PropPointers) < 2048);
// ********** End Function K2_OnDragStart Property Definitions *************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_K2_OnDragStart_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "K2_OnDragStart", 	Z_Construct_UFunction_UDraggableWindow_K2_OnDragStart_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_K2_OnDragStart_Statics::PropPointers), 
sizeof(DraggableWindow_eventK2_OnDragStart_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08480808, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_K2_OnDragStart_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_K2_OnDragStart_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(DraggableWindow_eventK2_OnDragStart_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UDraggableWindow_K2_OnDragStart()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_K2_OnDragStart_Statics::FuncParams);
	}
	return ReturnFunction;
}
// ********** End Class UDraggableWindow Function K2_OnDragStart ***********************************

// ********** Begin Class UDraggableWindow Function K2_OnResizeStart *******************************
struct DraggableWindow_eventK2_OnResizeStart_Parms
{
	FPointerEvent InMouseEvent;
};
static FName NAME_UDraggableWindow_K2_OnResizeStart = FName(TEXT("K2_OnResizeStart"));
void UDraggableWindow::K2_OnResizeStart(FPointerEvent const& InMouseEvent)
{
	DraggableWindow_eventK2_OnResizeStart_Parms Parms;
	Parms.InMouseEvent=InMouseEvent;
	UFunction* Func = FindFunctionChecked(NAME_UDraggableWindow_K2_OnResizeStart);
	ProcessEvent(Func,&Parms);
}
struct Z_Construct_UFunction_UDraggableWindow_K2_OnResizeStart_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Draggable Window" },
		{ "Comment", "/**\n\x09* protected UDraggableWindow::K2_OnResizeStart\n\x09* Event called when resizing is started. Only called if Enable Resizing is true.\n\x09* @param InMouseEvent [const FPointerEvent&] Mouse or touch action.\n\x09**/" },
		{ "DisplayName", "On Resize Start" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "protected UDraggableWindow::K2_OnResizeStart\nEvent called when resizing is started. Only called if Enable Resizing is true.\n@param InMouseEvent [const FPointerEvent&] Mouse or touch action." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InMouseEvent_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function K2_OnResizeStart constinit property declarations **********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_InMouseEvent;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function K2_OnResizeStart constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function K2_OnResizeStart Property Definitions *********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_K2_OnResizeStart_Statics::NewProp_InMouseEvent = { "InMouseEvent", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventK2_OnResizeStart_Parms, InMouseEvent), Z_Construct_UScriptStruct_FPointerEvent, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InMouseEvent_MetaData), NewProp_InMouseEvent_MetaData) }; // 1036461280
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UDraggableWindow_K2_OnResizeStart_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_K2_OnResizeStart_Statics::NewProp_InMouseEvent,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_K2_OnResizeStart_Statics::PropPointers) < 2048);
// ********** End Function K2_OnResizeStart Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_K2_OnResizeStart_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "K2_OnResizeStart", 	Z_Construct_UFunction_UDraggableWindow_K2_OnResizeStart_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_K2_OnResizeStart_Statics::PropPointers), 
sizeof(DraggableWindow_eventK2_OnResizeStart_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08480808, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_K2_OnResizeStart_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_K2_OnResizeStart_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(DraggableWindow_eventK2_OnResizeStart_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UDraggableWindow_K2_OnResizeStart()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_K2_OnResizeStart_Statics::FuncParams);
	}
	return ReturnFunction;
}
// ********** End Class UDraggableWindow Function K2_OnResizeStart *********************************

// ********** Begin Class UDraggableWindow Function K2_OnSetWindowTitle ****************************
struct DraggableWindow_eventK2_OnSetWindowTitle_Parms
{
	FText NewWindowTitle;
};
static FName NAME_UDraggableWindow_K2_OnSetWindowTitle = FName(TEXT("K2_OnSetWindowTitle"));
void UDraggableWindow::K2_OnSetWindowTitle(FText const& NewWindowTitle)
{
	DraggableWindow_eventK2_OnSetWindowTitle_Parms Parms;
	Parms.NewWindowTitle=NewWindowTitle;
	UFunction* Func = FindFunctionChecked(NAME_UDraggableWindow_K2_OnSetWindowTitle);
	ProcessEvent(Func,&Parms);
}
struct Z_Construct_UFunction_UDraggableWindow_K2_OnSetWindowTitle_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Draggable Window" },
		{ "Comment", "/**\n\x09* protected UDraggableWindow::K2_OnSetWindowTitle\n\x09* Event called when a new title is set for this window.\n\x09* @param NewWindowTitle [const FText &] New title.\n\x09**/" },
		{ "DisplayName", "On Set Window Title" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "protected UDraggableWindow::K2_OnSetWindowTitle\nEvent called when a new title is set for this window.\n@param NewWindowTitle [const FText &] New title." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NewWindowTitle_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function K2_OnSetWindowTitle constinit property declarations *******************
	static const UECodeGen_Private::FTextPropertyParams NewProp_NewWindowTitle;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function K2_OnSetWindowTitle constinit property declarations *********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function K2_OnSetWindowTitle Property Definitions ******************************
const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UDraggableWindow_K2_OnSetWindowTitle_Statics::NewProp_NewWindowTitle = { "NewWindowTitle", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventK2_OnSetWindowTitle_Parms, NewWindowTitle), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NewWindowTitle_MetaData), NewProp_NewWindowTitle_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UDraggableWindow_K2_OnSetWindowTitle_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_K2_OnSetWindowTitle_Statics::NewProp_NewWindowTitle,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_K2_OnSetWindowTitle_Statics::PropPointers) < 2048);
// ********** End Function K2_OnSetWindowTitle Property Definitions ********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_K2_OnSetWindowTitle_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "K2_OnSetWindowTitle", 	Z_Construct_UFunction_UDraggableWindow_K2_OnSetWindowTitle_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_K2_OnSetWindowTitle_Statics::PropPointers), 
sizeof(DraggableWindow_eventK2_OnSetWindowTitle_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x0C480808, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_K2_OnSetWindowTitle_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_K2_OnSetWindowTitle_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(DraggableWindow_eventK2_OnSetWindowTitle_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UDraggableWindow_K2_OnSetWindowTitle()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_K2_OnSetWindowTitle_Statics::FuncParams);
	}
	return ReturnFunction;
}
// ********** End Class UDraggableWindow Function K2_OnSetWindowTitle ******************************

// ********** Begin Class UDraggableWindow Function K2_OnWindowMaximizeToggled *********************
static FName NAME_UDraggableWindow_K2_OnWindowMaximizeToggled = FName(TEXT("K2_OnWindowMaximizeToggled"));
void UDraggableWindow::K2_OnWindowMaximizeToggled()
{
	UFunction* Func = FindFunctionChecked(NAME_UDraggableWindow_K2_OnWindowMaximizeToggled);
	ProcessEvent(Func,NULL);
}
struct Z_Construct_UFunction_UDraggableWindow_K2_OnWindowMaximizeToggled_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Draggable Window" },
		{ "Comment", "/**\n\x09 * protected UDraggableWindow::K2_OnWindowMaximizeToggled\n\x09 * Event called when this window is maximized or restored.\n\x09 */" },
		{ "DisplayName", "On Window Maximize Toggled" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "protected UDraggableWindow::K2_OnWindowMaximizeToggled\nEvent called when this window is maximized or restored." },
	};
#endif // WITH_METADATA

// ********** Begin Function K2_OnWindowMaximizeToggled constinit property declarations ************
// ********** End Function K2_OnWindowMaximizeToggled constinit property declarations **************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_K2_OnWindowMaximizeToggled_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "K2_OnWindowMaximizeToggled", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08080808, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_K2_OnWindowMaximizeToggled_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_K2_OnWindowMaximizeToggled_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UDraggableWindow_K2_OnWindowMaximizeToggled()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_K2_OnWindowMaximizeToggled_Statics::FuncParams);
	}
	return ReturnFunction;
}
// ********** End Class UDraggableWindow Function K2_OnWindowMaximizeToggled ***********************

// ********** Begin Class UDraggableWindow Function K2_StartMinimized ******************************
static FName NAME_UDraggableWindow_K2_StartMinimized = FName(TEXT("K2_StartMinimized"));
void UDraggableWindow::K2_StartMinimized()
{
	UFunction* Func = FindFunctionChecked(NAME_UDraggableWindow_K2_StartMinimized);
	ProcessEvent(Func,NULL);
}
struct Z_Construct_UFunction_UDraggableWindow_K2_StartMinimized_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Draggable Window" },
		{ "Comment", "/**\n\x09 * protected UDraggableWindow::K2_StartMinimized\n\x09 * Event called when the window starts minimized.\n\x09 */" },
		{ "DisplayName", "On Start Minimized" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "protected UDraggableWindow::K2_StartMinimized\nEvent called when the window starts minimized." },
	};
#endif // WITH_METADATA

// ********** Begin Function K2_StartMinimized constinit property declarations *********************
// ********** End Function K2_StartMinimized constinit property declarations ***********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_K2_StartMinimized_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "K2_StartMinimized", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08080808, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_K2_StartMinimized_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_K2_StartMinimized_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UDraggableWindow_K2_StartMinimized()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_K2_StartMinimized_Statics::FuncParams);
	}
	return ReturnFunction;
}
// ********** End Class UDraggableWindow Function K2_StartMinimized ********************************

// ********** Begin Class UDraggableWindow Function ToggleMaximize *********************************
struct Z_Construct_UFunction_UDraggableWindow_ToggleMaximize_Statics
{
	struct DraggableWindow_eventToggleMaximize_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Draggable Window" },
		{ "Comment", "/**\n\x09 * protected UDraggableWindow::ToggleMaximize\n\x09 * Maximizes or restores this window.\n\x09 * @return [bool] True if the window was maximized or restored.\n\x09 */" },
		{ "Keywords", "fullscreen, restore" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "protected UDraggableWindow::ToggleMaximize\nMaximizes or restores this window.\n@return [bool] True if the window was maximized or restored." },
	};
#endif // WITH_METADATA

// ********** Begin Function ToggleMaximize constinit property declarations ************************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ToggleMaximize constinit property declarations **************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ToggleMaximize Property Definitions ***********************************
void Z_Construct_UFunction_UDraggableWindow_ToggleMaximize_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((DraggableWindow_eventToggleMaximize_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UDraggableWindow_ToggleMaximize_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(DraggableWindow_eventToggleMaximize_Parms), &Z_Construct_UFunction_UDraggableWindow_ToggleMaximize_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UDraggableWindow_ToggleMaximize_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_ToggleMaximize_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_ToggleMaximize_Statics::PropPointers) < 2048);
// ********** End Function ToggleMaximize Property Definitions *************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_ToggleMaximize_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "ToggleMaximize", 	Z_Construct_UFunction_UDraggableWindow_ToggleMaximize_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_ToggleMaximize_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UDraggableWindow_ToggleMaximize_Statics::DraggableWindow_eventToggleMaximize_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04080409, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_ToggleMaximize_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_ToggleMaximize_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UDraggableWindow_ToggleMaximize_Statics::DraggableWindow_eventToggleMaximize_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UDraggableWindow_ToggleMaximize()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_ToggleMaximize_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UDraggableWindow::execToggleMaximize)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->ToggleMaximize();
	P_NATIVE_END;
}
// ********** End Class UDraggableWindow Function ToggleMaximize ***********************************

// ********** Begin Class UDraggableWindow Function UpdateWindowSize *******************************
struct Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize_Statics
{
	struct DraggableWindow_eventUpdateWindowSize_Parms
	{
		FVector2D InNewSize;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Draggable Window" },
		{ "Comment", "/**\n\x09* protected UDraggableWindow::UpdateWindowSize\n\x09* Updates the size of the draggable window.\n\x09* @param InNewSize [const FVector2D&] New size to change to.\n\x09* @return [bool] True if the size was changed successfully.\n\x09**/" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "protected UDraggableWindow::UpdateWindowSize\nUpdates the size of the draggable window.\n@param InNewSize [const FVector2D&] New size to change to.\n@return [bool] True if the size was changed successfully." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InNewSize_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function UpdateWindowSize constinit property declarations **********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_InNewSize;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function UpdateWindowSize constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function UpdateWindowSize Property Definitions *********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize_Statics::NewProp_InNewSize = { "InNewSize", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DraggableWindow_eventUpdateWindowSize_Parms, InNewSize), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InNewSize_MetaData), NewProp_InNewSize_MetaData) };
void Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((DraggableWindow_eventUpdateWindowSize_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(DraggableWindow_eventUpdateWindowSize_Parms), &Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize_Statics::NewProp_InNewSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize_Statics::PropPointers) < 2048);
// ********** End Function UpdateWindowSize Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UDraggableWindow, nullptr, "UpdateWindowSize", 	Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize_Statics::DraggableWindow_eventUpdateWindowSize_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C80409, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize_Statics::DraggableWindow_eventUpdateWindowSize_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UDraggableWindow::execUpdateWindowSize)
{
	P_GET_STRUCT_REF(FVector2D,Z_Param_Out_InNewSize);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->UpdateWindowSize(Z_Param_Out_InNewSize);
	P_NATIVE_END;
}
// ********** End Class UDraggableWindow Function UpdateWindowSize *********************************

// ********** Begin Class UDraggableWindow *********************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UDraggableWindow;
UClass* UDraggableWindow::GetPrivateStaticClass()
{
	using TClass = UDraggableWindow;
	if (!Z_Registration_Info_UClass_UDraggableWindow.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("DraggableWindow"),
			Z_Registration_Info_UClass_UDraggableWindow.InnerSingleton,
			StaticRegisterNativesUDraggableWindow,
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
	return Z_Registration_Info_UClass_UDraggableWindow.InnerSingleton;
}
UClass* Z_Construct_UClass_UDraggableWindow_NoRegister()
{
	return UDraggableWindow::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UDraggableWindow_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "DraggableWindow.h" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnableDrag_MetaData[] = {
		{ "Category", "Draggable Window Widget" },
		{ "Comment", "/* Allows you to drag this window. */" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "Allows you to drag this window." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnableResizing_MetaData[] = {
		{ "Category", "Draggable Window Widget" },
		{ "Comment", "/* Allows you to resize this window. */" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "Allows you to resize this window." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnableMaximizing_MetaData[] = {
		{ "Category", "Draggable Window Widget" },
		{ "Comment", "/* Allows the window to maximize. Resizing must also be enabled. */" },
		{ "EditCondition", "bEnableResizing" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "Allows the window to maximize. Resizing must also be enabled." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ResizeMinWidth_MetaData[] = {
		{ "Category", "Draggable Window Widget" },
		{ "Comment", "/* Minimum width (in pixels) you can resize to. */" },
		{ "EditCondition", "bEnableResizing" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "Minimum width (in pixels) you can resize to." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ResizeMaxWidth_MetaData[] = {
		{ "Category", "Draggable Window Widget" },
		{ "Comment", "/* Maximum width (in pixels) you can resize to. */" },
		{ "EditCondition", "bEnableResizing" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "Maximum width (in pixels) you can resize to." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ResizeMinHeight_MetaData[] = {
		{ "Category", "Draggable Window Widget" },
		{ "Comment", "/* Minimum height (in pixels) you can resize to. */" },
		{ "EditCondition", "bEnableResizing" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "Minimum height (in pixels) you can resize to." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ResizeMaxHeight_MetaData[] = {
		{ "Category", "Draggable Window Widget" },
		{ "Comment", "/* Maximum height (in pixels) you can resize to. */" },
		{ "EditCondition", "bEnableResizing" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "Maximum height (in pixels) you can resize to." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DragKey_MetaData[] = {
		{ "Category", "Draggable Window Widget" },
		{ "Comment", "/* Key used to drag or resize window. Defaults to Left Mouse Button (LMB) because it makes sense. */" },
		{ "EditCondition", "bEnableDrag" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
		{ "ToolTip", "Key used to drag or resize window. Defaults to Left Mouse Button (LMB) because it makes sense." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WindowTitleBorderWidget_MetaData[] = {
		{ "BindWidgetOptional", "" },
		{ "Category", "Draggable Window Widget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ChildWidgetCanvas_MetaData[] = {
		{ "BindWidgetOptional", "" },
		{ "Category", "Draggable Window Widget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ResizeAreaWidget_MetaData[] = {
		{ "BindWidgetOptional", "" },
		{ "Category", "Draggable Window Widget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ParentSlot_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ChildWidget_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bIsMaximized_MetaData[] = {
		{ "Category", "Draggable Window Widget" },
		{ "ModuleRelativePath", "DraggableWindow.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UDraggableWindow constinit property declarations *************************
	static void NewProp_bEnableDrag_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableDrag;
	static void NewProp_bEnableResizing_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableResizing;
	static void NewProp_bEnableMaximizing_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableMaximizing;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ResizeMinWidth;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ResizeMaxWidth;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ResizeMinHeight;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ResizeMaxHeight;
	static const UECodeGen_Private::FStructPropertyParams NewProp_DragKey;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_WindowTitleBorderWidget;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ChildWidgetCanvas;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ResizeAreaWidget;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ParentSlot;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ChildWidget;
	static void NewProp_bIsMaximized_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bIsMaximized;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UDraggableWindow constinit property declarations ***************************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("AddContentWidget"), .Pointer = &UDraggableWindow::execAddContentWidget },
		{ .NameUTF8 = UTF8TEXT("AddContentWidgetClass"), .Pointer = &UDraggableWindow::execAddContentWidgetClass },
		{ .NameUTF8 = UTF8TEXT("CenterWindowToScreen"), .Pointer = &UDraggableWindow::execCenterWindowToScreen },
		{ .NameUTF8 = UTF8TEXT("CloseWindow"), .Pointer = &UDraggableWindow::execCloseWindow },
		{ .NameUTF8 = UTF8TEXT("CreateDraggableWindow"), .Pointer = &UDraggableWindow::execCreateDraggableWindow },
		{ .NameUTF8 = UTF8TEXT("CreateDraggableWindowFromWidget"), .Pointer = &UDraggableWindow::execCreateDraggableWindowFromWidget },
		{ .NameUTF8 = UTF8TEXT("GetChildWidget"), .Pointer = &UDraggableWindow::execGetChildWidget },
		{ .NameUTF8 = UTF8TEXT("Internal_OnMouseButtonDown_ResizeArea"), .Pointer = &UDraggableWindow::execInternal_OnMouseButtonDown_ResizeArea },
		{ .NameUTF8 = UTF8TEXT("Internal_OnMouseButtonDown_WindowTitleBorder"), .Pointer = &UDraggableWindow::execInternal_OnMouseButtonDown_WindowTitleBorder },
		{ .NameUTF8 = UTF8TEXT("Internal_OnMouseButtonUp_WindowTitleBorder"), .Pointer = &UDraggableWindow::execInternal_OnMouseButtonUp_WindowTitleBorder },
		{ .NameUTF8 = UTF8TEXT("ToggleMaximize"), .Pointer = &UDraggableWindow::execToggleMaximize },
		{ .NameUTF8 = UTF8TEXT("UpdateWindowSize"), .Pointer = &UDraggableWindow::execUpdateWindowSize },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UDraggableWindow_AddContentWidget, "AddContentWidget" }, // 1737976977
		{ &Z_Construct_UFunction_UDraggableWindow_AddContentWidgetClass, "AddContentWidgetClass" }, // 2643128532
		{ &Z_Construct_UFunction_UDraggableWindow_CenterWindowToScreen, "CenterWindowToScreen" }, // 826182155
		{ &Z_Construct_UFunction_UDraggableWindow_CloseWindow, "CloseWindow" }, // 280150860
		{ &Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindow, "CreateDraggableWindow" }, // 3199460181
		{ &Z_Construct_UFunction_UDraggableWindow_CreateDraggableWindowFromWidget, "CreateDraggableWindowFromWidget" }, // 4164820993
		{ &Z_Construct_UFunction_UDraggableWindow_GetChildWidget, "GetChildWidget" }, // 2333667753
		{ &Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_ResizeArea, "Internal_OnMouseButtonDown_ResizeArea" }, // 3278785815
		{ &Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonDown_WindowTitleBorder, "Internal_OnMouseButtonDown_WindowTitleBorder" }, // 2349358855
		{ &Z_Construct_UFunction_UDraggableWindow_Internal_OnMouseButtonUp_WindowTitleBorder, "Internal_OnMouseButtonUp_WindowTitleBorder" }, // 2480598984
		{ &Z_Construct_UFunction_UDraggableWindow_K2_OnCloseWindow, "K2_OnCloseWindow" }, // 4271418684
		{ &Z_Construct_UFunction_UDraggableWindow_K2_OnContentWidgetAdded, "K2_OnContentWidgetAdded" }, // 3112206618
		{ &Z_Construct_UFunction_UDraggableWindow_K2_OnDragStart, "K2_OnDragStart" }, // 3942391288
		{ &Z_Construct_UFunction_UDraggableWindow_K2_OnResizeStart, "K2_OnResizeStart" }, // 3499643898
		{ &Z_Construct_UFunction_UDraggableWindow_K2_OnSetWindowTitle, "K2_OnSetWindowTitle" }, // 2852365114
		{ &Z_Construct_UFunction_UDraggableWindow_K2_OnWindowMaximizeToggled, "K2_OnWindowMaximizeToggled" }, // 2887675834
		{ &Z_Construct_UFunction_UDraggableWindow_K2_StartMinimized, "K2_StartMinimized" }, // 241948221
		{ &Z_Construct_UFunction_UDraggableWindow_ToggleMaximize, "ToggleMaximize" }, // 4281474538
		{ &Z_Construct_UFunction_UDraggableWindow_UpdateWindowSize, "UpdateWindowSize" }, // 1368806056
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UDraggableWindow>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UDraggableWindow_Statics

// ********** Begin Class UDraggableWindow Property Definitions ************************************
void Z_Construct_UClass_UDraggableWindow_Statics::NewProp_bEnableDrag_SetBit(void* Obj)
{
	((UDraggableWindow*)Obj)->bEnableDrag = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDraggableWindow_Statics::NewProp_bEnableDrag = { "bEnableDrag", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Bool , RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(uint8), sizeof(UDraggableWindow), &Z_Construct_UClass_UDraggableWindow_Statics::NewProp_bEnableDrag_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnableDrag_MetaData), NewProp_bEnableDrag_MetaData) };
void Z_Construct_UClass_UDraggableWindow_Statics::NewProp_bEnableResizing_SetBit(void* Obj)
{
	((UDraggableWindow*)Obj)->bEnableResizing = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDraggableWindow_Statics::NewProp_bEnableResizing = { "bEnableResizing", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Bool , RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(uint8), sizeof(UDraggableWindow), &Z_Construct_UClass_UDraggableWindow_Statics::NewProp_bEnableResizing_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnableResizing_MetaData), NewProp_bEnableResizing_MetaData) };
void Z_Construct_UClass_UDraggableWindow_Statics::NewProp_bEnableMaximizing_SetBit(void* Obj)
{
	((UDraggableWindow*)Obj)->bEnableMaximizing = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDraggableWindow_Statics::NewProp_bEnableMaximizing = { "bEnableMaximizing", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Bool , RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(uint8), sizeof(UDraggableWindow), &Z_Construct_UClass_UDraggableWindow_Statics::NewProp_bEnableMaximizing_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnableMaximizing_MetaData), NewProp_bEnableMaximizing_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UDraggableWindow_Statics::NewProp_ResizeMinWidth = { "ResizeMinWidth", nullptr, (EPropertyFlags)0x0020080000000015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDraggableWindow, ResizeMinWidth), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ResizeMinWidth_MetaData), NewProp_ResizeMinWidth_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UDraggableWindow_Statics::NewProp_ResizeMaxWidth = { "ResizeMaxWidth", nullptr, (EPropertyFlags)0x0020080000000015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDraggableWindow, ResizeMaxWidth), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ResizeMaxWidth_MetaData), NewProp_ResizeMaxWidth_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UDraggableWindow_Statics::NewProp_ResizeMinHeight = { "ResizeMinHeight", nullptr, (EPropertyFlags)0x0020080000000015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDraggableWindow, ResizeMinHeight), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ResizeMinHeight_MetaData), NewProp_ResizeMinHeight_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UDraggableWindow_Statics::NewProp_ResizeMaxHeight = { "ResizeMaxHeight", nullptr, (EPropertyFlags)0x0020080000000015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDraggableWindow, ResizeMaxHeight), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ResizeMaxHeight_MetaData), NewProp_ResizeMaxHeight_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UDraggableWindow_Statics::NewProp_DragKey = { "DragKey", nullptr, (EPropertyFlags)0x00200c0000000015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDraggableWindow, DragKey), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DragKey_MetaData), NewProp_DragKey_MetaData) }; // 2693575693
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UDraggableWindow_Statics::NewProp_WindowTitleBorderWidget = { "WindowTitleBorderWidget", nullptr, (EPropertyFlags)0x002008000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDraggableWindow, WindowTitleBorderWidget), Z_Construct_UClass_UBorder_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WindowTitleBorderWidget_MetaData), NewProp_WindowTitleBorderWidget_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UDraggableWindow_Statics::NewProp_ChildWidgetCanvas = { "ChildWidgetCanvas", nullptr, (EPropertyFlags)0x002008000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDraggableWindow, ChildWidgetCanvas), Z_Construct_UClass_UCanvasPanel_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ChildWidgetCanvas_MetaData), NewProp_ChildWidgetCanvas_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UDraggableWindow_Statics::NewProp_ResizeAreaWidget = { "ResizeAreaWidget", nullptr, (EPropertyFlags)0x002008000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDraggableWindow, ResizeAreaWidget), Z_Construct_UClass_UBorder_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ResizeAreaWidget_MetaData), NewProp_ResizeAreaWidget_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UDraggableWindow_Statics::NewProp_ParentSlot = { "ParentSlot", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDraggableWindow, ParentSlot), Z_Construct_UClass_UCanvasPanelSlot_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ParentSlot_MetaData), NewProp_ParentSlot_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UDraggableWindow_Statics::NewProp_ChildWidget = { "ChildWidget", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDraggableWindow, ChildWidget), Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ChildWidget_MetaData), NewProp_ChildWidget_MetaData) };
void Z_Construct_UClass_UDraggableWindow_Statics::NewProp_bIsMaximized_SetBit(void* Obj)
{
	((UDraggableWindow*)Obj)->bIsMaximized = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDraggableWindow_Statics::NewProp_bIsMaximized = { "bIsMaximized", nullptr, (EPropertyFlags)0x0020080000002014, UECodeGen_Private::EPropertyGenFlags::Bool , RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(uint8), sizeof(UDraggableWindow), &Z_Construct_UClass_UDraggableWindow_Statics::NewProp_bIsMaximized_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bIsMaximized_MetaData), NewProp_bIsMaximized_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UDraggableWindow_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDraggableWindow_Statics::NewProp_bEnableDrag,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDraggableWindow_Statics::NewProp_bEnableResizing,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDraggableWindow_Statics::NewProp_bEnableMaximizing,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDraggableWindow_Statics::NewProp_ResizeMinWidth,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDraggableWindow_Statics::NewProp_ResizeMaxWidth,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDraggableWindow_Statics::NewProp_ResizeMinHeight,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDraggableWindow_Statics::NewProp_ResizeMaxHeight,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDraggableWindow_Statics::NewProp_DragKey,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDraggableWindow_Statics::NewProp_WindowTitleBorderWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDraggableWindow_Statics::NewProp_ChildWidgetCanvas,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDraggableWindow_Statics::NewProp_ResizeAreaWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDraggableWindow_Statics::NewProp_ParentSlot,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDraggableWindow_Statics::NewProp_ChildWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDraggableWindow_Statics::NewProp_bIsMaximized,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UDraggableWindow_Statics::PropPointers) < 2048);
// ********** End Class UDraggableWindow Property Definitions **************************************
UObject* (*const Z_Construct_UClass_UDraggableWindow_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_DraggableWindowUMG,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UDraggableWindow_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UDraggableWindow_Statics::ClassParams = {
	&UDraggableWindow::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UDraggableWindow_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UDraggableWindow_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UDraggableWindow_Statics::Class_MetaDataParams), Z_Construct_UClass_UDraggableWindow_Statics::Class_MetaDataParams)
};
void UDraggableWindow::StaticRegisterNativesUDraggableWindow()
{
	UClass* Class = UDraggableWindow::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UDraggableWindow_Statics::Funcs));
}
UClass* Z_Construct_UClass_UDraggableWindow()
{
	if (!Z_Registration_Info_UClass_UDraggableWindow.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UDraggableWindow.OuterSingleton, Z_Construct_UClass_UDraggableWindow_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UDraggableWindow.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UDraggableWindow);
UDraggableWindow::~UDraggableWindow() {}
// ********** End Class UDraggableWindow ***********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Build_U5M_Mac_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h__Script_DraggableWindowUMG_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EOperatingSystemWindowStartupState_StaticEnum, TEXT("EOperatingSystemWindowStartupState"), &Z_Registration_Info_UEnum_EOperatingSystemWindowStartupState, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2372957549U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UDraggableWindow, UDraggableWindow::StaticClass, TEXT("UDraggableWindow"), &Z_Registration_Info_UClass_UDraggableWindow, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UDraggableWindow), 1309817298U) },
	};
}; // Z_CompiledInDeferFile_FID_Build_U5M_Mac_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h__Script_DraggableWindowUMG_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Build_U5M_Mac_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h__Script_DraggableWindowUMG_2815366854{
	TEXT("/Script/DraggableWindowUMG"),
	Z_CompiledInDeferFile_FID_Build_U5M_Mac_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h__Script_DraggableWindowUMG_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Build_U5M_Mac_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h__Script_DraggableWindowUMG_Statics::ClassInfo),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Build_U5M_Mac_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h__Script_DraggableWindowUMG_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Build_U5M_Mac_Sync_LocalBuilds_PluginTemp_HostProject_Plugins_DraggableWindowUMG_Source_DraggableWindowUMG_DraggableWindow_h__Script_DraggableWindowUMG_Statics::EnumInfo),
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
