// Copyright Nyaunix 2025 All Rights Reserved

#pragma once


#include <vector>
#include <algorithm>



// Work like Delegate/EventDispatcher.
// takes a value, 
// it is processed by all who have subscribed, 
// and returns the processed value.
// 
// For window:
//typedef UModule_Limit_None TC0;
//typedef TArray<float> TV0;
//typedef TArray<FName> TV1;
//typedef UWidget* TV2;
//typedef TV0(TC0::* TF0)(TV2, TV1, TV0);
template<typename TC0, typename TF0, typename TV0, typename TV1, typename TV2 >
struct NX_Delegate_MW {
	std::vector<TF0> Functions;
	std::vector<TC0*> Modules;

	void Event_Bind_Add(TC0* Module, TF0 InFunction) {
		Functions.push_back(InFunction);
		Modules.push_back(Module);
	};
	void Event_Bind_Remove(TC0* Module, TF0 InFunction) {
		Functions.erase(std::remove(Functions.begin(), Functions.end(), InFunction), Functions.end());
		Modules.erase(std::remove(Modules.begin(), Modules.end(), Module), Modules.end());
	};
	void Event_Bind_Clear() {
		Functions.clear();
		Modules.clear();
	}
	TV0 Event_Call_IO(TV2 InValue_0, TV1 InValue_1, TV0 InValue_2) {
		int i = 0;
		for (TF0 LFunction : Functions) {
			InValue_2 = (Modules[i]->*LFunction)(InValue_0, InValue_1, InValue_2);
			i++;
		}
		return InValue_2;
	};
};
