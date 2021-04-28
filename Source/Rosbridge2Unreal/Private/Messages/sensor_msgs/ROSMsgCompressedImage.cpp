#include "Messages/sensor_msgs/ROSMsgCompressedImage.h"
#include "DataHelpers.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "LogCategory.h"

UROSMsgCompressedImage* UROSMsgCompressedImage::CreateEmpty()
{
	UROSMsgCompressedImage* Message = NewObject<UROSMsgCompressedImage>();
	Message->Header = NewObject<UROSMsgHeader>(Message);
	Message->Data = TArray<uint8>();
	return Message;
}

UTexture2D* UROSMsgCompressedImage::CreateFittingTexture()
{
	if(!ImageWrapper.IsValid()) ReadData();
	
	UTexture2D* Texture = nullptr;

	switch(OutputFormat)
	{
	case ERGBFormat::RGBA:
		Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_R8G8B8A8);
		break;
	case ERGBFormat::BGRA:
		Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
		break;
	case ERGBFormat::Gray:
		Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_G8);
		break;
	default:
		UE_LOG(LogROSBridge, Warning, TEXT("ROSMsgCompressedImage found unsupported output format."));
	}
	
	Texture->UpdateResource();
	return Texture;
}

void UROSMsgCompressedImage::CopyToTexture(UTexture2D* Texture)
{
	ReadData();
	
	FUpdateTextureRegion2D* TextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, ImageWrapper->GetWidth(), ImageWrapper->GetHeight());
	TArray64<uint8>* Copy = new TArray64<uint8>(); /* Copy due to Async update needed */
	Copy->Reserve(ImageWrapper->GetHeight()*ImageWrapper->GetWidth()*OutputChannelNumber);
	if(ImageWrapper->GetRaw(OutputFormat, OutputBitDepth, *Copy))
	{
		Texture->UpdateTextureRegions(0, 1, TextureRegion, ImageWrapper->GetWidth()*OutputChannelNumber, OutputChannelNumber*OutputBitDepth, Copy->GetData(), [](auto InTextureData, auto InRegions)
		{
			delete InTextureData;
			delete InRegions;
		});
	}else
	{
		delete TextureRegion;
		delete Copy;
	}
}

FVector2D UROSMsgCompressedImage::GetDimensions()
{
	if(!ImageWrapper.IsValid())	return FVector2D(-1,-1);
	return FVector2D(ImageWrapper->GetWidth(), ImageWrapper->GetHeight());
}

void UROSMsgCompressedImage::ReadData()
{
	if(!ImageWrapper.IsValid()){
		IImageWrapperModule& Module = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		const EImageFormat DetectedFormat = Module.DetectImageFormat(Data.GetData(), Data.Num());
		if(DetectedFormat != EImageFormat::Invalid)
		{
			ImageWrapper = Module.CreateImageWrapper(DetectedFormat);
		}else
		{
			UE_LOG(LogROSBridge, Warning, TEXT("ROSMsgCompressedImage with unknown format '%s' found."), *Format);
			return;
		}
	}
	
	ImageWrapper->SetCompressed(Data.GetData(), Data.Num());
}

void UROSMsgCompressedImage::ToData(ROSData& OutMessage) const
{
	DataHelpers::AppendSubMessage(OutMessage, "header", Header);
	DataHelpers::AppendString(OutMessage, "format", Format);

	if(ImageWrapper.IsValid()){
		TArray64<uint8> CompressedData = ImageWrapper->GetCompressed(OutputCompressionQuality);
		DataHelpers::AppendBinary(OutMessage, "data", CompressedData.GetData(), CompressedData.Num());
	}
}

bool UROSMsgCompressedImage::FromData(const ROSData& Message)
{
	return
		DataHelpers::ExtractSubMessage(Message, "header", Header) &&
		DataHelpers::ExtractString(Message, "format", Format) &&
		DataHelpers::ExtractBinary(Message, "data", Data);
}
