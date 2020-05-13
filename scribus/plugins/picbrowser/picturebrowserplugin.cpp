/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "picturebrowserplugin.h"

// See scplugin.h and pluginmanager.{cpp,h} for detail on what these methods
// do. That documentatation is not duplicated here.
// Please don't implement the functionality of your plugin here; do that
// in mypluginimpl.h and mypluginimpl.cpp .

PictureBrowserPlugin::PictureBrowserPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

PictureBrowserPlugin::~PictureBrowserPlugin()
{
}

bool PictureBrowserPlugin::cleanupPlugin()
{
	closePictureBrowser();
	return true;
}

void PictureBrowserPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "Picture Browser";
	// Action text for menu, including &accel
	m_actionInfo.text = tr ( "&Picture Browser..." );
	// Menu
	m_actionInfo.menu = "Extras";
	// If needed, what item to add the menu item after
	m_actionInfo.menuAfterName = "extrasManageImages";
	// If needed, the keyboard shortcut for the plugin
	//m_actionInfo.keySequence = "CTRL+ALT+F3"
	// Should the menu item be enabled when the app starts
	// (even without a document open) ?
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.needsNumObjects = -1;
}

QString PictureBrowserPlugin::fullTrName() const
{
	return QObject::tr ( "Picture Browser" );
}

const ScActionPlugin::AboutData* PictureBrowserPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR ( about );
	return about;
}

void PictureBrowserPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT ( about );
	delete about;
}

bool PictureBrowserPlugin::run(ScribusDoc* doc, const QString& target)
{
	//picturebrowser isn't running yet, so create it
	if (!m_pictureBrowser)
	{
		m_pictureBrowser = new PictureBrowser(doc, nullptr);
		if (!m_pictureBrowser)
			return false;

		connect(m_pictureBrowser, SIGNAL (destroyed()), this, SLOT (pictureBrowserClosed()));
	}
	//picturebrowser exists, just bring it to the top
	else
	{
		m_pictureBrowser->hide();
	}

	Q_CHECK_PTR (m_pictureBrowser);

	m_pictureBrowser->setAttribute ( Qt::WA_DeleteOnClose );
	//show, get on top and activate window
	m_pictureBrowser->show();
	m_pictureBrowser->raise();
	m_pictureBrowser->activateWindow();

	return true;
}

void PictureBrowserPlugin::setDoc(ScribusDoc* doc)
{
	if (m_pictureBrowser)
		m_pictureBrowser->changedDocument(doc);
}

void PictureBrowserPlugin::unsetDoc()
{
	if (m_pictureBrowser)
		m_pictureBrowser->closedDocument();
}

void PictureBrowserPlugin::changedDoc(ScribusDoc* doc)
{
//	if (m_pictureBrowser)
//		m_pictureBrowser->changedDocument (doc);
}

void PictureBrowserPlugin::closePictureBrowser()
{
	if (m_pictureBrowser)
	{
		if (m_pictureBrowser->isVisible())
			m_pictureBrowser->close();
		delete m_pictureBrowser;
		m_pictureBrowser = nullptr;
	}
}

void PictureBrowserPlugin::pictureBrowserClosed()
{
	m_pictureBrowser = nullptr;
}

// Low level plugin API
int picturebrowser_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* picturebrowser_getPlugin()
{
	PictureBrowserPlugin* plug = new PictureBrowserPlugin();
	Q_CHECK_PTR ( plug );
	return plug;
}

void picturebrowser_freePlugin(ScPlugin* plugin)
{
	PictureBrowserPlugin* plug = dynamic_cast<PictureBrowserPlugin*> ( plugin );
	Q_ASSERT ( plug );
	delete plug;
}
