#include "..\..\..\Source\Console.h"
#include "..\..\..\Source\Stream\FileStream.h"
#include "..\..\..\Source\Stream\CharFormat.h"
#include "..\..\..\Source\Combinator\StringCombinator.h"
#include "FpmacroRuntime.h"
#include "FpmacroParser.h"

using namespace vl;
using namespace vl::console;
using namespace vl::stream;
using namespace vl::collections;
using namespace vl::combinator;
using namespace fpmacro;
using namespace fpmacro::parser;

/***********************************************************************
ExpressionCreateObjectVisitor
***********************************************************************/

class ExpressionCreateObjectVisitor : public Object, public FpmExpression::IVisitor
{
public:
	RunningObject::Ref			runningObject;

	static RunningObject::Ref Do(Ptr<FpmExpression> node)
	{
		ExpressionCreateObjectVisitor visitor;
		node->Accept(&visitor);
		return visitor.runningObject;
	}

	void Visit(FpmConcatExpression* node)
	{
		ConcatObject* result=new ConcatObject;
		FOREACH(Ptr<FpmExpression>, expression, node->expressions)
		{
			result->objects.Add(Do(expression));
		}
		runningObject=result;
	}

	void Visit(FpmArrayExpression* node)
	{
		ArrayObject* result=new ArrayObject;
		FOREACH(Ptr<FpmExpression>, expression, node->elements)
		{
			result->objects.Add(Do(expression));
		}
		runningObject=result;
	}

	void Visit(FpmInvokeExpression* node)
	{
		InvokeObject* result=new InvokeObject;
		result->target=Do(node->function);
		FOREACH(Ptr<FpmExpression>, expression, node->arguments)
		{
			result->arguments.Add(Do(expression));
		}
		runningObject=result;
	}

	void Visit(FpmBracketExpression* node)
	{
		ConcatObject* result=new ConcatObject;
		{
			TextObject* bracket=new TextObject;
			bracket->text=L"(";
			result->objects.Add(bracket);
		}
		result->objects.Add(Do(node->expression));
		{
			TextObject* bracket=new TextObject;
			bracket->text=L")";
			result->objects.Add(bracket);
		}
		runningObject=result;
	}

	void Visit(FpmReferenceExpression* node)
	{
		PredefinedObject::Type type=PredefinedObject::Unknown;
		bool found=true;
		if(node->name.value==L"$add")type=PredefinedObject::Add;
		else if(node->name.value==L"$sub")type=PredefinedObject::Sub;
		else if(node->name.value==L"$mul")type=PredefinedObject::Mul;
		else if(node->name.value==L"$div")type=PredefinedObject::Div;
		else if(node->name.value==L"$mod")type=PredefinedObject::Mod;
		else if(node->name.value==L"$lt")type=PredefinedObject::LT;
		else if(node->name.value==L"$le")type=PredefinedObject::LE;
		else if(node->name.value==L"$gt")type=PredefinedObject::GT;
		else if(node->name.value==L"$ge")type=PredefinedObject::GE;
		else if(node->name.value==L"$eq")type=PredefinedObject::EQ;
		else if(node->name.value==L"$ne")type=PredefinedObject::NE;
		else if(node->name.value==L"$if")type=PredefinedObject::If;
		else if(node->name.value==L"$loop")type=PredefinedObject::Loop;
		else if(node->name.value==L"$loopsep")type=PredefinedObject::LoopSep;
		else if(node->name.value==L"$head")type=PredefinedObject::Head;
		else if(node->name.value==L"$tail")type=PredefinedObject::Tail;
		else if(node->name.value==L"$length")type=PredefinedObject::Length;
		else if(node->name.value==L"$get")type=PredefinedObject::Get;
		else found=false;
		if(found)
		{
			PredefinedObject* result=new PredefinedObject;
			result->type=type;
			runningObject=result;
		}
		else
		{
			ReferenceObject* result=new ReferenceObject;
			result->name=node->name.value;
			runningObject=result;
		}
	}

	void Visit(FpmTextExpression* node)
	{
		TextObject* result=new TextObject;
		if(node->text.value.Length()==4 && node->text.value.Sub(0, 2)==L"$(")
		{
			result->text=node->text.value[2];
		}
		else
		{
			result->text=node->text.value;
		}
		runningObject=result;
	}
};

