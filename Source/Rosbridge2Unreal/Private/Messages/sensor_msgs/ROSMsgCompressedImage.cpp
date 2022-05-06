#include "Messages/sensor_msgs/ROSMsgCompressedImage.h"
#include "DataHelpers.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Engine/TextureRenderTarget2D.h"
#include "LogCategory.h"

UROSMsgCompressedImage::UROSMsgCompressedImage()
{
    ImageWrapperModule = &FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
}

UROSMsgCompressedImage* UROSMsgCompressedImage::CreateEmpty()
{
	UROSMsgCompressedImage* Message = NewObject<UROSMsgCompressedImage>();
	Message->Header = NewObject<UROSMsgHeader>(Message);
	Message->Data = TArray<uint8>();
	return Message;
}

UTexture2D* UROSMsgCompressedImage::CreateFittingTexture()
{
	if (!ImageWrapper.IsValid()) ReadCompressedData();
	
	UTexture2D* Texture = nullptr;
	
	const EPixelFormat PixelFormat = GetPixelFormatFromRGBFormat(OutputFormat);
	if(PixelFormat != PF_Unknown)
	{
	    Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PixelFormat);
	    Texture->UpdateResource();
	}

	return Texture;
}

void UROSMsgCompressedImage::CopyToTexture(UTexture2D* Texture)
{
	ReadCompressedData();
	
	FUpdateTextureRegion2D* TextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, ImageWrapper->GetWidth(), ImageWrapper->GetHeight());
	TArray64<uint8>* Copy = new TArray64<uint8>(); /* Copy due to Async update needed */
	Copy->Reserve(ImageWrapper->GetHeight()*ImageWrapper->GetWidth()*OutputChannelNumber);
	if (ImageWrapper->GetRaw(OutputFormat, OutputBitDepth, *Copy))
	{
		Texture->UpdateTextureRegions(0, 1, TextureRegion, ImageWrapper->GetWidth()*OutputChannelNumber, OutputChannelNumber*OutputBitDepth, Copy->GetData(), [](auto InTextureData, auto InRegions)
		{
			delete InTextureData;
			delete InRegions;
		});
	}
	else
	{
		delete TextureRegion;
		delete Copy;
	}
}

void UROSMsgCompressedImage::CopyFromRenderTarget(UTextureRenderTarget2D* TextureRenderTarget)
{
    if (!ImageWrapper.IsValid())
	{
		ImageWrapper = ImageWrapperModule->CreateImageWrapper((Format == EROSImageCompressionFormat::JPEG)?EImageFormat::JPEG:EImageFormat::PNG);
	}

	FRenderTarget* RenderTarget = TextureRenderTarget->GameThread_GetRenderTargetResource();
	const FIntPoint TextureSize = RenderTarget->GetSizeXY();

	TArray<FColor> PixelData;
    RenderTarget->ReadPixels(PixelData);
    ImageWrapper->SetRaw(PixelData.GetData(), PixelData.Num()*4, TextureSize.X, TextureSize.Y, ERGBFormat::BGRA, 8);
}

FVector2D UROSMsgCompressedImage::GetDimensions() const
{
	if (!ImageWrapper.IsValid())	return FVector2D(-1,-1);
	return FVector2D(ImageWrapper->GetWidth(), ImageWrapper->GetHeight());
}

void UROSMsgCompressedImage::ReadCompressedData()
{
	if (!ImageWrapper.IsValid())
	{
		const EImageFormat DetectedFormat = ImageWrapperModule->DetectImageFormat(Data.GetData(), Data.Num());
		if (DetectedFormat == EImageFormat::Invalid){
			UE_LOG(LogROSBridge, Warning, TEXT("ROSMsgCompressedImage error parsing image data"));
		    return;
	    }
		ImageWrapper = ImageWrapperModule->CreateImageWrapper(DetectedFormat);
	}
}

void UROSMsgCompressedImage::ToData(ROSData& OutMessage) const
{
	DataHelpers::Append<UROSMsgHeader*>(OutMessage, "header", Header);
	DataHelpers::Append<FString>(OutMessage, "format", (Format == EROSImageCompressionFormat::JPEG)?"jpeg":"png");

	if (ImageWrapper.IsValid())
	{
		const TArray64<uint8> CompressedData = ImageWrapper->GetCompressed(OutputCompressionQuality);
		DataHelpers::Append(OutMessage, "data", CompressedData);
	}
}

bool UROSMsgCompressedImage::FromData(const ROSData& Message)
{
	FString FormatInMessage;
	bool FormatRead = DataHelpers::Extract<FString>(Message, "format", FormatInMessage);
	if(FormatRead && FormatInMessage.Compare("PNG", ESearchCase::IgnoreCase))
	{
	    Format = EROSImageCompressionFormat::PNG;
	}
    else if(FormatRead && FormatInMessage.Compare("JPEG", ESearchCase::IgnoreCase))
	{
	    Format = EROSImageCompressionFormat::JPEG;
	}
    else
	{
	    FormatRead = false;
	}

	return FormatRead &&
		DataHelpers::Extract<UROSMsgHeader*>(Message, "header", Header) &&
		DataHelpers::ExtractBinary(Message, "data", Data);
}

EPixelFormat UROSMsgCompressedImage::GetPixelFormatFromRGBFormat(const ERGBFormat InFormat)
{
    switch(InFormat)
	{
	case ERGBFormat::RGBA:
		return PF_R8G8B8A8;
	case ERGBFormat::BGRA:
		return PF_B8G8R8A8;
	case ERGBFormat::Gray:
		return PF_G8;
	default:
		UE_LOG(LogROSBridge, Warning, TEXT("ROSMsgCompressedImage found unsupported output format."));
		return PF_Unknown;
	}
}
