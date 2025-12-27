// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Window.h"
#include "WindowContentInterface.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeWindow() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow_Module_NoRegister();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow_NoRegister();
NX_MODULARWINDOW_API UClass* Z_Construct_UClass_UWindow_Timer_Event_NoRegister();
NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_InteractEnd__DelegateSignature();
NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_InteractStart__DelegateSignature();
NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_PositionChanged__DelegateSignature();
NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_PositionEnd__DelegateSignature();
NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_PositionStart__DelegateSignature();
NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_SizeChanged__DelegateSignature();
NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_SizeEnd__DelegateSignature();
NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_SizeStart__DelegateSignature();
NX_MODULARWINDOW_API UFunction* Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature();
NX_MODULARWINDOW_API UScriptStruct* Z_Construct_UScriptStruct_FWindowCapabilities();
UMG_API UClass* Z_Construct_UClass_UCanvasPanelSlot_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UMG_API UClass* Z_Construct_UClass_UUserWidget_NoRegister();
UPackage* Z_Construct_UPackage__Script_NX_ModularWindow();
// ********** End Cross Module References **********************************************************

// ********** Begin Delegate FED_WindowClicked *****************************************************
struct Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature_Statics
{
	struct Window_eventED_WindowClicked_Parms
	{
		UWindow* ClickedWindow;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Clicked\n" },
#endif
		{ "ModuleRelativePath", "Public/Window.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Clicked" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ClickedWindow_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Delegate FED_WindowClicked constinit property declarations *********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ClickedWindow;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Delegate FED_WindowClicked constinit property declarations ***********************
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};

// ********** Begin Delegate FED_WindowClicked Property Definitions ********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature_Statics::NewProp_ClickedWindow = { "ClickedWindow", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventED_WindowClicked_Parms, ClickedWindow), Z_Construct_UClass_UWindow_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ClickedWindow_MetaData), NewProp_ClickedWindow_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature_Statics::NewProp_ClickedWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature_Statics::PropPointers) < 2048);
// ********** End Delegate FED_WindowClicked Property Definitions **********************************
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "ED_WindowClicked__DelegateSignature", 	Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature_Statics::PropPointers), 
sizeof(Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature_Statics::Window_eventED_WindowClicked_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature_Statics::Window_eventED_WindowClicked_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void UWindow::FED_WindowClicked_DelegateWrapper(const FMulticastScriptDelegate& ED_WindowClicked, UWindow* ClickedWindow)
{
	struct Window_eventED_WindowClicked_Parms
	{
		UWindow* ClickedWindow;
	};
	Window_eventED_WindowClicked_Parms Parms;
	Parms.ClickedWindow=ClickedWindow;
	ED_WindowClicked.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FED_WindowClicked *******************************************************

// ********** Begin Delegate FED_InteractStart *****************************************************
struct Z_Construct_UDelegateFunction_UWindow_ED_InteractStart__DelegateSignature_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Interact\n" },
#endif
		{ "ModuleRelativePath", "Public/Window.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Interact" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Delegate FED_InteractStart constinit property declarations *********************
// ********** End Delegate FED_InteractStart constinit property declarations ***********************
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_UWindow_ED_InteractStart__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "ED_InteractStart__DelegateSignature", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UWindow_ED_InteractStart__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_UWindow_ED_InteractStart__DelegateSignature_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UDelegateFunction_UWindow_ED_InteractStart__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_UWindow_ED_InteractStart__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void UWindow::FED_InteractStart_DelegateWrapper(const FMulticastScriptDelegate& ED_InteractStart)
{
	ED_InteractStart.ProcessMulticastDelegate<UObject>(NULL);
}
// ********** End Delegate FED_InteractStart *******************************************************

// ********** Begin Delegate FED_InteractEnd *******************************************************
struct Z_Construct_UDelegateFunction_UWindow_ED_InteractEnd__DelegateSignature_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Delegate FED_InteractEnd constinit property declarations ***********************
// ********** End Delegate FED_InteractEnd constinit property declarations *************************
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_UWindow_ED_InteractEnd__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "ED_InteractEnd__DelegateSignature", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UWindow_ED_InteractEnd__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_UWindow_ED_InteractEnd__DelegateSignature_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UDelegateFunction_UWindow_ED_InteractEnd__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_UWindow_ED_InteractEnd__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void UWindow::FED_InteractEnd_DelegateWrapper(const FMulticastScriptDelegate& ED_InteractEnd)
{
	ED_InteractEnd.ProcessMulticastDelegate<UObject>(NULL);
}
// ********** End Delegate FED_InteractEnd *********************************************************

// ********** Begin Delegate FED_PositionStart *****************************************************
struct Z_Construct_UDelegateFunction_UWindow_ED_PositionStart__DelegateSignature_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Position\n" },
#endif
		{ "ModuleRelativePath", "Public/Window.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Position" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Delegate FED_PositionStart constinit property declarations *********************
// ********** End Delegate FED_PositionStart constinit property declarations ***********************
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_UWindow_ED_PositionStart__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "ED_PositionStart__DelegateSignature", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UWindow_ED_PositionStart__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_UWindow_ED_PositionStart__DelegateSignature_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UDelegateFunction_UWindow_ED_PositionStart__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_UWindow_ED_PositionStart__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void UWindow::FED_PositionStart_DelegateWrapper(const FMulticastScriptDelegate& ED_PositionStart)
{
	ED_PositionStart.ProcessMulticastDelegate<UObject>(NULL);
}
// ********** End Delegate FED_PositionStart *******************************************************

// ********** Begin Delegate FED_PositionChanged ***************************************************
struct Z_Construct_UDelegateFunction_UWindow_ED_PositionChanged__DelegateSignature_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Delegate FED_PositionChanged constinit property declarations *******************
// ********** End Delegate FED_PositionChanged constinit property declarations *********************
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_UWindow_ED_PositionChanged__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "ED_PositionChanged__DelegateSignature", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UWindow_ED_PositionChanged__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_UWindow_ED_PositionChanged__DelegateSignature_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UDelegateFunction_UWindow_ED_PositionChanged__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_UWindow_ED_PositionChanged__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void UWindow::FED_PositionChanged_DelegateWrapper(const FMulticastScriptDelegate& ED_PositionChanged)
{
	ED_PositionChanged.ProcessMulticastDelegate<UObject>(NULL);
}
// ********** End Delegate FED_PositionChanged *****************************************************

// ********** Begin Delegate FED_PositionEnd *******************************************************
struct Z_Construct_UDelegateFunction_UWindow_ED_PositionEnd__DelegateSignature_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Delegate FED_PositionEnd constinit property declarations ***********************
// ********** End Delegate FED_PositionEnd constinit property declarations *************************
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_UWindow_ED_PositionEnd__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "ED_PositionEnd__DelegateSignature", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UWindow_ED_PositionEnd__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_UWindow_ED_PositionEnd__DelegateSignature_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UDelegateFunction_UWindow_ED_PositionEnd__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_UWindow_ED_PositionEnd__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void UWindow::FED_PositionEnd_DelegateWrapper(const FMulticastScriptDelegate& ED_PositionEnd)
{
	ED_PositionEnd.ProcessMulticastDelegate<UObject>(NULL);
}
// ********** End Delegate FED_PositionEnd *********************************************************

// ********** Begin Delegate FED_SizeStart *********************************************************
struct Z_Construct_UDelegateFunction_UWindow_ED_SizeStart__DelegateSignature_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Size\n" },
#endif
		{ "ModuleRelativePath", "Public/Window.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Size" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Delegate FED_SizeStart constinit property declarations *************************
// ********** End Delegate FED_SizeStart constinit property declarations ***************************
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_UWindow_ED_SizeStart__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "ED_SizeStart__DelegateSignature", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UWindow_ED_SizeStart__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_UWindow_ED_SizeStart__DelegateSignature_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UDelegateFunction_UWindow_ED_SizeStart__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_UWindow_ED_SizeStart__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void UWindow::FED_SizeStart_DelegateWrapper(const FMulticastScriptDelegate& ED_SizeStart)
{
	ED_SizeStart.ProcessMulticastDelegate<UObject>(NULL);
}
// ********** End Delegate FED_SizeStart ***********************************************************

// ********** Begin Delegate FED_SizeChanged *******************************************************
struct Z_Construct_UDelegateFunction_UWindow_ED_SizeChanged__DelegateSignature_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Delegate FED_SizeChanged constinit property declarations ***********************
// ********** End Delegate FED_SizeChanged constinit property declarations *************************
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_UWindow_ED_SizeChanged__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "ED_SizeChanged__DelegateSignature", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UWindow_ED_SizeChanged__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_UWindow_ED_SizeChanged__DelegateSignature_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UDelegateFunction_UWindow_ED_SizeChanged__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_UWindow_ED_SizeChanged__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void UWindow::FED_SizeChanged_DelegateWrapper(const FMulticastScriptDelegate& ED_SizeChanged)
{
	ED_SizeChanged.ProcessMulticastDelegate<UObject>(NULL);
}
// ********** End Delegate FED_SizeChanged *********************************************************

// ********** Begin Delegate FED_SizeEnd ***********************************************************
struct Z_Construct_UDelegateFunction_UWindow_ED_SizeEnd__DelegateSignature_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Delegate FED_SizeEnd constinit property declarations ***************************
// ********** End Delegate FED_SizeEnd constinit property declarations *****************************
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_UWindow_ED_SizeEnd__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "ED_SizeEnd__DelegateSignature", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UWindow_ED_SizeEnd__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_UWindow_ED_SizeEnd__DelegateSignature_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UDelegateFunction_UWindow_ED_SizeEnd__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_UWindow_ED_SizeEnd__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void UWindow::FED_SizeEnd_DelegateWrapper(const FMulticastScriptDelegate& ED_SizeEnd)
{
	ED_SizeEnd.ProcessMulticastDelegate<UObject>(NULL);
}
// ********** End Delegate FED_SizeEnd *************************************************************

// ********** Begin Class UWindow Function AddModule ***********************************************
struct Z_Construct_UFunction_UWindow_AddModule_Statics
{
	struct Window_eventAddModule_Parms
	{
		UWindow_Module* Module;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Modules//\n" },
#endif
		{ "ModuleRelativePath", "Public/Window.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Modules" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Function AddModule constinit property declarations *****************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Module;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function AddModule constinit property declarations *******************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function AddModule Property Definitions ****************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UWindow_AddModule_Statics::NewProp_Module = { "Module", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventAddModule_Parms, Module), Z_Construct_UClass_UWindow_Module_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_AddModule_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_AddModule_Statics::NewProp_Module,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_AddModule_Statics::PropPointers) < 2048);
// ********** End Function AddModule Property Definitions ******************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_AddModule_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "AddModule", 	Z_Construct_UFunction_UWindow_AddModule_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_AddModule_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_AddModule_Statics::Window_eventAddModule_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_AddModule_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_AddModule_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_AddModule_Statics::Window_eventAddModule_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_AddModule()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_AddModule_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execAddModule)
{
	P_GET_OBJECT(UWindow_Module,Z_Param_Module);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->AddModule(Z_Param_Module);
	P_NATIVE_END;
}
// ********** End Class UWindow Function AddModule *************************************************

// ********** Begin Class UWindow Function ApplyCapabilitiesFromContent ****************************
struct Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromContent_Statics
{
	struct Window_eventApplyCapabilitiesFromContent_Parms
	{
		UUserWidget* ContentWidget;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window|Capabilities" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ContentWidget_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function ApplyCapabilitiesFromContent constinit property declarations **********
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ContentWidget;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ApplyCapabilitiesFromContent constinit property declarations ************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ApplyCapabilitiesFromContent Property Definitions *********************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromContent_Statics::NewProp_ContentWidget = { "ContentWidget", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventApplyCapabilitiesFromContent_Parms, ContentWidget), Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ContentWidget_MetaData), NewProp_ContentWidget_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromContent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromContent_Statics::NewProp_ContentWidget,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromContent_Statics::PropPointers) < 2048);
// ********** End Function ApplyCapabilitiesFromContent Property Definitions ***********************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromContent_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "ApplyCapabilitiesFromContent", 	Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromContent_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromContent_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromContent_Statics::Window_eventApplyCapabilitiesFromContent_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromContent_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromContent_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromContent_Statics::Window_eventApplyCapabilitiesFromContent_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromContent()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromContent_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execApplyCapabilitiesFromContent)
{
	P_GET_OBJECT(UUserWidget,Z_Param_ContentWidget);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ApplyCapabilitiesFromContent(Z_Param_ContentWidget);
	P_NATIVE_END;
}
// ********** End Class UWindow Function ApplyCapabilitiesFromContent ******************************

// ********** Begin Class UWindow Function ApplyCapabilitiesFromStruct *****************************
struct Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromStruct_Statics
{
	struct Window_eventApplyCapabilitiesFromStruct_Parms
	{
		FWindowCapabilities Capabilities;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window|Capabilities" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Capabilities_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function ApplyCapabilitiesFromStruct constinit property declarations ***********
	static const UECodeGen_Private::FStructPropertyParams NewProp_Capabilities;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ApplyCapabilitiesFromStruct constinit property declarations *************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ApplyCapabilitiesFromStruct Property Definitions **********************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromStruct_Statics::NewProp_Capabilities = { "Capabilities", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventApplyCapabilitiesFromStruct_Parms, Capabilities), Z_Construct_UScriptStruct_FWindowCapabilities, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Capabilities_MetaData), NewProp_Capabilities_MetaData) }; // 235622890
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromStruct_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromStruct_Statics::NewProp_Capabilities,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromStruct_Statics::PropPointers) < 2048);
// ********** End Function ApplyCapabilitiesFromStruct Property Definitions ************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromStruct_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "ApplyCapabilitiesFromStruct", 	Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromStruct_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromStruct_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromStruct_Statics::Window_eventApplyCapabilitiesFromStruct_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromStruct_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromStruct_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromStruct_Statics::Window_eventApplyCapabilitiesFromStruct_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromStruct()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromStruct_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execApplyCapabilitiesFromStruct)
{
	P_GET_STRUCT_REF(FWindowCapabilities,Z_Param_Out_Capabilities);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ApplyCapabilitiesFromStruct(Z_Param_Out_Capabilities);
	P_NATIVE_END;
}
// ********** End Class UWindow Function ApplyCapabilitiesFromStruct *******************************

// ********** Begin Class UWindow Function BringToFront ********************************************
struct Z_Construct_UFunction_UWindow_BringToFront_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function BringToFront constinit property declarations **************************
// ********** End Function BringToFront constinit property declarations ****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_BringToFront_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "BringToFront", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_BringToFront_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_BringToFront_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UWindow_BringToFront()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_BringToFront_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execBringToFront)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->BringToFront();
	P_NATIVE_END;
}
// ********** End Class UWindow Function BringToFront **********************************************

// ********** Begin Class UWindow Function CanFullscreen *******************************************
struct Z_Construct_UFunction_UWindow_CanFullscreen_Statics
{
	struct Window_eventCanFullscreen_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window|Capabilities" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function CanFullscreen constinit property declarations *************************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function CanFullscreen constinit property declarations ***************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function CanFullscreen Property Definitions ************************************
void Z_Construct_UFunction_UWindow_CanFullscreen_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((Window_eventCanFullscreen_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UWindow_CanFullscreen_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Window_eventCanFullscreen_Parms), &Z_Construct_UFunction_UWindow_CanFullscreen_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_CanFullscreen_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_CanFullscreen_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_CanFullscreen_Statics::PropPointers) < 2048);
// ********** End Function CanFullscreen Property Definitions **************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_CanFullscreen_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "CanFullscreen", 	Z_Construct_UFunction_UWindow_CanFullscreen_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_CanFullscreen_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_CanFullscreen_Statics::Window_eventCanFullscreen_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_CanFullscreen_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_CanFullscreen_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_CanFullscreen_Statics::Window_eventCanFullscreen_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_CanFullscreen()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_CanFullscreen_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execCanFullscreen)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->CanFullscreen();
	P_NATIVE_END;
}
// ********** End Class UWindow Function CanFullscreen *********************************************

// ********** Begin Class UWindow Function CanMove *************************************************
struct Z_Construct_UFunction_UWindow_CanMove_Statics
{
	struct Window_eventCanMove_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window|Capabilities" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function CanMove constinit property declarations *******************************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function CanMove constinit property declarations *********************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function CanMove Property Definitions ******************************************
void Z_Construct_UFunction_UWindow_CanMove_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((Window_eventCanMove_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UWindow_CanMove_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Window_eventCanMove_Parms), &Z_Construct_UFunction_UWindow_CanMove_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_CanMove_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_CanMove_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_CanMove_Statics::PropPointers) < 2048);
// ********** End Function CanMove Property Definitions ********************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_CanMove_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "CanMove", 	Z_Construct_UFunction_UWindow_CanMove_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_CanMove_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_CanMove_Statics::Window_eventCanMove_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_CanMove_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_CanMove_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_CanMove_Statics::Window_eventCanMove_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_CanMove()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_CanMove_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execCanMove)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->CanMove();
	P_NATIVE_END;
}
// ********** End Class UWindow Function CanMove ***************************************************

