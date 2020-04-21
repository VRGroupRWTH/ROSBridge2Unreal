#pragma once

#include "Base64.h"
#include "CoreMinimal.h"
#include "TypeDefinitions.h"

namespace DataHelpers {

/* Misc Types */

	template<typename JSONType, typename UnrealType> bool ExtractInternal(const ROSData& Message, const char* Key, UnrealType& OutData){
		if(Key[0] == '\0' && Message.is<JSONType>())
		{
			OutData = static_cast<UnrealType>(Message.as<JSONType>());
			return true;
		}

		if(Message.contains(Key) && Message[Key].is<JSONType>())
		{
			OutData = static_cast<UnrealType>(Message[Key].as<JSONType>());
			return true;
		}
		return false;
	}
	
	static void AppendSubDocument(ROSData& Message, const char* Key, const ROSData& SubDocument)
	{
		Message.insert_or_assign(Key, SubDocument);
	}

	static bool ExtractSubDocument(const ROSData& Message, const char* Key, ROSData& OutDocument)
	{
		if(Message.contains(Key)){
			OutDocument = Message.at(Key);
			return true;
		}
		return false;
	}

	/* Just for debugging purposes */
	static FString InternalToString(const ROSData& Document)
	{
		std::string S;
		Document.dump(S, jsoncons::indenting::indent);
		return UTF8_TO_TCHAR(S.c_str());
	}
	
	static bool ExtractString(const ROSData& Message, const char* Key, FString& OutData)
	{
		if(Key[0] == '\0' && Message.is_string())
		{
			OutData = UTF8_TO_TCHAR(Message.as_cstring());
			return true;
		}
		
		if(Message.contains(Key) && Message[Key].is_string())
		{
			OutData = UTF8_TO_TCHAR(Message[Key].as_cstring());
			return true;
		}
		return false;
	}

	static void AppendString(ROSData& Message, const char* Key, const FString& InData)
	{
		Message.insert_or_assign(Key, TCHAR_TO_UTF8(*InData));
	}

	static bool ExtractBool(const ROSData& Message, const char* Key, bool& OutData)
	{
		if(Key[0] == '\0' && Message.is_bool())
		{
			OutData = static_cast<float>(Message.as_bool());
			return true;
		}
		
		if(Message.contains(Key) && Message[Key].is_bool())
		{
			OutData = Message[Key].as_bool();
			return true;
		}
		return false;
	}

	static void AppendBool(ROSData& Message, const char* Key, const bool& InData)
	{
		Message.insert_or_assign(Key, InData);
	}

	/* Does not compute the outputted data length (better performance) */
	static bool ExtractBinary(const ROSData& Message, const char* Key, uint8* OutData)
	{
		FString EncodedData;
		const bool KeyFound = ExtractString(Message, Key, EncodedData);
		if(KeyFound) FBase64::Decode(*EncodedData, EncodedData.Len(), OutData);
		return KeyFound;
	}

	/* Computes the outputted data length as well (less performance) */
	static bool ExtractBinary(const ROSData& Message, const char* Key, uint8* OutData, uint32& OutDataLength)
	{
		FString EncodedData;
		const bool KeyFound = ExtractString(Message, Key, EncodedData);
		if(!KeyFound) return false;
		
		FBase64::Decode(*EncodedData, EncodedData.Len(), OutData);
		OutDataLength = FBase64::GetDecodedDataSize(EncodedData);
		return true;
	}

	static void AppendBinary(ROSData& Message, const char* Key, const uint8* InData, const uint32 DataLength)
	{
		const FString EncodedData = FBase64::Encode(InData, DataLength);
		AppendString(Message, Key, EncodedData);
	}

/* All Number Types */

	/* Floating Point */

		static bool ExtractDouble(const ROSData& Message, const char* Key, double& OutData)
		{
			return ExtractInternal<double, double>(Message, Key, OutData);
		}

		static void AppendDouble(ROSData& Message, const char* Key, const double& InData)
		{
			Message.insert_or_assign(Key, InData);
		}

		/* Floats are internally encoded as double by ROSBridge */
		static bool ExtractFloat(const ROSData& Message, const char* Key, float& OutData)
		{
			if(Key[0] == '\0' && Message.is_double())
			{
				OutData = static_cast<float>(Message.as_double());
				return true;
			}
		
			if(Message.contains(Key) && Message[Key].is_double())
			{
				OutData = static_cast<float>(Message[Key].as_double());
				return true;
			}
			return false;
		}
	
		/* Floats are internally encoded as double by ROSBridge */
		static void AppendFloat(ROSData& Message, const char* Key, const float& InData)
		{
			Message.insert_or_assign(Key, static_cast<double>(InData));
		}

