#include "GuiResource.h"
#include "..\..\..\..\Common\Source\Stream\FileStream.h"
#include "..\..\..\..\Common\Source\Stream\Accessor.h"
#include "..\..\..\..\Common\Source\Stream\CharFormat.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace parsing::tabling;
		using namespace parsing::xml;

		WString GetFolderPath(const WString& filePath)
		{
			for(vint i=filePath.Length()-1;i>=0;i--)
			{
				if(filePath[i]==L'\\' || filePath[i]==L'/')
				{
					return filePath.Sub(0, i+1);
				}
			}
			return L"";
		}

		WString GetFileName(const WString& filePath)
		{
			for(vint i=filePath.Length()-1;i>=0;i--)
			{
				if(filePath[i]==L'\\' || filePath[i]==L'/')
				{
					return filePath.Sub(i+1, filePath.Length()-i-1);
				}
			}
			return filePath;
		}

		bool LoadTextFile(const WString& filePath, WString& text)
		{
			stream::FileStream fileStream(filePath, stream::FileStream::ReadOnly);
			return LoadTextFromStream(fileStream, text);
		}

		bool LoadTextFromStream(stream::IStream& stream, WString& text)
		{
			if(stream.IsAvailable())
			{
				stream::BomDecoder decoder;
				stream::DecoderStream decoderStream(stream, decoder);
				stream::StreamReader reader(decoderStream);
				text=reader.ReadToEnd();
				return true;
			}
			return false;
		}

/***********************************************************************
GuiImageData
***********************************************************************/

		GuiImageData::GuiImageData()
			:frameIndex(-1)
		{
		}

		GuiImageData::GuiImageData(Ptr<INativeImage> _image, vint _frameIndex)
			:image(_image)
			,frameIndex(_frameIndex)
		{
		}

		GuiImageData::~GuiImageData()
		{
		}

		Ptr<INativeImage> GuiImageData::GetImage()
		{
			return image;
		}

		vint GuiImageData::GetFrameIndex()
		{
			return frameIndex;
		}

/***********************************************************************
DocumentResolver
***********************************************************************/

		DocumentResolver::DocumentResolver(Ptr<DocumentResolver> _previousResolver)
			:previousResolver(_previousResolver)
		{
		}

		DocumentResolver::~DocumentResolver()
		{
		}

		Ptr<INativeImage> DocumentResolver::ResolveImage(const WString& protocol, const WString& path)
		{
			auto result=ResolveImageInternal(protocol, path);
			if(!result && previousResolver)
			{
				result=previousResolver->ResolveImage(protocol, path);
			}
			return result;
		}