// ********** Begin Class UWindow Function CanResize ***********************************************
struct Z_Construct_UFunction_UWindow_CanResize_Statics
{
	struct Window_eventCanResize_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window|Capabilities" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function CanResize constinit property declarations *****************************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function CanResize constinit property declarations *******************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function CanResize Property Definitions ****************************************
void Z_Construct_UFunction_UWindow_CanResize_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((Window_eventCanResize_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UWindow_CanResize_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Window_eventCanResize_Parms), &Z_Construct_UFunction_UWindow_CanResize_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_CanResize_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_CanResize_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_CanResize_Statics::PropPointers) < 2048);
// ********** End Function CanResize Property Definitions ******************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_CanResize_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "CanResize", 	Z_Construct_UFunction_UWindow_CanResize_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_CanResize_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_CanResize_Statics::Window_eventCanResize_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_CanResize_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_CanResize_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_CanResize_Statics::Window_eventCanResize_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_CanResize()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_CanResize_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execCanResize)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->CanResize();
	P_NATIVE_END;
}
// ********** End Class UWindow Function CanResize *************************************************

// ********** Begin Class UWindow Function GetCanvasSlot *******************************************
struct Z_Construct_UFunction_UWindow_GetCanvasSlot_Statics
{
	struct Window_eventGetCanvasSlot_Parms
	{
		UCanvasPanelSlot* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetCanvasSlot constinit property declarations *************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetCanvasSlot constinit property declarations ***************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetCanvasSlot Property Definitions ************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UWindow_GetCanvasSlot_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventGetCanvasSlot_Parms, ReturnValue), Z_Construct_UClass_UCanvasPanelSlot_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_GetCanvasSlot_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_GetCanvasSlot_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_GetCanvasSlot_Statics::PropPointers) < 2048);
