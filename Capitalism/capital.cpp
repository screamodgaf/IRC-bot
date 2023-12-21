#include "capital.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>
Capital::Capital(CapitalismStorage* capitalismStorage_)
{
    money =0.0; //calls at the beginning when the program starts x number of entries
    factory = "";
    rank = 0;
    capitalismStorage =capitalismStorage_;
    gainTotal = 0.f;
}

void Capital::setNick(std::string nick_)
{
    std::cout << termcolor::red << "setNick: nick_: " << nick_ << termcolor::reset<< std::endl;
    nick = nick_;
}

void Capital::setMoney(double money_)
{
    std::cout << termcolor::red << "setMoney: money_: " << money_ << termcolor::reset<< std::endl;
    money += money_;
}

void Capital::setRank(int rank_)
{
    std::cout << termcolor::red << "setRank: rank_: " << rank_ << termcolor::reset<< std::endl;
    rank += rank_;
}

void Capital::setAllStocks(std::vector<std::vector<std::string>>& singleUserStocks_v_) //initially
{
    singleUserStocks_v = singleUserStocks_v_;
    std::cout << termcolor::on_blue  << "singleUserStocks_v_.size: " <<this->getNick() << " " << singleUserStocks_v_.size()  << termcolor::reset<< std::endl;

}

void Capital::addNewStock(std::string newStockName, int amountBought, std::string boughtPrice)
{
    //std::vector<std::vector<std::string>> singleUserStocks_v;
    std::sort(singleUserStocks_v.begin(), singleUserStocks_v.end());
    singleUserStocks_v.push_back({newStockName, std::to_string(amountBought), boughtPrice });
}



