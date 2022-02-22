#include "PCH.h"
#include "Json.h"

#include "Application.h"
#include "rapidjson/Include/reader.h"
#include "rapidjson/Include/writer.h"
#include "rapidjson/Include/prettywriter.h"

using namespace rapidjson;

using JsonEncoding = UTF8<char8_t>;
using FJsonStringStream = rapidjson::GenericStringStream<JsonEncoding>;
using FJsonStringBuffer = rapidjson::GenericStringBuffer<JsonEncoding>;
using FJsonReader = rapidjson::GenericReader<JsonEncoding, JsonEncoding>;
using FJsonWriter = rapidjson::PrettyWriter<FJsonStringBuffer, JsonEncoding, JsonEncoding>;

namespace Xin
{
	struct FJsonReadHandler
	{
		bool Null()
		{
			return true;
		}

		bool Key(const char8 * str, SizeType length, bool copy)
		{
			AssertExpr(Array == nullptr);
			Object->Items.Add({ FString { str, length }, {} });
			return true;
		}

		bool Bool(bool b)
		{
			if (Array)
				Array->Items.Add(b);
			else
				Object->Items.Back().Value = b;
			return true;
		}

		bool Int(int i)
		{
			if (Array)
				Array->Items.Add(i);
			else
				Object->Items.Back().Value = i;
			return true;
		}

		bool Uint(unsigned u)
		{
			if (Array)
				Array->Items.Add(u);
			else
				Object->Items.Back().Value = u;
			return true;
		}

		bool Int64(int64_t i)
		{
			if (Array)
				Array->Items.Add(i);
			else
				Object->Items.Back().Value = i;
			return true;
		}

		bool Uint64(uint64 u)
		{
			if (Array)
				Array->Items.Add(u);
			else
				Object->Items.Back().Value = u;
			return true;
		}

		bool Double(double d)
		{
			if (Array)
				Array->Items.Add(d);
			else
				Object->Items.Back().Value = d;
			return true;
		}

		bool RawNumber(const char8 * str, SizeType length, bool copy)
		{
			if (Array)
				Array->Items.Add(FString { str, length });
			else
				Object->Items.Back().Value = FString { str, length };
			return true;
		}

		bool String(const char8 * str, SizeType length, bool copy)
		{
			FStringV StringContent { str, length };
			if (Array)
				Array->Items.Add(FString { StringContent });
			else
				Object->Items.Back().Value = FString { StringContent };
			return true;
		}

		bool StartArray()
		{
			TReferPtr<FJsonArray> JsonArray = new FJsonArray();
			Object->Items.Back().Value = JsonArray;
			Array = JsonArray.Get();
			ObjectStack.PushBack(Object);
			Object = nullptr;
			return true;
		}

		bool EndArray(SizeType elementCount)
		{
			AssertExpr(Array != nullptr);
			Array = nullptr;
			Object = ObjectStack.Empty() ? nullptr : ObjectStack.PopBack();
			return true;
		}

		bool StartObject()
		{
			if (RootObject)
			{
				Object = RootObject;
				RootObject = nullptr;
			}
			else
			{
				TReferPtr<FJsonObject> JsonObject = new FJsonObject();
				if (Array)
				{
					Array->Items.Add(JsonObject);
					ArrayStack.PushBack(Array);
					Array = nullptr;
				}
				else
				{
					Object->Items.Back().Value = JsonObject;
					ObjectStack.PushBack(Object);
				}
				Object = JsonObject.Get();
			}
			return true;
		}

		bool EndObject(SizeType memberCount)
		{
			if (ArrayStack.Size)
			{
				Array = ArrayStack.PopBack();
			}
			else
			{
				Object = ObjectStack.Empty() ? nullptr : ObjectStack.PopBack();
			}
			return true;
		}


	public:
		FJsonReadHandler(FJsonObject * RootObject) : RootObject(RootObject) { }

		FJsonObject * Object = nullptr;
		FJsonArray * Array = nullptr;
		FJsonObject * RootObject = nullptr;
		TList<FJsonObject *> ObjectStack;
		TList<FJsonArray *> ArrayStack;
	};

	static FJsonItem None;
	FJsonItem & FJsonObject::Find(FStringV Name)
	{
		for (FJsonItem & Item : Items)
		{
			if (Item.Name == Name)
				return Item;
		}

		return None;
	}

	const FJsonItem & FJsonObject::Find(FStringV Name) const
	{
		for (const FJsonItem & Item : Items)
		{
			if (Item.Name == Name)
				return Item;
		}
		return None;
	}

	FJsonItem & FJsonObject::operator[](FStringV Name)
	{
		for (FJsonItem & Item : Items)
		{
			if (Item.Name == Name)
				return Item;
		}

		return Items.Add({ Name, FVariant::None });
	}

	const FJsonItem & FJsonObject::operator[](FStringV Name) const
	{
		for (const FJsonItem & Item : Items)
		{
			if (Item.Name == Name)
				return Item;
		}
		return None;
	}


	FJsonDocument::FJsonDocument()
	{
		
	}

	FJsonDocument::~FJsonDocument()
	{
		
	}