// ********** End Function GetCanvasSlot Property Definitions **************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_GetCanvasSlot_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "GetCanvasSlot", 	Z_Construct_UFunction_UWindow_GetCanvasSlot_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_GetCanvasSlot_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_GetCanvasSlot_Statics::Window_eventGetCanvasSlot_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_GetCanvasSlot_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_GetCanvasSlot_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_GetCanvasSlot_Statics::Window_eventGetCanvasSlot_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_GetCanvasSlot()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_GetCanvasSlot_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execGetCanvasSlot)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UCanvasPanelSlot**)Z_Param__Result=P_THIS->GetCanvasSlot();
	P_NATIVE_END;
}
// ********** End Class UWindow Function GetCanvasSlot *********************************************

// ********** Begin Class UWindow Function GetPosition *********************************************
struct Z_Construct_UFunction_UWindow_GetPosition_Statics
{
	struct Window_eventGetPosition_Parms
	{
		FVector2D ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Position//\n//Getter//\n" },
#endif
		{ "ModuleRelativePath", "Public/Window.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Position\nGetter" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Function GetPosition constinit property declarations ***************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetPosition constinit property declarations *****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetPosition Property Definitions **************************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UWindow_GetPosition_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventGetPosition_Parms, ReturnValue), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_GetPosition_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_GetPosition_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_GetPosition_Statics::PropPointers) < 2048);
// ********** End Function GetPosition Property Definitions ****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_GetPosition_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "GetPosition", 	Z_Construct_UFunction_UWindow_GetPosition_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_GetPosition_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_GetPosition_Statics::Window_eventGetPosition_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_GetPosition_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_GetPosition_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_GetPosition_Statics::Window_eventGetPosition_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_GetPosition()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_GetPosition_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execGetPosition)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FVector2D*)Z_Param__Result=P_THIS->GetPosition();
	P_NATIVE_END;
}
// ********** End Class UWindow Function GetPosition ***********************************************

// ********** Begin Class UWindow Function GetSize *************************************************
struct Z_Construct_UFunction_UWindow_GetSize_Statics
{
	struct Window_eventGetSize_Parms
	{
		FVector2D ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Size//\n//Getter//\n" },
#endif
		{ "ModuleRelativePath", "Public/Window.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Size\nGetter" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Function GetSize constinit property declarations *******************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetSize constinit property declarations *********************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetSize Property Definitions ******************************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UWindow_GetSize_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventGetSize_Parms, ReturnValue), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_GetSize_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_GetSize_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_GetSize_Statics::PropPointers) < 2048);
// ********** End Function GetSize Property Definitions ********************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_GetSize_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "GetSize", 	Z_Construct_UFunction_UWindow_GetSize_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_GetSize_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_GetSize_Statics::Window_eventGetSize_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_GetSize_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_GetSize_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_GetSize_Statics::Window_eventGetSize_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_GetSize()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_GetSize_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execGetSize)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FVector2D*)Z_Param__Result=P_THIS->GetSize();
	P_NATIVE_END;
}
// ********** End Class UWindow Function GetSize ***************************************************

// ********** Begin Class UWindow Function GetZOrder ***********************************************
struct Z_Construct_UFunction_UWindow_GetZOrder_Statics
{
	struct Window_eventGetZOrder_Parms
	{
		int32 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetZOrder constinit property declarations *****************************
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetZOrder constinit property declarations *******************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetZOrder Property Definitions ****************************************
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UWindow_GetZOrder_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventGetZOrder_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_GetZOrder_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_GetZOrder_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_GetZOrder_Statics::PropPointers) < 2048);
// ********** End Function GetZOrder Property Definitions ******************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_GetZOrder_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "GetZOrder", 	Z_Construct_UFunction_UWindow_GetZOrder_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_GetZOrder_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_GetZOrder_Statics::Window_eventGetZOrder_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_GetZOrder_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_GetZOrder_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_GetZOrder_Statics::Window_eventGetZOrder_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_GetZOrder()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_GetZOrder_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execGetZOrder)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int32*)Z_Param__Result=P_THIS->GetZOrder();
	P_NATIVE_END;
}
// ********** End Class UWindow Function GetZOrder *************************************************

// ********** Begin Class UWindow Function Init ****************************************************
struct Z_Construct_UFunction_UWindow_Init_Statics
{
	struct Window_eventInit_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function Init constinit property declarations **********************************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function Init constinit property declarations ************************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function Init Property Definitions *********************************************
void Z_Construct_UFunction_UWindow_Init_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((Window_eventInit_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UWindow_Init_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Window_eventInit_Parms), &Z_Construct_UFunction_UWindow_Init_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_Init_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_Init_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_Init_Statics::PropPointers) < 2048);
// ********** End Function Init Property Definitions ***********************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_Init_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "Init", 	Z_Construct_UFunction_UWindow_Init_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_Init_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_Init_Statics::Window_eventInit_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_Init_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_Init_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_Init_Statics::Window_eventInit_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_Init()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_Init_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execInit)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->Init();
	P_NATIVE_END;
}
// ********** End Class UWindow Function Init ******************************************************

