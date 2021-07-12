#include "Watchy.h"
#include "Screen.h"

using namespace Watchy;

void _rtcConfig(String datetime);    
void _bmaConfig();
void _configModeCallback(WiFiManager *myWiFiManager);
uint16_t _readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
uint16_t _writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);

DS3232RTC Watchy::RTC(false); 
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> Watchy::display(GxEPD2_154_D67(CS, DC, RESET, BUSY));
tmElements_t Watchy::currentTime;
RTC_DATA_ATTR Screen *Watchy::screen = nullptr;
Screen *Watchy::watchFace; // set by main

RTC_DATA_ATTR int Watchy::menuIndex;
RTC_DATA_ATTR BMA423 Watchy::sensor;
RTC_DATA_ATTR bool Watchy::WIFI_CONFIGURED;
RTC_DATA_ATTR bool Watchy::BLE_CONFIGURED;
RTC_DATA_ATTR weatherData currentWeather;
RTC_DATA_ATTR int weatherIntervalCounter = WEATHER_UPDATE_INTERVAL;

void doMenu(int index);

class MenuScreen : public Screen
{
public:
    static const char *const menuItems[];

public:
    MenuScreen() : Screen("MenuScreen") {};
    void setMenuIndex(int i) { menuIndex = i; }
    virtual void show();
    virtual void menu();
    virtual void back();
    virtual void up();
    virtual void down();
};

const char *const MenuScreen::menuItems[] = {
    "Check Battery",
    "Vibrate Motor",
    "Show Accelerometer",
    "Set Time",
    "Setup WiFi",
    "Update Firmware"};

const unsigned int MENU_LENGTH = (sizeof(MenuScreen::menuItems)/sizeof(MenuScreen::menuItems[0]));

void MenuScreen::show()
{
    DEBUG("MenuScreen::show\n");
    display.fillScreen(GxEPD_BLACK);
    display.setFont(&FreeMonoBold9pt7b);

    for (int i = 0; i < MENU_LENGTH; i++)
    {
        int16_t yPos = 30 + (MENU_HEIGHT * i);
        display.setCursor(0, yPos);
        if (i == menuIndex)
        {
            int16_t x1, y1;
            uint16_t w, h;

            display.getTextBounds(menuItems[i], 0, yPos, &x1, &y1, &w, &h);
            display.fillRect(x1 - 1, y1 - 10, 200, h + 15, GxEPD_WHITE);
            display.setTextColor(GxEPD_BLACK);
            display.println(menuItems[i]);
        }
        else
        {
            display.setTextColor(GxEPD_WHITE);
            display.println(menuItems[i]);
        }
    }
}

void MenuScreen::menu()
{
    DEBUG("MenuScreen::menu\n");
    doMenu(menuIndex);
}

void MenuScreen::back()
{
    DEBUG("MenuScreen::back\n");
    RTC.alarm(ALARM_2); //resets the alarm flag in the RTC
    setScreen(watchFace);
}

void MenuScreen::up()
{
    DEBUG("MenuScreen::up\n");
    menuIndex--;
    if (menuIndex < 0)
    {
        menuIndex = MENU_LENGTH - 1;
    }
    showMenu(menuIndex, true);
}

void MenuScreen::down()
{
    DEBUG("MenuScreen::down\n");
    menuIndex++;
    if (menuIndex > MENU_LENGTH - 1)
    {
        menuIndex = 0;
    }
    showMenu(menuIndex, true);
}