	/* Signed Integer */
	
		static bool ExtractInt64(const ROSData& Message, const char* Key, int64& OutData)
		{
			return ExtractInternal<int64_t, int64>(Message, Key, OutData);
		}

		static void AppendInt64(ROSData& Message, const char* Key, const int64& InData)
		{
			Message.insert_or_assign(Key, static_cast<int64_t>(InData));
		}

		static bool ExtractInt32(const ROSData& Message, const char* Key, int32& OutData)
		{
			return ExtractInternal<int32_t, int32>(Message, Key, OutData);
		}

		static void AppendInt32(ROSData& Message, const char* Key, const int32& InData)
		{
			Message.insert_or_assign(Key, static_cast<int32_t>(InData));
		}

		static bool ExtractInt16(const ROSData& Message, const char* Key, int16& OutData)
		{
			//ROSBridge encodes smaller types in known bigger types. int16 -> int32
			return ExtractInternal<int32_t, int16>(Message, Key, OutData);
		}

		static void AppendInt16(ROSData& Message, const char* Key, const int16& InData)
		{
			//ROSBridge encodes smaller types in known bigger types. int16 -> int32
			Message.insert_or_assign(Key, static_cast<int32_t>(InData));
		}

		static bool ExtractInt8(const ROSData& Message, const char* Key, int8& OutData)
		{
			//ROSBridge encodes smaller types in known bigger types. int8 -> uint32
			return ExtractInternal<int32_t, int8>(Message, Key, OutData);
		}

		static void AppendInt8(ROSData& Message, const char* Key, const int8& InData)
		{
			//ROSBridge encodes smaller types in known bigger types. int8 -> int32
			Message.insert_or_assign(Key, static_cast<int32_t>(InData));
		}

	/* Unsigned Integer */

		static bool ExtractUInt64(const ROSData& Message, const char* Key, uint64& OutData)
		{
			return ExtractInternal<uint64_t, uint64>(Message, Key, OutData);
		}

		static void AppendUInt64(ROSData& Message, const char* Key, const uint64& InData)
		{
			Message.insert_or_assign(Key, static_cast<uint64_t>(InData));
		}

		static bool ExtractUInt32(const ROSData& Message, const char* Key, uint32& OutData)
		{	
			return ExtractInternal<uint32_t, uint32>(Message, Key, OutData);
		}

		static void AppendUInt32(ROSData& Message, const char* Key, const uint32& InData)
		{
			Message.insert_or_assign(Key, static_cast<uint32_t>(InData));
		}

		static bool ExtractUInt16(const ROSData& Message, const char* Key, uint16& OutData)
		{
			//ROSBridge encodes smaller types in known bigger types. uint8 -> uint32
			return ExtractInternal<uint32_t, uint16>(Message, Key, OutData);
		}

		static void AppendUInt16(ROSData& Message, const char* Key, const uint16& InData)
		{
			//ROSBridge encodes smaller types in known bigger types. uint16 -> uint32
			Message.insert_or_assign(Key, static_cast<uint32_t>(InData));
		}

		static bool ExtractUInt8(const ROSData& Message, const char* Key, uint8& OutData)
		{
			//ROSBridge encodes smaller types in known bigger types. uint8 -> uint32
			return ExtractInternal<uint32_t, uint8>(Message, Key, OutData);
		}

		static void AppendUInt8(ROSData& Message, const char* Key, const uint8& InData)
		{
			//ROSBridge encodes smaller types in known bigger types. uint8 -> uint32
			Message.insert_or_assign(Key, static_cast<uint32_t>(InData));
		}
	
/* Arrays */

	template<class T>
	static bool ExtractTArray(const ROSData& Message, const char* Key, TArray<T> Array, const TFunction<bool(const ROSData&, const char*, T&)>& ExtractElement)
	{
		if(!Message.contains(Key) || !Message[Key].is_array()) return false;

		for (uint64 i = 0; i < Message[Key].size(); i++) {
			T Temp;
			if (ExtractElement(Message[Key][i], "", Temp)){
				Array.Add(Temp);
			} else {
				return false;
			}
		}
		return true;
	}
	
	template<class T>
	static void AppendTArray(ROSData& Message, const char* Key, const TArray<T>& Array, const TFunction<void(ROSData&, const char*, T&)>& AppendElement)
	{
		ROSData ArrayNode = ROSData(jsoncons::json_array_arg);
		
		for(int i = 0; i < Array.Num(); i++)
		{
			ROSData NewValue;
			AppendElement(NewValue, "", Array[i]);
			ArrayNode.push_back(NewValue);
		}

		Message.insert_or_assign(Key, ArrayNode);
	}
}