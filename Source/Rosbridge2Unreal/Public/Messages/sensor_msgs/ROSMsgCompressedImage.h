#pragma once

#include "CoreMinimal.h"
#include "IImageWrapperModule.h"
#include "Messages/ROSMessageBase.h"
#include "Messages/std_msgs/ROSMsgHeader.h"
#include "ROSMsgCompressedImage.generated.h"

// Allowed compression types for sensor_msgs/CompressedImage
UENUM()
enum class EROSImageCompressionFormat { JPEG, PNG };

/*
 * This message contains a compressed image. It supports a few different image compressions (PNG, JPEG, BMP, ICO, EXR, ICNS)
 */

UCLASS(BlueprintType)
class ROSBRIDGE2UNREAL_API UROSMsgCompressedImage : public UROSMessageBase
{
	GENERATED_BODY()

public:
	/* Construction */
	UROSMsgCompressedImage();
	UFUNCTION() FString GetMessageType() override {return "sensor_msgs/CompressedImage";}
	UFUNCTION(BlueprintCallable) static UROSMsgCompressedImage* CreateEmpty();

	/* convenience method. */
	UFUNCTION(BlueprintCallable) UTexture2D* CreateFittingTexture();
	UFUNCTION(BlueprintCallable) void CopyToTexture(UTexture2D* Texture);
	UFUNCTION(BlueprintCallable) void CopyFromRenderTarget(UTextureRenderTarget2D* TextureRenderTarget);
	UFUNCTION(BlueprintPure) FVector2D GetDimensions() const;

	/* Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgHeader* Header;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EROSImageCompressionFormat Format = EROSImageCompressionFormat::JPEG;
	TArray64<uint8> Data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, ClampMax=100)) int OutputCompressionQuality = 100;
	
	/* Transformation Functions */
	void ToData(ROSData& OutMessage) const override;
	bool FromData(const ROSData& Message) override;
	UFUNCTION(BlueprintCallable) void ReadCompressedData();

private:
	TSharedPtr<IImageWrapper> ImageWrapper;
	ERGBFormat OutputFormat = ERGBFormat::RGBA;
	int32 OutputChannelNumber = 4;
	int32 OutputBitDepth = 8;
	IImageWrapperModule* ImageWrapperModule;
	
	static EPixelFormat GetPixelFormatFromRGBFormat(const ERGBFormat InFormat);
};
