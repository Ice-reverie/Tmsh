#include "FITKVarientParams.h"


namespace Core
{

    void FITKVarientParams::setValue(const QString& k, const QVariant v)
    {
        // 使用_insert方法将键k和值v插入到_settings哈希表中。
        _settings.insert(k, v);
    }

    QVariant FITKVarientParams::getValue(const QString& k) const
    {
        // 直接调用_settings哈希表的value方法，传入键k，返回与其对应的QVariant值。
        // 如果键k不存在，则可能返回一个默认初始化的QVariant。
        return _settings.value(k);
    }

    void FITKVarientParams::removeVarient(const QString & k)
    {
        _settings.remove(k);
    }

    void FITKVarientParams::clear()
    {
        _settings.clear();
    }

}