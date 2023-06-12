/*
 * Copyright 2023, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _H_LSPClientWrapper
#define _H_LSPClientWrapper

#include <SupportDefs.h>
#include <Locker.h>
#include "LSPClient.h"
#include "json_fwd.hpp"
#include <atomic>
#include <thread>
#include <optional>
#include <OS.h>

class  LSPTextDocument;
struct TextDocumentContentChangeEvent;
struct Range;
struct Position;
struct CodeActionContext;
struct CompletionContext;
struct TweakArgs;
struct WorkspaceEdit;
struct FileEvent;
struct WorkspaceEdit;
struct ConfigurationSettings;
enum class TypeHierarchyDirection: int;

class LSPClientWrapper : public MessageHandler, public LSPClient {
	
public:
			LSPClientWrapper();
	virtual ~LSPClientWrapper() = default;
		
	bool	Create(const char* uri);
	bool	Dispose();
	
	void	RegisterTextDocument(LSPTextDocument* fw);
	void	UnregisterTextDocument(LSPTextDocument* fw);

    void onNotify(std::string method, value &params);
    void onResponse(RequestID ID, value &result);
    void onError(RequestID ID, value &error);
    void onRequest(std::string method, value &params, value &ID);

public:
    RequestID Initialize(option<DocumentUri> rootUri = {});
    RequestID Shutdown();
    RequestID Sync();
    void Exit();
    void Initialized(json& result);
    
    RequestID RegisterCapability();
    void DidOpen(MessageHandler*, DocumentUri uri, string_ref text, string_ref languageId = "cpp");
    void DidClose(MessageHandler*, DocumentUri uri);
    void DidChange(MessageHandler*, DocumentUri uri, std::vector<TextDocumentContentChangeEvent> &changes,
                   option<bool> wantDiagnostics = {});
    void DidSave(MessageHandler*, DocumentUri uri);    
    RequestID RangeFomatting(MessageHandler*, DocumentUri uri, Range range);
    RequestID FoldingRange(MessageHandler*, DocumentUri uri);
    RequestID SelectionRange(MessageHandler*, DocumentUri uri, std::vector<Position> &positions);
    RequestID OnTypeFormatting(MessageHandler*, DocumentUri uri, Position position, string_ref ch);
    RequestID Formatting(MessageHandler*, DocumentUri uri);
    RequestID CodeAction(MessageHandler*, DocumentUri uri, Range range, CodeActionContext context);
    RequestID Completion(MessageHandler*, DocumentUri uri, Position position, CompletionContext& context);
    RequestID SignatureHelp(MessageHandler*, DocumentUri uri, Position position);
    RequestID GoToDefinition(MessageHandler*, DocumentUri uri, Position position);    
    RequestID GoToImplementation(MessageHandler*, DocumentUri uri, Position position);    
    RequestID GoToDeclaration(MessageHandler*, DocumentUri uri, Position position);
    RequestID References(MessageHandler*, DocumentUri uri, Position position);
    RequestID SwitchSourceHeader(MessageHandler*, DocumentUri uri);
    RequestID Rename(MessageHandler*, DocumentUri uri, Position position, string_ref newName);
    RequestID Hover(MessageHandler*, DocumentUri uri, Position position);
    RequestID DocumentSymbol(MessageHandler*, DocumentUri uri);
    RequestID DocumentColor(MessageHandler*, DocumentUri uri);
    RequestID DocumentHighlight(MessageHandler*, DocumentUri uri, Position position);
    RequestID SymbolInfo(MessageHandler*, DocumentUri uri, Position position);
    RequestID TypeHierarchy(MessageHandler*, DocumentUri uri, Position position, TypeHierarchyDirection direction, int resolve);
    RequestID DocumentLink(MessageHandler* fw, DocumentUri uri);
    RequestID WorkspaceSymbol(MessageHandler*, string_ref query);
    //RequestID ExecuteCommand(MessageHandler*, string_ref cmd, option<TweakArgs> tweakArgs = {}, option<WorkspaceEdit> workspaceEdit = {});
    RequestID DidChangeWatchedFiles(MessageHandler*, std::vector<FileEvent> &changes);
    RequestID DidChangeConfiguration(MessageHandler*, ConfigurationSettings &settings);
 

    RequestID 	SendRequest(RequestID id, string_ref method, value params = json());
    void 		SendNotify(string_ref method, value params = json());
    
    std::string&	allCommitCharacters() { return fAllCommitCharacters; }
    std::string&	triggerCharacters() { return fTriggerCharacters; }

private:

	static int32 thread_func(void *data);

	LSPTextDocument*	_DocumentByURI(const std::string& uri);

	typedef std::map<std::string, LSPTextDocument*> MapFile;

	MapFile	fTextDocs;

	std::atomic<bool> fInitialized;
	std::atomic<bool> fOnEroor;

	thread_id	fReaderThread;
	
	std::string fAllCommitCharacters;
	std::string fTriggerCharacters;
	
	std::string fRootURI;

};


#endif // _H_LSPClientWrapper