/***********************************************************************
DefinitionCreateObjectVisitor
***********************************************************************/

class DefinitionCreateObjectVisitor : public Object, public FpmDefinition::IVisitor
{
public:
	RunningObject::Environment::Ref		environment;
	RunningObject::Ref					runningObject;

	DefinitionCreateObjectVisitor(RunningObject::Environment::Ref _environment)
		:environment(_environment)
	{
	}

	static RunningObject::Ref Do(Ptr<FpmDefinition> node, RunningObject::Environment::Ref environment)
	{
		DefinitionCreateObjectVisitor visitor(environment);
		node->Accept(&visitor);
		return visitor.runningObject;
	}

	void Visit(FpmExpressionDefinition* node)
	{
	}

	void Visit(FpmReferenceDefinition* node)
	{
	}
};

/***********************************************************************
ExpressionToStringVisitor
***********************************************************************/

class ExpressionToStringVisitor : public Object, public FpmExpression::IVisitor
{
public:
	WString						prefix;
	TextWriter&					writer;

	ExpressionToStringVisitor(const WString& _prefix, TextWriter& _writer)
		:prefix(_prefix)
		,writer(_writer)
	{
	}

	static void Do(Ptr<FpmExpression> node, WString prefix, TextWriter& writer)
	{
		ExpressionToStringVisitor visitor(prefix, writer);
		node->Accept(&visitor);
	}

	void Visit(FpmConcatExpression* node)
	{
		writer.WriteString(prefix);
		writer.WriteLine(L"CONCAT {");
		FOREACH(Ptr<FpmExpression>, expression, node->expressions)
		{
			Do(expression, prefix+L"    ", writer);
		}
		writer.WriteString(prefix);
		writer.WriteLine(L"}");
	}

	void Visit(FpmArrayExpression* node)
	{
		writer.WriteString(prefix);
		writer.WriteLine(L"ARRAY {");
		FOREACH(Ptr<FpmExpression>, expression, node->elements)
		{
			Do(expression, prefix+L"    ", writer);
		}
		writer.WriteString(prefix);
		writer.WriteLine(L"}");
	}

	void Visit(FpmInvokeExpression* node)
	{
		writer.WriteString(prefix);
		writer.WriteLine(L"INVOKE {");
		Do(node->function, prefix+L"    ", writer);

		writer.WriteString(prefix+L"    ");
		writer.WriteLine(L"ARGUMENTS {");
		FOREACH(Ptr<FpmExpression>, expression, node->arguments)
		{
			Do(expression, prefix+L"        ", writer);
		}
		writer.WriteString(prefix+L"    ");
		writer.WriteLine(L"}");

		writer.WriteString(prefix);
		writer.WriteLine(L"}");
	}

	void Visit(FpmBracketExpression* node)
	{
		writer.WriteString(prefix);
		writer.WriteLine(L"CONCAT {");

		writer.WriteLine(prefix);
		writer.WriteLine(L"    TEXT: (");
		Do(node->expression, prefix+L"    ", writer);
		writer.WriteLine(prefix);
		writer.WriteLine(L"    TEXT: )");

		writer.WriteString(prefix);
		writer.WriteLine(L"}");
	}

	void Visit(FpmReferenceExpression* node)
	{
		writer.WriteString(prefix);
		writer.WriteString(L"NAME : ");
		writer.WriteLine(node->name.value);
	}

	void Visit(FpmTextExpression* node)
	{
		writer.WriteString(prefix);
		writer.WriteString(L"TEXT : ");
		if(node->text.value.Length()==4 && node->text.value.Sub(0, 2)==L"$(")
		{
			writer.WriteLine(node->text.value[2]);
		}
		else
		{
			writer.WriteLine(node->text.value);
		}
	}
};

/***********************************************************************
DefinitionToStringVisitor
***********************************************************************/

class DefinitionToStringVisitor : public Object, public FpmDefinition::IVisitor
{
public:
	WString						prefix;
	TextWriter&					writer;

	DefinitionToStringVisitor(const WString& _prefix, TextWriter& _writer)
		:prefix(_prefix)
		,writer(_writer)
	{
	}

	static void Do(Ptr<FpmDefinition> node, WString prefix, TextWriter& writer)
	{
		DefinitionToStringVisitor visitor(prefix, writer);
		node->Accept(&visitor);
	}

