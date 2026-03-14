//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
//  This class is the interface to load all the different desktop plugins
//===========================================
#ifndef _LUMINA_DESKTOP_NEW_DESKTOP_PLUGIN_H
#define _LUMINA_DESKTOP_NEW_DESKTOP_PLUGIN_H

#include <QDebug>

//List all the individual plugin includes here
#include "LDPlugin.h"
//#include "SamplePlugin.h"
//#include "calendar/CalendarPlugin.h"  //Removed for minimal footprint
#include "applauncher/AppLauncherPlugin.h"
#include "desktopview/DesktopViewPlugin.h"
//#include "notepad/NotepadPlugin.h"  //Removed for minimal footprint
//#include "audioplayer/PlayerWidget.h"  //Removed for minimal footprint
//#include "systemmonitor/MonitorWidget.h"  //Removed for minimal footprint
//#include "quickcontainer/QuickDPlugin.h"
//#include "messagecenter/MessageCenter.h"
//#include "rssreader/RSSFeedPlugin.h"  //Removed for minimal footprint

class NewDP{
public:
	static LDPlugin* createPlugin(QString plugin, QWidget* parent=0){
	  //qDebug() << "Create Plugin:" << plugin;
	  LDPlugin *plug = 0;
	  /*if(plugin.section("---",0,0)=="sample"){
	    plug = new SamplePlugin(parent, plugin);
	  }else */
	  /*if(plugin.section("---",0,0)=="calendar"){  //Removed for minimal footprint
	    plug = new CalendarPlugin(parent, plugin);
	  }else*/
	  if(plugin.section("---",0,0).section("::",0,0)=="applauncher"){
	    //This plugin can be pre-initialized to a file path after the "::" delimiter
	    plug = new AppLauncherPlugin(parent, plugin);
	  }else if(plugin.section("---",0,0)=="desktopview"){
	    plug = new DesktopViewPlugin(parent, plugin);
	  }else{
	    qWarning() << "Invalid Desktop Plugin:"<<plugin << " -- Ignored";
	  }
	  //qDebug() << " -- done";
	  return plug;
	}

};

#endif