/***********************************************************************
DocumentModel
***********************************************************************/

		namespace document_serialization_visitors
		{
			class SerializeRunVisitor : public Object, public DocumentRun::IVisitor
			{
			protected:

				const XmlElementWriter& Text(DocumentTextRun* run, const XmlElementWriter& writer)
				{
					return writer.Text(run->text);
				}

				const XmlElementWriter& Antialias(DocumentTextRun* run, const XmlElementWriter& writer)
				{
					if(!run->style.antialias)
					{
						return Text(run, writer.Element(L"na")).End();
					}
					else if(!run->style.verticalAntialias)
					{
						return Text(run, writer);
					}
					else
					{
						return Text(run, writer.Element(L"va")).End();
					}
				}

				const XmlElementWriter& Strikeline(DocumentTextRun* run, const XmlElementWriter& writer)
				{
					if(run->style.strikeline)
					{
						return Antialias(run, writer.Element(L"s")).End();
					}
					else
					{
						return Antialias(run, writer);
					}
				}

				const XmlElementWriter& Underline(DocumentTextRun* run, const XmlElementWriter& writer)
				{
					if(run->style.underline)
					{
						return Strikeline(run, writer.Element(L"u")).End();
					}
					else
					{
						return Strikeline(run, writer);
					}
				}

				const XmlElementWriter& Italic(DocumentTextRun* run, const XmlElementWriter& writer)
				{
					if(run->style.italic)
					{
						return Underline(run, writer.Element(L"i")).End();
					}
					else
					{
						return Underline(run, writer);
					}
				}

				const XmlElementWriter& Bold(DocumentTextRun* run, const XmlElementWriter& writer)
				{
					if(run->style.bold)
					{
						return Italic(run, writer.Element(L"b")).End();
					}
					else
					{
						return Italic(run, writer);
					}
				}

				const XmlElementWriter& Font(DocumentTextRun* run, const XmlElementWriter& writer)
				{
					const XmlElementWriter& font=writer.Element(L"font");
					font.Attribute(L"face", run->style.fontFamily);
					font.Attribute(L"size", itow(run->style.size));
					font.Attribute(L"color", run->color.ToString());
					Bold(run, font);
					return writer;
				}
			public:
				Ptr<XmlElement> container;

				void Visit(DocumentTextRun* run)override
				{
					if(run->text!=L"")
					{
						XmlElementWriter writer(container);
						Font(run, writer);
					}
				}

				void Visit(DocumentImageRun* run)override
				{
					XmlElementWriter writer(container);
					writer
						.Element(L"img")
						.Attribute(L"width", itow(run->size.x))
						.Attribute(L"height", itow(run->size.y))
						.Attribute(L"baseline", itow(run->baseline))
						.Attribute(L"frameIndex", itow(run->frameIndex))
						.Attribute(L"source", run->source)
						;
				}
			};

			class DeserializeNodeVisitor : public XmlNode::IVisitor
			{
			public:
				List<Pair<FontProperties, Color>>	styleStack;
				Ptr<DocumentParagraph>				paragraph;
				Ptr<DocumentLine>					line;
				Ptr<DocumentResolver>				resolver;

				DeserializeNodeVisitor(Ptr<DocumentParagraph> _paragraph, Ptr<DocumentResolver> _resolver)
					:paragraph(_paragraph)
					,resolver(_resolver)
				{
					styleStack.Add(Pair<FontProperties, Color>(GetCurrentController()->ResourceService()->GetDefaultFont(), Color()));
				}

				void PrintText(const WString& text)
				{
					if(!line)
					{
						line=new DocumentLine;
						paragraph->lines.Add(line);
					}
					Ptr<DocumentTextRun> run=new DocumentTextRun;
					run->style=styleStack[styleStack.Count()-1].key;
					run->color=styleStack[styleStack.Count()-1].value;
					run->text=text;
					line->runs.Add(run);
				}

				void Visit(XmlText* node)override
				{
					PrintText(node->content.value);
				}

				void Visit(XmlCData* node)override
				{
					PrintText(node->content.value);
				}

				void Visit(XmlAttribute* node)override
				{
				}

				void Visit(XmlComment* node)override
				{
				}

				void Visit(XmlElement* node)override
				{
					if(node->name.value==L"br")
					{
						if(!line) PrintText(L"");
						line=0;
					}
					else if(node->name.value==L"img")
					{
						if(!line)
						{
							line=new DocumentLine;
							paragraph->lines.Add(line);
						}
						Ptr<DocumentImageRun> run=new DocumentImageRun;
						if(Ptr<XmlAttribute> source=XmlGetAttribute(node, L"source"))
						{
							run->source=source->value.value;
							Pair<vint, vint> index=INVLOC.FindFirst(run->source, L"://", Locale::IgnoreCase);
							if(index.key!=-1)
							{
								WString protocol=run->source.Sub(0, index.key);
								WString path=run->source.Sub(index.key+index.value, run->source.Length()-index.key-index.value);
								run->image=resolver->ResolveImage(protocol, path);
								if(run->image && run->image->GetFrameCount()>0)
								{
									run->size=run->image->GetFrame(0)->GetSize();
									run->baseline=run->size.y;
									run->frameIndex=0;
								}
							}

							FOREACH(Ptr<XmlAttribute>, att, node->attributes)
							{
								if(att->name.value==L"width")
								{
									run->size.x=wtoi(att->value.value);
								}
								else if(att->name.value==L"height")
								{
									run->size.y=wtoi(att->value.value);
								}
								else if(att->name.value==L"baseline")
								{
									run->baseline=wtoi(att->value.value);
								}
								else if(att->name.value==L"frameIndex")
								{
									run->frameIndex=wtoi(att->value.value);
								}
							}
						}
						line->runs.Add(run);
					}
					else if(node->name.value==L"font")
					{
						auto style=styleStack[styleStack.Count()-1];
						FOREACH(Ptr<XmlAttribute>, att, node->attributes)
						{
							if(att->name.value==L"face")
							{
								style.key.fontFamily=att->value.value;
							}
							else if(att->name.value==L"size")
							{
								style.key.size=wtoi(att->value.value);
							}
							else if(att->name.value==L"color")
							{
								style.value=Color::Parse(att->value.value);
							}
						}
						styleStack.Add(style);
						FOREACH(Ptr<XmlNode>, sub, node->subNodes)
						{
							sub->Accept(this);
						}
						styleStack.RemoveAt(styleStack.Count()-1);
					}
					else if(node->name.value==L"b")
					{
						auto style=styleStack[styleStack.Count()-1];
						style.key.bold=true;
						styleStack.Add(style);
						FOREACH(Ptr<XmlNode>, sub, node->subNodes)
						{
							sub->Accept(this);
						}
						styleStack.RemoveAt(styleStack.Count()-1);
					}
					else if(node->name.value==L"i")
					{
						auto style=styleStack[styleStack.Count()-1];
						style.key.italic=true;
						styleStack.Add(style);
						FOREACH(Ptr<XmlNode>, sub, node->subNodes)
						{
							sub->Accept(this);
						}
						styleStack.RemoveAt(styleStack.Count()-1);
					}
					else if(node->name.value==L"u")
					{
						auto style=styleStack[styleStack.Count()-1];
						style.key.underline=true;
						styleStack.Add(style);
						FOREACH(Ptr<XmlNode>, sub, node->subNodes)
						{
							sub->Accept(this);
						}
						styleStack.RemoveAt(styleStack.Count()-1);
					}
					else if(node->name.value==L"s")
					{
						auto style=styleStack[styleStack.Count()-1];
						style.key.strikeline=true;
						styleStack.Add(style);
						FOREACH(Ptr<XmlNode>, sub, node->subNodes)
						{
							sub->Accept(this);
						}
						styleStack.RemoveAt(styleStack.Count()-1);
					}
					else if(node->name.value==L"va")
					{
						auto style=styleStack[styleStack.Count()-1];
						style.key.antialias=true;
						style.key.verticalAntialias=true;
						styleStack.Add(style);
						FOREACH(Ptr<XmlNode>, sub, node->subNodes)
						{
							sub->Accept(this);
						}
						styleStack.RemoveAt(styleStack.Count()-1);
					}
					else if(node->name.value==L"na")
					{
						auto style=styleStack[styleStack.Count()-1];
						style.key.antialias=false;
						style.key.verticalAntialias=false;
						styleStack.Add(style);
						FOREACH(Ptr<XmlNode>, sub, node->subNodes)
						{
							sub->Accept(this);
						}
						styleStack.RemoveAt(styleStack.Count()-1);
					}
					else
					{
						FOREACH(Ptr<XmlNode>, sub, node->subNodes)
						{
							sub->Accept(this);
						}
					}
				}

				void Visit(XmlInstruction* node)override
				{
				}

				void Visit(XmlDocument* node)override
				{
				}
			};
		}
		using namespace document_serialization_visitors;

		Ptr<DocumentModel> DocumentModel::LoadFromXml(Ptr<parsing::xml::XmlDocument> xml, Ptr<DocumentResolver> resolver)
		{
			Ptr<DocumentModel> model=new DocumentModel;
			if(xml->rootElement->name.value==L"Doc")
			if(Ptr<XmlElement> content=XmlGetElement(xml->rootElement, L"Content"))
			FOREACH(Ptr<XmlElement>, p, XmlGetElements(content, L"p"))
			{
				Ptr<DocumentParagraph> paragraph=new DocumentParagraph;
				model->paragraphs.Add(paragraph);
				DeserializeNodeVisitor visitor(paragraph, resolver);
				p->Accept(&visitor);
			}
			return model;
		}

		Ptr<DocumentModel> DocumentModel::LoadFromXml(Ptr<parsing::xml::XmlDocument> xml, const WString& workingDirectory)
		{
			Ptr<DocumentFileProtocolResolver> resolver=new DocumentFileProtocolResolver(workingDirectory);
			return LoadFromXml(xml, resolver);
		}

		Ptr<parsing::xml::XmlDocument> DocumentModel::SaveToXml()
		{
			SerializeRunVisitor visitor;
			Ptr<XmlDocument> xml=new XmlDocument;
			Ptr<XmlElement> doc=new XmlElement;
			doc->name.value=L"Doc";
			xml->rootElement=doc;
			{
				Ptr<XmlElement> content=new XmlElement;
				content->name.value=L"Content";
				doc->subNodes.Add(content);

				FOREACH(Ptr<DocumentParagraph>, p, paragraphs)
				{
					Ptr<XmlElement> paragraph=new XmlElement;
					paragraph->name.value=L"p";
					content->subNodes.Add(paragraph);

					FOREACH(Ptr<DocumentLine>, l, p->lines)
					{
						FOREACH(Ptr<DocumentRun>, r, l->runs)
						{
							visitor.container=paragraph;
							r->Accept(&visitor);
						}
						{
							Ptr<XmlElement> line=new XmlElement;
							line->name.value=L"br";
							paragraph->subNodes.Add(line);
						}
					}
				}
			}
			return xml;
		}

