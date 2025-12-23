#include "FITKAbstractMainWinGenerator.h"
#include "FITKGlobalData.h"
#include "FITKAppFramework.h"
#include "FITKAppSettings.h"
#include <QFile>
#include <QApplication>
#include <QTranslator>
#include <QDebug>

namespace AppFrame
{

    bool FITKAbstractMainwindowGenerator::loadQssStyle()
    {
        //打开qss文件
        QString styleFile = this->getQssStyle();
        if (styleFile.isEmpty()) return false;
        QFile qssFile(styleFile);
        if (!qssFile.open(QFile::ReadOnly)) return false;
        //读取qss
        const QString q = qssFile.readAll();
        qssFile.close();
        //加载qss
        qApp->setStyleSheet(q);
        return true;
    }

    bool FITKAbstractMainwindowGenerator::loadTranslatorQmFiles()
    {
        FITKAppSettings* settings = FITKAPP->getAppSettings();
        if (settings != nullptr)
        {
            bool loadZHCN = settings ->isLoadZHCN();
            if (!loadZHCN) return false;
        }

        //获取全部翻译文件
        QStringList qmFiles = this->getTranslatorQmFiles();
        for (QString qmfile : qmFiles)
        {
            //遍历加载翻译文件
            if(qmfile.isEmpty()) continue;
            QTranslator* translator = new QTranslator(this);
            bool ok = translator->load(qmfile);
            //加载失败则报错，释放translator
            //加载翻译
            if (ok)
            {
                qApp->installTranslator(translator);
            }
            
            
            if (!ok)
            {
                qDebug() << QString("Failed to load lang file: %1").arg(qmfile);
                delete translator;
                continue;
            }
            
        }
        return true;
    }

    QWidget* FITKAbstractMainwindowGenerator::genMainWindow()
    {
        //虚函数错误值
        return nullptr;
    }

    void FITKAbstractMainwindowGenerator::showMaximize(bool max)
    {
        //最大化
        _showMaximize = max;
    }

    bool FITKAbstractMainwindowGenerator::showMaximize()
    {
        return _showMaximize;
    }

    void FITKAbstractMainwindowGenerator::showWelcomePage()
    {

    }

    bool FITKAbstractMainwindowGenerator::showPreSettingPage()
    {
        return true;
    }

   
    QString FITKAbstractMainwindowGenerator::getQssStyle()
    {
        return QString();
    }

    QStringList FITKAbstractMainwindowGenerator::getTranslatorQmFiles()
    {
        return QStringList();
    }


}