	void Visit(FpmExpressionDefinition* node)
	{
	}

	void Visit(FpmReferenceDefinition* node)
	{
	}
};

/***********************************************************************
Functional Macro Code Tree
***********************************************************************/

class Expression : public Object
{
public:
	typedef Ptr<Expression>			Ref;
	typedef List<Ref>				List;

	virtual void					ToString(const WString& prefix, TextWriter& writer)const=0;
	virtual RunningObject::Ref		CreateObject()=0;
};

class ConcatExpression : public Expression
{
public:
	Expression::List				expressions;

	void ToString(const WString& prefix, TextWriter& writer)const
	{
		writer.WriteString(prefix);
		writer.WriteLine(L"CONCAT {");
		for(int i=0;i<expressions.Count();i++)
		{
			expressions.Get(i)->ToString(prefix+L"    ", writer);
		}
		writer.WriteString(prefix);
		writer.WriteLine(L"}");
	}

	RunningObject::Ref CreateObject()
	{
		ConcatObject* result=new ConcatObject;
		for(int i=0;i<expressions.Count();i++)
		{
			result->objects.Add(expressions[i]->CreateObject());
		}
		return result;
	}
};

class ArrayExpression : public Expression
{
public:
	Expression::List				elements;

	void ToString(const WString& prefix, TextWriter& writer)const
	{
		writer.WriteString(prefix);
		writer.WriteLine(L"ARRAY {");
		for(int i=0;i<elements.Count();i++)
		{
			elements.Get(i)->ToString(prefix+L"    ", writer);
		}
		writer.WriteString(prefix);
		writer.WriteLine(L"}");
	}

	RunningObject::Ref CreateObject()
	{
		ArrayObject* result=new ArrayObject;
		for(int i=0;i<elements.Count();i++)
		{
			result->objects.Add(elements[i]->CreateObject());
		}
		return result;
	}
};

class InvokeExpression : public Expression
{
public:
	Expression::Ref					function;
	Expression::List				arguments;

	void ToString(const WString& prefix, TextWriter& writer)const
	{
		writer.WriteString(prefix);
		writer.WriteLine(L"INVOKE {");
		function->ToString(prefix+L"    ", writer);

		writer.WriteString(prefix+L"    ");
		writer.WriteLine(L"ARGUMENTS {");
		for(int i=0;i<arguments.Count();i++)
		{
			arguments.Get(i)->ToString(prefix+L"        ", writer);
		}
		writer.WriteString(prefix+L"    ");
		writer.WriteLine(L"}");

		writer.WriteString(prefix);
		writer.WriteLine(L"}");
	}

	RunningObject::Ref CreateObject()
	{
		InvokeObject* result=new InvokeObject;
		result->target=function->CreateObject();
		for(int i=0;i<arguments.Count();i++)
		{
			result->arguments.Add(arguments[i]->CreateObject());
		}
		return result;
	}
};

class ReferenceExpression : public Expression
{
public:
	WString							name;

	void ToString(const WString& prefix, TextWriter& writer)const
	{
		writer.WriteString(prefix);
		writer.WriteString(L"NAME : ");
		writer.WriteLine(name);
	}

	RunningObject::Ref CreateObject()
	{
		PredefinedObject::Type type=PredefinedObject::Unknown;
		bool found=true;
		if(name==L"$add")type=PredefinedObject::Add;
		else if(name==L"$sub")type=PredefinedObject::Sub;
		else if(name==L"$mul")type=PredefinedObject::Mul;
		else if(name==L"$div")type=PredefinedObject::Div;
		else if(name==L"$mod")type=PredefinedObject::Mod;
		else if(name==L"$lt")type=PredefinedObject::LT;
		else if(name==L"$le")type=PredefinedObject::LE;
		else if(name==L"$gt")type=PredefinedObject::GT;
		else if(name==L"$ge")type=PredefinedObject::GE;
		else if(name==L"$eq")type=PredefinedObject::EQ;
		else if(name==L"$ne")type=PredefinedObject::NE;
		else if(name==L"$if")type=PredefinedObject::If;
		else if(name==L"$loop")type=PredefinedObject::Loop;
		else if(name==L"$loopsep")type=PredefinedObject::LoopSep;
		else if(name==L"$head")type=PredefinedObject::Head;
		else if(name==L"$tail")type=PredefinedObject::Tail;
		else if(name==L"$length")type=PredefinedObject::Length;
		else if(name==L"$get")type=PredefinedObject::Get;
		else found=false;
		if(found)
		{
			PredefinedObject* result=new PredefinedObject;
			result->type=type;
			return result;
		}
		else
		{
			ReferenceObject* result=new ReferenceObject;
			result->name=name;
			return result;
		}
	}
};

