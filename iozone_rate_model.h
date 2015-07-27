#ifndef IOZONE_RATE_MODEL_H
#define IOZONE_RATE_MODEL_H

#include <QObject>
#include <QVector>
#include <cmath>

#ifndef INSERT_TYPE
#define INSERT_TYPE
#define INSERT_TYPE_UPDATE 0
#define INSERT_TYPE_RESET 1
#endif

#ifndef OUTPUT_TYPE
#define OUTPUT_TYPE
#define OUTPUT_TYPE_WRITE 0
#define OUTPUT_TYPE_REWRITE 1
#define OUTPUT_TYPE_READ 2
#define OUTPUT_TYPE_REREAD 3
#define OUTPUT_TYPE_RANDREAD 4
#define OUTPUT_TYPE_RANDWRITE 5
#endif

/* 第一维文件系统类型 第二维块大小 第三维速率类型
    rawData[fsType]["reclen"(row)][rateType(column)]
*/
typedef QVector<QVector<QVector<unsigned long long> > > RawRateData;

/*
 *  存储iozone测试结果的数据模型
*/
class iozoneRateModel : public QObject
{
    Q_OBJECT
public:
    explicit iozoneRateModel(QObject *parent = 0);
    void setRawRateData(const RawRateData &inputData);
    /* rateType即outputType */
    void addRawRateData(int fsType, long long reclen, int rateType, unsigned long long rate);
    int rowCount(int fsType) const;/* 不同块大小种类数 */
    int columnCount(int fsType) const;
    unsigned long long data(int fsType, int row, int column) const;/* 不存在时返回0 */

private:
    RawRateData rawData;
};

#endif // IOZONE_RATE_MODEL_H
