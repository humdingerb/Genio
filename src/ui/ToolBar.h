/*
 * Copyright 2018 Kacper Kasper <kacperkasper@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef TOOLBAR_H
#define TOOLBAR_H


#include <Control.h>
#include <ToolBar.h>

#include <string>
#include <unordered_map>


class BHandler;


class ToolBar : public BPrivate::BToolBar {
public:
					ToolBar(BHandler* defaultTarget = nullptr);

	virtual void	Draw(BRect updateRect);

	void			AddAction(uint32 command, const char* toolTipText,
						const char* iconName = nullptr, bool lockable = false);

	void			AddAction(BMessage* msg, const char* toolTipText,
						const char* iconName = nullptr, bool lockable = false);

	void			ChangeIconSize(float newSize);

	void			SetEnabled(bool enable);

	void			SetTarget(BHandler* defaultTarget);

	void			ToggleActionPressed(uint32 command);

private:
	BHandler*		fDefaultTarget;
	float			fIconSize;
	// command, iconName
	std::unordered_map<uint32, std::string> fActionIcons;
};


#endif // TOOLBAR_H