MenuScreen menuScreen;
Screen *Watchy::menuScreenP = &menuScreen;

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void Watchy::init(String datetime){
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause(); //get wake up reason
    Wire.begin(SDA, SCL); //init i2c

    switch (wakeup_reason)
    {
        #ifdef ESP_RTC
        case ESP_SLEEP_WAKEUP_TIMER: //ESP Internal RTC
            RTC.read(currentTime);
            currentTime.Minute++;
            tmElements_t tm;
            tm.Month = currentTime.Month;
            tm.Day = currentTime.Day;
            tm.Year = currentTime.Year;
            tm.Hour = currentTime.Hour;
            tm.Minute = currentTime.Minute;
            tm.Second = 0;
            time_t t = makeTime(tm);
            RTC.set(t);
            RTC.read(currentTime);           
            showWatchFace(true); //partial updates on tick
            break;        
        #endif
        case ESP_SLEEP_WAKEUP_EXT0: //RTC Alarm
            RTC.alarm(ALARM_2); //resets the alarm flag in the RTC
            showWatchFace(true); //partial updates on tick
            break;
        case ESP_SLEEP_WAKEUP_EXT1: //button Press
            handleButtonPress();
            break;
        default: //reset
            #ifndef ESP_RTC
            _rtcConfig(datetime);
            #endif
            _bmaConfig();
            if (screen == nullptr)
            {
                screen = watchFace;
            }
            showWatchFace(false); //full update on reset
            break;
    }
    deepSleep();
}