class TextExpression : public Expression
{
public:
	WString							text;

	void ToString(const WString& prefix, TextWriter& writer)const
	{
		writer.WriteString(prefix);
		writer.WriteString(L"TEXT : ");
		writer.WriteLine(text);
	}

	RunningObject::Ref CreateObject()
	{
		TextObject* result=new TextObject;
		result->text=text;
		return result;
	}
};

class Definition : public Object
{
public:
	typedef Ptr<Definition>			Ref;

	virtual void					ToString(const WString& prefix, TextWriter& writer)const=0;
	virtual RunningObject::Ref		CreateObject(RunningObject::Environment::Ref environment)=0;
};

class ExpressionDefinition : public Definition
{
public:
	Expression::Ref					expression;

	void ToString(const WString& prefix, TextWriter& writer)const
	{
		expression->ToString(prefix, writer);
	}

	RunningObject::Ref CreateObject(RunningObject::Environment::Ref environment)
	{
		return expression->CreateObject();
	}
};

class ReferenceDefinition : public Definition
{
public:
	WString							name;
	List<WString>					parameters;
	List<Definition::Ref>			definitions;

	void ToString(const WString& prefix, TextWriter& writer)const
	{
		writer.WriteString(prefix);
		writer.WriteLine(L"REFERENCE {");
		writer.WriteString(prefix);
		writer.WriteString(L"    ");
		writer.WriteString(name);
		writer.WriteString(L" : ");
		for(int i=0;i<parameters.Count();i++)
		{
			if(i)writer.WriteString(L" , ");
			writer.WriteString(parameters.Get(i));
		}
		writer.WriteLine(L"");
		for(int i=0;i<definitions.Count();i++)
		{
			definitions.Get(i)->ToString(prefix+L"    ", writer);
		}
		writer.WriteString(prefix);
		writer.WriteLine(L"}");
	}

	RunningObject::Ref CreateObject(RunningObject::Environment::Ref environment)
	{
		ExecutableObject* result=new ExecutableObject;
		result->name=name;
		result->predefinedEnvironment=new RunningObject::Environment;
		for(int i=0;i<parameters.Count();i++)
		{
			result->parameters.Add(parameters[i]);
		}
		ConcatObject* concat=new ConcatObject;
		for(int i=0;i<definitions.Count();i++)
		{
			RunningObject::Ref item=definitions[i]->CreateObject(result->predefinedEnvironment);
			if(item)
			{
				if(concat->objects.Count())
				{
					TextObject* newLine=new TextObject;
					newLine->text=L"\r\n";
					concat->objects.Add(newLine);
				}
				concat->objects.Add(item);
			}
		}
		result->runningObject=concat;
		environment->objects.Add(name, result);
		return 0;
	}
};

class Macro : public Object
{
public:
	typedef Ptr<Macro>				Ref;

	List<Definition::Ref>			definitions;

	void ToString(const WString& prefix, TextWriter& writer)const
	{
		writer.WriteString(prefix);
		writer.WriteLine(L"MACRO {");;
		for(int i=0;i<definitions.Count();i++)
		{
			definitions.Get(i)->ToString(prefix+L"    ", writer);
		}
		writer.WriteString(prefix);
		writer.WriteLine(L"}");
	}

	RunningObject::Ref CreateObject()
	{
		ExecutableObject* result=new ExecutableObject;
		result->predefinedEnvironment=new RunningObject::Environment;
		ConcatObject* concat=new ConcatObject;
		for(int i=0;i<definitions.Count();i++)
		{
			RunningObject::Ref item=definitions[i]->CreateObject(result->predefinedEnvironment);
			if(item)
			{
				if(concat->objects.Count())
				{
					TextObject* newLine=new TextObject;
					newLine->text=L"\r\n";
					concat->objects.Add(newLine);
				}
				concat->objects.Add(item);
			}
		}
		result->runningObject=concat;
		return result;
	}
};

