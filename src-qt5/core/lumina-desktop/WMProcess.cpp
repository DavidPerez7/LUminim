//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "WMProcess.h"
#include "LSession.h"

WMProcess::WMProcess() : QProcess(){
  connect(this,SIGNAL(finished(int, QProcess::ExitStatus)),this,SLOT(processFinished(int, QProcess::ExitStatus)) );
  this->setProcessChannelMode(QProcess::MergedChannels);
  QString log = QString(getenv("XDG_CONFIG_HOME"))+"/lumina-desktop/logs/wm.log";
  if(QFile::exists(log)){ QFile::remove(log); }
  this->setStandardOutputFile(log);
  //ssaver = new QProcess(0);
  inShutdown = false;
}

WMProcess::~WMProcess(){

}

// =======================
//    PUBLIC FUNCTIONS
// =======================
void WMProcess::startWM(){
  inShutdown = false;
  QString cmd = setupWM();
  if(!isRunning()){this->start(cmd); }
  /*if(ssaver->state() == QProcess::NotRunning  \
	&&  LSession::handle()->sessionSettings()->value("WindowManager", "fluxbox").toString() != "lumina-wm"){
     ssaver->start("xscreensaver -no-splash");
  }*/
}

void WMProcess::stopWM(){
  if(isRunning()){
    inShutdown = true;
    this->kill();
    if(!this->waitForFinished(10000)){ this->terminate(); };
  }else{
    qWarning() << "WM already closed - did it crash?";
  }
}

void WMProcess::restartWM(){
  qDebug() << "Restarting WM";
  if(isRunning()){
    inShutdown = true;
    this->kill();
    if(!this->waitForFinished(5000) ){ this->terminate(); };
    inShutdown = false;
  }
  this->startWM();
}

void WMProcess::updateWM(){
  if(isRunning()){
    qDebug() << "Updating WM (Openbox)";
    ::kill(this->pid(), SIGHUP); //send openbox the signal to reconfigure (SIGHUP)
  }
}
// =======================
//    PRIVATE FUNCTIONS
// =======================
bool WMProcess::isRunning(){
  return (this->state() != QProcess::NotRunning);
}

QString WMProcess::setupWM(){
  QString confDir = QString( getenv("XDG_CONFIG_HOME"))+"/lumina-desktop";
  if(!QDir(confDir).exists()){ QDir().mkpath(confDir); }
  
  // LUminim Engine: Openbox Only
  QString cmd = "openbox --config-file "+confDir+"/openbox-rc.xml";
  return cmd;
}

void WMProcess::cleanupConfig(){
  //QString confDir = QDir::homePath()+"/.config/openbox";
  //if(!QFile::exists(confDir+"/rc.xml")){ return; } //Make sure that there is a current config file
  //if(QFile::exists(confDir+"/lumina-rc.xml")){ QFile::remove(confDir+"/lumina-rc.xml"); }
  //QFile::rename(confDir+"/rc.xml",confDir+"/lumina-rc.xml");
  //if(QFile::exists(confDir+"/openbox-rc.xml")){ QFile::rename(confDir+"/openbox-rc.xml",confDir+"/rc.xml"); }
}
// =======================
//     PRIVATE SLOTS
// =======================
void WMProcess::processFinished(int exitcode, QProcess::ExitStatus status){
  if(!inShutdown){
    if(exitcode == 0 && status == QProcess::NormalExit){
      cleanupConfig();
      emit WMShutdown();
    }else{
      //restart the Window manager
      qDebug() << "WM Stopped Unexpectedly: Restarting it...";
      this->startWM();
    }
  }else{
    cleanupConfig();
  }
}
