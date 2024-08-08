#pragma once

#include "CoreMinimal.h"
#include "TypeDefinitions.h"

#include <type_traits>	  // For is_base_of, unreal does not seem to have an equivalent

namespace DataHelpers
{
	// Internal functionality
	namespace Internal
	{
		template <typename JSONType, typename UnrealType>
		bool Extract(const ROSData& Message, const char* Key, UnrealType& OutData)
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
		bool Append(ROSData& Message, const char* Key, const UnrealType& InData)
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

		template<typename Allocator>
		static bool DecodeBinary(const ROSData& Message, TArray<uint8, Allocator>& OutData)
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
		static FString DataToString(const ROSData& Document)
		{
			std::string S;
			Document.dump(S, jsoncons::indenting::indent);
			return UTF8_TO_TCHAR(S.c_str());
		}

		template <typename T, typename Enable = void>
		struct DataConverter;

#define DEFINE_SIMPLE_DATA_TYPE(InternalType, UnrealType)                                         \
	template <>                                                                                   \
	struct DataConverter<UnrealType>                                                              \
	{                                                                                             \
		static inline void Append(ROSData& OutMessage, const char* Key, const UnrealType& Value)  \
		{                                                                                         \
			Internal::Append<InternalType>(OutMessage, Key, Value);                               \
		}                                                                                         \
		static inline bool Extract(const ROSData& Message, const char* Key, UnrealType& OutValue) \
		{                                                                                         \
			return Internal::Extract<InternalType>(Message, Key, OutValue);                       \
		}                                                                                         \
	};

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
#undef DEFINE_SIMPLE_DATA_TYPE

		// String type
		template <>
		struct DataConverter<FString>
		{
			static inline void Append(ROSData& OutMessage, const char* Key, const FString& Value)
			{
				Internal::Append<const char*>(OutMessage, Key, TCHAR_TO_UTF8(*Value));
			}

			static inline bool Extract(const ROSData& Message, const char* Key, FString& OutValue)
			{
				const char* CString;
				if (Internal::Extract<const char*>(Message, Key, CString))
				{
					OutValue = UTF8_TO_TCHAR(CString);
					return true;
				}
				else
				{
					return false;
				}
			}
		};

		// Message types
		template <typename T>
		struct DataConverter<T*, typename TEnableIf<std::is_base_of<UROSMessageBase, T>::value>::Type>
		{
			static inline void Append(ROSData& OutMessage, const char* Key, const T* Message)
			{
				ROSData Data;
				Message->ToData(Data);
				Internal::DataConverter<ROSData>::Append(OutMessage, Key, Data);
			}

			static inline bool Extract(const ROSData& Message, const char* Key, T*& OutMessageInstance)
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
		};

		template <typename T, typename Allocator>
		struct DataConverter<TArray<T, Allocator>>
		{
			static inline void Append(ROSData& OutMessage, const char* Key, const TArray<T, Allocator>& Array)
			{
				ROSData ArrayData = ROSData(jsoncons::json_array_arg);
				ArrayData.reserve(Array.Num());

				for (const auto& Value : Array)
				{
					const std::string Path = "/" + std::to_string(ArrayData.size());
					Internal::DataConverter<T>::Append(ArrayData, Path.c_str(), Value);
				}

				Internal::DataConverter<ROSData>::Append(OutMessage, Key, ArrayData);
			}

			static inline bool Extract(const ROSData& Message, const char* Key, TArray<T, Allocator>& Array)
			{
				ROSData ArrayData;
				if (!Internal::DataConverter<ROSData>::Extract(Message, Key, ArrayData) || !ArrayData.is_array())
				{
					return false;
				}

				Array.SetNum(ArrayData.size());
				for (uint64 i = 0; i < ArrayData.size(); i++)
				{
					const std::string Path = "/" + std::to_string(i);
					if (!Internal::DataConverter<T>::Extract(ArrayData, Path.c_str(), Array[i]))
					{
						return false;
					}
				}
				return true;
			}
		};

	}	 // namespace Internal

	template <typename T>
	inline void Append(ROSData& OutMessage, const char* Key, const T& Value)
	{
		Internal::DataConverter<T>::Append(OutMessage, Key, Value);
	}

	template <typename T>
	inline bool Extract(const ROSData& Message, const char* Key, T& OutValue)
	{
		return Internal::DataConverter<T>::Extract(Message, Key, OutValue);
	}

	template<typename Allocator>
	static bool ExtractBinary(const ROSData& Message, const char* Key, TArray<uint8, Allocator>& OutData)
	{
		if (Key[0] == '/')
		{
			return Internal::DecodeBinary(jsoncons::jsonpointer::get(Message, Key), OutData);
		}

		if (Message.contains(Key))
		{
			return Internal::DecodeBinary(Message[Key], OutData);
		}
		return false;
	}

	template<typename Allocator>
	static void AppendBinary(ROSData& Message, const char* Key, const TArray<uint8, Allocator> InData)
	{
		AppendBinary(Message, Key, InData.GetData(), InData.Num());
	}

	static void AppendBinary(ROSData& Message, const char* Key, const uint8* InData, const uint32 DataLength)
	{
		const ROSData Data =
			ROSData(jsoncons::byte_string_arg, std::vector<uint8_t>(InData, InData + DataLength));

		if (Key[0] == '/')
		{
			jsoncons::jsonpointer::insert_or_assign(Message, Key, Data);
		}
		else
		{
			Message.insert_or_assign(Key, Data);
		}
	}
}	 // namespace DataHelpers
