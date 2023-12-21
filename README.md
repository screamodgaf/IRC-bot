Compilation requires Qt5 or Qt6 libraries (including network module)
for dependencies on Linux (Ubuntu) run:
pip install openai==0.28
pip install yfinance

put openai key into chatbot.py file in line 21 
put openweathermap key into weather.py file in line 16 
put the config folder into the build directory

proxy server address is set in Settings.cpp
 
in the Settings.cpp file, it's possible to set own proxy
to activate proxy, uncomment line 137 in settings.cpp (only after proxy is set in settings.cpp)
