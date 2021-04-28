// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Services/ROSServiceBase.h"
#include "ROSSrvTopics.generated.h"

/**
 * 
 */
UCLASS()
class ROSBRIDGE2UNREAL_API UROSSrvTopics : public UROSServiceBase
{
public:
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetServiceType() override {return "rosapi/Topics";}
	UFUNCTION(BlueprintCallable) static UROSSrvTopics* CreateEmpty();

	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FString> Topics;
	
private:
	/* Transformation Functions */
	/* Request */
	void RequestToData(ROSData& OutMessage) const override;
	bool RequestFromData(const ROSData& Message) override;

	/* Response */
	void ResponseToData(ROSData& OutMessage) const override;
	bool ResponseFromData(const ROSData& Message) override;
};
