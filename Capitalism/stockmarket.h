#ifndef STOCKMARKET_H
#define STOCKMARKET_H
#include <QObject>
#include <vector>


class StockMarket : public QObject
{
    Q_OBJECT
public:
    explicit StockMarket(QObject *parent = nullptr);
    ~StockMarket();
    std::vector<std::string> process(std::string &indexCore );
signals:
private:
    std::vector<std::string> stockAndPrice_v;

};

#endif // STOCKMARKET_H