	bool FJsonDocument::LoadFromFile(FStringV FilePath)
	{
		FJsonReadHandler Handler(this);
		FString FileContent = FFile::ReadString(FilePath);
		FJsonStringStream StringSteam(FileContent.Data);
		FJsonReader Reader;
		ParseResult Result = Reader.Parse(StringSteam, Handler);
		if (Result.IsError())
		{
			LogWarning(u8"Json parse error at {0}{1}"V, FilePath, Result.Offset());
			return false;
		}
		return true;
	}

	template<typename JsonWriterT>
	static void WriteArray(JsonWriterT & JsonWriter, const FJsonArray & JsonArray);

	template<typename JsonWriterT>
	static void WriteObject(JsonWriterT & JsonWriter, const FJsonObject & Object);

	template<typename JsonWriterT>
	static void WriteVariant(JsonWriterT & JsonWriter, const FVariant & Variant)
	{
		switch (Variant.Kind)
		{
		case EVariantKind::Integer: JsonWriter.Int64(Variant.IntegerValue); break;
		case EVariantKind::Float32: JsonWriter.Double(Variant.Float32Value); break;
		case EVariantKind::Float64: JsonWriter.Double(Variant.Float64Value); break;
		case EVariantKind::Vec2I: break;
		case EVariantKind::Vec3I: break;
		case EVariantKind::Vec4I: break;
		case EVariantKind::Vec2U: break;
		case EVariantKind::Vec3U: break;
		case EVariantKind::Vec4U: break;
		case EVariantKind::Vec2F: break;
		case EVariantKind::Vec3F: break;
		case EVariantKind::Vec4F: break;
		case EVariantKind::Vector2: break;
		case EVariantKind::Vector3: break;
		case EVariantKind::Vector4: break;
		case EVariantKind::String:
			JsonWriter.String(Variant.StringValue.Data, (uint32)Variant.StringValue.Length, false);
			break;
		case EVariantKind::ReferPointer:
			if (Variant.Type() == FJsonArray::StaticType())
			{
				JsonWriter.StartArray();
				FJsonArray & JsonArray = Variant.Get<FJsonArray &>();
				for (const FVariant & Item : JsonArray.Items)
					WriteVariant(JsonWriter, Item);
				JsonWriter.EndArray();
			}
			else
			{
				FJsonObject & JsonObject = Variant.Get<FJsonObject &>();
				WriteObject(JsonWriter, JsonObject);
			}
			break;
		default:
			AssertFailed(u8"Invalid variant type."V);
			break;
		}
	}

	template<typename JsonWriterT>
	static void WriteObject(JsonWriterT & JsonWriter, const FJsonObject & Object)
	{
		JsonWriter.StartObject();
		for (const auto & Item : Object.Items)
		{
			JsonWriter.Key(Item.Name.Data, (SizeType)Item.Name.Length);
			WriteVariant(JsonWriter, Item.Value);
		}
		JsonWriter.EndObject(rapidjson::SizeType(Object.Items.Size));
	}

	bool FJsonDocument::SaveToFile(FStringV FilePath)
	{
		FJsonStringBuffer StringBuffer;
		FJsonWriter JsonWriter(StringBuffer, nullptr);
		WriteObject(JsonWriter, *this);
		FFile::WriteString(FilePath, { StringBuffer.GetString(), StringBuffer.GetLength() });
		return true;
	}

	class FJsonOutputStringStream
	{
	public:
		using Ch = char8_t;

		IDataStream & Stream;
		FString Buffer;

		//~FJsonOutputStringStream()
		//{
		//	if (Buffer.Size)
		//		Flush();
		//}

		void Flush()
		{
			Stream.WriteData(Buffer.Data, Buffer.Length);
			Buffer.Clear();
		}

		void Put(char8_t C)
		{
			Buffer.Append(C);
		}
	};

	class FJsonInputStringStream
	{
	public:
		using Ch = char8_t;

		IDataStream & Stream;

		void Put(char8_t C)
		{
			//Buffer.Append(C);
		}

		Ch Peek() const
		{
			Ch C;
			if (Stream.PeekData(&C, 1) == 1)
				return C;
			else
				return 0;
		}
		Ch Take()
		{
			Ch C;
			Stream >> C;
			return C;
		}

		uintx Tell() const
		{
			return Stream.Position();
		}

		Ch * PutBegin() { RAPIDJSON_ASSERT(false); return 0; }
		uintx PutEnd(Ch *) { RAPIDJSON_ASSERT(false); return 0; }
	};

	IDataStream & operator << (IDataStream & Stream, const FJsonObject & Object)
	{
		using FCustomJsonWriter = rapidjson::PrettyWriter<FJsonOutputStringStream, JsonEncoding, JsonEncoding>;

		FJsonOutputStringStream JsonOutputStringStream { Stream };
		FCustomJsonWriter JsonWriter(JsonOutputStringStream, nullptr);
		JsonWriter.SetIndent(u8' ', 2);
		WriteObject(JsonWriter, Object);
		return Stream;
	}

	IDataStream & operator >> (IDataStream & Stream, FJsonObject & Object)
	{
		FJsonReadHandler Handler(&Object);
		FJsonInputStringStream JsonInputStringStream { Stream };
		FJsonReader Reader;
		ParseResult Result = Reader.Parse(JsonInputStringStream, Handler);
		if (Result.IsError())
		{
			LogWarning(u8"Json parse error at {0}"V, Result.Offset());
		}
		return Stream;
	}
}
