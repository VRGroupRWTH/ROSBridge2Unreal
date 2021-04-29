#pragma once

#include <type_traits> // For is_base_of, unreal does not seem to have an equivalent
#include "CoreMinimal.h"
#include "TypeDefinitions.h"

namespace DataHelpers
{

	/* Misc Types */

	namespace Internal
	{
		template <typename JSONType, typename UnrealType>
		bool Extract(const ROSData &Message, const char *Key, UnrealType &OutData)
		{
			if (Key[0] == '/')
			{
				ROSData Element = jsoncons::jsonpointer::get(Message, Key);
				if (!Element.is<JSONType>())
					return false;
				OutData = static_cast<UnrealType>(Element.as<JSONType>());
				return true;
			}

			if (Message.contains(Key) && Message[Key].is<JSONType>())
			{
				OutData = static_cast<UnrealType>(Message[Key].as<JSONType>());
				return true;
			}
			return false;
		}

		template <typename JSONType, typename UnrealType>
		bool Append(ROSData &Message, const char *Key, const UnrealType &InData)
		{
			if (Key[0] == '/')
			{
				jsoncons::jsonpointer::insert_or_assign(Message, Key, ROSData(static_cast<JSONType>(InData)));
			}
			else
			{
				Message.insert_or_assign(Key, static_cast<JSONType>(InData));
			}
			return false;
		}

		static bool DecodeBinary(const ROSData &Message, TArray<uint8> &OutData)
		{
			if (Message.is_byte_string_view()) /* Real byte string in BSON */
			{
				const jsoncons::byte_string_view Bytes = Message.as_byte_string_view();
				OutData.SetNumUninitialized(Bytes.size());
				FMemory::Memcpy(OutData.GetData(), Bytes.data(), Bytes.size());
				return true;
			}
			else if (Message.is_string_view())
			{ /* Binary in Base64 is decoded as string in JSON */
				const jsoncons::string_view Bytes = Message.as_string_view();
				OutData.SetNumUninitialized(FBase64::GetDecodedDataSize(Bytes.data(), Bytes.length()));
				return FBase64::Decode(Bytes.data(), Bytes.length(), OutData.GetData());
			}
			return false;
		}

		/* Just for debugging purposes */
		static FString DataToString(const ROSData &Document)
		{
			std::string S;
			Document.dump(S, jsoncons::indenting::indent);
			return UTF8_TO_TCHAR(S.c_str());
		}
	}

#define DEFINE_SIMPLE_DATA_TYPE(InternalType, UnrealType)                                          \
	template <>                                                                                    \
	inline void Append<UnrealType>(ROSData & OutMessage, const char *Key, const UnrealType &Value) \
	{                                                                                              \
		Internal::Append<UnrealType>(OutMessage, Key, Value);                                      \
	}                                                                                              \
	template <>                                                                                    \
	inline bool Extract<UnrealType>(const ROSData &Message, const char *Key, UnrealType &OutValue) \
	{                                                                                              \
		return Internal::Extract<UnrealType>(Message, Key, OutValue);                              \
	}

	template <typename T>
	inline void Append(ROSData &OutMessage, const char *Key, const T &Value);

	template <typename T>
	inline bool Extract(const ROSData &Message, const char *Key, T &OutValue);

	DEFINE_SIMPLE_DATA_TYPE(int64, int64)
	DEFINE_SIMPLE_DATA_TYPE(int32, int32)
	DEFINE_SIMPLE_DATA_TYPE(int32, int16)
	DEFINE_SIMPLE_DATA_TYPE(int32, int8)

	DEFINE_SIMPLE_DATA_TYPE(uint64, uint64)
	DEFINE_SIMPLE_DATA_TYPE(uint32, uint32)
	DEFINE_SIMPLE_DATA_TYPE(uint32, uint16)
	DEFINE_SIMPLE_DATA_TYPE(uint32, uint8)

	DEFINE_SIMPLE_DATA_TYPE(double, double)
	DEFINE_SIMPLE_DATA_TYPE(double, float)

	DEFINE_SIMPLE_DATA_TYPE(bool, bool)

	DEFINE_SIMPLE_DATA_TYPE(ROSData, ROSData)

