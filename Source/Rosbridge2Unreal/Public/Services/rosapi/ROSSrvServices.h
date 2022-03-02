#pragma once

#include "CoreMinimal.h"
#include "Services/ROSServiceBase.h"
#include "ROSSrvServices.generated.h"

/**
 * 
 */
UCLASS()
class ROSBRIDGE2UNREAL_API UROSSrvServices : public UROSServiceBase
{
public:
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetServiceType() override {return "rosapi/Services";}
	UFUNCTION(BlueprintCallable) static UROSSrvServices* CreateEmpty();

	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FString> Services;
	
private:
	/* Transformation Functions */
	/* Request */
	void RequestToData(ROSData& OutMessage) const override;
	bool RequestFromData(const ROSData& Message) override;

	/* Response */
	void ResponseToData(ROSData& OutMessage) const override;
	bool ResponseFromData(const ROSData& Message) override;
};