void Watchy::deepSleep(){
  #ifndef ESP_RTC
  esp_sleep_enable_ext0_wakeup(RTC_PIN, 0); //enable deep sleep wake on RTC interrupt
  #endif  
  #ifdef ESP_RTC
  esp_sleep_enable_timer_wakeup(60000000);
  #endif 
  esp_sleep_enable_ext1_wakeup(BTN_PIN_MASK, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press
  esp_deep_sleep_start();
}

void _rtcConfig(String datetime){
    if(datetime != NULL){
        const time_t FUDGE(30);//fudge factor to allow for upload time, etc. (seconds, YMMV)
        tmElements_t tm;
        tm.Year = getValue(datetime, ':', 0).toInt() - YEAR_OFFSET;//offset from 1970, since year is stored in uint8_t        
        tm.Month = getValue(datetime, ':', 1).toInt();
        tm.Day = getValue(datetime, ':', 2).toInt();
        tm.Hour = getValue(datetime, ':', 3).toInt();
        tm.Minute = getValue(datetime, ':', 4).toInt();
        tm.Second = getValue(datetime, ':', 5).toInt();

        time_t t = makeTime(tm) + FUDGE;
        RTC.set(t);

    }
    //https://github.com/JChristensen/DS3232RTC
    RTC.squareWave(SQWAVE_NONE); //disable square wave output
    //RTC.set(compileTime()); //set RTC time to compile time
    RTC.setAlarm(ALM2_EVERY_MINUTE, 0, 0, 0, 0); //alarm wakes up Watchy every minute
    RTC.alarmInterrupt(ALARM_2, true); //enable alarm interrupt
    RTC.read(currentTime);
}

bool pollButtonsAndDispatch() // returns true if button was pressed
{
        if (digitalRead(MENU_BTN_PIN) == 1)
        {
            DEBUG("%ld: pollButtonsAndDispatch menu\n", millis());
            screen->menu();
            return true;
        }
        if (digitalRead(BACK_BTN_PIN) == 1)
        {
            DEBUG("%ld: pollButtonsAndDispatch back\n", millis());
            screen->back();
            return true;
        }
        if (digitalRead(UP_BTN_PIN) == 1)
        {
            DEBUG("%ld: pollButtonsAndDispatch up\n", millis());
            screen->up();
            return true;
        }
        if (digitalRead(DOWN_BTN_PIN) == 1)
        {
            DEBUG("%ld: pollButtonsAndDispatch down\n", millis());
            screen->down();
            return true;
        }
        return false;
}

void fastEventLoop() {
    // TODO need a way for handlers to say they're done with the ui
    const int timeout = 5000;
    long timeoutMillis = millis() + timeout;
    DEBUG("%ld: fastEventLoop start timeout %ld\n", millis(), timeoutMillis);
    pinMode(MENU_BTN_PIN, INPUT);
    pinMode(BACK_BTN_PIN, INPUT);
    pinMode(UP_BTN_PIN, INPUT);
    pinMode(DOWN_BTN_PIN, INPUT);
    while (millis() < timeoutMillis)
    {
        if (pollButtonsAndDispatch()) {
            timeoutMillis = millis() + timeout;
        }
    }
    DEBUG("%ld: fastEventLoop done\n", millis());
}

void Watchy::handleButtonPress()
{
    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
    DEBUG("handleButtonPress %llx\n", wakeupBit);
    switch (wakeupBit & BTN_PIN_MASK)
    {
    case MENU_BTN_MASK:
        DEBUG("Watchy::handleButtonPress %s->menu()\n", screen->name);
        screen->menu();
        break;
    case BACK_BTN_MASK:
        DEBUG("Watchy::handleButtonPress %s->back()\n", screen->name);
        screen->back();
        break;
    case UP_BTN_MASK:
        DEBUG("Watchy::handleButtonPress %s->up()\n", screen->name);
        screen->up();
        break;
    case DOWN_BTN_MASK:
        DEBUG("Watchy::handleButtonPress %s->down()\n", screen->name);
        screen->down();
        break;
    default:
        break;
    }

    fastEventLoop();

    display.hibernate();
}

void Watchy::showMenu(byte menuIndex, bool partialRefresh){
    menuScreen.setMenuIndex(menuIndex);
    setScreen(&menuScreen, partialRefresh);
}

class BatteryScreen : public Screen {
    public:
        BatteryScreen() : Screen("BatteryScreen") {};
        void show()
        {
            display.init(0, false); //_initial_refresh to false to prevent full update on init
            display.setFullWindow();
            display.fillScreen(GxEPD_BLACK);
            display.setFont(&FreeMonoBold9pt7b);
            display.setTextColor(GxEPD_WHITE);
            display.setCursor(20, 30);
            display.println("Battery Voltage:");
            float voltage = getBatteryVoltage();
            display.setCursor(70, 80);
            display.print(voltage);
            display.println("V");
    }
    void up() { setScreen(&menuScreen); }
    void down() { setScreen(&menuScreen); }
    void back() { setScreen(&menuScreen); }
    void menu() { setScreen(&menuScreen); }
};

BatteryScreen batteryScreen;

class BuzzScreen : public Screen
{
public:
    BuzzScreen() : Screen("BuzzScreen") {};
    void show()
    {
        display.init(0, false); //_initial_refresh to false to prevent full update on init
        display.setFullWindow();
        display.fillScreen(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setTextColor(GxEPD_WHITE);
        display.setCursor(70, 80);
        display.println("Buzz!");
        display.display(true); // TODO: delete me
        vibMotor();
        setScreen(&menuScreen);
    }
    void up() { setScreen(&menuScreen); }
    void down() { setScreen(&menuScreen); }
    void back() { setScreen(&menuScreen); }
    void menu() { setScreen(&menuScreen); }
};

BuzzScreen buzzScreen;

void Watchy::vibMotor(uint8_t intervalMs, uint8_t length){
    pinMode(VIB_MOTOR_PIN, OUTPUT);
    bool motorOn = false;
    for(int i=0; i<length; i++){
        motorOn = !motorOn;
        digitalWrite(VIB_MOTOR_PIN, motorOn);
        delay(intervalMs);
    }
}

class SetTimeScreen : public Screen
{
public:
    SetTimeScreen() : Screen("SetTimeScreen") {};
    void show()
    {
        RTC.read(currentTime);

        int8_t minute = currentTime.Minute;
        int8_t hour = currentTime.Hour;
        int8_t day = currentTime.Day;
        int8_t month = currentTime.Month;
        int8_t year = currentTime.Year + YEAR_OFFSET - 1970;

        int8_t setIndex = SET_HOUR;

        int8_t blink = 0;

        pinMode(DOWN_BTN_PIN, INPUT);
        pinMode(UP_BTN_PIN, INPUT);
        pinMode(MENU_BTN_PIN, INPUT);
        pinMode(BACK_BTN_PIN, INPUT);

        display.init(0, true); //_initial_refresh to false to prevent full update on init
        display.setFullWindow();

        while (1)
        {

            if (digitalRead(MENU_BTN_PIN) == 1)
            {
                setIndex++;
                if (setIndex > SET_DAY)
                {
                    break;
                }
            }
            if (digitalRead(BACK_BTN_PIN) == 1)
            {
                if (setIndex != SET_HOUR)
                {
                    setIndex--;
                }
            }

            blink = 1 - blink;

            if (digitalRead(DOWN_BTN_PIN) == 1)
            {
                blink = 1;
                switch (setIndex)
                {
                case SET_HOUR:
                    hour == 23 ? (hour = 0) : hour++;
                    break;
                case SET_MINUTE:
                    minute == 59 ? (minute = 0) : minute++;
                    break;
                case SET_YEAR:
                    year == 99 ? (year = 20) : year++;
                    break;
                case SET_MONTH:
                    month == 12 ? (month = 1) : month++;
                    break;
                case SET_DAY:
                    day == 31 ? (day = 1) : day++;
                    break;
                default:
                    break;
                }
            }

            if (digitalRead(UP_BTN_PIN) == 1)
            {
                blink = 1;
                switch (setIndex)
                {
                case SET_HOUR:
                    hour == 0 ? (hour = 23) : hour--;
                    break;
                case SET_MINUTE:
                    minute == 0 ? (minute = 59) : minute--;
                    break;
                case SET_YEAR:
                    year == 20 ? (year = 99) : year--;
                    break;
                case SET_MONTH:
                    month == 1 ? (month = 12) : month--;
                    break;
                case SET_DAY:
                    day == 1 ? (day = 31) : day--;
                    break;
                default:
                    break;
                }
            }

            display.fillScreen(GxEPD_BLACK);
            display.setTextColor(GxEPD_WHITE);
            display.setFont(&DSEG7_Classic_Bold_53);

            display.setCursor(5, 80);
            if (setIndex == SET_HOUR)
            { //blink hour digits
                display.setTextColor(blink ? GxEPD_WHITE : GxEPD_BLACK);
            }
            if (hour < 10)
            {
                display.print("0");
            }
            display.print(hour);

            display.setTextColor(GxEPD_WHITE);
            display.print(":");

            display.setCursor(108, 80);
            if (setIndex == SET_MINUTE)
            { //blink minute digits
                display.setTextColor(blink ? GxEPD_WHITE : GxEPD_BLACK);
            }
            if (minute < 10)
            {
                display.print("0");
            }
            display.print(minute);

            display.setTextColor(GxEPD_WHITE);

            display.setFont(&FreeMonoBold9pt7b);
            display.setCursor(45, 150);
            if (setIndex == SET_YEAR)
            { //blink minute digits
                display.setTextColor(blink ? GxEPD_WHITE : GxEPD_BLACK);
            }
            display.print(1970 + year);

            display.setTextColor(GxEPD_WHITE);
            display.print("/");

            if (setIndex == SET_MONTH)
            { //blink minute digits
                display.setTextColor(blink ? GxEPD_WHITE : GxEPD_BLACK);
            }
            if (month < 10)
            {
                display.print("0");
            }
            display.print(month);

            display.setTextColor(GxEPD_WHITE);
            display.print("/");

            if (setIndex == SET_DAY)
            { //blink minute digits
                display.setTextColor(blink ? GxEPD_WHITE : GxEPD_BLACK);
            }
            if (day < 10)
            {
                display.print("0");
            }
            display.print(day);
            display.display(true); //partial refresh
        }

        display.hibernate();

        const time_t FUDGE(10); //fudge factor to allow for upload time, etc. (seconds, YMMV)
        tmElements_t tm;
        tm.Month = month;
        tm.Day = day;
        tm.Year = year + 1970 - YEAR_OFFSET; //offset from 1970, since year is stored in uint8_t
        tm.Hour = hour;
        tm.Minute = minute;
        tm.Second = 0;

        time_t t = makeTime(tm) + FUDGE;
        RTC.set(t);
        setScreen(&menuScreen);
    }
    void up() { setScreen(&menuScreen); }
    void down() { setScreen(&menuScreen); }
    void back() { setScreen(&menuScreen); }
    void menu() { setScreen(&menuScreen); }
};

SetTimeScreen setTimeScreen;

class AccelerometerScreen : public Screen
{
public:
    AccelerometerScreen() : Screen("AccelerometerScreen"){};
    void show()
    {
        // TODO consider using the RTC to tell us to update every 200 ms
        display.init(0, true); //_initial_refresh to false to prevent full update on init
        display.setFullWindow();
        display.fillScreen(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setTextColor(GxEPD_WHITE);

        Accel acc;

        const int interval = 200;
        long timeout = millis() + interval;

        pinMode(BACK_BTN_PIN, INPUT);

        while (!pollButtonsAndDispatch())
        {
            if (millis() < timeout)
            {
                continue;
            }

            timeout = millis() + interval;

            display.fillScreen(GxEPD_BLACK);
            display.setCursor(0, 30);

            // Get acceleration data
            if (!sensor.getAccel(acc))
            {
                display.println("getAccel FAIL");
                continue;
            }

            display.printf("  X: %d\n  Y: %d\n  Z: %d\n", acc.x, acc.y, acc.z);

            display.setCursor(30, 130);
            uint8_t direction = sensor.getDirection();
            switch (direction)
            {
            case DIRECTION_DISP_DOWN:
                display.println("FACE DOWN");
                break;
            case DIRECTION_DISP_UP:
                display.println("FACE UP");
                break;
            case DIRECTION_BOTTOM_EDGE:
                display.println("BOTTOM EDGE");
                break;
            case DIRECTION_TOP_EDGE:
                display.println("TOP EDGE");
                break;
            case DIRECTION_RIGHT_EDGE:
                display.println("RIGHT EDGE");
                break;
            case DIRECTION_LEFT_EDGE:
                display.println("LEFT EDGE");
                break;
            default:
                display.println("ERROR!!!");
                break;
            }
            display.display(true); //full refresh
        }
        setScreen(&menuScreen);
    }
    void up() { setScreen(&menuScreen); }
    void down() { setScreen(&menuScreen); }
    void back() { setScreen(&menuScreen); }
    void menu() { setScreen(&menuScreen); }
};

AccelerometerScreen accelerometerScreen;

void Watchy::showWatchFace(bool partialRefresh){
  display.init(0, false); //_initial_refresh to false to prevent full update on init
  display.setFullWindow();
  DEBUG("Watchy::showWatchFace %s->show()\n", screen->name);
  screen->show();
  display.display(partialRefresh); //partial refresh
  display.hibernate();
}

void Watchy::setScreen(Screen *s, bool partialRefresh)
{
    if (s == nullptr)
    {
        DEBUG("setScreen nullptr\n");
        return;
    }
    DEBUG("setScreen %08lx (%s)\n", (long unsigned)s, s->name);
    screen = s;
    showWatchFace(partialRefresh);
}

weatherData Watchy::getWeatherData(){
    if(weatherIntervalCounter >= WEATHER_UPDATE_INTERVAL){ //only update if WEATHER_UPDATE_INTERVAL has elapsed i.e. 30 minutes
        if(connectWiFi()){//Use Weather API for live data if WiFi is connected
            HTTPClient http;
            http.setConnectTimeout(3000);//3 second max timeout
            String weatherQueryURL = String(OPENWEATHERMAP_URL) + String(CITY_NAME) + String(",") + String(COUNTRY_CODE) + String("&units=") + String(TEMP_UNIT) + String("&appid=") + String(OPENWEATHERMAP_APIKEY);
            http.begin(weatherQueryURL.c_str());
            int httpResponseCode = http.GET();
            if(httpResponseCode == 200) {
                String payload = http.getString();
                JSONVar responseObject = JSON.parse(payload);
                currentWeather.temperature = int(responseObject["main"]["temp"]);
                currentWeather.weatherConditionCode = int(responseObject["weather"][0]["id"]);            
            }else{
                //http error
            }
            http.end();
            //turn off radios
            WiFi.mode(WIFI_OFF);
            btStop();
        }else{//No WiFi, use RTC Temperature
            uint8_t temperature = RTC.temperature() / 4; //celsius
            if(strcmp(TEMP_UNIT, "imperial") == 0){
                temperature = temperature * 9. / 5. + 32.; //fahrenheit
            }
            currentWeather.temperature = temperature;
            currentWeather.weatherConditionCode = 800;
        }
        weatherIntervalCounter = 0;
    }else{
        weatherIntervalCounter++;
    }
    return currentWeather;
}

float Watchy::getBatteryVoltage(){
    return analogRead(ADC_PIN) / 4096.0 * 7.23;
}

uint16_t _readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
{
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)address, (uint8_t)len);
    uint8_t i = 0;
    while (Wire.available()) {
        data[i++] = Wire.read();
    }
    return 0;
}

uint16_t _writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
{
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(data, len);
    return (0 !=  Wire.endTransmission());
}

void _bmaConfig(){
 
    if (sensor.begin(_readRegister, _writeRegister, delay) == false) {
        //fail to init BMA
        return;
    }

    // Accel parameter structure
    Acfg cfg;
    /*!
        Output data rate in Hz, Optional parameters:
            - BMA4_OUTPUT_DATA_RATE_0_78HZ
            - BMA4_OUTPUT_DATA_RATE_1_56HZ
            - BMA4_OUTPUT_DATA_RATE_3_12HZ
            - BMA4_OUTPUT_DATA_RATE_6_25HZ
            - BMA4_OUTPUT_DATA_RATE_12_5HZ
            - BMA4_OUTPUT_DATA_RATE_25HZ
            - BMA4_OUTPUT_DATA_RATE_50HZ
            - BMA4_OUTPUT_DATA_RATE_100HZ
            - BMA4_OUTPUT_DATA_RATE_200HZ
            - BMA4_OUTPUT_DATA_RATE_400HZ
            - BMA4_OUTPUT_DATA_RATE_800HZ
            - BMA4_OUTPUT_DATA_RATE_1600HZ
    */
    cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
    /*!
        G-range, Optional parameters:
            - BMA4_ACCEL_RANGE_2G
            - BMA4_ACCEL_RANGE_4G
            - BMA4_ACCEL_RANGE_8G
            - BMA4_ACCEL_RANGE_16G
    */
    cfg.range = BMA4_ACCEL_RANGE_2G;
    /*!
        Bandwidth parameter, determines filter configuration, Optional parameters:
            - BMA4_ACCEL_OSR4_AVG1
            - BMA4_ACCEL_OSR2_AVG2
            - BMA4_ACCEL_NORMAL_AVG4
            - BMA4_ACCEL_CIC_AVG8
            - BMA4_ACCEL_RES_AVG16
            - BMA4_ACCEL_RES_AVG32
            - BMA4_ACCEL_RES_AVG64
            - BMA4_ACCEL_RES_AVG128
    */
    cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;

    /*! Filter performance mode , Optional parameters:
        - BMA4_CIC_AVG_MODE
        - BMA4_CONTINUOUS_MODE
    */
    cfg.perf_mode = BMA4_CONTINUOUS_MODE;

    // Configure the BMA423 accelerometer
    sensor.setAccelConfig(cfg);

    // Enable BMA423 accelerometer
    // Warning : Need to use feature, you must first enable the accelerometer
    // Warning : Need to use feature, you must first enable the accelerometer
    sensor.enableAccel();

    struct bma4_int_pin_config config ;
    config.edge_ctrl = BMA4_LEVEL_TRIGGER;
    config.lvl = BMA4_ACTIVE_HIGH;
    config.od = BMA4_PUSH_PULL;
    config.output_en = BMA4_OUTPUT_ENABLE;
    config.input_en = BMA4_INPUT_DISABLE;
    // The correct trigger interrupt needs to be configured as needed
    sensor.setINTPinConfig(config, BMA4_INTR1_MAP);

    struct bma423_axes_remap remap_data;
    remap_data.x_axis = 1;
    remap_data.x_axis_sign = 0xFF;
    remap_data.y_axis = 0;
    remap_data.y_axis_sign = 0xFF;
    remap_data.z_axis = 2;
    remap_data.z_axis_sign = 0xFF;
    // Need to raise the wrist function, need to set the correct axis
    sensor.setRemapAxes(&remap_data);

    // Enable BMA423 isStepCounter feature
    sensor.enableFeature(BMA423_STEP_CNTR, true);
    // Enable BMA423 isTilt feature
    sensor.enableFeature(BMA423_TILT, true);
    // Enable BMA423 isDoubleClick feature
    sensor.enableFeature(BMA423_WAKEUP, true);

    // Reset steps
    sensor.resetStepCounter();

    // Turn on feature interrupt
    sensor.enableStepCountInterrupt();
    sensor.enableTiltInterrupt();
    // It corresponds to isDoubleClick interrupt
    sensor.enableWakeupInterrupt();  
}

class SetupWifiScreen : public Screen
{
public:
    SetupWifiScreen() : Screen("SetupWifiScreen") {};
    void show()
    {
        WiFiManager wifiManager;
        wifiManager.resetSettings();
        wifiManager.setTimeout(WIFI_AP_TIMEOUT);
        wifiManager.setAPCallback(_configModeCallback);
        if (!wifiManager.autoConnect(WIFI_AP_SSID))
        {                           //WiFi setup failed
            display.init(0, false); //_initial_refresh to false to prevent full update on init
            display.setFullWindow();
            display.fillScreen(GxEPD_BLACK);
            display.setFont(&FreeMonoBold9pt7b);
            display.setTextColor(GxEPD_WHITE);
            display.setCursor(0, 30);
            display.println("Setup failed &");
            display.println("timed out!");
            display.display(false); //full refresh
            display.hibernate();
        }
        else
        {
            display.init(0, false); //_initial_refresh to false to prevent full update on init
            display.setFullWindow();
            display.fillScreen(GxEPD_BLACK);
            display.setFont(&FreeMonoBold9pt7b);
            display.setTextColor(GxEPD_WHITE);
            display.println("Connected to");
            display.println(WiFi.SSID());
            display.display(false); //full refresh
            display.hibernate();
        }
        //turn off radios
        WiFi.mode(WIFI_OFF);
        btStop();
    }
    void up() { setScreen(&menuScreen); }
    void down() { setScreen(&menuScreen); }
    void back() { setScreen(&menuScreen); }
    void menu() { setScreen(&menuScreen); }
};

SetupWifiScreen setupWifiScreen;

void _configModeCallback (WiFiManager *myWiFiManager) {
  display.init(0, false); //_initial_refresh to false to prevent full update on init
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_WHITE);
  display.setCursor(0, 30);
  display.println("Connect to");
  display.print("SSID: ");
  display.println(WIFI_AP_SSID);
  display.print("IP: ");
  display.println(WiFi.softAPIP());
  display.display(false); //full refresh
  display.hibernate();
}

