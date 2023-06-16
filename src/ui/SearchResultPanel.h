/*
 * Copyright 2023, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef SearchResultPanel_H
#define SearchResultPanel_H

#include <ColumnListView.h>
#include <SupportDefs.h>
#include <TabView.h>
#include "GrepThread.h"

// For now this is specific to manage only the FindInFiles results
// Can be extended to handle more generic 'search' results (find references)

class SearchResultPanel : public BColumnListView {
public:
		SearchResultPanel();

		void StartSearch(BString command, BString projectPath);

		virtual void MessageReceived(BMessage* msg);
		virtual void	AttachedToWindow();
		

private:
		void	UpdateTabLabel();
		void	ClearSearch();
		void 	UpdateSearch(BMessage* msg);
		GrepThread*	fGrepThread;
		BString 	fProjectPath;
};


#endif // ProblemsPanel_H