/***********************************************************************
DocumentFileProtocolResolver
***********************************************************************/

		Ptr<INativeImage> DocumentFileProtocolResolver::ResolveImageInternal(const WString& protocol, const WString& path)
		{
			if(INVLOC.ToUpper(protocol)!=L"FILE") return 0;
			WString filename=path;
			if(filename.Length()>=2 && filename[1]!=L':')
			{
				filename=workingDirectory+filename;
			}
			return GetCurrentController()->ImageService()->CreateImageFromFile(filename);
		}

		DocumentFileProtocolResolver::DocumentFileProtocolResolver(const WString& _workingDirectory, Ptr<DocumentResolver> previousResolver)
			:DocumentResolver(previousResolver)
			,workingDirectory(_workingDirectory)
		{
		}

/***********************************************************************
DocumentResProtocolResolver
***********************************************************************/

		Ptr<INativeImage> DocumentResProtocolResolver::ResolveImageInternal(const WString& protocol, const WString& path)
		{
			if(INVLOC.ToUpper(protocol)!=L"RES") return 0;
			Ptr<GuiImageData> image=resource->GetValueByPath(path).Cast<GuiImageData>();
			if(image)
			{
				return image->GetImage();
			}
			else
			{
				return 0;
			}
		}

		DocumentResProtocolResolver::DocumentResProtocolResolver(Ptr<GuiResource> _resource, Ptr<DocumentResolver> previousResolver)
			:DocumentResolver(previousResolver)
			,resource(_resource)
		{
		}