std::string Capital::sellStock(std::string sellStockName, double amountSold, double currentPrice)
{
    int originalAmountSold = amountSold;
    int nrStocksToSellInAnotherIteration = 0;
    if(amountSold <=0) return ""; //troll

    double pastPrize;
    double moneyForSoldStocksInThisItrton = 0.f;
    int index = 0;
    std::cout << termcolor::green << "singleUserStocks_v.size(): " << singleUserStocks_v.size() << termcolor::reset<< std::endl;

    for (int i = 0; i < singleUserStocks_v.size(); ++i)
    {
        double gain =0;
        //        std::cout << "current stock: " << singleUserStocks_v[i][0] << std::endl;
        //        std::cout << "number of current stocks: " << singleUserStocks_v[i][1] << std::endl;

        std::cout << termcolor::cyan << sellStockName  << " == " << singleUserStocks_v[i][0] << termcolor::reset<< std::endl;
        if(sellStockName == singleUserStocks_v[i][0])  //stock name matches
        {

            int stocksPossesed = std::stoi(singleUserStocks_v[i][1]); //number of stocks of this kind in possesion
            if(stocksPossesed<= 0) {
                std::cout << termcolor::red << "SKIPPING stocksPossesed<= continue " << "\n" << termcolor::reset<< std::endl;
                continue;
            }

            std::cout << termcolor::red << "amountSold: " << amountSold << termcolor::reset<< std::endl;
            std::cout << termcolor::red << "stocksPossesed: " << stocksPossesed << termcolor::reset<< std::endl;


            pastPrize = std::stod(singleUserStocks_v[i][2]); //stock prize from file

            // nrStocksToSellInAnotherIteration = -5

            nrStocksToSellInAnotherIteration = stocksPossesed - amountSold; //from the oryginal amount of stocks we wanted to sell, substract the number of sold stocks in this iteration, so in the next interation less stocks will need to be sold
            std::cout << termcolor::red << "nrStocksToSellInAnotherIteration<=0: " << nrStocksToSellInAnotherIteration << termcolor::reset<< std::endl;

            if(stocksPossesed - amountSold <=0)
            {
                singleUserStocks_v.erase(singleUserStocks_v.begin()+i);
                i--; // KEY LINE - if you remove something from within loop, which reduces the size (singleUserStocks_v.size()) - you have to go 1 back, or the loop will end
                std::cout << "singleUserStocks_v.size(): " << singleUserStocks_v.size() << std::endl;
                //in this condition possesedMinusSold = the total number of stocks it had (cause we delete all entry)
            }
            else //having more stocks in this entry, then the sold number, so no need for further checking
            {
                singleUserStocks_v[i][1] = std::to_string(stocksPossesed - amountSold);
            }

            int numberOfStocksSoldInTHISiteration;
            if(stocksPossesed - amountSold >=0) numberOfStocksSoldInTHISiteration = amountSold;
            else numberOfStocksSoldInTHISiteration = stocksPossesed;


            amountSold = std::abs(nrStocksToSellInAnotherIteration); //so in next iteration we ll remove less than the original number of stocks to sell

            // price:
            std::cout << "PRIZE" << "\n";

            //std::cout << termcolor::red << "stockSoldFromEachEntry: " << stockSoldFromEachEntry << termcolor::reset<< std::endl;
            gain += (currentPrice - pastPrize)* numberOfStocksSoldInTHISiteration;
            gainTotal +=gain;
            postAboutGainFromEachEntry += ( "zarobiłeś " + std::to_string(gain) + " na sprzedaży " + std::to_string(numberOfStocksSoldInTHISiteration) + " " + sellStockName + " ");



            std::cout << termcolor::magenta << "moneyForSoldStocksInThisItrton: " << moneyForSoldStocksInThisItrton << " currentPrice: " << currentPrice << " numberOfStocksSoldInTHISiteration: " << numberOfStocksSoldInTHISiteration << termcolor::reset<< std::endl;

            moneyForSoldStocksInThisItrton += currentPrice*numberOfStocksSoldInTHISiteration;
            numberOfStocksSoldInTHISiteration = 0; //zeroing for a next iteration
            //if no nrStocksToSellInAnotherIteration = 0

            //TO CONFIRM:
            //adding the money that were earned to users account:
            capitalismStorage->addMoney(nick, std::abs(numberOfStocksSoldInTHISiteration * currentPrice));



            if(nrStocksToSellInAnotherIteration>=0){
                std::cout << termcolor::red << "breaking out : " << nrStocksToSellInAnotherIteration << termcolor::reset<< std::endl;
                break; //no need to check the vector further
            }

        }
    } //end of for

    std::cout << termcolor::blue << "out of LOOP" << termcolor::reset<< std::endl;
    std::cout << termcolor::cyan << "moneyForSoldStocksInThisItrton: " << moneyForSoldStocksInThisItrton << termcolor::reset<< std::endl;

    std::cout << "gainTotal: " << gainTotal << std::endl;

    std::string post = postAboutGainFromEachEntry + " w sumie zarobiłeś na sprzedaży: " + std::to_string(gainTotal);
    std::cout << termcolor::magenta << "post: " << post << termcolor::reset<< std::endl;

    //capitalismStorage->getCapitalismGamePtr()->doPosting(post);  - cant do this cause of thread, so we need to use signal slot:


    setMoney(moneyForSoldStocksInThisItrton);
    gainTotal = 0.f;
    amountSold = 0;
    nrStocksToSellInAnotherIteration =0;
    postAboutGainFromEachEntry.clear();

    return post;
}

//std::string Capital::sellStock(std::string sellStockName, double amountSold, double currentPrice)
//{
//    if(amountSold <=0) return ""; //troll
//    double earnedMoney = currentPrice *amountSold;
//    double pastPrize;
//    double gain = 0;

//    for (int i = 0; i < singleUserStocks_v.size(); ++i)
//    {
//        std::cout << "current stock: " << singleUserStocks_v[i][0] << std::endl;

//        if(sellStockName == singleUserStocks_v[i][0])  //stock name
//        {
//            double stocksPossesed = std::stoi(singleUserStocks_v[i][1]); //number of stocks of this kind in possesion
//            double stockSoldFromEachEntry = stocksPossesed;