/***********************************************************************
Syntax Analyzer
***********************************************************************/

class SyntaxAnalyzer : public Object
{
protected:
	Node<StringInput<wchar_t>, WString>						PLAIN_TEXT, BRACKET_OPEN, BRACKET_CLOSE, ARRAY_OPEN, ARRAY_CLOSE, NAME, COMMA, COMMENT, STRING, NEW_LINE, ESCAPE, SPACE, OTHERS;
	Node<StringInput<wchar_t>, Ptr<Expression::List>>		exp_list;
	Node<StringInput<wchar_t>, Ptr<List<WString>>>			name_list;
	Node<StringInput<wchar_t>, Ptr<List<Definition::Ref>>>	def_list;
	Node<StringInput<wchar_t>, Ptr<ReferenceDefinition>>	ref_head;
	Rule<StringInput<wchar_t>, Expression::Ref>				exp, unit_exp, concat_exp, array_exp, invoke_exp, reference_exp, text_exp;
	Rule<StringInput<wchar_t>, Expression::Ref>				exp_nc, unit_exp_nc, concat_exp_nc, text_exp_nc;
	Rule<StringInput<wchar_t>, Definition::Ref>				def, exp_def, ref_def;
	Rule<StringInput<wchar_t>, Macro::Ref>					macro_start;

	static Expression::Ref ToText(const WString& input)
	{
		TextExpression* expression=new TextExpression;
		if(input[0]==L'$')
		{
			expression->text=input[2];
		}
		else
		{
			expression->text=input;
		}
		return expression;
	}

	static Expression::Ref ToReference(const WString& input)
	{
		ReferenceExpression* expression=new ReferenceExpression;
		expression->name=input;
		return expression;
	}

	static Expression::Ref ToConcat(const Ptr<Expression::List>& input)
	{
		if(input->Count()==1)
		{
			return input->Get(0);
		}
		else
		{
			ConcatExpression* expression=new ConcatExpression;
			CopyFrom(expression->expressions, *input.Obj());
			return expression;
		}
	}

	static Expression::Ref ToArray(const Ptr<Expression::List>& input)
	{
		ArrayExpression* expression=new ArrayExpression;
		CopyFrom(expression->elements, *input.Obj());
		return expression;
	}

	static Expression::Ref ToInvoke(const ParsingPair<Expression::Ref, Ptr<Expression::List>>& input)
	{
		InvokeExpression* expression=new InvokeExpression;
		expression->function=input.First();
		CopyFrom(expression->arguments,*input.Second().Obj());
		return expression;
	}

	static Expression::Ref ToBracket(const ParsingList<ParsingPair<Expression::Ref, ParsingList<WString>>>& input)
	{
		ConcatExpression* expression=new ConcatExpression;
		TextExpression* left=new TextExpression;
		left->text=L"(";
		expression->expressions.Add(left);
		if(input.Head())
		{
			expression->expressions.Add(input.Head()->Value().First());
			if(input.Head()->Value().Second().Head())
			{
				TextExpression* right=new TextExpression;
				right->text=L")";
				expression->expressions.Add(right);
			}
		}
		return expression;
	}

	static Definition::Ref ToExpDef(const Expression::Ref& input)
	{
		ExpressionDefinition* definition=new ExpressionDefinition;
		definition->expression=input;
		return definition;
	}

	static Ptr<ReferenceDefinition> ToRefDefHead(const ParsingPair<WString, Ptr<List<WString>>>& input)
	{
		ReferenceDefinition* definition=new ReferenceDefinition;
		definition->name=input.First();
		CopyFrom(definition->parameters,*input.Second().Obj());
		return definition;
	}

	static Definition::Ref ToRefDef(const ParsingPair<Ptr<ReferenceDefinition>, Ptr<List<Definition::Ref>>>& input)
	{
		CopyFrom(input.First()->definitions,*input.Second().Obj());
		return input.First();
	}

	static Macro::Ref ToMacro(const Ptr<List<Definition::Ref>>& input)
	{
		Macro* macro=new Macro;
		CopyFrom(macro->definitions, *input.Obj());
		return macro;
	}
public:
	typedef CombinatorException<StringInput<wchar_t>>		ExceptionType;