/***********************************************************************
GuiResourceNodeBase
***********************************************************************/

		GuiResourceNodeBase::GuiResourceNodeBase()
			:parent(0)
		{
		}

		GuiResourceNodeBase::~GuiResourceNodeBase()
		{
		}

		const WString& GuiResourceNodeBase::GetName()
		{
			return name;
		}

		GuiResourceFolder* GuiResourceNodeBase::GetParent()
		{
			return parent;
		}

/***********************************************************************
GuiResourceItem
***********************************************************************/

		GuiResourceItem::GuiResourceItem()
		{
		}

		GuiResourceItem::~GuiResourceItem()
		{
		}

		Ptr<Object> GuiResourceItem::GetContent()
		{
			return content;
		}

		void GuiResourceItem::SetContent(Ptr<Object> value)
		{
			content=value;
		}

		Ptr<GuiImageData> GuiResourceItem::AsImage()
		{
			return content.Cast<GuiImageData>();
		}

		Ptr<parsing::xml::XmlDocument> GuiResourceItem::AsXml()
		{
			return content.Cast<XmlDocument>();
		}

		Ptr<ObjectBox<WString>> GuiResourceItem::AsString()
		{
			return content.Cast<ObjectBox<WString>>();
		}

		Ptr<DocumentModel> GuiResourceItem::AsDocument()
		{
			return content.Cast<DocumentModel>();
		}