// ********** Begin Class UWindow Function IsAnchorPoint *******************************************
struct Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics
{
	struct Window_eventIsAnchorPoint_Parms
	{
		bool bIsX;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "CPP_Default_bIsX", "true" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function IsAnchorPoint constinit property declarations *************************
	static void NewProp_bIsX_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bIsX;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function IsAnchorPoint constinit property declarations ***************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function IsAnchorPoint Property Definitions ************************************
void Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::NewProp_bIsX_SetBit(void* Obj)
{
	((Window_eventIsAnchorPoint_Parms*)Obj)->bIsX = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::NewProp_bIsX = { "bIsX", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Window_eventIsAnchorPoint_Parms), &Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::NewProp_bIsX_SetBit, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((Window_eventIsAnchorPoint_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Window_eventIsAnchorPoint_Parms), &Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::NewProp_bIsX,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::PropPointers) < 2048);
// ********** End Function IsAnchorPoint Property Definitions **************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "IsAnchorPoint", 	Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::Window_eventIsAnchorPoint_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::Window_eventIsAnchorPoint_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_IsAnchorPoint()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_IsAnchorPoint_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execIsAnchorPoint)
{
	P_GET_UBOOL(Z_Param_bIsX);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsAnchorPoint(Z_Param_bIsX);
	P_NATIVE_END;
}
// ********** End Class UWindow Function IsAnchorPoint *********************************************

// ********** Begin Class UWindow Function OnCapabilitiesChanged ***********************************
static FName NAME_UWindow_OnCapabilitiesChanged = FName(TEXT("OnCapabilitiesChanged"));
void UWindow::OnCapabilitiesChanged()
{
	UFunction* Func = FindFunctionChecked(NAME_UWindow_OnCapabilitiesChanged);
	ProcessEvent(Func,NULL);
}
struct Z_Construct_UFunction_UWindow_OnCapabilitiesChanged_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window|Capabilities" },
		{ "DisplayName", "On Capabilities Changed" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function OnCapabilitiesChanged constinit property declarations *****************
// ********** End Function OnCapabilitiesChanged constinit property declarations *******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_OnCapabilitiesChanged_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "OnCapabilitiesChanged", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_OnCapabilitiesChanged_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_OnCapabilitiesChanged_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UWindow_OnCapabilitiesChanged()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_OnCapabilitiesChanged_Statics::FuncParams);
	}
	return ReturnFunction;
}
// ********** End Class UWindow Function OnCapabilitiesChanged *************************************

// ********** Begin Class UWindow Function RemoveModule ********************************************
struct Z_Construct_UFunction_UWindow_RemoveModule_Statics
{
	struct Window_eventRemoveModule_Parms
	{
		UWindow_Module* Module;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function RemoveModule constinit property declarations **************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Module;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function RemoveModule constinit property declarations ****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function RemoveModule Property Definitions *************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UWindow_RemoveModule_Statics::NewProp_Module = { "Module", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventRemoveModule_Parms, Module), Z_Construct_UClass_UWindow_Module_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_RemoveModule_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_RemoveModule_Statics::NewProp_Module,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_RemoveModule_Statics::PropPointers) < 2048);
// ********** End Function RemoveModule Property Definitions ***************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_RemoveModule_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "RemoveModule", 	Z_Construct_UFunction_UWindow_RemoveModule_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_RemoveModule_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_RemoveModule_Statics::Window_eventRemoveModule_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_RemoveModule_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_RemoveModule_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_RemoveModule_Statics::Window_eventRemoveModule_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_RemoveModule()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_RemoveModule_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execRemoveModule)
{
	P_GET_OBJECT(UWindow_Module,Z_Param_Module);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->RemoveModule(Z_Param_Module);
	P_NATIVE_END;
}
// ********** End Class UWindow Function RemoveModule **********************************************

// ********** Begin Class UWindow Function SetByParameter ******************************************
struct Z_Construct_UFunction_UWindow_SetByParameter_Statics
{
	struct Window_eventSetByParameter_Parms
	{
		TArray<FName> InNames;
		TArray<float> InValues;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//SetByParameter//\n" },
#endif
		{ "ModuleRelativePath", "Public/Window.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "SetByParameter" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Function SetByParameter constinit property declarations ************************
	static const UECodeGen_Private::FNamePropertyParams NewProp_InNames_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_InNames;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InValues_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_InValues;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetByParameter constinit property declarations **************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetByParameter Property Definitions ***********************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UWindow_SetByParameter_Statics::NewProp_InNames_Inner = { "InNames", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UWindow_SetByParameter_Statics::NewProp_InNames = { "InNames", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventSetByParameter_Parms, InNames), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UWindow_SetByParameter_Statics::NewProp_InValues_Inner = { "InValues", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UWindow_SetByParameter_Statics::NewProp_InValues = { "InValues", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventSetByParameter_Parms, InValues), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_SetByParameter_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetByParameter_Statics::NewProp_InNames_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetByParameter_Statics::NewProp_InNames,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetByParameter_Statics::NewProp_InValues_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetByParameter_Statics::NewProp_InValues,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetByParameter_Statics::PropPointers) < 2048);
// ********** End Function SetByParameter Property Definitions *************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_SetByParameter_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "SetByParameter", 	Z_Construct_UFunction_UWindow_SetByParameter_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetByParameter_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_SetByParameter_Statics::Window_eventSetByParameter_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetByParameter_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_SetByParameter_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_SetByParameter_Statics::Window_eventSetByParameter_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_SetByParameter()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_SetByParameter_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execSetByParameter)
{
	P_GET_TARRAY(FName,Z_Param_InNames);
	P_GET_TARRAY(float,Z_Param_InValues);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetByParameter(Z_Param_InNames,Z_Param_InValues);
	P_NATIVE_END;
}
// ********** End Class UWindow Function SetByParameter ********************************************

// ********** Begin Class UWindow Function SetByParameterPosition **********************************
struct Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics
{
	struct Window_eventSetByParameterPosition_Parms
	{
		TArray<FName> InNames;
		TArray<float> InValues;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetByParameterPosition constinit property declarations ****************
	static const UECodeGen_Private::FNamePropertyParams NewProp_InNames_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_InNames;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InValues_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_InValues;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetByParameterPosition constinit property declarations ******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetByParameterPosition Property Definitions ***************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::NewProp_InNames_Inner = { "InNames", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::NewProp_InNames = { "InNames", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventSetByParameterPosition_Parms, InNames), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::NewProp_InValues_Inner = { "InValues", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::NewProp_InValues = { "InValues", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventSetByParameterPosition_Parms, InValues), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::NewProp_InNames_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::NewProp_InNames,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::NewProp_InValues_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::NewProp_InValues,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::PropPointers) < 2048);
// ********** End Function SetByParameterPosition Property Definitions *****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "SetByParameterPosition", 	Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::Window_eventSetByParameterPosition_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::Window_eventSetByParameterPosition_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_SetByParameterPosition()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_SetByParameterPosition_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execSetByParameterPosition)
{
	P_GET_TARRAY(FName,Z_Param_InNames);
	P_GET_TARRAY(float,Z_Param_InValues);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetByParameterPosition(Z_Param_InNames,Z_Param_InValues);
	P_NATIVE_END;
}
// ********** End Class UWindow Function SetByParameterPosition ************************************

// ********** Begin Class UWindow Function SetByParameterSize **************************************
struct Z_Construct_UFunction_UWindow_SetByParameterSize_Statics
{
	struct Window_eventSetByParameterSize_Parms
	{
		TArray<FName> InNames;
		TArray<float> InValues;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetByParameterSize constinit property declarations ********************
	static const UECodeGen_Private::FNamePropertyParams NewProp_InNames_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_InNames;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InValues_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_InValues;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetByParameterSize constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetByParameterSize Property Definitions *******************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::NewProp_InNames_Inner = { "InNames", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::NewProp_InNames = { "InNames", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventSetByParameterSize_Parms, InNames), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::NewProp_InValues_Inner = { "InValues", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::NewProp_InValues = { "InValues", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventSetByParameterSize_Parms, InValues), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::NewProp_InNames_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::NewProp_InNames,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::NewProp_InValues_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::NewProp_InValues,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::PropPointers) < 2048);
// ********** End Function SetByParameterSize Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "SetByParameterSize", 	Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::Window_eventSetByParameterSize_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::Window_eventSetByParameterSize_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_SetByParameterSize()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_SetByParameterSize_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execSetByParameterSize)
{
	P_GET_TARRAY(FName,Z_Param_InNames);
	P_GET_TARRAY(float,Z_Param_InValues);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetByParameterSize(Z_Param_InNames,Z_Param_InValues);
	P_NATIVE_END;
}
// ********** End Class UWindow Function SetByParameterSize ****************************************

// ********** Begin Class UWindow Function SetCanFullscreen ****************************************
struct Z_Construct_UFunction_UWindow_SetCanFullscreen_Statics
{
	struct Window_eventSetCanFullscreen_Parms
	{
		bool bNewCanFullscreen;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window|Capabilities" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetCanFullscreen constinit property declarations **********************
	static void NewProp_bNewCanFullscreen_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bNewCanFullscreen;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetCanFullscreen constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetCanFullscreen Property Definitions *********************************
void Z_Construct_UFunction_UWindow_SetCanFullscreen_Statics::NewProp_bNewCanFullscreen_SetBit(void* Obj)
{
	((Window_eventSetCanFullscreen_Parms*)Obj)->bNewCanFullscreen = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UWindow_SetCanFullscreen_Statics::NewProp_bNewCanFullscreen = { "bNewCanFullscreen", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Window_eventSetCanFullscreen_Parms), &Z_Construct_UFunction_UWindow_SetCanFullscreen_Statics::NewProp_bNewCanFullscreen_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_SetCanFullscreen_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetCanFullscreen_Statics::NewProp_bNewCanFullscreen,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetCanFullscreen_Statics::PropPointers) < 2048);
// ********** End Function SetCanFullscreen Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_SetCanFullscreen_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "SetCanFullscreen", 	Z_Construct_UFunction_UWindow_SetCanFullscreen_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetCanFullscreen_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_SetCanFullscreen_Statics::Window_eventSetCanFullscreen_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetCanFullscreen_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_SetCanFullscreen_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_SetCanFullscreen_Statics::Window_eventSetCanFullscreen_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_SetCanFullscreen()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_SetCanFullscreen_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execSetCanFullscreen)
{
	P_GET_UBOOL(Z_Param_bNewCanFullscreen);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetCanFullscreen(Z_Param_bNewCanFullscreen);
	P_NATIVE_END;
}
// ********** End Class UWindow Function SetCanFullscreen ******************************************

// ********** Begin Class UWindow Function SetCanMove **********************************************
struct Z_Construct_UFunction_UWindow_SetCanMove_Statics
{
	struct Window_eventSetCanMove_Parms
	{
		bool bNewCanMove;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window|Capabilities" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetCanMove constinit property declarations ****************************
	static void NewProp_bNewCanMove_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bNewCanMove;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetCanMove constinit property declarations ******************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetCanMove Property Definitions ***************************************
void Z_Construct_UFunction_UWindow_SetCanMove_Statics::NewProp_bNewCanMove_SetBit(void* Obj)
{
	((Window_eventSetCanMove_Parms*)Obj)->bNewCanMove = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UWindow_SetCanMove_Statics::NewProp_bNewCanMove = { "bNewCanMove", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Window_eventSetCanMove_Parms), &Z_Construct_UFunction_UWindow_SetCanMove_Statics::NewProp_bNewCanMove_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_SetCanMove_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetCanMove_Statics::NewProp_bNewCanMove,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetCanMove_Statics::PropPointers) < 2048);
// ********** End Function SetCanMove Property Definitions *****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_SetCanMove_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "SetCanMove", 	Z_Construct_UFunction_UWindow_SetCanMove_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetCanMove_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_SetCanMove_Statics::Window_eventSetCanMove_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetCanMove_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_SetCanMove_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_SetCanMove_Statics::Window_eventSetCanMove_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_SetCanMove()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_SetCanMove_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execSetCanMove)
{
	P_GET_UBOOL(Z_Param_bNewCanMove);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetCanMove(Z_Param_bNewCanMove);
	P_NATIVE_END;
}
// ********** End Class UWindow Function SetCanMove ************************************************

// ********** Begin Class UWindow Function SetCanResize ********************************************
struct Z_Construct_UFunction_UWindow_SetCanResize_Statics
{
	struct Window_eventSetCanResize_Parms
	{
		bool bNewCanResize;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window|Capabilities" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetCanResize constinit property declarations **************************
	static void NewProp_bNewCanResize_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bNewCanResize;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetCanResize constinit property declarations ****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetCanResize Property Definitions *************************************
void Z_Construct_UFunction_UWindow_SetCanResize_Statics::NewProp_bNewCanResize_SetBit(void* Obj)
{
	((Window_eventSetCanResize_Parms*)Obj)->bNewCanResize = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UWindow_SetCanResize_Statics::NewProp_bNewCanResize = { "bNewCanResize", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Window_eventSetCanResize_Parms), &Z_Construct_UFunction_UWindow_SetCanResize_Statics::NewProp_bNewCanResize_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_SetCanResize_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetCanResize_Statics::NewProp_bNewCanResize,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetCanResize_Statics::PropPointers) < 2048);
// ********** End Function SetCanResize Property Definitions ***************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_SetCanResize_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "SetCanResize", 	Z_Construct_UFunction_UWindow_SetCanResize_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetCanResize_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_SetCanResize_Statics::Window_eventSetCanResize_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetCanResize_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_SetCanResize_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_SetCanResize_Statics::Window_eventSetCanResize_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_SetCanResize()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_SetCanResize_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execSetCanResize)
{
	P_GET_UBOOL(Z_Param_bNewCanResize);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetCanResize(Z_Param_bNewCanResize);
	P_NATIVE_END;
}
// ********** End Class UWindow Function SetCanResize **********************************************

// ********** Begin Class UWindow Function SetPosition *********************************************
struct Z_Construct_UFunction_UWindow_SetPosition_Statics
{
	struct Window_eventSetPosition_Parms
	{
		FVector2D NewPosition;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Setter//\n" },
#endif
		{ "ModuleRelativePath", "Public/Window.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Setter" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Function SetPosition constinit property declarations ***************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_NewPosition;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetPosition constinit property declarations *****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetPosition Property Definitions **************************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UWindow_SetPosition_Statics::NewProp_NewPosition = { "NewPosition", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventSetPosition_Parms, NewPosition), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_SetPosition_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetPosition_Statics::NewProp_NewPosition,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetPosition_Statics::PropPointers) < 2048);
// ********** End Function SetPosition Property Definitions ****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_SetPosition_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "SetPosition", 	Z_Construct_UFunction_UWindow_SetPosition_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetPosition_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_SetPosition_Statics::Window_eventSetPosition_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetPosition_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_SetPosition_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_SetPosition_Statics::Window_eventSetPosition_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_SetPosition()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_SetPosition_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execSetPosition)
{
	P_GET_STRUCT(FVector2D,Z_Param_NewPosition);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetPosition(Z_Param_NewPosition);
	P_NATIVE_END;
}
// ********** End Class UWindow Function SetPosition ***********************************************

// ********** Begin Class UWindow Function SetPositionX ********************************************
struct Z_Construct_UFunction_UWindow_SetPositionX_Statics
{
	struct Window_eventSetPositionX_Parms
	{
		float NewPosition;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetPositionX constinit property declarations **************************
	static const UECodeGen_Private::FFloatPropertyParams NewProp_NewPosition;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetPositionX constinit property declarations ****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetPositionX Property Definitions *************************************
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UWindow_SetPositionX_Statics::NewProp_NewPosition = { "NewPosition", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventSetPositionX_Parms, NewPosition), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_SetPositionX_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetPositionX_Statics::NewProp_NewPosition,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetPositionX_Statics::PropPointers) < 2048);
// ********** End Function SetPositionX Property Definitions ***************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_SetPositionX_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "SetPositionX", 	Z_Construct_UFunction_UWindow_SetPositionX_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetPositionX_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_SetPositionX_Statics::Window_eventSetPositionX_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetPositionX_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_SetPositionX_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_SetPositionX_Statics::Window_eventSetPositionX_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_SetPositionX()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_SetPositionX_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execSetPositionX)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_NewPosition);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetPositionX(Z_Param_NewPosition);
	P_NATIVE_END;
}
// ********** End Class UWindow Function SetPositionX **********************************************

// ********** Begin Class UWindow Function SetPositionY ********************************************
struct Z_Construct_UFunction_UWindow_SetPositionY_Statics
{
	struct Window_eventSetPositionY_Parms
	{
		float NewPosition;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetPositionY constinit property declarations **************************
	static const UECodeGen_Private::FFloatPropertyParams NewProp_NewPosition;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetPositionY constinit property declarations ****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetPositionY Property Definitions *************************************
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UWindow_SetPositionY_Statics::NewProp_NewPosition = { "NewPosition", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventSetPositionY_Parms, NewPosition), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_SetPositionY_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetPositionY_Statics::NewProp_NewPosition,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetPositionY_Statics::PropPointers) < 2048);
// ********** End Function SetPositionY Property Definitions ***************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_SetPositionY_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "SetPositionY", 	Z_Construct_UFunction_UWindow_SetPositionY_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetPositionY_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_SetPositionY_Statics::Window_eventSetPositionY_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetPositionY_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_SetPositionY_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_SetPositionY_Statics::Window_eventSetPositionY_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_SetPositionY()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_SetPositionY_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execSetPositionY)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_NewPosition);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetPositionY(Z_Param_NewPosition);
	P_NATIVE_END;
}
// ********** End Class UWindow Function SetPositionY **********************************************

// ********** Begin Class UWindow Function SetSize *************************************************
struct Z_Construct_UFunction_UWindow_SetSize_Statics
{
	struct Window_eventSetSize_Parms
	{
		FVector2D NewSize;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Setter//\n" },
#endif
		{ "ModuleRelativePath", "Public/Window.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Setter" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Function SetSize constinit property declarations *******************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_NewSize;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetSize constinit property declarations *********************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetSize Property Definitions ******************************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UWindow_SetSize_Statics::NewProp_NewSize = { "NewSize", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventSetSize_Parms, NewSize), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_SetSize_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetSize_Statics::NewProp_NewSize,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetSize_Statics::PropPointers) < 2048);
// ********** End Function SetSize Property Definitions ********************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_SetSize_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "SetSize", 	Z_Construct_UFunction_UWindow_SetSize_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetSize_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_SetSize_Statics::Window_eventSetSize_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetSize_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_SetSize_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_SetSize_Statics::Window_eventSetSize_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_SetSize()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_SetSize_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execSetSize)
{
	P_GET_STRUCT(FVector2D,Z_Param_NewSize);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetSize(Z_Param_NewSize);
	P_NATIVE_END;
}
// ********** End Class UWindow Function SetSize ***************************************************

// ********** Begin Class UWindow Function SetSizeX ************************************************
struct Z_Construct_UFunction_UWindow_SetSizeX_Statics
{
	struct Window_eventSetSizeX_Parms
	{
		float NewSize;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetSizeX constinit property declarations ******************************
	static const UECodeGen_Private::FFloatPropertyParams NewProp_NewSize;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetSizeX constinit property declarations ********************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetSizeX Property Definitions *****************************************
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UWindow_SetSizeX_Statics::NewProp_NewSize = { "NewSize", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventSetSizeX_Parms, NewSize), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_SetSizeX_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetSizeX_Statics::NewProp_NewSize,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetSizeX_Statics::PropPointers) < 2048);
// ********** End Function SetSizeX Property Definitions *******************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_SetSizeX_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "SetSizeX", 	Z_Construct_UFunction_UWindow_SetSizeX_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetSizeX_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_SetSizeX_Statics::Window_eventSetSizeX_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetSizeX_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_SetSizeX_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_SetSizeX_Statics::Window_eventSetSizeX_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_SetSizeX()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_SetSizeX_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execSetSizeX)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_NewSize);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetSizeX(Z_Param_NewSize);
	P_NATIVE_END;
}
// ********** End Class UWindow Function SetSizeX **************************************************

// ********** Begin Class UWindow Function SetSizeY ************************************************
struct Z_Construct_UFunction_UWindow_SetSizeY_Statics
{
	struct Window_eventSetSizeY_Parms
	{
		float NewSize;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetSizeY constinit property declarations ******************************
	static const UECodeGen_Private::FFloatPropertyParams NewProp_NewSize;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetSizeY constinit property declarations ********************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetSizeY Property Definitions *****************************************
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UWindow_SetSizeY_Statics::NewProp_NewSize = { "NewSize", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventSetSizeY_Parms, NewSize), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_SetSizeY_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetSizeY_Statics::NewProp_NewSize,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetSizeY_Statics::PropPointers) < 2048);
// ********** End Function SetSizeY Property Definitions *******************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_SetSizeY_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "SetSizeY", 	Z_Construct_UFunction_UWindow_SetSizeY_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetSizeY_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_SetSizeY_Statics::Window_eventSetSizeY_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetSizeY_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_SetSizeY_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_SetSizeY_Statics::Window_eventSetSizeY_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_SetSizeY()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_SetSizeY_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execSetSizeY)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_NewSize);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetSizeY(Z_Param_NewSize);
	P_NATIVE_END;
}
// ********** End Class UWindow Function SetSizeY **************************************************

// ********** Begin Class UWindow Function SetWindowCapabilities ***********************************
struct Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics
{
	struct Window_eventSetWindowCapabilities_Parms
	{
		bool bMove;
		bool bResize;
		bool bFullscreen;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window|Capabilities" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetWindowCapabilities constinit property declarations *****************
	static void NewProp_bMove_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bMove;
	static void NewProp_bResize_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bResize;
	static void NewProp_bFullscreen_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bFullscreen;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetWindowCapabilities constinit property declarations *******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetWindowCapabilities Property Definitions ****************************
void Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::NewProp_bMove_SetBit(void* Obj)
{
	((Window_eventSetWindowCapabilities_Parms*)Obj)->bMove = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::NewProp_bMove = { "bMove", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Window_eventSetWindowCapabilities_Parms), &Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::NewProp_bMove_SetBit, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::NewProp_bResize_SetBit(void* Obj)
{
	((Window_eventSetWindowCapabilities_Parms*)Obj)->bResize = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::NewProp_bResize = { "bResize", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Window_eventSetWindowCapabilities_Parms), &Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::NewProp_bResize_SetBit, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::NewProp_bFullscreen_SetBit(void* Obj)
{
	((Window_eventSetWindowCapabilities_Parms*)Obj)->bFullscreen = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::NewProp_bFullscreen = { "bFullscreen", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Window_eventSetWindowCapabilities_Parms), &Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::NewProp_bFullscreen_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::NewProp_bMove,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::NewProp_bResize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::NewProp_bFullscreen,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::PropPointers) < 2048);
// ********** End Function SetWindowCapabilities Property Definitions ******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "SetWindowCapabilities", 	Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::Window_eventSetWindowCapabilities_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::Window_eventSetWindowCapabilities_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_SetWindowCapabilities()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_SetWindowCapabilities_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execSetWindowCapabilities)
{
	P_GET_UBOOL(Z_Param_bMove);
	P_GET_UBOOL(Z_Param_bResize);
	P_GET_UBOOL(Z_Param_bFullscreen);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetWindowCapabilities(Z_Param_bMove,Z_Param_bResize,Z_Param_bFullscreen);
	P_NATIVE_END;
}
// ********** End Class UWindow Function SetWindowCapabilities *************************************

// ********** Begin Class UWindow Function SetZOrder ***********************************************
struct Z_Construct_UFunction_UWindow_SetZOrder_Statics
{
	struct Window_eventSetZOrder_Parms
	{
		int32 NewZOrder;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetZOrder constinit property declarations *****************************
	static const UECodeGen_Private::FIntPropertyParams NewProp_NewZOrder;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetZOrder constinit property declarations *******************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetZOrder Property Definitions ****************************************
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UWindow_SetZOrder_Statics::NewProp_NewZOrder = { "NewZOrder", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Window_eventSetZOrder_Parms, NewZOrder), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWindow_SetZOrder_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWindow_SetZOrder_Statics::NewProp_NewZOrder,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetZOrder_Statics::PropPointers) < 2048);
// ********** End Function SetZOrder Property Definitions ******************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_SetZOrder_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "SetZOrder", 	Z_Construct_UFunction_UWindow_SetZOrder_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetZOrder_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UWindow_SetZOrder_Statics::Window_eventSetZOrder_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_SetZOrder_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_SetZOrder_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWindow_SetZOrder_Statics::Window_eventSetZOrder_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWindow_SetZOrder()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_SetZOrder_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execSetZOrder)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_NewZOrder);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetZOrder(Z_Param_NewZOrder);
	P_NATIVE_END;
}
// ********** End Class UWindow Function SetZOrder *************************************************

// ********** Begin Class UWindow Function Uninit **************************************************
struct Z_Construct_UFunction_UWindow_Uninit_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function Uninit constinit property declarations ********************************
// ********** End Function Uninit constinit property declarations **********************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_Uninit_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "Uninit", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_Uninit_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_Uninit_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UWindow_Uninit()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_Uninit_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execUninit)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->Uninit();
	P_NATIVE_END;
}
// ********** End Class UWindow Function Uninit ****************************************************

// ********** Begin Class UWindow Function UpdateUIForCapabilities *********************************
struct Z_Construct_UFunction_UWindow_UpdateUIForCapabilities_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Window|Capabilities" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function UpdateUIForCapabilities constinit property declarations ***************
// ********** End Function UpdateUIForCapabilities constinit property declarations *****************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWindow_UpdateUIForCapabilities_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWindow, nullptr, "UpdateUIForCapabilities", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWindow_UpdateUIForCapabilities_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWindow_UpdateUIForCapabilities_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UWindow_UpdateUIForCapabilities()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWindow_UpdateUIForCapabilities_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWindow::execUpdateUIForCapabilities)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->UpdateUIForCapabilities();
	P_NATIVE_END;
}
// ********** End Class UWindow Function UpdateUIForCapabilities ***********************************

// ********** Begin Class UWindow ******************************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UWindow;
UClass* UWindow::GetPrivateStaticClass()
{
	using TClass = UWindow;
	if (!Z_Registration_Info_UClass_UWindow.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("Window"),
			Z_Registration_Info_UClass_UWindow.InnerSingleton,
			StaticRegisterNativesUWindow,
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
	return Z_Registration_Info_UClass_UWindow.InnerSingleton;
}
UClass* Z_Construct_UClass_UWindow_NoRegister()
{
	return UWindow::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UWindow_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Window.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Modules_MetaData[] = {
		{ "Category", "Window" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanMove_MetaData[] = {
		{ "Category", "Window|Capabilities" },
		{ "DisplayName", "Can Move Window" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanResize_MetaData[] = {
		{ "Category", "Window|Capabilities" },
		{ "DisplayName", "Can Resize Window" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanFullscreen_MetaData[] = {
		{ "Category", "Window|Capabilities" },
		{ "DisplayName", "Can Fullscreen Window" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Window_Timer_Event_MetaData[] = {
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CanvasSlot_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ED_WindowClicked_MetaData[] = {
		{ "Category", "Events" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ED_InteractStart_MetaData[] = {
		{ "Category", "Events" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ED_InteractEnd_MetaData[] = {
		{ "Category", "Events" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ED_PositionStart_MetaData[] = {
		{ "Category", "Events" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ED_PositionChanged_MetaData[] = {
		{ "Category", "Events" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ED_PositionEnd_MetaData[] = {
		{ "Category", "Events" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ED_SizeStart_MetaData[] = {
		{ "Category", "Events" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ED_SizeChanged_MetaData[] = {
		{ "Category", "Events" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ED_SizeEnd_MetaData[] = {
		{ "Category", "Events" },
		{ "ModuleRelativePath", "Public/Window.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UWindow constinit property declarations **********************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Modules_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Modules;
	static void NewProp_bCanMove_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanMove;
	static void NewProp_bCanResize_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanResize;
	static void NewProp_bCanFullscreen_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanFullscreen;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Window_Timer_Event;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_CanvasSlot;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_ED_WindowClicked;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_ED_InteractStart;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_ED_InteractEnd;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_ED_PositionStart;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_ED_PositionChanged;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_ED_PositionEnd;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_ED_SizeStart;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_ED_SizeChanged;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_ED_SizeEnd;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UWindow constinit property declarations ************************************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("AddModule"), .Pointer = &UWindow::execAddModule },
		{ .NameUTF8 = UTF8TEXT("ApplyCapabilitiesFromContent"), .Pointer = &UWindow::execApplyCapabilitiesFromContent },
		{ .NameUTF8 = UTF8TEXT("ApplyCapabilitiesFromStruct"), .Pointer = &UWindow::execApplyCapabilitiesFromStruct },
		{ .NameUTF8 = UTF8TEXT("BringToFront"), .Pointer = &UWindow::execBringToFront },
		{ .NameUTF8 = UTF8TEXT("CanFullscreen"), .Pointer = &UWindow::execCanFullscreen },
		{ .NameUTF8 = UTF8TEXT("CanMove"), .Pointer = &UWindow::execCanMove },
		{ .NameUTF8 = UTF8TEXT("CanResize"), .Pointer = &UWindow::execCanResize },
		{ .NameUTF8 = UTF8TEXT("GetCanvasSlot"), .Pointer = &UWindow::execGetCanvasSlot },
		{ .NameUTF8 = UTF8TEXT("GetPosition"), .Pointer = &UWindow::execGetPosition },
		{ .NameUTF8 = UTF8TEXT("GetSize"), .Pointer = &UWindow::execGetSize },
		{ .NameUTF8 = UTF8TEXT("GetZOrder"), .Pointer = &UWindow::execGetZOrder },
		{ .NameUTF8 = UTF8TEXT("Init"), .Pointer = &UWindow::execInit },
		{ .NameUTF8 = UTF8TEXT("IsAnchorPoint"), .Pointer = &UWindow::execIsAnchorPoint },
		{ .NameUTF8 = UTF8TEXT("RemoveModule"), .Pointer = &UWindow::execRemoveModule },
		{ .NameUTF8 = UTF8TEXT("SetByParameter"), .Pointer = &UWindow::execSetByParameter },
		{ .NameUTF8 = UTF8TEXT("SetByParameterPosition"), .Pointer = &UWindow::execSetByParameterPosition },
		{ .NameUTF8 = UTF8TEXT("SetByParameterSize"), .Pointer = &UWindow::execSetByParameterSize },
		{ .NameUTF8 = UTF8TEXT("SetCanFullscreen"), .Pointer = &UWindow::execSetCanFullscreen },
		{ .NameUTF8 = UTF8TEXT("SetCanMove"), .Pointer = &UWindow::execSetCanMove },
		{ .NameUTF8 = UTF8TEXT("SetCanResize"), .Pointer = &UWindow::execSetCanResize },
		{ .NameUTF8 = UTF8TEXT("SetPosition"), .Pointer = &UWindow::execSetPosition },
		{ .NameUTF8 = UTF8TEXT("SetPositionX"), .Pointer = &UWindow::execSetPositionX },
		{ .NameUTF8 = UTF8TEXT("SetPositionY"), .Pointer = &UWindow::execSetPositionY },
		{ .NameUTF8 = UTF8TEXT("SetSize"), .Pointer = &UWindow::execSetSize },
		{ .NameUTF8 = UTF8TEXT("SetSizeX"), .Pointer = &UWindow::execSetSizeX },
		{ .NameUTF8 = UTF8TEXT("SetSizeY"), .Pointer = &UWindow::execSetSizeY },
		{ .NameUTF8 = UTF8TEXT("SetWindowCapabilities"), .Pointer = &UWindow::execSetWindowCapabilities },
		{ .NameUTF8 = UTF8TEXT("SetZOrder"), .Pointer = &UWindow::execSetZOrder },
		{ .NameUTF8 = UTF8TEXT("Uninit"), .Pointer = &UWindow::execUninit },
		{ .NameUTF8 = UTF8TEXT("UpdateUIForCapabilities"), .Pointer = &UWindow::execUpdateUIForCapabilities },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UWindow_AddModule, "AddModule" }, // 3584439650
		{ &Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromContent, "ApplyCapabilitiesFromContent" }, // 2875520653
		{ &Z_Construct_UFunction_UWindow_ApplyCapabilitiesFromStruct, "ApplyCapabilitiesFromStruct" }, // 2066120473
		{ &Z_Construct_UFunction_UWindow_BringToFront, "BringToFront" }, // 2379404869
		{ &Z_Construct_UFunction_UWindow_CanFullscreen, "CanFullscreen" }, // 1065799606
		{ &Z_Construct_UFunction_UWindow_CanMove, "CanMove" }, // 773663886
		{ &Z_Construct_UFunction_UWindow_CanResize, "CanResize" }, // 2443550901
		{ &Z_Construct_UDelegateFunction_UWindow_ED_InteractEnd__DelegateSignature, "ED_InteractEnd__DelegateSignature" }, // 2052133379
		{ &Z_Construct_UDelegateFunction_UWindow_ED_InteractStart__DelegateSignature, "ED_InteractStart__DelegateSignature" }, // 2130607333
		{ &Z_Construct_UDelegateFunction_UWindow_ED_PositionChanged__DelegateSignature, "ED_PositionChanged__DelegateSignature" }, // 845455829
		{ &Z_Construct_UDelegateFunction_UWindow_ED_PositionEnd__DelegateSignature, "ED_PositionEnd__DelegateSignature" }, // 1106888172
		{ &Z_Construct_UDelegateFunction_UWindow_ED_PositionStart__DelegateSignature, "ED_PositionStart__DelegateSignature" }, // 1746493592
		{ &Z_Construct_UDelegateFunction_UWindow_ED_SizeChanged__DelegateSignature, "ED_SizeChanged__DelegateSignature" }, // 621733914
		{ &Z_Construct_UDelegateFunction_UWindow_ED_SizeEnd__DelegateSignature, "ED_SizeEnd__DelegateSignature" }, // 2634580753
		{ &Z_Construct_UDelegateFunction_UWindow_ED_SizeStart__DelegateSignature, "ED_SizeStart__DelegateSignature" }, // 3281382979
		{ &Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature, "ED_WindowClicked__DelegateSignature" }, // 3431736968
		{ &Z_Construct_UFunction_UWindow_GetCanvasSlot, "GetCanvasSlot" }, // 3088544937
		{ &Z_Construct_UFunction_UWindow_GetPosition, "GetPosition" }, // 2705603702
		{ &Z_Construct_UFunction_UWindow_GetSize, "GetSize" }, // 3026954935
		{ &Z_Construct_UFunction_UWindow_GetZOrder, "GetZOrder" }, // 1762083856
		{ &Z_Construct_UFunction_UWindow_Init, "Init" }, // 799753732
		{ &Z_Construct_UFunction_UWindow_IsAnchorPoint, "IsAnchorPoint" }, // 4101381259
		{ &Z_Construct_UFunction_UWindow_OnCapabilitiesChanged, "OnCapabilitiesChanged" }, // 3870925771
		{ &Z_Construct_UFunction_UWindow_RemoveModule, "RemoveModule" }, // 507808343
		{ &Z_Construct_UFunction_UWindow_SetByParameter, "SetByParameter" }, // 1278062167
		{ &Z_Construct_UFunction_UWindow_SetByParameterPosition, "SetByParameterPosition" }, // 769237354
		{ &Z_Construct_UFunction_UWindow_SetByParameterSize, "SetByParameterSize" }, // 2118073525
		{ &Z_Construct_UFunction_UWindow_SetCanFullscreen, "SetCanFullscreen" }, // 4232169470
		{ &Z_Construct_UFunction_UWindow_SetCanMove, "SetCanMove" }, // 1817983582
		{ &Z_Construct_UFunction_UWindow_SetCanResize, "SetCanResize" }, // 706102308
		{ &Z_Construct_UFunction_UWindow_SetPosition, "SetPosition" }, // 864881297
		{ &Z_Construct_UFunction_UWindow_SetPositionX, "SetPositionX" }, // 616258176
		{ &Z_Construct_UFunction_UWindow_SetPositionY, "SetPositionY" }, // 1548024486
		{ &Z_Construct_UFunction_UWindow_SetSize, "SetSize" }, // 3939956260
		{ &Z_Construct_UFunction_UWindow_SetSizeX, "SetSizeX" }, // 1354722972
		{ &Z_Construct_UFunction_UWindow_SetSizeY, "SetSizeY" }, // 2188207627
		{ &Z_Construct_UFunction_UWindow_SetWindowCapabilities, "SetWindowCapabilities" }, // 2959328962
		{ &Z_Construct_UFunction_UWindow_SetZOrder, "SetZOrder" }, // 2669024189
		{ &Z_Construct_UFunction_UWindow_Uninit, "Uninit" }, // 252755484
		{ &Z_Construct_UFunction_UWindow_UpdateUIForCapabilities, "UpdateUIForCapabilities" }, // 3463361832
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWindow>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UWindow_Statics

// ********** Begin Class UWindow Property Definitions *********************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWindow_Statics::NewProp_Modules_Inner = { "Modules", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UWindow_Module_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UWindow_Statics::NewProp_Modules = { "Modules", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWindow, Modules), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Modules_MetaData), NewProp_Modules_MetaData) };
void Z_Construct_UClass_UWindow_Statics::NewProp_bCanMove_SetBit(void* Obj)
{
	((UWindow*)Obj)->bCanMove = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UWindow_Statics::NewProp_bCanMove = { "bCanMove", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UWindow), &Z_Construct_UClass_UWindow_Statics::NewProp_bCanMove_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanMove_MetaData), NewProp_bCanMove_MetaData) };
void Z_Construct_UClass_UWindow_Statics::NewProp_bCanResize_SetBit(void* Obj)
{
	((UWindow*)Obj)->bCanResize = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UWindow_Statics::NewProp_bCanResize = { "bCanResize", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UWindow), &Z_Construct_UClass_UWindow_Statics::NewProp_bCanResize_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanResize_MetaData), NewProp_bCanResize_MetaData) };
void Z_Construct_UClass_UWindow_Statics::NewProp_bCanFullscreen_SetBit(void* Obj)
{
	((UWindow*)Obj)->bCanFullscreen = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UWindow_Statics::NewProp_bCanFullscreen = { "bCanFullscreen", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UWindow), &Z_Construct_UClass_UWindow_Statics::NewProp_bCanFullscreen_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanFullscreen_MetaData), NewProp_bCanFullscreen_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWindow_Statics::NewProp_Window_Timer_Event = { "Window_Timer_Event", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWindow, Window_Timer_Event), Z_Construct_UClass_UWindow_Timer_Event_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Window_Timer_Event_MetaData), NewProp_Window_Timer_Event_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWindow_Statics::NewProp_CanvasSlot = { "CanvasSlot", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWindow, CanvasSlot), Z_Construct_UClass_UCanvasPanelSlot_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CanvasSlot_MetaData), NewProp_CanvasSlot_MetaData) };
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UWindow_Statics::NewProp_ED_WindowClicked = { "ED_WindowClicked", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWindow, ED_WindowClicked), Z_Construct_UDelegateFunction_UWindow_ED_WindowClicked__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ED_WindowClicked_MetaData), NewProp_ED_WindowClicked_MetaData) }; // 3431736968
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UWindow_Statics::NewProp_ED_InteractStart = { "ED_InteractStart", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWindow, ED_InteractStart), Z_Construct_UDelegateFunction_UWindow_ED_InteractStart__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ED_InteractStart_MetaData), NewProp_ED_InteractStart_MetaData) }; // 2130607333
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UWindow_Statics::NewProp_ED_InteractEnd = { "ED_InteractEnd", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWindow, ED_InteractEnd), Z_Construct_UDelegateFunction_UWindow_ED_InteractEnd__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ED_InteractEnd_MetaData), NewProp_ED_InteractEnd_MetaData) }; // 2052133379
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UWindow_Statics::NewProp_ED_PositionStart = { "ED_PositionStart", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWindow, ED_PositionStart), Z_Construct_UDelegateFunction_UWindow_ED_PositionStart__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ED_PositionStart_MetaData), NewProp_ED_PositionStart_MetaData) }; // 1746493592
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UWindow_Statics::NewProp_ED_PositionChanged = { "ED_PositionChanged", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWindow, ED_PositionChanged), Z_Construct_UDelegateFunction_UWindow_ED_PositionChanged__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ED_PositionChanged_MetaData), NewProp_ED_PositionChanged_MetaData) }; // 845455829
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UWindow_Statics::NewProp_ED_PositionEnd = { "ED_PositionEnd", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWindow, ED_PositionEnd), Z_Construct_UDelegateFunction_UWindow_ED_PositionEnd__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ED_PositionEnd_MetaData), NewProp_ED_PositionEnd_MetaData) }; // 1106888172
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UWindow_Statics::NewProp_ED_SizeStart = { "ED_SizeStart", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWindow, ED_SizeStart), Z_Construct_UDelegateFunction_UWindow_ED_SizeStart__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ED_SizeStart_MetaData), NewProp_ED_SizeStart_MetaData) }; // 3281382979
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UWindow_Statics::NewProp_ED_SizeChanged = { "ED_SizeChanged", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWindow, ED_SizeChanged), Z_Construct_UDelegateFunction_UWindow_ED_SizeChanged__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ED_SizeChanged_MetaData), NewProp_ED_SizeChanged_MetaData) }; // 621733914
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UWindow_Statics::NewProp_ED_SizeEnd = { "ED_SizeEnd", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWindow, ED_SizeEnd), Z_Construct_UDelegateFunction_UWindow_ED_SizeEnd__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ED_SizeEnd_MetaData), NewProp_ED_SizeEnd_MetaData) }; // 2634580753
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UWindow_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Statics::NewProp_Modules_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Statics::NewProp_Modules,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Statics::NewProp_bCanMove,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Statics::NewProp_bCanResize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Statics::NewProp_bCanFullscreen,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Statics::NewProp_Window_Timer_Event,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Statics::NewProp_CanvasSlot,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Statics::NewProp_ED_WindowClicked,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Statics::NewProp_ED_InteractStart,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Statics::NewProp_ED_InteractEnd,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Statics::NewProp_ED_PositionStart,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Statics::NewProp_ED_PositionChanged,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Statics::NewProp_ED_PositionEnd,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Statics::NewProp_ED_SizeStart,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Statics::NewProp_ED_SizeChanged,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWindow_Statics::NewProp_ED_SizeEnd,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UWindow_Statics::PropPointers) < 2048);
// ********** End Class UWindow Property Definitions ***********************************************
UObject* (*const Z_Construct_UClass_UWindow_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_NX_ModularWindow,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UWindow_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UWindow_Statics::ClassParams = {
	&UWindow::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UWindow_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UWindow_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UWindow_Statics::Class_MetaDataParams), Z_Construct_UClass_UWindow_Statics::Class_MetaDataParams)
};
void UWindow::StaticRegisterNativesUWindow()
{
	UClass* Class = UWindow::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UWindow_Statics::Funcs));
}
UClass* Z_Construct_UClass_UWindow()
{
	if (!Z_Registration_Info_UClass_UWindow.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWindow.OuterSingleton, Z_Construct_UClass_UWindow_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UWindow.OuterSingleton;
}
UWindow::UWindow(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UWindow);
UWindow::~UWindow() {}
// ********** End Class UWindow ********************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h__Script_NX_ModularWindow_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UWindow, UWindow::StaticClass, TEXT("UWindow"), &Z_Registration_Info_UClass_UWindow, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWindow), 1981169440U) },
	};
}; // Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h__Script_NX_ModularWindow_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h__Script_NX_ModularWindow_3568844695{
	TEXT("/Script/NX_ModularWindow"),
	Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h__Script_NX_ModularWindow_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Outercorp_Plugins_Modularwcdb682fc546fV1_Source_NX_ModularWindow_Public_Window_h__Script_NX_ModularWindow_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
