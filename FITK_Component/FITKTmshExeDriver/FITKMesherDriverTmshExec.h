/**********************************************************************
 * @file   FITKMesherDriverTmshExec.h
 * @brief  Tmsh Exec网格驱动器
 * @date   2025-12-22
 *********************************************************************/
#ifndef __FITKMESHERDRIVERTMSHEXEC_H___
#define __FITKMESHERDRIVERTMSHEXEC_H___

#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"
#include <QObject>
#include <QPointer>

namespace GUI
{
	class GUITmshSettings;
}

namespace Tmsh
{
	class FITKMesherDriverTmshExec : public Interface::FITKAbstractMesherDriver
	{
		Q_OBJECT
	public:
		explicit FITKMesherDriverTmshExec();
		virtual ~FITKMesherDriverTmshExec();

		virtual void startMesher(QStringList info = QStringList()) override;
		virtual void stopMesher(QStringList info = QStringList()) override;

	protected:
		void tmshMeshGenSetting();
		void startTmshMeshGenerationOnce();
		QStringList buildSingleStageArgs() const;

	private slots:
		void writerModelFileDriven();
		void onSingleStageFinished();

	private:
		bool _pipelineEnabled{ false };
		QPointer<GUI::GUITmshSettings> _settingsDialog{};
	};
}

#endif
