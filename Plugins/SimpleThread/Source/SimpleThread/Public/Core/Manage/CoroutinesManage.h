// Copyright (C) RenZhai.2019.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Interface/ThreadManageInterface.h"
#include "Core/Manage/ManageBase/ThreadManageBase.h"

//After bind, you can set the time and how long to execute            
//Create returns a handle after binding, and the programmer decides when to execute it 
struct SIMPLETHREAD_API FCoroutinesManage :FTreadTemplateBase<ICoroutinesContainer, FCoroutinesHandle>
{
public:
	FCoroutinesManage();

	//Used to monitor task status 
	void Tick(float );
public:
	template<class UserClass, typename... VarTypes>
	void BindRaw(float InTotalTime, UserClass *TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		*this << InTotalTime << FSimpleDelegate::CreateRaw(TargetClass, InMethod, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	void BindUObject(float InTotalTime, UserClass *TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		*this << InTotalTime << FSimpleDelegate::CreateUObject(TargetClass, InMethod, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	void BindSP(float InTotalTime, const TSharedRef<UserClass>& TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		*this << InTotalTime << FSimpleDelegate::CreateSP(TargetClass, InMethod, Vars...);
	}

	template<typename FunctorType, typename... VarTypes>
	void BindLambda(float InTotalTime, FunctorType &&InMethod, VarTypes... Vars)
	{
		*this << InTotalTime << FSimpleDelegate::CreateLambda(InMethod, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	void BindUFunction(float InTotalTime, UserClass *TargetClass, const FName& InMethod, VarTypes... Vars)
	{
		*this << InTotalTime << FSimpleDelegate::CreateUFunction(TargetClass, InMethod, Vars...);
	}
	/////////////////////////////////////

	template<typename FunctorType, typename... VarTypes>
	void BindOnGoingLambda(float InTotalTime, FunctorType&& InMethod, VarTypes... Vars)
	{
		*this << InTotalTime << FSimpleOnGoingDelegate::CreateLambda(InMethod, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	void BindOnGoingRaw(float InTotalTime, UserClass* TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		*this << InTotalTime << FSimpleOnGoingDelegate::CreateRaw(TargetClass, InMethod, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	void BindOnGoingUObject(float InTotalTime, UserClass* TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		*this << InTotalTime << FSimpleOnGoingDelegate::CreateUObject(TargetClass, InMethod, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	void BindOnGoingSP(float InTotalTime, const TSharedRef<UserClass>& TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		*this << InTotalTime << FSimpleOnGoingDelegate::CreateSP(TargetClass, InMethod, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	void BindOnGoingUFunction(float InTotalTime, UserClass* TargetClass, const FName& InMethod, VarTypes... Vars)
	{
		*this << InTotalTime << FSimpleOnGoingDelegate::CreateUFunction(TargetClass, InMethod, Vars...);
	}
};