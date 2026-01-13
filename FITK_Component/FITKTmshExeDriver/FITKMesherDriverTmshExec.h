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
    class GUITetGenSettings;
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
        void tetgenMeshGenSetting();

        void startTmshMeshGenerationDriven(int stage);
        QStringList buildStageArgs(int stage) const;

	private slots:
		void writerModelFileDriven();
		void onSingleStageFinished();

    private:
        bool _pipelineEnabled{ false };
        int _runningStage{ 0 }; // 0: idle, 1: stage1, 2: stage2
        QPointer<GUI::GUITmshSettings> _settingsDialog{};

        QPointer<GUI::GUITetGenSettings> _tetgenSettingsDialog{};

    };
}

#endif
