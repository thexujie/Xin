#include "PCH.h"
#include "Xml.h"

#include "rapidxml/Include/rapidxml.hpp"
#include "rapidxml/Include/rapidxml_print.hpp"

namespace Xin
{
	using xml_document = rapidxml::xml_document<char8>();
	using xml_node = rapidxml::xml_node<char8>;
	using xml_attribute = rapidxml::xml_attribute<char8>;

	FXmlNode FXmlNode::GetParent() const
	{
		if (!Node)
			return {};

		return FXmlNode(Document, Node->parent());
	}

	FStringV FXmlNode::GetName() const
	{
		if (!Node)
			return {};

		return FStringV(Node->name(), Node->name_size());
	}

	FXmlNode FXmlNode::GetFirstChild() const
	{
		if (!Node)
			return {};
		return FXmlNode(Document, Node->first_node());
	}

	FXmlNode FXmlNode::GetFirstChild(FStringV Name) const
	{
		if (!Node)
			return {};
		return FXmlNode(Document, Node->first_node(Name.Data, Name.Size));
	}

	uintx FXmlNode::GetChildrenCount() const
	{
		if (!Node)
			return 0;

		uintx Count = 0;
		xml_node * CurrNode = Node->first_node();
		while (CurrNode)
		{
			++Count;
			CurrNode = CurrNode->next_sibling();
		}
		return Count;
	}
	
	TList<FXmlNode> FXmlNode::GetChildren() const
	{
		if (!Node)
			return {};

		TList<FXmlNode> Nodes;
		xml_node * CurrNode = Node->first_node();
		while (CurrNode)
		{
			Nodes.Add(FXmlNode(Document, CurrNode));
			CurrNode = CurrNode->next_sibling();
		}
		return Nodes;
	}

	TList<FXmlNode> FXmlNode::GetChildren(FStringV Name) const
	{
		if (!Node)
			return {};

		TList<FXmlNode> Nodes;
		xml_node * CurrNode = Node->first_node(Name.Data, Name.Size);
		while (CurrNode)
		{
			Nodes.Add(FXmlNode(Document, CurrNode));
			CurrNode = CurrNode->next_sibling(Name.Data, Name.Size);
		}
		return Nodes;
	}


	TList<FXmlAttribute> FXmlNode::GetAttributes() const
	{
		if (!Node)
			return {};

		TList<FXmlAttribute> Attributes;
		xml_attribute * CurrAttribute = Node->first_attribute();
		while (CurrAttribute)
		{
			Attributes.Add(FXmlAttribute(Document, CurrAttribute));
			CurrAttribute = CurrAttribute->next_attribute();
		}
		return Attributes;
	}
	
	TList<FXmlAttribute> FXmlNode::GetAttributes(FStringV Name) const
	{
		if (!Node)
			return {};

		TList<FXmlAttribute> Attributes;
		xml_attribute * CurrAttribute = Node->first_attribute(Name.Data, Name.Size);
		while (CurrAttribute)
		{
			Attributes.Add(FXmlAttribute(Document, CurrAttribute));
			CurrAttribute = CurrAttribute->next_attribute(Name.Data, Name.Size);
		}
		return Attributes;
	}
	
	FStringV FXmlNode::GetAttribute(FStringV Name, FStringV DefaultValue) const
	{
		if (!Node)
			return {};

		xml_attribute * Attribute = Node->first_attribute(Name.Data, Name.Size);
		if (!Attribute)
			return DefaultValue;

		return FStringV(Attribute->value(), Attribute->value_size());
	}

	bool FXmlNode::SetAttribute(FStringV Name, FStringV Value) const
	{
		if (!Node)
			return false;

		xml_attribute * Attribute = Node->first_attribute(Name.Data, Name.Size);
		if (Attribute)
		{
			Attribute->value(Value.Data, Value.Size);
			return true;
		}
		else
		{
			Attribute = Document->allocate_attribute(Name.Data, Value.Data, Name.Size, Value.Size);
			Node->append_attribute(Attribute);
			return false;
		}
	}

	void FXmlNode::AppendChild(FXmlNode & Child)
	{
		if (!Node || !Child.Node)
			return;

		Node->append_node(Child.Node);
	}

	FXmlNode FXmlNode::CreateChild(FStringV Name)
	{
		if (!Node)
			return {};
		xml_node * NewNode = nullptr;
		if (Name.Empty())
			NewNode = Document->allocate_node(rapidxml::node_element, nullptr, nullptr, 0, 0);
		else
			NewNode = Document->allocate_node(rapidxml::node_element, Name.Data, nullptr, Name.Size, 0);
		Node->append_node(NewNode);
		return FXmlNode(Document, NewNode);
	}

	void FXmlNode::RemoveChild(FXmlNode & Child)
	{
		if (!Node || !Child.Node)
			return;
		Node->remove_node(Child.Node);
	}