/***********************************************************************
GuiResourceFolder
***********************************************************************/

		void GuiResourceFolder::LoadResourceFolderXml(DelayLoading& delayLoading, const WString& containingFolder, Ptr<parsing::xml::XmlElement> folderXml, Ptr<parsing::tabling::ParsingTable> xmlParsingTable)
		{
			ClearItems();
			ClearFolders();
			FOREACH(Ptr<XmlElement>, element, XmlGetElements(folderXml))
			{
				WString name;
				if(Ptr<XmlAttribute> nameAtt=XmlGetAttribute(element, L"name"))
				{
					name=nameAtt->value.value;
				}
				if(element->name.value==L"Folder")
				{
					if(name!=L"")
					{
						Ptr<GuiResourceFolder> folder=new GuiResourceFolder;
						if(AddFolder(name, folder))
						{
							WString newContainingFolder=containingFolder;
							Ptr<XmlElement> newFolderXml=element;
							if(Ptr<XmlAttribute> contentAtt=XmlGetAttribute(element, L"content"))
							{
								if(contentAtt->value.value==L"Link")
								{
									WString filePath=containingFolder+XmlGetValue(element);
									WString text;
									if(LoadTextFile(filePath, text))
									{
										Ptr<XmlDocument> xml=XmlParseDocument(text, xmlParsingTable);
										if(xml)
										{
											newContainingFolder=GetFolderPath(filePath);
											newFolderXml=xml->rootElement;
										}
									}
								}
							}
							folder->LoadResourceFolderXml(delayLoading, newContainingFolder, newFolderXml, xmlParsingTable);
						}
					}
				}
				else
				{
					WString filePath;
					if(Ptr<XmlAttribute> contentAtt=XmlGetAttribute(element, L"content"))
					{
						if(contentAtt->value.value==L"File")
						{
							filePath=containingFolder+XmlGetValue(element);
							if(name==L"")
							{
								name=GetFileName(filePath);
							}
						}
					}

					Ptr<GuiResourceItem> item=new GuiResourceItem;
					if(AddItem(name, item))
					{
						if(filePath!=L"")
						{
							stream::FileStream fileStream(filePath, stream::FileStream::ReadOnly);
							if(fileStream.IsAvailable())
							{
								if(element->name.value==L"Xml" || element->name.value==L"Doc")
								{
									WString text;
									if(LoadTextFromStream(fileStream, text))
									{
										Ptr<XmlDocument> xml=XmlParseDocument(text, xmlParsingTable);
										if(xml)
										{
											item->SetContent(xml);
											if(element->name.value==L"Doc")
											{
												delayLoading.documentModelFolders.Add(item, containingFolder);
											}
										}
									}
								}
								else if(element->name.value==L"Text")
								{
									WString text;
									if(LoadTextFromStream(fileStream, text))
									{
										item->SetContent(new ObjectBox<WString>(text));
									}
								}
								else if(element->name.value==L"Image")
								{
									Ptr<INativeImage> image=GetCurrentController()->ImageService()->CreateImageFromStream(fileStream);
									if(image)
									{
										Ptr<GuiImageData> imageData=new GuiImageData(image, 0);
										item->SetContent(imageData);
									}
								}
							}
						}
						else
						{
							if(element->name.value==L"Xml" || element->name.value==L"Doc")
							{
								WString text=XmlGetValue(element);
								Ptr<XmlDocument> xml=XmlParseDocument(text, xmlParsingTable);
								if(xml)
								{
									item->SetContent(xml);
									if(element->name.value==L"Doc")
									{
										delayLoading.documentModelFolders.Add(item, containingFolder);
									}
								}
							}
							else if(element->name.value==L"Text")
							{
								WString text=XmlGetValue(element);
								item->SetContent(new ObjectBox<WString>(text));
							}
							else if(element->name.value==L"Image")
							{
							}
						}
						if(!item->GetContent())
						{
							RemoveItem(name);
						}
					}
				}
			}
		}

		GuiResourceFolder::GuiResourceFolder()
		{
		}

		GuiResourceFolder::~GuiResourceFolder()
		{
		}

		const GuiResourceFolder::ItemList& GuiResourceFolder::GetItems()
		{
			return items.Values();
		}

		Ptr<GuiResourceItem> GuiResourceFolder::GetItem(const WString& name)
		{
			vint index=items.Keys().IndexOf(name);
			return index==-1?0:items.Values().Get(index);
		}

		bool GuiResourceFolder::AddItem(const WString& name, Ptr<GuiResourceItem> item)
		{
			if(item->GetParent()!=0 || items.Keys().Contains(name)) return false;
			items.Add(name, item);
			item->parent=this;
			item->name=name;
			return true;
		}

		Ptr<GuiResourceItem> GuiResourceFolder::RemoveItem(const WString& name)
		{
			Ptr<GuiResourceItem> item=GetItem(name);
			if(!item) return 0;
			items.Remove(name);
			return item;
		}

		void GuiResourceFolder::ClearItems()
		{
			items.Clear();
		}

		const GuiResourceFolder::FolderList& GuiResourceFolder::GetFolders()
		{
			return folders.Values();
		}

		Ptr<GuiResourceFolder> GuiResourceFolder::GetFolder(const WString& name)
		{
			vint index=folders.Keys().IndexOf(name);
			return index==-1?0:folders.Values().Get(index);
		}

		bool GuiResourceFolder::AddFolder(const WString& name, Ptr<GuiResourceFolder> folder)
		{
			if(folder->GetParent()!=0 || folders.Keys().Contains(name)) return false;
			folders.Add(name, folder);
			folder->parent=this;
			folder->name=name;
			return true;
		}

		Ptr<GuiResourceFolder> GuiResourceFolder::RemoveFolder(const WString& name)
		{
			Ptr<GuiResourceFolder> folder=GetFolder(name);
			if(!folder) return 0;
			folders.Remove(name);
			return folder;
		}

		void GuiResourceFolder::ClearFolders()
		{
			folders.Clear();
		}

		Ptr<Object> GuiResourceFolder::GetValueByPath(const WString& path)
		{
			const wchar_t* buffer=path.Buffer();
			const wchar_t* index=wcschr(buffer, L'\\');
			if(!index) index=wcschr(buffer, '/');

			if(index)
			{
				WString name=path.Sub(0, index-buffer);
				Ptr<GuiResourceFolder> folder=GetFolder(name);
				if(folder)
				{
					vint start=index-buffer+1;
					return folder->GetValueByPath(path.Sub(start, path.Length()-start));
				}
			}
			else
			{
				Ptr<GuiResourceItem> item=GetItem(path);
				if(item)
				{
					return item->GetContent();
				}
			}
			return 0;
		}

