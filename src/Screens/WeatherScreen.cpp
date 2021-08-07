#include "WeatherScreen.h"

#include "OptimaLTStd12pt7b.h"
#include "GetWeather.h"

using namespace Watchy;

// should be std::unordered_map<uint16_t, const char*>
// sadly that is about 4k larger. :(
const struct {
  const int16_t code;
  const char *msg;
} codeMap[] = {
    {200, "thunderstorm & light rain"},
    {201, "thunderstorm & rain"},
    {202, "thunderstorm & heavy rain"},
    {210, "light thunderstorm"},
    {211, "thunderstorm"},
    {212, "heavy thunderstorm"},
    {221, "ragged thunderstorm"},
    {230, "thunderstorm & light drizzle"},
    {231, "thunderstorm & drizzle"},
    {232, "thunderstorm & heavy drizzle"},
    {300, "light drizzle"},
    {301, "drizzle"},
    {302, "heavy drizzle"},
    {310, "light drizzle rain"},
    {311, "drizzle rain"},
    {312, "heavy drizzle rain"},
    {313, "drizzle & rain showers"},
    {314, "drizzle & heavy rain showers"},
    {321, "drizzle showers"},
    {500, "light rain"},
    {501, "moderate rain"},
    {502, "heavy rain"},
    {503, "very heavy rain"},
    {504, "extreme rain"},
    {511, "freezing rain"},
    {520, "light rain showers"},
    {521, "rain showers"},
    {522, "heavy rain showers"},
    {531, "ragged rain showers"},
    {600, "light snow"},
    {601, "snow"},
    {602, "heavy snow"},
    {611, "sleet"},
    {612, "light sleet showers"},
    {613, "sleet showers"},
    {615, "light rain & snow"},
    {616, "rain & snow"},
    {620, "light snow showers"},
    {621, "snow showers"},
    {622, "heavy snow showers"},
    {701, "mist"},
    {711, "smoke"},
    {721, "haze"},
    {731, "sand/dust whirls"},
    {741, "fog"},
    {751, "sand"},
    {761, "dust"},
    {762, "volcanic ash"},
    {771, "squalls"},
    {781, "tornado"},
    {800, "clear"},          // <= 10%
    {801, "light clouds"},   // 11-25%
    {802, "partly cloudy"},  // 26-50%
    {803, "mostly cloudy"},  // 51-80%
    {804, "cloudy"}          // > 80%
};

constexpr const uint16_t NumCodeMap = sizeof(codeMap) / sizeof(codeMap[0]);

const char* weatherConditionCodeToString(uint16_t code) {
  static_assert(NumCodeMap > 0, "codeMap should not be empty");

  auto i = &codeMap[0];
  auto x = &codeMap[NumCodeMap];
  while (x - i > 1) {
    auto m = i + ((x - i) / 2);
    if (m->code > code) {
      x = m;
    } else {
      i = m;
    }
  }
  if (i->code == code) {
    return i->msg;
  }
  return "unknown";
}

void WeatherScreen::show() {
  Watchy::display.fillScreen(bgColor);
  display.setFont(OptimaLTStd12pt7b);
  display.setTextWrap(true);
  auto wd = Watchy_GetWeather::getWeather();
  display.printf("\n%d deg\n%s", wd.temperature,
                 weatherConditionCodeToString(wd.weatherConditionCode));
}
