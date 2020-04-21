// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TypeDefinitions.h"
#include "ROSServiceBase.h"

#include "ROSSrvAddTwoInts.generated.h"


UCLASS(Blueprintable)
class ROSBRIDGE2UNREAL_API UROSSrvAddTwoInts : public UROSServiceBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSSrvAddTwoInts(){};
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetServiceType() override {return "beginner_tutorials/AddTwoInts";};
	UFUNCTION(BlueprintCallable, BlueprintPure) static UROSSrvAddTwoInts* CreateRequest(int64 A, int64 B);
	
	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int64 A;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int64 B;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int64 Sum;
	UFUNCTION(BlueprintCallable, BlueprintPure) int SumAsInt() const;

private:
	/* Transformation Functions */
	/* Request */
	void RequestToData(ROSData& Message) const override;
	bool RequestFromData(const ROSData& Message) override;

	/* Response */
	void ResponseToData(ROSData& Message) const override;
	bool ResponseFromData(const ROSData& Message) override;
};

