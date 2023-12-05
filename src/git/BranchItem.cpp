/*
 * Copyright 2018, Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "BranchItem.h"

BranchItem::BranchItem(const char* text,
						uint32 branchType,
						uint32 outlineLevel,
						bool expanded,
						const char *iconName)
	:
	StyledItem(text, outlineLevel, expanded, iconName),
	fBranchType(branchType)
{
}


/* virtual */
BranchItem::~BranchItem()
{
}


uint32
BranchItem::BranchType() const
{
	return fBranchType;
}

