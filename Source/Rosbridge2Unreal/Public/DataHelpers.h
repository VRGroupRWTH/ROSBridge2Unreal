#pragma once

#include "Base64.h"
#include "CoreMinimal.h"
#include "TypeDefinitions.h"

namespace DataHelpers {

/* Misc Types */

	template<typename JSONType, typename UnrealType>
	bool ExtractInternal(const ROSData& Message, const char* Key, UnrealType& OutData){
		if(Key[0] == '/')
		{
			ROSData Element = jsoncons::jsonpointer::get(Message, Key);
			if(!Element.is<JSONType>()) return false;
			OutData = static_cast<UnrealType>(Element.as<JSONType>());
			return true;
		}

		if(Message.contains(Key) && Message[Key].is<JSONType>())
		{
			OutData = static_cast<UnrealType>(Message[Key].as<JSONType>());
			return true;
		}
		return false;
	}

	template<typename JSONType, typename UnrealType>
	bool AppendInternal(ROSData& Message, const char* Key, const UnrealType& InData){
		if(Key[0] == '/')
		{
			jsoncons::jsonpointer::insert_or_assign(Message, Key, ROSData(static_cast<JSONType>(InData)));
		}
		else
		{
			Message.insert_or_assign(Key,static_cast<JSONType>(InData));
		}
		return false;
	}
	
	static void AppendSubDocument(ROSData& Message, const char* Key, const ROSData& SubDocument)
	{
		if(Key[0] == '/')
		{
			jsoncons::jsonpointer::insert_or_assign(Message, Key, SubDocument);
		}
		else
		{
			Message.insert_or_assign(Key,SubDocument);
		}
	}