/***********************************************************************
GuiResource
***********************************************************************/

		GuiResource::GuiResource()
		{
		}

		GuiResource::~GuiResource()
		{
		}

		Ptr<GuiResource> GuiResource::LoadFromXml(const WString& filePath)
		{
			Ptr<ParsingTable> table;
			Ptr<XmlDocument> xml;
			Ptr<GuiResource> resource;
			{
				WString text;
				if(LoadTextFile(filePath, text))
				{
					table=XmlLoadTable();
					xml=XmlParseDocument(text, table);
				}
			}
			if(xml)
			{
				resource=new GuiResource;
				DelayLoading delayLoading;
				resource->LoadResourceFolderXml(delayLoading, GetFolderPath(filePath), xml->rootElement, table);

				for(vint i=0;i<delayLoading.documentModelFolders.Count();i++)
				{
					Ptr<GuiResourceItem> item=delayLoading.documentModelFolders.Keys()[i];
					WString folder=delayLoading.documentModelFolders.Values().Get(i);
					if(Ptr<XmlDocument> xml=item->AsXml())
					{
						Ptr<DocumentResolver> resolver=new DocumentResProtocolResolver(resource, new DocumentFileProtocolResolver(folder));
						Ptr<DocumentModel> model=DocumentModel::LoadFromXml(xml, resolver);
						item->SetContent(model);
					}
				}
			}
			return resource;
		}
	}
}