	SyntaxAnalyzer()
	{
		PLAIN_TEXT		= rgx(L"[^/$(), /t/r/n//\"]+");
		BRACKET_OPEN	= str(L"(");
		BRACKET_CLOSE	= str(L")");
		NAME			= rgx(L"/$[a-zA-Z_]/w*");
		COMMA			= str(L",");
		COMMENT			= rgx(L"(////[^/r/n]*|///*([^*]|/*+[^*//])*/*+//)");
		STRING			= rgx(L"\"([^\\\\\"]|\\\\\\.)*\"");
		NEW_LINE		= str(L"\r\n");
		ESCAPE			= rgx(L"/$/(/./)");
		SPACE			= rgx(L"[ /t]+");
		OTHERS			= rgx(L"[//\"]");

		exp_list		= list(opt(exp_nc + *(COMMA >> exp_nc)));
		name_list		= list(opt(NAME + *(*SPACE >> COMMA >> *SPACE >> NAME)));
		def_list		= list(+(*NEW_LINE >> def << *NEW_LINE));
		ref_head		= ((str(L"$$define")>>*SPACE>>NAME) + (*SPACE>>(BRACKET_OPEN >> name_list << BRACKET_CLOSE)))[ToRefDefHead]<<*SPACE;

		text_exp_nc		= (COMMENT | STRING | PLAIN_TEXT | ESCAPE | SPACE | OTHERS)[ToText];
		unit_exp_nc		= array_exp | invoke_exp | reference_exp | text_exp_nc | str(L"()")[ToText] | (BRACKET_OPEN >> opt(exp + opt(BRACKET_CLOSE)))[ToBracket];
		concat_exp_nc	= list(+unit_exp_nc)[ToConcat];
		exp_nc			= concat_exp_nc;

		array_exp		= (str(L"$array") >> BRACKET_OPEN >> exp_list << BRACKET_CLOSE)[ToArray];
		reference_exp	= NAME[ToReference];
		text_exp		= (COMMA | COMMENT | STRING | PLAIN_TEXT | ESCAPE | SPACE | OTHERS)[ToText];
		invoke_exp		= (reference_exp + (BRACKET_OPEN >> exp_list << BRACKET_CLOSE))[ToInvoke];
		unit_exp		= array_exp | invoke_exp | reference_exp | text_exp | str(L"()")[ToText] | (BRACKET_OPEN >> opt(exp + opt(BRACKET_CLOSE)))[ToBracket];
		concat_exp		= list(+unit_exp)[ToConcat];
		exp				= concat_exp;

		exp_def			= exp[ToExpDef];
		ref_def			= (ref_head + (list(loop(exp_def, 1, 1)) | (str(L"$$begin") >> def_list << str(L"$$end"))))[ToRefDef];
		def				= exp_def | ref_def;

		macro_start		= def_list[ToMacro];
	}

	Macro::Ref Parse(const WString& code)
	{
		return macro_start.ParseFull(code, false);
	}
};

/***********************************************************************
Interpretor
***********************************************************************/

void RunMacro(const WString& input, TextWriter& output, TextWriter& log, SyntaxAnalyzer& analyzer)
{
	try
	{
		Macro::Ref macro=analyzer.Parse(input);
		log.WriteLine(L"==============================================================================================");
		log.WriteLine(L"Syntax Tree");
		log.WriteLine(L"==============================================================================================");
		macro->ToString(L"", log);
		log.WriteLine(L"==============================================================================================");
		log.WriteLine(L"Running Result");
		log.WriteLine(L"==============================================================================================");

		RunningObject::Ref object=macro->CreateObject();
		RunningObject::Environment::Ref environment=new RunningObject::Environment;
		List<RunningObject::Ref> arguments;
		try
		{
			object=RunningObject::Invoke(object, environment, arguments);
			TextObject* textObject=dynamic_cast<TextObject*>(object.Obj());
			if(textObject)
			{
				output.WriteString(textObject->text);
			}
			else
			{
				log.WriteLine(L"运行结果不是一个字符串。");
			}
		}
		catch(const Exception& e)
		{
			log.WriteLine(e.Message());
		}
	}
	catch(const CombinatorException<StringInput<wchar_t>>& e)
	{
		log.WriteLine(L"==============================================================================================");
		log.WriteLine(L"Remain Text");
		log.WriteLine(L"==============================================================================================");
		log.WriteLine(e.GetInput().CurrentString());
		log.WriteLine(L"==============================================================================================");
		log.WriteLine(L"Errors");
		log.WriteLine(L"==============================================================================================");
		for(int i=0;i<e.GetGlobalInfo().errors.Count();i++)
		{
			log.WriteLine(e.GetGlobalInfo().errors.Get(i)->Message());
			log.WriteLine(e.GetGlobalInfo().errors.Get(i)->GetPosition().CurrentString());
			log.WriteLine(L"----------------------------------------------------------------------------------------------");
		}
		log.WriteLine(L"==============================================================================================");
		log.WriteLine(L"Candidate Errors");
		log.WriteLine(L"==============================================================================================");
		for(int i=0;i<e.GetGlobalInfo().candidateErrors.Count();i++)
		{
			log.WriteLine(e.GetGlobalInfo().candidateErrors.Get(i)->Message());
			log.WriteLine(e.GetGlobalInfo().candidateErrors.Get(i)->GetPosition().CurrentString());
			log.WriteLine(L"----------------------------------------------------------------------------------------------");
		}
	}
}