	static bool ExtractSubDocument(const ROSData& Message, const char* Key, ROSData& OutDocument)
	{
		if(Key[0] == '/')
		{
			OutDocument = jsoncons::jsonpointer::get(Message, Key);
			return true;
		}
		else if(Message.contains(Key))
		{
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
		if(Key[0] == '/')
		{
			const ROSData Element = jsoncons::jsonpointer::get(Message, Key);
			if(!Element.is_string()) return false;
			OutData = UTF8_TO_TCHAR(Element.as_cstring());
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
		if(Key[0] == '/')
		{
			jsoncons::jsonpointer::insert_or_assign(Message, Key, ROSData());
		}
		else
		{
			Message.insert_or_assign(Key,TCHAR_TO_UTF8(*InData));
		}
	}

	static bool ExtractBool(const ROSData& Message, const char* Key, bool& OutData)
	{
		return ExtractInternal<bool, bool>(Message, Key, OutData);
	}

	static void AppendBool(ROSData& Message, const char* Key, const bool& InData)
	{
		AppendInternal<bool>(Message, Key, InData);
	}

	/* Does not compute the outputted data length (better performance) */
	static bool ExtractBinary(const ROSData& Message, const char* Key, uint8*& OutData, uint32& OutDataLength)
	{
		if(Key[0] == '/')
		{
			const ROSData Element = jsoncons::jsonpointer::get(Message, Key);
			if(!Element.is_byte_string()) return false;
			jsoncons::basic_byte_string<> Bytes = Element.as_byte_string();
			OutData = Bytes.data();
			OutDataLength = Bytes.size();
			return true;
		}
		
		if(Message.contains(Key) && Message[Key].is_byte_string())
		{
			jsoncons::basic_byte_string<> Bytes = Message[Key].as_byte_string();
			OutData = Bytes.data();
			OutDataLength = Bytes.size();
			return true;
		}
		return false;
	}

	static void AppendBinary(ROSData& Message, const char* Key, const uint8* InData, const uint32 DataLength)
	{
		const ROSData Data = ROSData(jsoncons::byte_string_arg, std::vector<uint8_t>(InData, InData+DataLength), jsoncons::semantic_tag::base64);

		if(Key[0] == '/')
		{
			jsoncons::jsonpointer::insert_or_assign(Message, Key, Data);
		}
		else
		{
			Message.insert_or_assign(Key,Data);
		}
	}

/* All Number Types */

	/* Floating Point */

		static bool ExtractDouble(const ROSData& Message, const char* Key, double& OutData)
		{
			return ExtractInternal<double, double>(Message, Key, OutData);
		}

		static void AppendDouble(ROSData& Message, const char* Key, const double& InData)
		{
			AppendInternal<double>(Message, Key, InData);
		}

		/* Floats are internally encoded as double by ROSBridge */
		static bool ExtractFloat(const ROSData& Message, const char* Key, float& OutData)
		{
			return ExtractInternal<double, float>(Message, Key, OutData);
		}
	
		/* Floats are internally encoded as double by ROSBridge */
		static void AppendFloat(ROSData& Message, const char* Key, const float& InData)
		{
			AppendInternal<double>(Message, Key, InData);
		}

	/* Signed Integer */
	
		static bool ExtractInt64(const ROSData& Message, const char* Key, int64& OutData)
		{
			return ExtractInternal<int64_t, int64>(Message, Key, OutData);
		}

		static void AppendInt64(ROSData& Message, const char* Key, const int64& InData)
		{
			AppendInternal<int64_t>(Message, Key, InData);
		}

		static bool ExtractInt32(const ROSData& Message, const char* Key, int32& OutData)
		{
			return ExtractInternal<int32_t, int32>(Message, Key, OutData);
		}

		static void AppendInt32(ROSData& Message, const char* Key, const int32& InData)
		{
			AppendInternal<int32_t>(Message, Key, InData);
		}

		static bool ExtractInt16(const ROSData& Message, const char* Key, int16& OutData)
		{
			//ROSBridge encodes smaller types in known bigger types. int16 -> int32
			return ExtractInternal<int32_t, int16>(Message, Key, OutData);
		}

		static void AppendInt16(ROSData& Message, const char* Key, const int16& InData)
		{
			//ROSBridge encodes smaller types in known bigger types. int16 -> int32
			AppendInternal<int32_t>(Message, Key, InData);
		}

		static bool ExtractInt8(const ROSData& Message, const char* Key, int8& OutData)
		{
			//ROSBridge encodes smaller types in known bigger types. int8 -> uint32
			return ExtractInternal<int32_t, int8>(Message, Key, OutData);
		}

		static void AppendInt8(ROSData& Message, const char* Key, const int8& InData)
		{
			//ROSBridge encodes smaller types in known bigger types. int8 -> int32
			AppendInternal<int32_t>(Message, Key, InData);
		}

	/* Unsigned Integer */

		static bool ExtractUInt64(const ROSData& Message, const char* Key, uint64& OutData)
		{
			return ExtractInternal<uint64_t, uint64>(Message, Key, OutData);
		}

		static void AppendUInt64(ROSData& Message, const char* Key, const uint64& InData)
		{
			AppendInternal<uint64_t>(Message, Key, InData);
		}

		static bool ExtractUInt32(const ROSData& Message, const char* Key, uint32& OutData)
		{	
			return ExtractInternal<uint32_t, uint32>(Message, Key, OutData);
		}

		static void AppendUInt32(ROSData& Message, const char* Key, const uint32& InData)
		{
			AppendInternal<uint32_t>(Message, Key, InData);
		}

		static bool ExtractUInt16(const ROSData& Message, const char* Key, uint16& OutData)
		{
			//ROSBridge encodes smaller types in known bigger types. uint8 -> uint32
			return ExtractInternal<uint32_t, uint16>(Message, Key, OutData);
		}

		static void AppendUInt16(ROSData& Message, const char* Key, const uint16& InData)
		{
			//ROSBridge encodes smaller types in known bigger types. uint16 -> uint32
			AppendInternal<uint32_t>(Message, Key, InData);
		}

		static bool ExtractUInt8(const ROSData& Message, const char* Key, uint8& OutData)
		{
			//ROSBridge encodes smaller types in known bigger types. uint8 -> uint32
			return ExtractInternal<uint32_t, uint8>(Message, Key, OutData);
		}

		static void AppendUInt8(ROSData& Message, const char* Key, const uint8& InData)
		{
			//ROSBridge encodes smaller types in known bigger types. uint8 -> uint32
			AppendInternal<uint32_t>(Message, Key, InData);
		}
	
/* Arrays */

	template<typename T>
	static bool ExtractTArrayOfUObjects(const ROSData& Message, const char* Key, TArray<T*>& Array, UObject* ParentObject, const TFunction<bool(const ROSData&, const char*, T*&)>& ExtractElement)
	{
		if(!Message.contains(Key) || !Message[Key].is_array()) return false;

		Array.SetNum(Message[Key].size());
		
		for (uint64 i = 0; i < Message[Key].size(); i++) {
			T* Temp = NewObject<T>(ParentObject); //Needs to be used for UObject
			if (ExtractElement(Message[Key], ("/" + std::to_string(i)).c_str(), Temp)){
				Array[i] = MoveTempIfPossible(Temp);
			} else {
				return false;
			}
		}
		return true;
	}
	
	template<typename T>
	static bool ExtractTArray(const ROSData& Message, const char* Key, TArray<T>& Array, const TFunction<bool(const ROSData&, const char*, T&)>& ExtractElement)
	{
		if(!Message.contains(Key) || !Message[Key].is_array()) return false;

		Array.SetNum(Message[Key].size());
		
		for (uint64 i = 0; i < Message[Key].size(); i++) {
			T Temp;
			if (ExtractElement(Message[Key], ("/" + std::to_string(i)).c_str(), Temp)){
				Array[i] = MoveTempIfPossible(Temp);
			} else {
				return false;
			}
		}
		return true;
	}
	
	template<typename T>
	static void AppendTArray(ROSData& Message, const char* Key, const TArray<T>& Array, const TFunction<void(ROSData&, const char*, const T&)>& AppendElement)
	{
		ROSData ArrayNode = ROSData(jsoncons::json_array_arg);
		ArrayNode.reserve(Array.Num());
		
		for(int i = 0; i < Array.Num(); i++)
		{
			AppendElement(ArrayNode, ("/" + std::to_string(i)).c_str(), Array[i]);
		}

		Message.insert_or_assign(Key, ArrayNode);
	}
}