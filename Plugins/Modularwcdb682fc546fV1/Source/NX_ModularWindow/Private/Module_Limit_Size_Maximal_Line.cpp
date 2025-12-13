// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Limit_Size_Maximal_Line.h"


TArray<float> UModule_Limit_Size_Maximal_Line::ModifyByParameter(UWidget* InWindow, TArray<FName> InNames, TArray<float> InValues)
{
    TArray<float> ToReturn;
    for (float CurrentValue : InValues) {
        ToReturn.Add(FMath::Max(CurrentValue, 0));
    }
    return ToReturn;
}
