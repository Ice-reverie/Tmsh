/**********************************************************************
 * @file   FITKMesherDriverTmshExec.h
 * @brief  Tmsh Exec 网格驱动器（单阶段版，同时支持 Tmsh & TetGen）
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
    ~FITKMesherDriverTmshExec();

    void startMesher(QStringList info = QStringList()) override;
    void stopMesher(QStringList info = QStringList()) override;

protected:
    /* 弹窗入口 */
    void tmshMeshGenSetting();
    void tetgenMeshGenSetting();

    /* 选择算法后统一路由 */
    void runMesher(int method);

private slots:
    /* 单阶段完成回调 */
    void onSingleStageFinished();

private:
    /* 界面缓存 */
    QPointer<GUI::GUITmshSettings>   _settingsDialog;
    QPointer<GUI::GUITetGenSettings> _tetgenSettingsDialog;

    /* 单阶段公共逻辑 */
    void writerModelFileTmshSingle();
    void writerModelFileTetGenSingle();
    void exportGeometryAndSetPaths();
    void startMeshGenerationOnce();
    QStringList buildSingleStageArgs() const;
};
} // namespace Tmsh

#endif // __FITKMESHERDRIVERTMSHEXEC_H___
