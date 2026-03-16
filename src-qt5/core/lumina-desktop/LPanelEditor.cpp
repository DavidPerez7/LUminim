//===========================================
//  Lumina-DE source code - Luminim Edition
//  Panel Editor Implementation
//===========================================
#include "LPanelEditor.h"
#include "LPanel.h"
#include "LSession.h"
#include <QDebug>
#include <QApplication>

LPanelEditor::LPanelEditor(QSettings *file, const QString &prefix, QWidget *panel) 
	: QObject(panel), settings(file), panelPrefix(prefix), panelWidget(panel), 
	  editModeActive(false), draggedWidget(nullptr), draggedPluginIndex(-1), isHorizontal(true) {
	// Constructor - inicializar el editor
	if(!settings || prefix.isEmpty() || !panel) {
		qWarning() << "LPanelEditor: Inicialización inválida";
	}
	isHorizontal = isHorizontalPanel();
}

LPanelEditor::~LPanelEditor() {
	// Destructor
}

void LPanelEditor::setEditMode(bool enabled) {
	if(enabled == editModeActive) return;
	editModeActive = enabled;
	
	if(enabled) {
		qDebug() << "Panel Editor: Modo edición ACTIVADO";
		setupEditorUI();
		updatePluginControls();
	} else {
		qDebug() << "Panel Editor: Modo edición DESACTIVADO";
		saveChanges();
	}
}

void LPanelEditor::addQuickLaunch(const QString &desktopFile) {
	if(desktopFile.isEmpty()) return;
	
	QStringList plugins = getPluginList();
	
	// Agregar nuevo quick launch al final
	QString newPlugin = "applauncher:::" + desktopFile;
	plugins << newPlugin;
	
	settings->setValue(panelPrefix + "pluginlist", plugins);
	qDebug() << "Panel Editor: Quick Launch agregado:" << desktopFile;
	
	saveChanges();
}

void LPanelEditor::removePlugin(const QString &pluginName) {
	QStringList plugins = getPluginList();
	
	for(int i = 0; i < plugins.length(); i++) {
		if(plugins[i].startsWith(pluginName)) {
			plugins.removeAt(i);
			qDebug() << "Panel Editor: Plugin removido:" << pluginName;
			break;
		}
	}
	
	settings->setValue(panelPrefix + "pluginlist", plugins);
	saveChanges();
}

void LPanelEditor::movePlugin(const QString &pluginName, int newIndex) {
	QStringList plugins = getPluginList();
	
	int oldIndex = -1;
	for(int i = 0; i < plugins.length(); i++) {
		if(plugins[i].startsWith(pluginName)) {
			oldIndex = i;
			break;
		}
	}
	
	if(oldIndex == -1 || newIndex < 0 || newIndex >= plugins.length()) return;
	
	QString plugin = plugins.takeAt(oldIndex);
	plugins.insert(newIndex, plugin);
	
	settings->setValue(panelPrefix + "pluginlist", plugins);
	qDebug() << "Panel Editor: Plugin movido de" << oldIndex << "a" << newIndex;
	
	saveChanges();
}

void LPanelEditor::resizePlugin(const QString &pluginName, int newSize) {
	if(newSize < 20 || newSize > 200) return; // Limites razonables
	
	settings->setValue(panelPrefix + pluginName + "/size", newSize);
	qDebug() << "Panel Editor: Plugin redimensionado:" << pluginName << "a" << newSize;
	
	saveChanges();
}

void LPanelEditor::setupEditorUI() {
	// Configurar UI minimalista para edición
	// Por ahora es un placeholder
	qDebug() << "Panel Editor: UI de edición configurada";
}

void LPanelEditor::updatePluginControls() {
	// Actualizar controles de los plugins en modo edición
	qDebug() << "Panel Editor: Controles de plugins actualizados";
}

void LPanelEditor::saveChanges() {
	settings->sync();
	qDebug() << "Panel Editor: Cambios guardados en configuración";
}

QStringList LPanelEditor::getPluginList() const {
	return settings->value(panelPrefix + "pluginlist", QStringList()).toStringList();
}

QString LPanelEditor::getLocationString() const {
	return settings->value(panelPrefix + "location", "top").toString();
}

bool LPanelEditor::isHorizontalPanel() const {
	QString loc = getLocationString().toLower();
	return (loc == "top" || loc == "bottom");
}

bool LPanelEditor::startDrag(const QPoint &pos) {
	if(!editModeActive) return false;
	
	draggedPluginIndex = getPluginAtPosition(pos);
	if(draggedPluginIndex >= 0) {
		dragStartPos = pos;
		qDebug() << "Panel Editor: Iniciando arrastre de plugin" << draggedPluginIndex;
		return true;
	}
	return false;
}

void LPanelEditor::updateDrag(const QPoint &pos) {
	if(draggedPluginIndex < 0) return;
	
	int distance = isHorizontal ? 
		(pos.x() - dragStartPos.x()) : 
		(pos.y() - dragStartPos.y());
	
	// Si ha movido lo suficiente, cambiar plugin
	if(qAbs(distance) > 30) {  // Threshold de 30 píxeles
		int newIndex = getPluginAtPosition(pos);
		if(newIndex >= 0 && newIndex != draggedPluginIndex) {
			swapPlugins(draggedPluginIndex, newIndex);
			draggedPluginIndex = newIndex;
			dragStartPos = pos;
		}
	}
}

void LPanelEditor::endDrag(const QPoint &pos) {
	if(draggedPluginIndex >= 0) {
		qDebug() << "Panel Editor: Arrastre completado";
		draggedPluginIndex = -1;
		dragStartPos = QPoint();
		saveChanges();
	}
}

int LPanelEditor::getPluginAtPosition(const QPoint &pos) const {
	// Placeholder - en una implementación real, esto calcularía 
	// la posición del plugin basándose en las coordenadas
	return -1;
}

void LPanelEditor::swapPlugins(int index1, int index2) {
	if(index1 < 0 || index2 < 0) return;
	
	QStringList plugins = getPluginList();
	if(index1 >= plugins.length() || index2 >= plugins.length()) return;
	
	plugins.swapItemsAt(index1, index2);
	settings->setValue(panelPrefix + "pluginlist", plugins);
	qDebug() << "Panel Editor: Plugins intercambiados" << index1 << index2;
}