//            pastPrize = std::stod(singleUserStocks_v[i][2]); //stock prize from file
//            double temp = stocksPossesed;
//            stocksPossesed -= amountSold;
//            amountSold -= temp;
//            std::cout << termcolor::green << "stockSoldFromEachEntry: " << stockSoldFromEachEntry << " stocksPossesed: " << stocksPossesed << termcolor::reset<< std::endl;


//            if(stocksPossesed <=0)
//            {
//                singleUserStocks_v.erase(singleUserStocks_v.begin()+i);
//                std::cout << "singleUserStocks_v.size(): " << singleUserStocks_v.size() << std::endl;
//                //in this condition stockSoldFromEachEntry = the total number of stocks it had (cause we delete all entry)
//            }
//            else //TODO missing singleUserStocks_v.erase(singleUserStocks_v.begin()+i); ???????
//            {
//                singleUserStocks_v[i][1] = std::to_string(stocksPossesed);
//                stockSoldFromEachEntry -=  std::stod(singleUserStocks_v[i][1]);
//            }

//            std::cout << termcolor::red << "stockSoldFromEachEntry: " << stockSoldFromEachEntry << termcolor::reset<< std::endl;
//            gain += (currentPrice - pastPrize)* stockSoldFromEachEntry;
//            std::cout << "currentPrice: " << currentPrice << " pastPrize: " << pastPrize << "  singular gain: " << gain << std::endl;
//        }
//    } //end of for

//    std::cout << "final gain: " << gain << std::endl;
//    std::string post = "zarobiłeś na sprzedaży: " + std::to_string(gain);
//    //capitalismStorage->getCapitalismGamePtr()->doPosting(post); - cant do this cause of thread, so we need to use signal slot:

//    std::cout << "setMoney(gain)" << std::endl;
//    setMoney(earnedMoney);

//    return post;
//}

std::string Capital::checkStock(std::string stockName, double currentPrice)
{
    double pastPrize = 0;
    double gain = 0;

    std::cout << "Capital::checkStoc, singleUserStocks_v.size() : " << singleUserStocks_v.size() << "\n";
    for (int i = 0; i < singleUserStocks_v.size(); ++i) {

        if(stockName == singleUserStocks_v[i][0]  )  //stock name
        {

            pastPrize = std::stod(singleUserStocks_v[i][2]);
            std::cout << termcolor::red << "stock name: " << singleUserStocks_v[i][0] << termcolor::reset<< std::endl;
            gain += (currentPrice - pastPrize);
            std::cout << "currentPrice: " << currentPrice << " pastPrize: " << pastPrize << "  singular gain: " << gain << std::endl;
        }
    }
    std::cout << "final gain: " << gain << std::endl;
    std::string post = "zarobiłbys na sprzedaży (cena obecna - cena zakupu) : " + capitalismGame->round_float(gain);
    //capitalismStorage->getCapitalismGamePtr()->doPosting(post); - cant do this cause of thread, so we need to use signal slot:

    return post;
}


void Capital::setFactory(std::string factory_)
{
    std::cout << "setFactory: factory_: " << factory_ << std::endl;
    factory = factory_;
}

std::string& Capital::getNick()
{
    return nick;
}

double &Capital::getMoney()
{
    //money = round_upMoney(money); //remove extra zeros
    return money;
}

int &Capital::getRank()
{
    return rank;
}

std::vector<std::vector<std::string>>& Capital::getStocks()
{
    return singleUserStocks_v;
}

std::string& Capital::getFactory()
{
    return factory;
}

std::vector<std::string> &Capital::getAttributesForSave()
{
    //creates list of attributes for save


    std::vector<std::string> temp =
        {
            nick + " ", std::to_string(money) + " " + std::to_string(rank) + " " + factory
        };
    // was: nick+ " ", std::to_string(money) + " " + factory
    return temp;
}