/***********************************************************************
Main function
***********************************************************************/

WString GetDirectory(const WString& fileName)
{
	int index=0;
	for(int i=0;i<fileName.Length();i++)
	{
		if(fileName[i]==L'\\')
		{
			index=i;
		}
	}
	return fileName.Left(index+1);
}

WString DereferenceMacroFile(const WString& inputPath)
{
	WString result;
	WString directory=GetDirectory(inputPath);

	FileStream inputFileStream(inputPath, FileStream::ReadOnly);
	BomDecoder inputDecoder;
	DecoderStream inputStream(inputFileStream, inputDecoder);
	StreamReader inputReader(inputStream);

	while(!inputReader.IsEnd())
	{
		WString line=inputReader.ReadLine();
		if(line.Length()>=10 && line.Left(10)==L"$$include:")
		{
			result+=DereferenceMacroFile(directory+line.Right(line.Length()-10));
		}
		else
		{
			result+=line+L"\r\n";
		}
	}
	return result;
}

extern WString GetBaseDirectory();

int wmain(int argc, wchar_t* argv[])
{
	WString baseDirectory=GetBaseDirectory();

	Console::SetTitle(L"Vczh Functional Macro for C++ Code Generation");
	Console::SetColor(false, true, false, true);
	Console::WriteLine(L"fpm>Files : "+itow(argc-1));
	SyntaxAnalyzer analyzer;
	for(int i=1;i<argc;i++)
	{
		Console::WriteLine(L"------------------------------------------------------------");
		WString inputPath=argv[i];
		if(inputPath.Length()<2 || inputPath[1]!=L':')
		{
			inputPath=baseDirectory+inputPath;
		}
		Console::WriteLine(L"fpm>Making : "+inputPath);
		if(inputPath.Length()<4 || inputPath.Right(4)!=L".fpm")
		{
			Console::SetColor(true, false, false, true);
			Console::WriteLine(L"error> The extenion name of the input file path must be \".fpm\".");
			Console::SetColor(false, true, false, true);
		}
		else
		{
			WString outputPath=inputPath.Left(inputPath.Length()-4);
			Console::WriteLine(L"fpm>Output path : "+outputPath);
			WString logPath=outputPath+L".log";
			Console::WriteLine(L"fpm>Log path : "+logPath);

			FileStream outputFileStream(outputPath, FileStream::WriteOnly);
			if(!outputFileStream.IsAvailable())
			{
				Console::SetColor(true, false, false, true);
				Console::WriteLine(L"error> \""+outputPath+L"\" can not be opened for writing.");
				Console::SetColor(false, true, false, true);
				continue;
			}
			BomEncoder outputEncoder(BomEncoder::Mbcs);
			EncoderStream outputStream(outputFileStream, outputEncoder);
			StreamWriter outputWriter(outputStream);

			FileStream logFileStream(logPath, FileStream::WriteOnly);
			BomEncoder logEncoder(BomEncoder::Mbcs);
			EncoderStream logStream(logFileStream, logEncoder);
			StreamWriter logWriter(logStream);

			RunMacro(DereferenceMacroFile(inputPath), outputWriter, logWriter, analyzer);
		}
	}
	Console::WriteLine(L"Finished!");
	return 0;
}