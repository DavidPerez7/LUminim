//===========================================
//  Lumina-DE source code - Luminim Edition
//  Panel Editor - Minimalista y Optimizado
//  Available under the 3-clause BSD license
//===========================================
//  Panel editor minimalista para permitir al usuario:
//  - Agregar Quick Launch shortcuts
//  - Mover plugins (drag & drop)
//  - Redimensionar plugins
//===========================================
#ifndef _LUMINA_DESKTOP_PANEL_EDITOR_H
#define _LUMINA_DESKTOP_PANEL_EDITOR_H

#include <QWidget>
#include <QBoxLayout>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>

class LPanelEditor : public QObject {
	Q_OBJECT
private:
	QSettings *settings;
	QString panelPrefix;
	QWidget *panelWidget;
	bool editModeActive;
	QWidget *draggedWidget;
	QPoint dragStartPos;
	int draggedPluginIndex;
	bool isHorizontal;

public:
	LPanelEditor(QSettings *file, const QString &prefix, QWidget *panel);
	~LPanelEditor();

	bool isEditMode() const { return editModeActive; }
	void setEditMode(bool enabled);

	// Operaciones del editor
	void addQuickLaunch(const QString &desktopFile);
	void removePlugin(const QString &pluginName);
	void movePlugin(const QString &pluginName, int newIndex);
	void resizePlugin(const QString &pluginName, int newSize);
	void swapPlugins(int index1, int index2);

	// Persistencia
	void saveChanges();
	QStringList getPluginList() const;
	
	// Drag & Drop
	bool startDrag(const QPoint &pos);
	void updateDrag(const QPoint &pos);
	void endDrag(const QPoint &pos);

private:
	void setupEditorUI();
	void updatePluginControls();
	QString getLocationString() const;
	int getPluginAtPosition(const QPoint &pos) const;
	bool isHorizontalPanel() const;

};

#endif
