#pragma once

#include "Core.Types.h"

namespace rapidxml
{
	template<class CharT>
	class xml_node;

	template<class CharT>
	class xml_attribute;

	template<class CharT>
	class xml_document;
}

namespace Xin
{
	struct FXmlAttribute;
	
	struct CORE_API FXmlNode
	{
		friend class FXmlDocument;

		FXmlNode() = default;
		FXmlNode(rapidxml::xml_document<char8> * DocumentIn, rapidxml::xml_node<char8> * NodeIn) : Document(DocumentIn), Node(NodeIn)
		{

#ifdef _DEBUG
			DebugName = GetName();
#endif
		}
		bool Valid() const { return !!Node; }
		explicit operator bool() const { return !!Node; }
		
		FXmlNode GetParent() const;
		FStringV GetName() const;

		FXmlNode GetFirstChild() const;
		FXmlNode GetFirstChild(FStringV Name) const;

		uintx GetChildrenCount() const;
		TList<FXmlNode> GetChildren() const;
		TList<FXmlNode> GetChildren(FStringV Name) const;

		TList<FXmlAttribute> GetAttributes() const;
		TList<FXmlAttribute> GetAttributes(FStringV Name) const;

		FStringV GetAttribute(FStringV Name, FStringV DefaultValue = FStringV()) const;
		bool SetAttribute(FStringV Name, FStringV Value) const;

		void AppendChild(FXmlNode & Child);
		FXmlNode CreateChild(FStringV Name);

		void RemoveChild(FXmlNode & Child);
		void RemoveAllChildren();

	private:
#ifdef _DEBUG
		FStringV DebugName;
#endif
		rapidxml::xml_document<char8> * Document = nullptr;
		rapidxml::xml_node<char8> * Node = nullptr;
	};

	struct CORE_API FXmlAttribute
	{
		friend class FXmlDocument;

		FXmlAttribute() = default;
		FXmlAttribute(rapidxml::xml_document<char8> * DocumentIn, rapidxml::xml_attribute<char8> * AttributeIn) : Document(DocumentIn), Attribute(AttributeIn)
		{

#ifdef _DEBUG
			DebugName = GetName();
			DebugValue = GetValue();
#endif
		}

		bool Valid() const { return !!Attribute; }
		explicit operator bool() const { return !!Attribute; }

		FXmlNode GetOwner() const;
		FStringV GetName() const;
		FStringV GetValue() const;

	private:
#ifdef _DEBUG
		FStringV DebugName;
		FStringV DebugValue;
#endif
		rapidxml::xml_document<char8> * Document = nullptr;
		rapidxml::xml_attribute<char8> * Attribute = nullptr;
	};


	class CORE_API FXmlDocument
	{
	public:
		FXmlDocument();
		FXmlDocument(FStringV FilePath) { LoadFromFile(FilePath); }
		~FXmlDocument();

	public:
		FXmlNode GetRootNode() const { return RootNode; }
		FXmlNode FindRootNode(FStringV Name) const;

		FXmlNode CreateRootNode(FStringV Name);

	public:
		bool LoadFromFile(FStringV FilePath_);
		bool SaveToFile(FStringV FilePath_);

	public:
#ifdef _DEBUG
		FString SourceFilePath;
#endif
		FXmlNode RootNode;
	};

	class CORE_API FXmlAttributes
	{
	public:
		FXmlAttributes() = delete;

	public:
		static FString GetString(FStringV XmlPath, FStringV NodePath, FStringV AttributeName);
		static bool GetBool(FStringV XmlPath, FStringV NodePath, FStringV AttributeName)
		{
			return Strings::ToBool(GetString(XmlPath, NodePath, AttributeName));
		}
	};
}
