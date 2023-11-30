/*
 * Copyright 2023 Nexus6 <nexus6.haiku@icloud.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 * Based on TrackGit (https://github.com/HaikuArchives/TrackGit)
 * Original author: Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 * Copyright Hrishikesh Hiraskar and other HaikuArchives contributors (see GitHub repo for details)
 */

#pragma once

#include <SupportDefs.h>
#include <Catalog.h>
#include <Notification.h>

#include <git2.h>

#include <cstdio>
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>

#include "GException.h"
#include "Log.h"
#include "Utils.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "GitRepository"

using namespace std;
using namespace std::filesystem;

namespace Genio::Git {

	const int CANCEL_CREDENTIALS = -123;

	enum PullResult {
		UpToDate,
		FastForwarded,
		Merged
	};

	// Git exceptions

	class GitException : public GException {
	public:

		GitException(int error, BString const& message)
			:
			GException(error, message),
			_error(error),
			_message(message)
		{
			LogError("GitException %s, error = %d" , message.String(), error);
		}

		int							Error() noexcept { return _error; }
		BString						Message() noexcept { return _message; }

	private:
		int							_error;
		BString						_message;
	};


	class GitConflictException : public GitException {
	public:

		GitConflictException(int error, BString const& message,
			std::vector<std::string> files)
			:
			GitException(error, message),
			_files(files)
		{
			LogError("GitConflictException %s, error = %d, files = %d" , message.String(), error, files.size());
		}

		std::vector<std::string>	GetFiles() noexcept { return _files; }

	private:
		std::vector<std::string> 	_files;
	};


	class GitRepository {
	public:

		typedef vector<pair<string, string>> RepoFiles;

		// Payload to search for merge branch.
		struct fetch_payload {
			char branch[100];
			git_oid branch_oid;
		};


										GitRepository(BString path);
										~GitRepository();

		const BPath&					Clone(const string& url, const BPath& localPath,
												git_indexer_progress_cb callback,
												git_credential_acquire_cb authentication_callback);

		void							Init(BString localPath);

		vector<BString>					GetTags();

		vector<BString>					GetBranches(git_branch_t type = GIT_BRANCH_LOCAL);
		int								SwitchBranch(BString branch);
		BString							GetCurrentBranch();
		void							DeleteBranch(BString branch, git_branch_t type);
		void							RenameBranch(BString old_name, BString new_name,
											git_branch_t type);
		void							CreateBranch(BString existingBranchName,
											git_branch_t type, BString newBranchName);

		void							Fetch(bool prune = false);
		void							Merge(BString source, BString dest);
		PullResult						Pull(BString branchName);
		void 							PullRebase();
		void 							Push();

		git_signature*					_GetSignature();

		void 							StashSave(BString message);
		void 							StashPop();
		void 							StashApply();

		RepoFiles						GetFiles();

	private:

		git_repository 					*fRepository;
		BString							fRepositoryPath;

		BString							_ConfigGet(git_config *cfg,
											const char *key);
		void							_ConfigSet(git_config *cfg,
											const char *key, const char* value);

		int 							check(int status,
											std::function<void(void)> execute_on_fail = nullptr,
											std::function<bool(const int)> custom_checker = nullptr);

		int 							_FastForward(const git_oid *target_oid, int is_unborn);
		int								_CreateCommit(git_index* index, const char* message);

	};

}