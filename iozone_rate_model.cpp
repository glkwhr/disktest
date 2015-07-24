#include "iozone_rate_model.h"

iozoneRateModel::iozoneRateModel(QObject *parent) :
    QObject(parent)
{
}

void iozoneRateModel::setRawRateData(const RawRateData &inputData)
{   /* 根据底层数据输入设置model数据 */
    //beginResetModel();/* 通知关联的view即将重置数据 */
    rawData.resize(0);
    rawData = inputData;/* 重置数据 */
    //endResetModel();
}

void iozoneRateModel::addRawRateData(int fsType, long long reclen, int rateType, unsigned long long rate)
{
    int row;
    row = (int)log2( (double)reclen)-2;
    if ( rawData.count() <= fsType )
        rawData.resize(fsType+1);
    if ( rawData[fsType].count() <= row )
        rawData[fsType].resize(row+1);
    if ( rawData[fsType][row].count() <= rateType )
        rawData[fsType][row].resize(rateType+1);
    rawData[fsType][row][rateType] = rate;
}

int iozoneRateModel::rowCount(int fsType) const
{   /* 获得指定文件系统测试结果行数 */
    if ( rawData.count() <= fsType ) {
        /* 该文件系统的数据还未录入 */
        return -1;
    }
    else
        return rawData[fsType].count();
}

int iozoneRateModel::columnCount(int fsType) const
{   /* 获得指定文件系统测试结果列数 */
    if ( rawData.count() <= fsType ) {
        return -1;
    }
    else if ( rawData[fsType].count() == 0 ) {
        return -1;
    }
    return rawData[fsType][0].count();
}

unsigned long long iozoneRateModel::data(int fsType, int row, int column) const
{   /* 获得指定文件系统测试结果指定位置的数据 */
    /* 返回0的时候表示该项数据不存在或还未测得 */
    if ( rawData.count() <= fsType )
        return 0;
    else if ( rawData[fsType].count() <= row )
        return 0;
    else if ( rawData[fsType][row].count() <= column )
        return 0;
    /* 以上皆为该位置数据不存在情况 */
    else
        return rawData[fsType][row][column];
}