	template <>
	inline void Append<FString>(ROSData &OutMessage, const char *Key, const FString &Value)
	{
		Internal::Append<const char *>(OutMessage, Key, TCHAR_TO_UTF8(*Value));
	}

	template <>
	inline bool Extract<FString>(const ROSData &Message, const char *Key, FString &OutValue)
	{
		const char *CString;
		if (Internal::Extract<const char *>(Message, Key, CString))
		{
			OutValue = UTF8_TO_TCHAR(CString);
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename T>
	inline typename TEnableIf<std::is_base_of<UROSMessageBase, T>::value, void>::Type Append(ROSData &OutMessage, const char *Key, const T *Message)
	{
		ROSData Data;
		Message->ToData(Data);
		Append<ROSData>(OutMessage, Key, Data);
	}

	template <typename T>
	inline typename TEnableIf<std::is_base_of<UROSMessageBase, T>::value, bool>::Type Extract(const ROSData &Message, const char *Key, T *&OutMessageInstance)
	{
		if (OutMessageInstance == nullptr)
		{
			OutMessageInstance = NewObject<T>();
		}

		if (Key[0] == '/')
		{
			return OutMessageInstance->FromData(jsoncons::jsonpointer::get(Message, Key));
		}
		else if (Message.contains(Key))
		{
			return OutMessageInstance->FromData(Message.at(Key));
		}
		else
		{
			return false;
		}
	}

	template <typename T>
	inline void Append(ROSData &OutMessage, const char *Key, const TArray<T>& Array)
	{
		ROSData ArrayData = ROSData(jsoncons::json_array_arg);
		ArrayData.reserve(Array.Num());

		for (const auto& Value : Array)
		{
			const std::string path = "/" + std::to_string(ArrayData.size());
			Append(ArrayData, Key, Value);
		}

		OutMessage.insert_or_assign(Key, ArrayData);
	}

	static bool ExtractBinary(const ROSData &Message, const char *Key, TArray<uint8> &OutData)
	{
		if (Key[0] == '/')
		{
			return Internal::DecodeBinary(jsoncons::jsonpointer::get(Message, Key), OutData);
		}

		if (Message.contains(Key))
		{
			return Internal::DecodeBinary(Message[Key], OutData);
			;
		}
		return false;
	}

	static void AppendBinary(ROSData &Message, const char *Key, const uint8 *InData, const uint32 DataLength)
	{
		const ROSData Data = ROSData(jsoncons::byte_string_arg, std::vector<uint8_t>(InData, InData + DataLength), jsoncons::semantic_tag::base64);

		if (Key[0] == '/')
		{
			jsoncons::jsonpointer::insert_or_assign(Message, Key, Data);
		}
		else
		{
			Message.insert_or_assign(Key, Data);
		}
	}

	/* Arrays */

	template <typename T>
	static bool ExtractTArrayOfUObjects(const ROSData &Message, const char *Key, TArray<T *> &Array, UObject *ParentObject, const TFunction<bool(const ROSData &, const char *, T *&)> &ExtractElement)
	{
		if (!Message.contains(Key) || !Message[Key].is_array())
			return false;

		Array.SetNum(Message[Key].size());

		for (uint64 i = 0; i < Message[Key].size(); i++)
		{
			T *Temp = NewObject<T>(ParentObject); //Needs to be used for UObject
			if (ExtractElement(Message[Key], ("/" + std::to_string(i)).c_str(), Temp))
			{
				Array[i] = MoveTempIfPossible(Temp);
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	template <typename T>
	static bool ExtractTArray(const ROSData &Message, const char *Key, TArray<T> &Array, const TFunction<bool(const ROSData &, const char *, T &)> &ExtractElement)
	{
		if (!Message.contains(Key) || !Message[Key].is_array())
			return false;

		Array.SetNum(Message[Key].size());

		for (uint64 i = 0; i < Message[Key].size(); i++)
		{
			T Temp;
			if (ExtractElement(Message[Key], ("/" + std::to_string(i)).c_str(), Temp))
			{
				Array[i] = MoveTempIfPossible(Temp);
			}
			else
			{
				return false;
			}
		}
		return true;
	}
}