	void FXmlNode::RemoveAllChildren()
	{
		if (!Node)
			return;
		Node->remove_all_nodes();
	}

	FXmlNode FXmlAttribute::GetOwner() const
	{
		if (!Attribute)
			return {};

		return FXmlNode(Document, Attribute->parent());
	}
	FStringV FXmlAttribute::GetName() const
	{
		if (!Attribute)
			return {};

		return FStringV(Attribute->name(), Attribute->name_size());
	}
	
	FStringV FXmlAttribute::GetValue() const
	{
		if (!Attribute)
			return {};

		return FStringV(Attribute->value(), Attribute->value_size());
	}
	
	FXmlDocument::FXmlDocument()
	{
	}

	FXmlDocument::~FXmlDocument()
	{
		if (RootNode.Document)
		{
			delete RootNode.Document;
			RootNode.Document = nullptr;
		}
	}

	FXmlNode FXmlDocument::FindRootNode(FStringV Name) const
	{
		if (!RootNode)
			return {};

		return FXmlNode(RootNode.Document, RootNode.Document->first_node(Name.Data, Name.Size));
	}

	FXmlNode FXmlDocument::CreateRootNode(FStringV Name)
	{
		if (!RootNode.Document)
			RootNode.Document = new rapidxml::xml_document<char8>();
		else
			RootNode.Document->clear();
		RootNode.Node = nullptr;

		FStringV RootHeader = u8"xml version='1.0' encoding='utf-8'"V;
		xml_node * HeadNode = RootNode.Document->allocate_node(rapidxml::node_pi, nullptr, RootHeader.Data, 0, RootHeader.Size);
		RootNode.Document->append_node(HeadNode);

		RootNode.Node = RootNode.Document->allocate_node(rapidxml::node_element, Name.Data, nullptr, Name.Size, 0);
		RootNode.Document->append_node(RootNode.Node);
		return RootNode;
	}

	bool FXmlDocument::LoadFromFile(FStringV FilePath_)
	{
		if (!RootNode.Document)
			RootNode.Document = new rapidxml::xml_document<char8>();
		else
			RootNode.Document->clear();
		RootNode.Node = nullptr;

		EFileBOM FileBOM = EFileBOM::None;
		FString FileTextContent = FFile::ReadString(FilePath_, &FileBOM);
		if (FileTextContent.Empty())
			return false;

		if (FileTextContent.StartsWith(u8"<?xml"V))
		{
			FStringV ENCODING_STR = u8"encoding=\""V;
			uintx EncodingStringIndex = FileTextContent.FindFirst(ENCODING_STR);
			if (EncodingStringIndex != NullIndex)
			{
				EncodingStringIndex += ENCODING_STR.Size;
				uintx EncodingStringEndIndex = FileTextContent.View(EncodingStringIndex).FindFirst(u8'\"');
				if (EncodingStringEndIndex != NullIndex)
				{
					FStringV XmlEncoding = FileTextContent.View(EncodingStringIndex, EncodingStringEndIndex);
#ifdef XE_ANSI
					if (StringCompareIC(XmlEncoding, FStringView(Str("utf-8"))) == 0)
					{
						FileTextContent = Strings::Utf8ToAnsi(FU8StringView(reinterpret_cast<const char8_t *>(FileTextContent.Data), FileTextContent.Size));
					}
#else
					if (StringCompareIC(XmlEncoding, u8"ansi"V) == 0)
					{
						FileTextContent = FEncoding::ToUtf8(FAStringV((const char *)(FileTextContent.Data), FileTextContent.Length));
					}
#endif
				}
			}
		}
		RootNode.Document->parse<0>(FileTextContent.Data);
		RootNode.Node = RootNode.Document->first_node();
#ifdef _DEBUG
		RootNode.DebugName = RootNode.GetName();
		SourceFilePath = FilePath_;
#endif
		return true;
	}

	bool FXmlDocument::SaveToFile(FStringV FilePath_)
	{
		std::u8string text;
		rapidxml::print(std::back_inserter(text), *(RootNode.Document), 0);
		//FStringV FileTextContent = Strings::FromWide(text.c_str(), int(text.length()));
		FFile::WriteString(FilePath_, { text.data(), text.length() });
		//FFile::WriteString(FilePath_, text);
		return true;
	}

	FString FXmlAttributes::GetString(FStringV XmlPath, FStringV NodePath, FStringV AttributeName)
	{
		FXmlDocument Xml(XmlPath);
		if (!Xml.GetRootNode().Valid())
			return {};

		FXmlNode Node = Xml.GetRootNode();
		TList<FStringV> Names = Split(NodePath, { u8"/\\"V });
		for (uintx Index = 0; Index < Names.GetSize(); ++Index)
		{
			if (Names[Index].Empty())
				continue;

			Node = Node.GetFirstChild(Names[Index]);
		}
		return Node.GetAttribute(AttributeName);
	}
}
