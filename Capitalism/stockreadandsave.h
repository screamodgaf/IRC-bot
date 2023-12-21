#ifndef STOCKREADANDSAVE_H
#define STOCKREADANDSAVE_H

#include <QObject>

class StockReadAndSave : public QObject
{
    Q_OBJECT
public:
    explicit StockReadAndSave(QObject *parent = nullptr);
    std::vector<std::vector<std::string> > getStockVec(std::string& line);
    ~StockReadAndSave();
    std::string embraceAllStocks(std::string jointEntry);
        std::string makeSingleStockEntry(std::vector<std::vector<std::string>> singleUserStocks_v);
signals:
private:

    std::string separateBy_vertSign(std::string& allUserLine);
    void separateByComas(std::string& extractedEntry);
    void separateByMonkey(std::string& colectiveLine);
    std::string findNick(std::string& fullUSerLine);
    void passLineFromCapitalistTXT(std::string& line);
private:
    std::vector<std::vector<std::string>> singleUserStocks_v;


};

#endif // STOCKREADANDSAVE_H
