#pragma once

#include "CoreMinimal.h"
#include "IImageWrapper.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/std_msgs/ROSMsgHeader.h"
#include "ROSMsgCompressedImage.generated.h"

/*
 * This message contains a compressed image. It supports a few different image compressions (PNG, JPEG, BMP, ICO, EXR, ICNS)
 */

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgCompressedImage : public UROSMessageBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSMsgCompressedImage(){};
	UFUNCTION() FString GetMessageType() override {return "sensor_msgs/CompressedImage";}
	UFUNCTION(BlueprintCallable) static UROSMsgCompressedImage* CreateEmpty();

	/* convenience method. */
	UFUNCTION(BlueprintCallable) UTexture2D* CreateFittingTexture();
	UFUNCTION(BlueprintCallable) void CopyToTexture(UTexture2D* Texture);
	UFUNCTION(BlueprintPure) FVector2D GetDimensions();
	void ReadData();

	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgHeader* Header;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Format = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<uint8> Data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, ClampMax=100)) int OutputCompressionQuality = 100;
	
	/* Transformation Functions */
	void ToData(ROSData& Message) const override;
	bool FromData(const ROSData& Message) override;

private:
	TSharedPtr<IImageWrapper> ImageWrapper;
	ERGBFormat OutputFormat = ERGBFormat::RGBA;
	int32 OutputChannelNumber = 4;
	int32 OutputBitDepth = 8;
};
