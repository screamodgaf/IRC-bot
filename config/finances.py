import yfinance as yf
import datetime
import sys

#print(yf.__file__)

x = len(sys.argv)
if x==2:
    input = sys.argv[1]
elif x==3:
    input=sys.argv[1] + ' ' + sys.argv[2]

#input = "GOOG"
#data = yf.download("TSLA", start="2020-07-18", end="2020-07-18").info()
data = yf.Ticker(input).info
investment = data['shortName']
#print(data)
#print(investment)    //Tesla, Inc.
#print(data['previousClose'])

ticketdata = yf.Ticker(input)
today = datetime.datetime.today().isoformat()
#print('Today = ' + today)
tickerDF = ticketdata.history()
priceLast = tickerDF['Close'].iloc[-1]

print(investment)
print(priceLast)