bool Watchy::connectWiFi(){
    if(WL_CONNECT_FAILED == WiFi.begin()){//WiFi not setup, you can also use hard coded credentials with WiFi.begin(SSID,PASS);
        WIFI_CONFIGURED = false;
    }else{
        if(WL_CONNECTED == WiFi.waitForConnectResult()){//attempt to connect for 10s
            WIFI_CONFIGURED = true;
        }else{//connection failed, time out
            WIFI_CONFIGURED = false;
            //turn off radios
            WiFi.mode(WIFI_OFF);
            btStop();
        }
    }
    return WIFI_CONFIGURED;
}

class UpdateFWScreen : public Screen
{
public:
    UpdateFWScreen() : Screen("UpdateFWScreen"){};
    void show()
    {
        display.init(0, false); //_initial_refresh to false to prevent full update on init
        display.setFullWindow();
        display.fillScreen(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setTextColor(GxEPD_WHITE);
        display.setCursor(0, 30);
        display.println("Please Visit");
        display.println("watchy.sqfmi.com");
        display.println("with a Bluetooth");
        display.println("enabled device");
        display.println(" ");
        display.println("Press menu button");
        display.println("again when ready");
        display.println(" ");
        display.println("Keep USB powered");
        display.display(false); //full refresh
        display.hibernate();
    }
    void up() { setScreen(&menuScreen); }
    void down() { setScreen(&menuScreen); }
    void back() { setScreen(&menuScreen); }
    void menu() { Watchy::updateFWBegin(); }
};

UpdateFWScreen updateFWScreen;

void Watchy::updateFWBegin(){
    display.init(0, false); //_initial_refresh to false to prevent full update on init
    display.setFullWindow();
    display.fillScreen(GxEPD_BLACK);
    display.setFont(&FreeMonoBold9pt7b);
    display.setTextColor(GxEPD_WHITE);
    display.setCursor(0, 30);
    display.println("Bluetooth Started");
    display.println(" ");
    display.println("Watchy BLE OTA");
    display.println(" ");
    display.println("Waiting for");
    display.println("connection...");
    display.display(false); //full refresh

    BLE BT;
    BT.begin("Watchy BLE OTA");
    int prevStatus = -1;
    int currentStatus;

    while(1){
    currentStatus = BT.updateStatus();
    if(prevStatus != currentStatus || prevStatus == 1){
        if(currentStatus == 0){
        display.setFullWindow();
        display.fillScreen(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setTextColor(GxEPD_WHITE);
        display.setCursor(0, 30);
        display.println("BLE Connected!");
        display.println(" ");
        display.println("Waiting for");
        display.println("upload...");
        display.display(false); //full refresh
        }
        if(currentStatus == 1){
        display.setFullWindow();
        display.fillScreen(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setTextColor(GxEPD_WHITE);
        display.setCursor(0, 30);
        display.println("Downloading");
        display.println("firmware:");
        display.println(" ");
        display.print(BT.howManyBytes());
        display.println(" bytes");
        display.display(true); //partial refresh        
        }
        if(currentStatus == 2){
        display.setFullWindow();
        display.fillScreen(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setTextColor(GxEPD_WHITE);
        display.setCursor(0, 30);
        display.println("Download");
        display.println("completed!");
        display.println(" ");
        display.println("Rebooting...");
        display.display(false); //full refresh

        delay(2000);
        esp_restart();           
        }
        if(currentStatus == 4){
        display.setFullWindow();
        display.fillScreen(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setTextColor(GxEPD_WHITE);
        display.setCursor(0, 30);
        display.println("BLE Disconnected!");
        display.println(" ");
        display.println("exiting...");
        display.display(false); //full refresh
        delay(1000);
        break;
        }
        prevStatus = currentStatus;
    }
    delay(100);
    }

    //turn off radios
    WiFi.mode(WIFI_OFF);
    btStop();
    showMenu(menuIndex, false);
}

// time_t compileTime()
// {   
//     const time_t FUDGE(10);    //fudge factor to allow for upload time, etc. (seconds, YMMV)
//     const char *compDate = __DATE__, *compTime = __TIME__, *months = "JanFebMarAprMayJunJulAugSepOctNovDec";
//     char compMon[3], *m;

//     strncpy(compMon, compDate, 3);
//     compMon[3] = '\0';
//     m = strstr(months, compMon);

//     tmElements_t tm;
//     tm.Month = ((m - months) / 3 + 1);
//     tm.Day = atoi(compDate + 4);
//     tm.Year = atoi(compDate + 7) - YEAR_OFFSET; // offset from 1970, since year is stored in uint8_t
//     tm.Hour = atoi(compTime);
//     tm.Minute = atoi(compTime + 3);
//     tm.Second = atoi(compTime + 6);

//     time_t t = makeTime(tm);
//     return t + FUDGE;        //add fudge factor to allow for compile time
// }

void doMenu(int index) {
    switch (index)
    {
    case 0:
        setScreen(&batteryScreen);
        break;
    case 1:
        setScreen(&buzzScreen);
        break;
    case 2:
        setScreen(&accelerometerScreen);
        break;
    case 3:
        setScreen(&setTimeScreen);
        break;
    case 4:
        setScreen(&setupWifiScreen);
        break;
    case 5:
        setScreen(&updateFWScreen);
        break;
    default:
        break;
    }
}
